/*
 * Copyright 2026 Pie Laboratories
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef CORE_GENERATOR_DOT_HPP
#define CORE_GENERATOR_DOT_HPP

#include <coroutine>
#include <exception>
#include <utility>
#include <iterator>
#include <optional>

namespace Core {
    template <typename T>
    class Generator
    {
    public:
        struct promise_type
        {
            std::optional<T>   m_value {};
            std::exception_ptr m_exception;
        
            Generator get_return_object()
            {
                return Generator { std::coroutine_handle<promise_type>::from_promise(*this) };
            }
            std::suspend_always initial_suspend() noexcept { return {}; }
            std::suspend_always final_suspend() noexcept   { return {}; }
            std::suspend_always yield_value(T value) noexcept
            {
                m_value = std::move(value);
                return {};
            }
            void return_void() noexcept {}
            void unhandled_exception() { m_exception = std::current_exception(); }
        };

        Generator() noexcept = default;
        explicit Generator(std::coroutine_handle<promise_type> handle) : m_handle(handle) {}
        Generator(const Generator &) = delete;
        Generator &operator=(const Generator &) = delete;
        Generator(Generator &&move) noexcept : m_handle(std::exchange(move.m_handle, {})) {}
        Generator &operator=(Generator &&move) noexcept
        {
            if (this != &move)
            {
                if (m_handle) m_handle.destroy();
                m_handle = std::exchange(move.m_handle, {});
            }
            return *this;
        }
        ~Generator() { if (m_handle) m_handle.destroy(); }

        class iterator
        {
        public:
            using iterator_category = std::input_iterator_tag;
            using value_type        = T;
            using difference_type   = std::ptrdiff_t;

            iterator() noexcept = default;
            explicit iterator(std::coroutine_handle<promise_type> handle) : m_handle(handle) {}

            iterator &operator++()
            {
                m_handle.resume();
                if (m_handle.done())
                {
                    auto exception = m_handle.promise().m_exception;
                    m_handle = {};
                    if (exception) std::rethrow_exception(exception);
                }
                return *this;
            }
            void operator++(int) { ++*this; }
            T &operator*() const { return m_handle.promise().m_value.value(); }
            bool operator==(std::default_sentinel_t) const { return !m_handle || m_handle.done(); }

        private:
            std::coroutine_handle<promise_type> m_handle {};
        };

        iterator begin()
        {
            if (!m_handle) return iterator {};
            m_handle.resume();
            if (m_handle.done())
            {
                if (m_handle.promise().m_exception)
                    std::rethrow_exception(m_handle.promise().m_exception);
                return iterator {};
            }
            return iterator { m_handle };
        }
        std::default_sentinel_t end() const noexcept { return {}; }

    private:
        std::coroutine_handle<promise_type> m_handle {};
    };
}
#endif  /* CORE_GENERATOR_DOT_HPP */
