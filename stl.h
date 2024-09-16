#pragma once
#include <vector>
#include <../types.h>
namespace stl
{

	template<typename T>
	struct Node
	{
		T data;
		Node<T>* next;
	};

	template<typename T>
	class LinkedList
	{
	public:
		LinkedList() = default;
		LinkedList(T el);
		LinkedList(const LinkedList&) = delete;
		LinkedList& operator=(const LinkedList&) = delete;
		Node<T>* Add(T el);
		T front() const;
		T back() const;
		void PrintLinkedList();
	private:
		Node<T>* frontPtr;
		Node<T>* backPtr;
	};
	template<typename T>
	inline LinkedList<T>::LinkedList(T el)
		: frontPtr {new Node<T>(el, nullptr)}, backPtr {frontPtr} {}

	template<typename T>
	inline Node<T>* LinkedList<T>::Add(T el)
	{
		Node<T>* temp = new Node<T>(el);
		while (backPtr->next != nullptr)
		{
			backPtr = backPtr->next;
		}
		backPtr->next = temp;
		backPtr = temp;

		return backPtr;
	}

	template<typename T>
	inline T LinkedList<T>::front() const
	{
		return frontPtr->data;
	}

	template<typename T>
	inline T LinkedList<T>::back() const
	{
		return backPtr->data;
	}

	template<typename T>
	inline void LinkedList<T>::PrintLinkedList()
	{
		Node<T>* temp = front;
		while (temp != nullptr)
		{
			auto element = temp->data;
			temp = temp->next;
		}
	}













	template <typename T>
	class Queue
	{
	public:
		Queue() = default;
		Queue(LinkedList& list);
		~Queue();
		void Enqueue(T el);
		void Dequeue();
		T Peek();
		size_t size() const;
		T front() const;
		bool isEmpty() const;
	private:
		LinkedList<T> data;
		i32 MAX_CAPACITY = 64;
	};



}

