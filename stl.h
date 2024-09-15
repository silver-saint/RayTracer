#pragma once
#include <vector>
#include <../types.h>
namespace stl
{
	template <typename T>
	class Queue
	{
	public:
		Queue() = default;
		Queue(i32 size);
		~Queue();
		void Enqueue(T el);
		void Dequeue();
		T Peek();
		size_t size() const;
		T front() const;
		bool isEmpty() const;
	private:
		std::vector<T> data;
		i32 MAX_CAPACITY = 64;
	};

	//Cannot exceed MAX capacity;
	template<typename T>
	inline Queue<T>::Queue(i32 size)
	{
		if (size >= MAX_CAPACITY)
		{
			size = MAX_CAPACITY;
		}
		data.reserve(size);

	}

	template<typename T>
	inline Queue<T>::~Queue()
	{
		data.clear();
	}

	template<typename T>
	inline void Queue<T>::Enqueue(T el)
	{
		if (data.size == data.capacity)
		{
			OutputDebugString(L"Cannot enqueue another element");
		}
		else
		{
			data.push_back(el);
		}
	}

	template<typename T>
	inline void Queue<T>::Dequeue()
	{
		if (data.size > 0)
		{
			T el = data.front();
			auto it = std::find(data.begin(), data.end(), el);

			if (it != data.end())
			{
				data.erase(it);
			}
		}
		else
		{
			OutputDebugString(L"Cannot dequeue another element");
		}
	}

	template<typename T>
	inline T Queue<T>::Peek()
	{
		return data.front();
	}

	template<typename T>
	inline size_t Queue<T>::size() const
	{
		return data.size();
	}

	template<typename T>
	inline T Queue<T>::front() const
	{
		return data[0];
	}

	template<typename T>
	inline bool Queue<T>::isEmpty() const
	{
		return data.size() == 0;
	}

}

