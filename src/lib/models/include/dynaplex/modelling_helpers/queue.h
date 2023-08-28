#pragma once
#include<vector>
#include <limits>
#include <iterator>
#include "dynaplex/error.h"
#include "dynaplex/vargroup.h"




namespace DynaPlex {
    template<typename T>
	class Queue
	{
		static_assert(DynaPlex::Concepts::DP_ElementType<T>, " DynaPlex::Queue<T> - T must be of DP_ElementType, i.e. double, int64_t, string, or DynaPlex::VarGroupConvertible.");

	public: 

		using value_type = T;
		using size_type = std::size_t;
		using reference = value_type&;
		using const_reference = const value_type&;
		using pointer = value_type*;
		using const_pointer = const value_type*;
	private:


		size_t first_item;
		size_t num_items;
		std::vector<T> items;

		size_t GetVectorIndex(const size_t& unlooped_index) const {
			if (unlooped_index >= items.size()) {
				return unlooped_index - items.size();
			}
			return unlooped_index;
		}

	public:
		size_t Capacity() const {
			return items.size();
		}



		class const_iterator {
		private:
			size_t current;
			const Queue* queue_ptr;
		public:
			using iterator_category = std::forward_iterator_tag;
			using value_type = T;
			using difference_type = std::ptrdiff_t;
			using pointer = value_type*;
			using reference = value_type&;
			using const_pointer = const value_type*;

			const_iterator(size_t current, const Queue* queue_ptr)
				: current(current), queue_ptr(queue_ptr) {}

			bool operator!=(const_iterator& rhs) { return current != rhs.current; }
			bool operator==(const_iterator& rhs) const { return current == rhs.current && queue_ptr == rhs.queue_ptr; }
			const T& operator*() {
				return queue_ptr->items[queue_ptr->GetVectorIndex(current)];
			}
			void operator++() {
				++current;
			}
		};

		const_iterator begin() const {
			return const_iterator(first_item, this);
		}

		const_iterator end() const {
			return const_iterator(first_item + num_items, this);
		}


		Queue(size_t n)
			: first_item{ 0 }, num_items{ n }, items(n, 0) {

		}


		Queue()
			: first_item{ 0 }, num_items{ 0 }, items(0) {}
		Queue(size_t n, const T value)
			: first_item{ 0 }, num_items{ n }, items(n, value) {}

		Queue(const Queue& other) = default;

		Queue(std::initializer_list<T> init)
			: first_item{ 0 }, num_items{ init.size() }, items(init) {}

		Queue(Queue&& other) noexcept
			: first_item{ other.first_item }, num_items{ other.num_items }, items(std::move(other.items)) {
			other.num_items = 0;
		}
		void push_back(T item) {

			if (num_items == items.size()) {
				//Expand the vector
				size_t new_capacity = (items.size() == 0) ? 4 : items.size() * 2;
				std::vector<T> new_items(new_capacity);
				int i = 0;
				auto stop = end();
				for (auto it = begin(); it != stop; ++it) {
					new_items[i++] = *it;
				}
				first_item = 0;
				items = std::move(new_items);
			}
			items[GetVectorIndex(first_item + num_items++)] = item;
		}

		T& back() {
			if (IsEmpty()) {
				throw DynaPlex::Error("Queue: queue is empty");
			}
			return items[GetVectorIndex(first_item + num_items - 1)];
		}

		const T& back() const {
			if (IsEmpty()) {
				throw DynaPlex::Error("Queue: queue is empty");
			}
			return items[GetVectorIndex(first_item + num_items - 1)];
		}

		bool IsEmpty() const {
			return num_items == 0;
		}

		T pop_front() {
			if (IsEmpty()) {
				throw DynaPlex::Error("Queue: queue is empty");
			}
			T front = items[first_item];
			items[first_item++] = 0;
			num_items--;
			if (first_item == items.size()) {
				first_item = 0;
			}
			return front;
		}

		T& front() {
			if (IsEmpty()) {
				throw DynaPlex::Error("Queue: queue is empty");
			}
			return items[first_item];
		}

		const T& front() const {
			if (IsEmpty()) {
				throw DynaPlex::Error("Queue: queue is empty");
			}
			return items[first_item];
		}

		T sum()
		{
			T sum = 0;
			auto stop = end();
			for (auto it = begin(); it != stop; ++it) {
				sum += *it;
			}
			return sum;
		}

		void clear()
		{
			items.clear();
			first_item = 0;
			num_items = 0;
		}

		friend bool operator==(const Queue<T>& lhs, const Queue<T>& rhs) {
			if (lhs.num_items != rhs.num_items) {
				return false;
			}
			auto lhsIter = lhs.begin();
			auto rhsIter = rhs.begin();
			auto lhsEnd = lhs.end();
			while (lhsIter != lhsEnd) {
				if (*(lhsIter) != *(rhsIter)) {
					return false;
				}
				++lhsIter;
				++rhsIter;
			}
			return true;
		}

		friend bool operator!=(const Queue<T>& lhs, const Queue<T>& rhs) {
			return !(lhs == rhs);
		}

	};

}
