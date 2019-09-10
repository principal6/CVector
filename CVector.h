#pragma once

#include <iostream>
#include <initializer_list>
#include <cassert>

#define SAFE_DELETE_ARRAY(arr) if (arr) { delete[] arr; arr = nullptr; }

using DataType = int;

class CVector
{
	class CVectorBaseIterator
	{
	public:
		bool operator==(const CVectorBaseIterator& b) const { return (m_At == b.m_At) ? true : false; }
		bool operator!=(const CVectorBaseIterator& b) const { return (m_At != b.m_At) ? true : false; }

	protected:
		CVectorBaseIterator(DataType* At) : m_At{ At } {}
		virtual ~CVectorBaseIterator() {}
		virtual CVectorBaseIterator& operator++() = 0;

	protected:
		DataType* m_At{};
	};

	class CVectorIterator final : public CVectorBaseIterator
	{
	public:
		CVectorIterator(DataType* At) : CVectorBaseIterator(At) {}
		virtual ~CVectorIterator() {}

		CVectorIterator& operator++() override { ++m_At; return *this; }
		DataType& operator*() { return *m_At; }
	};

	class CVectorReverseIterator final : public CVectorBaseIterator
	{
	public:
		CVectorReverseIterator(DataType* At) : CVectorBaseIterator(At) {}
		virtual ~CVectorReverseIterator() {}

		CVectorReverseIterator& operator++() override { --m_At; return *this; }
		DataType& operator*() { return *m_At; }
	};

	class CVectorConstIterator final : public CVectorBaseIterator
	{
	public:
		CVectorConstIterator(DataType* At) : CVectorBaseIterator(At) {}
		virtual ~CVectorConstIterator() {}

		CVectorConstIterator& operator++() override { ++m_At; return *this; }
		const DataType& operator*() const { return *m_At; }
	};

	class CVectorConstReverseIterator final : public CVectorBaseIterator
	{
	public:
		CVectorConstReverseIterator(DataType* At) : CVectorBaseIterator(At) {}
		virtual ~CVectorConstReverseIterator() {}

		CVectorConstReverseIterator& operator++() override { --m_At; return *this; }
		const DataType& operator*() const { return *m_At; }
	};

	using iterator_base = CVectorBaseIterator;
	using iterator = CVectorIterator;
	using reverse_iterator = CVectorReverseIterator;
	using const_iterator = CVectorConstIterator;
	using const_reverse_iterator = CVectorConstReverseIterator;

public:
	CVector() {}
	CVector(std::initializer_list<DataType> il)
	{
		for (auto& i : il)
		{
			push_back(i);
		}
	}
	template <class Iter>
	CVector(const Iter& begin, const Iter& end)
	{
		for (auto it = begin; it != end; ++it)
		{
			push_back(*it);
		}
	}
	CVector(const CVector& b)
	{
		*this = b;
	}
	CVector(CVector&& b) noexcept
	{
		*this = std::move(b);
	}
	~CVector() { SAFE_DELETE_ARRAY(m_Data); }

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

public:
	iterator begin() { return iterator(&m_Data[0]); }
	iterator end() { return iterator(&m_Data[m_Size]); }
	reverse_iterator rbegin() { return reverse_iterator(&m_Data[m_Size - 1]); }
	reverse_iterator rend() { return reverse_iterator(&m_Data[-1]); }
	const_iterator cbegin() { return const_iterator(&m_Data[0]); }
	const_iterator cend() { return const_iterator(&m_Data[m_Size]); }
	const_reverse_iterator crbegin() { return const_reverse_iterator(&m_Data[m_Size - 1]); }
	const_reverse_iterator crend() { return const_reverse_iterator(&m_Data[-1]); }

private:
	DataType* m_Data{};
	size_t m_Size{};
	size_t m_Capacity{};
};

using vector = CVector;