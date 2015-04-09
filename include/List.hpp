/*********************************************************************
Matt Marchant 2013 - 2015
http://trederia.blogspot.com

This software is provided 'as-is', without any express or
implied warranty. In no event will the authors be held
liable for any damages arising from the use of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute
it freely, subject to the following restrictions:

1. The origin of this software must not be misrepresented;
you must not claim that you wrote the original software.
If you use this software in a product, an acknowledgment
in the product documentation would be appreciated but
is not required.

2. Altered source versions must be plainly marked as such,
and must not be misrepresented as being the original software.

3. This notice may not be removed or altered from any
source distribution.
*********************************************************************/

//creates a doubly linked listed of objects

#ifndef LIST_H_
#define LIST_H_

#include <EnableInheritance.hpp>

namespace data
{
	template <class T>
	class List final
	{
	public:
		List() : m_head(nullptr), m_tail(nullptr), m_size(0u){}
		~List()
		{
			while(m_head != nullptr)
			{
				popFront();
			}
		}

		List(const List& other) = delete;
		List operator =(const T& rhv) = delete;

		void PushFront(const T& val)
		{
			Node* newNode = new Node();
			newNode->val = new T(val);

			if(m_head)
			{
				m_head->prev = newNode;
				newNode->next = m_head;
				m_head = newNode;
			}
			else
			{
				//list is empty
				m_head = m_tail = newNode;
			}
			m_size++;
		}
		void PopFront()
		{
			if(m_size)
			{
				Node* old = m_head;
				if(m_head != m_tail)
				{
					m_head = m_head->next;
					m_head->prev = nullptr;
				}
				else
				{
					m_head = m_tail = nullptr;
				}

				delete old->val;
				delete old;
				m_size--;
			}
		}

		void PushBack(const T& val)
		{
			Node* newNode = new Node();
			newNode->val = new T(val);

			if(m_tail)
			{
				m_tail->next = newNode;
				newNode->prev = m_tail;
				m_tail = newNode;
			}
			else
			{
				m_head = m_tail = newNode;
			}
			m_size++;
		}
		void PopBack()
		{
			if(m_size)
			{
				Node* old = m_tail;
				if(m_tail != m_head)
				{
					m_tail = m_tail->prev;
					m_tail->next = nullptr;
				}
				else
				{
					m_tail = m_head = nullptr;
				}
				delete old->val;
				delete old;

				m_size--;
			}
		}

		unsigned Size() const {return m_size;}
		T* Front(){return (m_head) ? m_head->val : nullptr;}
		T* Back(){return (m_tail) ? m_tail->val : nullptr;}

	private:
		struct Node final
		{
			T* val;
			Node* prev;
			Node* next;
			Node() : val(nullptr), prev (nullptr), next(nullptr){}
		};

		Node* m_head;
		Node* m_tail;
		unsigned m_size;
	};
}


#endif /* LIST_H_ */