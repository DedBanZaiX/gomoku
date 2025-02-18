#pragma once
#include <stdexcept>

namespace asd_mvv
{
	template<typename T>
	class ListIterator;

	template <typename T>
	class Node
	{
	public:
		Node* pNext;
		Node* pPrev;
		T data;
		Node(T data = T(), Node* pPrev = nullptr, Node* pNext = nullptr)
		{
			this->data = data;
			this->pPrev = pPrev;
			this->pNext = pNext;
		}
	};

	/// <summary>
	/// Двусвязный список
	/// </summary>
	template <typename T>
	class List
	{
	public:
		typedef ListIterator<T> iterator;
		List() : Size(0), head(nullptr), tail(nullptr) {}
		const int GetSize() { return Size; }
		void pushBack(T data);
		void popFront();
		void clear();
		void pushFront(T data);
		void popBack();
		iterator begin() { return iterator(head); }
		iterator end() { return iterator(nullptr); }
		iterator rbegin() { return iterator(tail); }
		iterator rend() { return iterator(nullptr); }
		~List() { clear(); }

		/// <summary>
		/// Обращение к элементу списка.
		/// ВНИМАНИЕ: каждое обращение идет с начала или конца списка!
		/// </summary>
		T& operator [] (const int index);

		/// <summary>
		/// После удаления итератор указывает 
		/// на элемент следующий после удаленного
		/// </summary>
		void erase(iterator& it);

		/// <summary>
		/// После вставки итератор указывает 
		/// на вставленный элемент
		/// </summary>
		void insert(iterator& it, T data);

	private:
		int Size;
		Node<T>* head;
		Node<T>* tail;
		void* searchPrevious(int index);
	};

	template<typename T>
	class ListIterator
	{
	private:
		template<typename T>
		friend void List<T>::erase(iterator& it);
		friend void List<T>::insert(iterator& it, T data);
		Node<T>* current;
	public:
		ListIterator(Node<T>* ptr) : current(ptr) {}

		T& operator*() const
		{
			return current->data;
		}
		ListIterator& operator++()
		{
			current = current->pNext;
			return *this;
		}
		ListIterator& operator--()
		{
			current = current->pPrev;
			return *this;
		}
		bool operator!=(const ListIterator& other) const
		{
			return current != other.current;
		}
	};

	template<typename T>
	T& List<T>::operator[](const int index)
	{
		if (index < 0 || index >= Size)
		{
			throw std::out_of_range("Index is out of range");
		}
		Node<T>* current = nullptr;
		if (index < Size / 2)
		{
			current = head;
			for (int counter = 0; counter < index; ++counter)
			{
				current = current->pNext;
			}
		}
		else
		{
			current = tail;
			for (int counter = Size - 1; counter > index; --counter)
			{
				current = current->pPrev;
			}
		}
		return current->data;
	}

	template<typename T>
	void List<T>::pushBack(T data)
	{
		if (head == nullptr)
		{
			head = new Node<T>(data);
			tail = head;
		}
		else
		{
			Node<T>* current = tail;
			current->pNext = new Node<T>(data, current, nullptr);
			tail = current->pNext;
		}
		Size++;
	}

	template<typename T>
	inline void List<T>::popFront()
	{
		if (head->pNext != nullptr)
		{
			Node<T>* temp = head;
			head = head->pNext;
			head->pPrev = nullptr;
			delete temp;
		}
		else
		{
			delete head;
			head = nullptr;
			tail = nullptr;
		}
		Size--;
	}

	template<typename T>
	inline void List<T>::clear()
	{
		while (Size)
		{
			popFront();
		}
	}

	template<typename T>
	void List<T>::pushFront(T data)
	{
		if (head == nullptr)
		{
			head = new Node<T>(data);
			tail = head;
		}
		else
		{
			head = new Node<T>(data, nullptr, head);
			head->pNext->pPrev = head;
		}
		Size++;
	}

	template<typename T>
	inline void List<T>::insert(iterator& it, T data)
	{
		if (it.current == tail)
		{
			pushBack(data);
		}
		else
		{
			Node<T>* temp = it.current->pNext;
			it.current->pNext = new Node<T>(data, it.current, temp);
			it.current = it.current->pNext;
			++Size;
		}
	}

	template<typename T>
	void List<T>::erase(List<T>::iterator& it)
	{
		if (it.current == head)
		{
			it.current = it.current->pNext;
			popFront();
		}
		else if (it.current == tail)
		{
			it.current = nullptr;
			popBack();
		}
		else
		{
			Node<T>* temp = it.current;
			it.current->pPrev->pNext = it.current->pNext;
			it.current->pNext->pPrev = it.current->pPrev;
			it.current = it.current->pNext;
			delete temp;
			--Size;
		}
	}

	template<typename T>
	void List<T>::popBack()
	{
		Node<T>* previous = (Node<T>*)searchPrevious(Size - 1);
		delete previous->pNext;
		previous->pNext = nullptr;
		Size--;
		if (Size == 0)
		{
			tail = nullptr;
			head = nullptr;
		}
	}

	template<typename T>
	void* List<T>::searchPrevious(int index)
	{
		Node<T>* previous = nullptr;
		if (index < Size / 2)
		{
			previous = this->head;
			for (int counter = 0; counter < index - 1; ++counter)
			{
				previous = previous->pNext;
			}
		}
		else
		{
			previous = this->tail;
			for (int counter = Size - 1; counter > index; --counter)
			{
				previous = previous->pPrev;
			}
		}
		return previous;
	}

}