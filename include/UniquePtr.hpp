/*********************************************************************
Matt Marchant 2013 - 2014
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

#ifndef UNIQUEPTR_H_
#define UNIQUEPTR_H_

#include <stdlib.h>

namespace data
{
template <typename T> class unique_ptr final
{
public:
	unique_ptr(T* ptr = nullptr) : m_ptr(ptr){};
	unique_ptr(unique_ptr<T>&& other)
	{
		m_ptr = other.m_ptr;	
		other.m_ptr = nullptr;
	};
	
	unique_ptr<T>& operator = (unique_ptr<T>&& other)
	{
		if(other.m_ptr == m_ptr) return *this;
		
		m_ptr = other.m_ptr;
		other.m_ptr = nullptr;
		return *this;
	};
		
	~unique_ptr(){free(m_ptr);};

	T* get(){return m_ptr;}
	void reset(T* ptr = nullptr)
	{
		free(m_ptr);
		m_ptr = ptr;
	}

	//operator overloads
	T& operator* () {return *m_ptr;};
	T* operator-> () {return m_ptr;};
private:
	unique_ptr(const unique_ptr<T>& other){};
	unique_ptr<T> operator = (const unique_ptr<T>& lhv){};
	T* m_ptr;
};

} //namespace data

#endif /* UNIQUEPTR_H_ */