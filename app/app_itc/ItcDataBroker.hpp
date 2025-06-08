// file: ItcDataBroker.hpp
#pragma once

#include <any> // 用于存储任意类型的数据
#include <map>
#include <mutex>
#include <optional> // 用于安全地返回可能不存在的数据
#include <stdexcept> // 用于异常处理
#include <string>

namespace itc {

    class ItcDataBroker {
    private:
        // 使用 std::map 存储数据，key是主题名，value是任意类型的数据
        std::map<std::string, std::any> m_data_map;
        // 使用互斥锁来保护map的读写，确保线程安全
        mutable std::mutex m_mutex;

        ItcDataBroker() = default; // 私有构造函数

    public:
        // 禁止拷贝和赋值
        ItcDataBroker(const ItcDataBroker &) = delete;
        ItcDataBroker &operator=(const ItcDataBroker &) = delete;

        /**
         * @brief 获取数据代理的唯一实例（单例模式）。
         */
        static ItcDataBroker &instance() {
            static ItcDataBroker broker;
            return broker;
        }

        /**
         * @brief 生产者线程调用此函数来设置或更新一个主题的数据。
         * @tparam T 数据的类型 (e.g., std::string, int).
         * @param topic 主题的唯一名称 (e.g., "keypad_last_key").
         * @param value 要设置的值。
         */
        template<typename T>
        void set(const std::string &topic, T value) {
            std::lock_guard<std::mutex> lock(m_mutex);
            m_data_map[topic] = std::move(value);
        }

        /**
         * @brief 消费者（UI）线程调用此函数来获取一个主题的最新数据。
         * @tparam T 期望获取的数据类型。
         * @param topic 主题的名称。
         * @return std::optional<T> 如果主题存在且类型匹配，则包含数据；否则为空。
         */
        template<typename T>
        std::optional<T> get(const std::string &topic) {
            std::lock_guard<std::mutex> lock(m_mutex);

            auto it = m_data_map.find(topic);
            if (it == m_data_map.end()) {
                return std::nullopt; // 主题不存在
            }

            try {
                // 尝试将 std::any 转换为期望的类型 T
                return std::any_cast<T>(it->second);
            } catch (const std::bad_any_cast &e) {
                // 类型不匹配，这也是一种错误情况
                return std::nullopt;
            }
        }

        /**
         * @brief 从数据映射中移除一个主题。
         */
        void remove(const std::string &topic) {
            std::lock_guard<std::mutex> lock(m_mutex);
            m_data_map.erase(topic);
        }
    };

} // namespace itc
