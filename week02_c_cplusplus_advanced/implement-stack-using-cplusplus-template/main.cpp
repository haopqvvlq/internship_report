#include <iostream>
template <typename T>
struct Node
{
    T data;
    Node *next;
};

template <typename T>
void push(Node<T> *&head, T value)
{
    Node<T> *newNode = new Node<T>;
    newNode->data = value;
    newNode->next = head;
    head = newNode;
}

template <typename T>
void pop(Node<T> *&head)
{
    if (head == nullptr)
    {
        printf("Stack is empty!\n");
        return;
    }

    Node<T> *temp = head;
    head = head->next;
    delete temp;
}

template <typename T>
void display(Node<T> *head)
{
    if (head == nullptr)
    {
        printf("Stack is empty!\n");
        return;
    }

    while (head != nullptr)
    {
        std::cout << head->data << " ";
        head = head->next;
    }

    std::cout << '\n';
}

int main()
{
    Node<int> *head = nullptr;

    push(head, 10);
    push(head, 20);
    push(head, 30);

    display(head);

    pop(head);

    display(head);
}