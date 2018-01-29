#pragma once
#include"Dym_alloc.h"
#define max(a,b) (a>b?a:b)
template<typename Iter,typename T>
void uninitialized_fill(Iter start, Iter end, T& value) {
	for (Iter i = start; i != end; i++)
	{
		new (&*i) T(value);
	}
}

template<typename Iter>
void copy(Iter oStart, Iter oEnd, Iter start)
{
	for (Iter i = oStart; i != oEnd; i++, start++)
	{
		*start = *i;
	}
}

template<typename Iter>
void copy_backward(Iter oStart, Iter oEnd, Iter start)
{
	
	for (size_t i = oEnd - oStart-1;i>=0;i--)
	{
		*(start + i) = *(oStart + i);
	}
}
template<typename T>
struct Dequene_iterator {
	typedef Dequene_iterator<T> iterator;
	typedef T** map_pointer;
	typedef Dequene_iterator self;
	typedef int difference_type;
	T* cur;
	T* first;
	T* last;
	map_pointer node;
	Dequene_iterator() :
		cur(0),
		first(0),
		last(0) {}
	size_t bufferSize()
	{
		size_t size = sizeof(T);
		return size < 512 ? 512 / size : 1;
	}
	void setNewNode(map_pointer new_node)
	{
		node = new_node;
		first = *node;
		last = *node + difference_type(bufferSize());
	}
	T& operator*() const {
		return *cur;
	}
	T* operator->() const {
		return cur;
	}

	self& operator++()
	{
		if (++cur == last)
		{
			setNewNode(node + 1);
			cur = first;
		}
		return *this;
	}
	self& operator++(int) {
		self tmp = *this;
		++(*this);
		return tmp;
	}

	self& operator--()
	{
		if (cur == first)
		{
			setNewNode(node - 1);
			cur = last;
		}
		--cur;
		return *this;
	}
	self& operator--(int) {
		self tmp = *this;
		--(*this);
		return tmp;
	}
	self& operator+=(difference_type n)
	{
		difference_type offset = cur + n - first;
		if (offset >= 0 && difference_type(bufferSize()) > offset) {
			cur += n;
		}
		else {
			difference_type t = offset / bufferSize();
			if (offset > 0)
			{
				n -= (t - 1)*bufferSize() + (last - cur);
				setNewNode(node + t);
				cur = first + n;
			}
			else
			{
				t = 1 - t;
				n += (t - 1)*bufferSize() + (cur - first);
				setNewNode(node - t);
				cur = last + n;
			}
		}
		return *this;
	}
	self& operator-=(difference_type n)
	{
		return (*this) += (-n);
	}
	self& operator+(difference_type n)
	{
		self tmp = *this;
		tmp += n;
		return tmp;
	}
	self& operator-(difference_type n) // µü´úÆ÷Î»ÒÆ
	{
		return (*this) + (-n);
	}
	difference_type operator-(self& s) const {
		difference_type m_size = node - s.node;
		m_size = m_size*bufferSize() + (s.last - s.cur) - (last - cur);
		return m_size;
	}
	bool operator==(self& s)
	{
		return cur == s.cur;
	}
	bool operator!=(self& s)
	{
		return !((*this) == s);
	}
};

template<typename T>
class TDequene
{

public:
	typedef T value_type;
	typedef value_type* pointer;
	typedef value_type& ref;

	typedef Dequene_iterator<T> iterator;

	typedef Dym_alloc alloc;
protected:
	typedef pointer* map_pointer;

	
	map_pointer _map;
	size_t map_size;
	iterator start;
	iterator finish;

	size_t bufferSize()
	{
		size_t size = sizeof(T);
		return size < 512 ? 512 / size : 1;
	}
public:
	typedef Dequene_iterator<T> iterator;

	iterator begin() {
		return start;
	}
	iterator end() {
		return finish;
	}
	ref operator[](size_t id) {
		return *(start + id);
	}
	ref front() {
		return *start;
	}
	ref back() {
		return *(finish - 1);
	}
	size_t size()
	{
		return finish - start;
	}
	bool empty()
	{
		return finish == start;

	}
	void push_back(const value_type& value);
	void push_front(const value_type& value);
	void push_back_aux(const value_type& value);
	void push_front_aux(const value_type& value);
	void pop_back() {
		if (!empty())
		{
			finish--;
			destory(finish.cur);
			if (finish.cur == finish.last-1)
			{
				alloc::deallocate(*(finish.node+1), sizeof(T)*bufferSize());
			}
			
		}
	}
	void pop_front() {
		if (!empty())
		{
			destory(start.cur);
			if (start.cur == start.last - 1)
			{
				alloc::deallocate(*start.node, sizeof(T)*bufferSize());
			}
			start++;
			
		}
	}
public:
	TDequene() :
		map_size(0),
		_map(0),
		start(),
		finish() {
		fill_initialize(0, value_type()); ///////
	}
	void fill_initialize(size_t n,const value_type& value);
	void creatMapAndNode(size_t n);
	void check_map_at_back();
	void check_map_at_front();
	void rellocateMap(bool isfront,size_t need_node = 2);
	pointer creadNode();
};

template<typename T>
void TDequene<T>::fill_initialize(size_t n,const typename TDequene<T>::value_type& value)
{
	creatMapAndNode(n);
	uninitialized_fill(start, start + n, value);
}

template<typename T>
void TDequene<T>::creatMapAndNode(size_t n)
{
	size_t need_size = n / bufferSize() + 1;
	map_size = max(need_size + 2, 8);
	_map = static_cast<pointer*>(alloc::allocate(map_size * sizeof(pointer)));
	size_t id = (map_size - need_size) / 2;
	map_pointer new_start = _map + id;

	for (size_t i = 0;i<need_size;new_start++,i++)
	{
		*new_start = (T*)alloc::allocate(bufferSize() * sizeof(T));
	}

	
	start.setNewNode(_map + id);
	start.cur = start.first;
	finish = start + n;
}

template<typename T>
void TDequene<T>::check_map_at_back()
{
	if (_map + map_size <= finish.node + 1)
	{
		rellocateMap(false);
	}
}

template<typename T>
void TDequene<T>::check_map_at_front()
{
	if (_map >= start.node)
	{
		rellocateMap(true);
	}
}

template<typename T>
void TDequene<T>::push_back(const value_type & value)
{
	if (finish.last - 1 > finish.cur)
	{
		construct(finish.cur, value);
		finish++;
	}
	else
	{
		push_back_aux(value);
	}
}

template<typename T>
void TDequene<T>::push_front(const value_type & value)
{
	if (start.cur > start.first)
	{
		start--;
		construct(start.cur, value);
	}
	else
	{
		push_front_aux(value);
	}
}

template<typename T>
void TDequene<T>::push_back_aux(const value_type & value)
{
	check_map_at_back();
	*(finish.node + 1) = creadNode();
	construct(finish.cur, value);
	finish++;
}

template<typename T>
void TDequene<T>::push_front_aux(const value_type & value)
{
	check_map_at_front();
	*(start.node - 1) = creadNode();
	start--;
	construct(start.cur, value);
}

template<typename T>
void TDequene<T>::rellocateMap(bool isfront, size_t need_node)
{
	size_t old_num_node = finish.node - start.node + 1;
	size_t new_num_node = old_num_node + need_node;

	map_pointer new_start;
	if (new_num_node * 2 <= map_size)
	{
		new_start = _map + (map_size - new_num_node) / 2 + (isfront ? need_node : 0);
		if (new_start < start.node)
		{
			copy(start.node, finish.node + 1, new_start);
		}
		else
		{
			copy_backward(start.node, finish.node + 1, new_start);
		}
	}
	else {
		size_t new_map_size = map_size + max(map_size, need_node) + 2;

		map_pointer new_map = static_cast<map_pointer>(alloc::allocate(new_map_size * sizeof(pointer)));

		new_start = new_map + (new_map_size - new_num_node) / 2 + (isfront ? need_node : 0);
		copy(start.node, finish.node + 1, new_start);
		alloc::deallocate(_map, map_size * sizeof(pointer));
		map_size = new_map_size;
		_map = new_map;
		
	}
	start.setNewNode(new_start);
	finish.setNewNode(new_start + old_num_node - 1);
}


template<typename T>
typename TDequene<T>::pointer TDequene<T>::creadNode()
{
	return (T*)alloc::allocate(bufferSize()*sizeof(T));
}