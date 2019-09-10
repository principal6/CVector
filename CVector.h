#pragma once

#include <iostream>
#include <initializer_list>
#include <cassert>

#define SAFE_DELETE_ARRAY(arr) if (arr) { delete[] arr; arr = nullptr; }

using DataType = int;

class CVector
{
public:
	CVector() {};
	CVector(std::initializer_list<DataType> il)
	{
		for (auto& i : il)
		{
			push_back(i);
		}
	};
	CVector(const CVector& b)
	{
		*this = b;
	};
	CVector(CVector&& b) noexcept
	{
		*this = std::move(b);
	};
	~CVector() { SAFE_DELETE_ARRAY(m_Data); };

	CVector& operator=(const CVector& b)
	{
		reserve(b.m_Capacity);
		memcpy(m_Data, b.m_Data, sizeof(DataType) * m_Capacity);

		resize(b.m_Size);
		shrink_to_fit();

		return *this;
	}

	CVector& operator=(CVector&& b) noexcept
	{
		m_Data = b.m_Data;
		m_Capacity = b.m_Capacity;
		m_Size = b.m_Size;

		b.m_Data = nullptr;
		b.m_Capacity = 0;
		b.m_Size = 0;

		return *this;
	}

public:
	void clear() noexcept
	{
		while (!empty())
		{
			pop_back();
		}
	}

	void push_back(DataType Value)
	{
		if (empty()) reserve(1);
		if (m_Size == m_Capacity) reserve(m_Capacity * 2);

		m_Data[m_Size] = Value;
		++m_Size;
	}

	void pop_back() noexcept
	{
		if (empty()) return;

		--m_Size;
	}

	void reserve(size_t new_capacity)
	{
		if (m_Capacity >= new_capacity) return;

		DataType* temp{ new DataType[m_Capacity] };
		memcpy(temp, m_Data, sizeof(DataType) * m_Capacity);

		SAFE_DELETE_ARRAY(m_Data);

		m_Data = new DataType[new_capacity]{};
		memcpy(m_Data, temp, sizeof(DataType) * m_Capacity);

		SAFE_DELETE_ARRAY(temp);

		m_Capacity = new_capacity;
	}

	void resize(size_t new_size)
	{
		if (new_size > m_Capacity)
		{
			reserve(new_size);
		}

		m_Size = new_size;
	}

	void shrink_to_fit()
	{
		if (m_Capacity == m_Size) return;

		DataType* temp{ new DataType[m_Size] };
		memcpy(temp, m_Data, sizeof(DataType) * m_Size);

		SAFE_DELETE_ARRAY(m_Data);

		m_Data = new DataType[m_Size]{};
		memcpy(m_Data, temp, sizeof(DataType) * m_Size);

		SAFE_DELETE_ARRAY(temp);

		m_Capacity = m_Size;
	}

	bool empty() const noexcept { return (m_Size == 0) ? true : false; }
	size_t size() const noexcept { return m_Size; }
	size_t capacity() const noexcept { return m_Capacity; }

public:
	DataType& operator[](size_t Index)
	{
		return m_Data[Index];
	}

	DataType& at(size_t Index)
	{
		if (Index >= m_Size)
		{
			throw std::out_of_range("Script out of range.");
		}

		return m_Data[Index];
	}

	DataType& front()
	{
		assert(m_Size);
		return m_Data[0];
	}

	DataType& back()
	{
		assert(m_Size);
		return m_Data[m_Size - 1];
	}

private:
	DataType* m_Data{};
	size_t m_Size{};
	size_t m_Capacity{};
};

using vector = CVector;