/*
 * Copyright (C) 2017 midnightBITS
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use, copy,
 * modify, merge, publish, distribute, sublicense, and/or sell copies
 * of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include <tangle/nav/loader.h>
#include <tangle/nav/impl/loader_impl.h>

namespace tangle { namespace nav {
	loader::loader(std::shared_ptr<loader_impl> impl)
		: m_impl(impl)
	{
	}

	loader::loader() = default;
	loader::loader(const loader&) = default;
	loader& loader::operator=(const loader&) = default;
	loader::loader(loader&&) = default;
	loader& loader::operator=(loader&&) = default;

	loader& loader::on_opened(const std::function<bool(loader&)>&)
	{
		return *this;
	}
	loader& loader::on_data(const std::function<void(loader&, const void*, size_t)>&)
	{
		return *this;
	}

	bool loader::exists() const
	{
		if (!m_impl)
			return false;
		return m_impl->exists();
	}

	bool loader::is_link() const
	{
		if (!m_impl)
			return false;
		return m_impl->is_link();
	}
}}
