/******************************************************************************
 *  myled.c
 *
 *  Linux BSP Practice
 *  Device Tree -> Platform Driver -> GPIO Subsystem
 *
 *  Mục tiêu:
 *      - Bind với Platform Device được tạo từ Device Tree
 *      - Đọc GPIO từ Device Tree
 *      - Điều khiển LED thông qua GPIO Subsystem
 *
 *  Milestone hiện tại:
 *      Southbound
 *      Device Tree
 *          ↓
 *      Platform Device
 *          ↓
 *      Platform Driver
 *          ↓
 *      GPIO Subsystem
 *          ↓
 *      LED
 ******************************************************************************/

#include <linux/module.h>          // Module programming
#include <linux/platform_device.h> // Platform Driver / Platform Device
#include <linux/of.h>              // Device Tree (Open Firmware)
#include <linux/gpio/consumer.h>   // GPIO Descriptor API (gpiod_*)
#include <linux/slab.h>            // devm_kzalloc()
#include <linux/err.h>             // IS_ERR(), PTR_ERR()

/******************************************************************************
 * Driver private data
 *
 * Mỗi Platform Device sẽ có một vùng dữ liệu riêng của driver.
 *
 * Ví dụ:
 *
 *      Device A --------+
 *                       |
 *                    struct myled_data
 *
 *      Device B --------+
 *                       |
 *                    struct myled_data
 *
 * Sau này struct này sẽ mở rộng thêm:
 *
 *      - Character Device
 *      - Mutex
 *      - Timer
 *      - Workqueue
 *      - ...
 ******************************************************************************/
struct myled_data
{
    /*
     * GPIO Descriptor.
     *
     * Đây KHÔNG phải là số GPIO (gpio1_28).
     *
     * Đây là một "handle" (descriptor) do GPIO Subsystem cấp.
     *
     * Driver sẽ thao tác thông qua descriptor thay vì số GPIO.
     */
    struct gpio_desc *led;
};

/******************************************************************************
 * probe()
 *
 * Hàm quan trọng nhất của Platform Driver.
 *
 * Kernel sẽ tự động gọi probe() khi thỏa mãn cả 2:
 *
 *      1. Có Platform Device.
 *      2. compatible trong Device Tree khớp với of_match_table.
 *
 * Nhiệm vụ:
 *
 *      - Khởi tạo phần cứng
 *      - Đọc Device Tree
 *      - Lấy GPIO
 *      - Chuẩn bị driver hoạt động
 ******************************************************************************/
static int myled_probe(struct platform_device *pdev) // pdev là một struct được kernel tự tạo ứng với Platform Device sau khi parse xong Device Tree
{
    struct myled_data *data;

    /*
     * In log.
     *
     * dev_info() sẽ tự động thêm tên thiết bị vào log.
     *
     * Xem bằng:
     *
     *      dmesg
     */
    dev_info(&pdev->dev, "Probe called\n");

    /**********************************************************************
     * Cấp phát bộ nhớ cho private data.
     *
     * devm_kzalloc():
     *
     *      devm = Device Managed
     *
     * Bộ nhớ sẽ được Kernel tự động giải phóng khi driver bị remove.
     *
     * kzalloc:
     *
     *      k = Kernel
     *      z = Zero (toàn bộ memory được memset = 0)
     **********************************************************************/
    data = devm_kzalloc(&pdev->dev,
                        sizeof(*data),
                        GFP_KERNEL);

    if (!data)
        return -ENOMEM;

    /**********************************************************************
     * Đọc GPIO từ Device Tree.
     *
     * Kernel sẽ tự tìm property:
     *
     *      gpios = <&gpio1 28 GPIO_ACTIVE_HIGH>;
     *
     * rồi:
     *
     *      Device Tree
     *            ↓
     *      GPIO Controller
     *            ↓
     *      GPIO Descriptor
     *
     * GPIOD_OUT_LOW:
     *
     *      - cấu hình Output
     *      - mức logic ban đầu = LOW
     **********************************************************************/
    data->led = devm_gpiod_get(&pdev->dev,
                               NULL,
                               GPIOD_OUT_LOW);

    /*
     * Nếu lấy GPIO thất bại
     * (không tồn tại, đang bị chiếm, ...)
     */
    if (IS_ERR(data->led))
    {
        dev_err(&pdev->dev, "Cannot get GPIO\n");
        return PTR_ERR(data->led);
    }

    /**********************************************************************
     * Lưu private data.
     *
     * Sau này trong remove():
     *
     *      platform_get_drvdata()
     *
     * sẽ lấy lại được con trỏ này.
     **********************************************************************/
    platform_set_drvdata(pdev, data);

    /**********************************************************************
     * Điều khiển LED.
     *
     * GPIO Descriptor API:
     *
     *      gpiod_set_value()
     *
     * sẽ thông qua GPIO Subsystem để điều khiển GPIO Controller.
     *
     * Nếu GPIO_ACTIVE_HIGH:
     *
     *      value = 1
     *
     * nghĩa là LED sáng.
     **********************************************************************/
    gpiod_set_value(data->led, 1);

    dev_info(&pdev->dev, "LED ON\n");

    return 0;
}

/******************************************************************************
 * remove()
 *
 * Được Kernel gọi khi:
 *
 *      rmmod myled
 *
 * hoặc Platform Device bị remove.
 ******************************************************************************/
static int myled_remove(struct platform_device *pdev)
{
    struct myled_data *data;

    /*
     * Lấy lại private data đã lưu trong probe().
     */
    data = platform_get_drvdata(pdev);

    /*
     * Tắt LED trước khi driver rời khỏi hệ thống.
     */
    gpiod_set_value(data->led, 0);

    dev_info(&pdev->dev, "LED OFF\n");

    /*
     * Không cần gọi kfree().
     *
     * Vì toàn bộ tài nguyên được cấp phát bằng devm_*
     * sẽ tự động được Kernel thu hồi.
     */
    return 0;
}

/******************************************************************************
 * Device Tree Match Table
 *
 * Đây là bảng cho Kernel biết:
 *
 * Driver này hỗ trợ những compatible nào.
 *
 * Khi Kernel thấy:
 *
 *      compatible = "mycompany,myled";
 *
 * trong Device Tree,
 *
 * nó sẽ bind Platform Device với driver này.
 ******************************************************************************/
static const struct of_device_id myled_of_match[] =
    {
        {.compatible = "mycompany,myled"},
        {} /* Kết thúc bảng */
};

/*
 * Xuất Match Table vào meta data của Module: cứ hiểu hình thức là phải có đi
 *
 * Giúp:
 *
 *      - Module Auto Loading
 *      - modinfo
 *      - depmod
 */
MODULE_DEVICE_TABLE(of, myled_of_match);

/******************************************************************************
 * Platform Driver
 *
 * Đây là đối tượng đại diện cho Driver.
 *
 * Kernel sẽ đăng ký đối tượng này vào Platform Bus.
 ******************************************************************************/
static struct platform_driver myled_driver =
    {
        /*
         * Callback khi bind thành công.
         */
        .probe = myled_probe,

        /*
         * Callback khi unbind.
         */
        .remove = myled_remove,

        .driver =
            {
                /*
                 * Tên Driver.
                 *
                 * Chủ yếu dùng để hiển thị.
                 */
                .name = "myled",

                /*
                 * Bảng compatible hỗ trợ.
                 */
                .of_match_table = myled_of_match,
            },
};

/******************************************************************************
 * Đăng ký Platform Driver.
 *
 * Macro này tương đương:
 *
 *      module_init(...)
 *      module_exit(...)
 *
 * và bên trong sẽ gọi:
 *
 *      platform_driver_register()
 *      platform_driver_unregister()
 ******************************************************************************/
module_platform_driver(myled_driver);

/******************************************************************************
 * Module Information
 ******************************************************************************/
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Your Name");
MODULE_DESCRIPTION("Platform GPIO LED Driver");