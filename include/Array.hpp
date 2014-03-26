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


#ifndef ARRAY_H_
#define ARRAY_H_

#include <stdlib.h>
#include <assert.h>
#include <avr/io.h>

namespace data
{
	
template <class T, uint32_t>
class Array final
{
public:
	//-------------------------------
	// Constructors
	//-------------------------------
	Array(T, uint32_t size)
	: m_pointer	(static_cast<T*>(malloc(size * sizeof(T)))),
	m_size		(size)
	{
		for(auto i = 0u; i < m_size; ++i)
			m_pointer[i]();
	}
	//copy
	Array(const Array& a)
	: m_pointer (static_cast<T*>(malloc((a.m_size * sizeof(T))))),
	m_size		(a.m_size)
	{
		for(auto i = 0u; i < a.m_size; ++i)
			m_pointer[i] = a[i];
	}
	//move
	Array(Array&& a)
	: m_pointer	(nullptr),
	m_size		(0u)
	{
		m_pointer = a.m_pointer;
		m_size = a.m_size;
		a.m_size = 0u;
		a.m_pointer = nullptr;
	}
	//TODO construct from braced initialiser list
	//-------------------------------
	~Array()
	{
		free(m_pointer);
	}
	//-------------------------------
	// Index operator
	//-------------------------------
	T& operator [](const uint32_t index)
	{
		assert(index < m_size);
		return *(m_pointer + index);
	}
	
	const T& operator [](const uint32_t index) const
	{
		assert(index < m_size);
		return *(m_pointer + index);
	}
	//-------------------------------
	// Assignment operator
	//-------------------------------
	//copy
	Array& operator = (const Array& a)
	{
		if(this != &a)
		{
			free(m_pointer);
			m_size = a.m_size;
			m_pointer = malloc(m_size * sizeof(T));
			
			for(auto i = 0u; i < m_size; ++i)
				m_pointer[i] = a[i];
		}
		return *this;
	}
	//move
	Array& operator = (Array&& a)
	{
		if(this != &a)
		{
			free(m_pointer);
			m_size = a.m_size;
			m_pointer = a.m_pointer;
			
			a.m_size = 0;
			m_pointer = nullptr;			
		}
		return *this;
	}
	
	//------------------------------
	uint32_t Size() const
	{
		return m_size;
	}
	
private:

	T* m_pointer;
	uint32_t m_size;
};
} //namespace data
#endif /* ARRAY_H_ */