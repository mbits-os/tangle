// Copyright (c) 2021 midnightBITS
// This code is licensed under MIT license (see LICENSE for details)

#pragma once

#if 0  // __cplusplus > 201703L

#include <span>

namespace tangle::browser {
	template <typename Item>
	using span = std::span<Item>;
}

#else  // __cplusplus > 201703L

namespace tangle::browser {
	template <typename Item>
	struct span {
		using pointer = Item*;
		using const_pointer = Item const*;
		using iterator = Item*;
		using const_iterator = Item const*;
		using reference = Item&;
		using const_reference = Item const&;

		span() = default;
		span(span const&) = default;
		span(span&&) = default;
		span& operator=(span const&) = default;
		span& operator=(span&&) = default;

		span(pointer ptr, size_t count) : data_{ptr}, size_{count} {}
		span(iterator from, iterator to)
		    : data_{from}, size_{static_cast<size_t>(to - from)} {}

		span(std::vector<Item>& container)
		    : data_{container.data()}, size_{container.size()} {}

		iterator begin() noexcept { return data_; }
		iterator end() noexcept { return data_ + size_; }
		const_iterator begin() const noexcept { return data_; }
		const_iterator end() const noexcept { return data_ + size_; }

		reference front() noexcept { return data_[0]; }
		reference back() noexcept { return data_[size_ - 1]; }
		const_reference front() const noexcept { return data_[0]; }
		const_reference back() const noexcept { return data_[size_ - 1]; }

		pointer data() noexcept { return data_; }
		const_pointer data() const noexcept { return data_; }

		size_t size() const noexcept { return size_; }
		bool empty() const noexcept { return size_ == 0; }

		span subspan(size_t offset, size_t count = -1) const noexcept {
			if (offset > size_) offset = size_;
			auto const avail = size_ - offset;
			if (avail < count) count = avail;
			return {data_ + offset, count};
		}

	private:
		pointer data_{};
		size_t size_{};
	};
}  // namespace tangle::browser

#endif  // __cplusplus > 201703L
