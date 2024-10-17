#pragma once
#include <string>
#include <map>
#include <exception>
#include <sstream>
#include "../../Utility/StringOperations.hpp"

namespace core
{
    namespace dtb
    {
        class KeyValueDatabase
        {
            std::map<std::string, std::string> _values;

            static bool _validate(const std::string &str)
            {
                for (const auto i : str)
                    if (!(('0' <= i && i <= '9') || ('a' <= i && i <= 'z') || ('A' <= i && i <= 'Z') || i == '_' || i == '.' || i == '-'))
                        return false;
                return true;
            }

        public:
            std::string &operator[](const std::string &value)
            {
                if (!_validate(value))
                    throw std::invalid_argument(util::concatenate_params("kvdatabase ivalid key: \"", value, "\""));

                return this->_values[value];
            }
            const std::string &operator[](const std::string &value) const
            {
                return this->_values.at(value);
            }

            auto begin() const { return this->_values.begin(); }
            auto begin() { return this->_values.begin(); }

            auto end() const { return this->_values.end(); }
            auto end() { return this->_values.end(); }

            void erase(const std::string &key)
            {
                this->_values.erase(key);
            }

            std::string seralize() const
            {
                std::string s = "";
                for (const auto &[key, val] : this->_values)
                    s += util::concatenate_params('#', key, " ", val.size(), " ", val);
                
                return s;
            }
            void load(const std::string &s)
            {
                std::stringstream stream(s);

                while (!stream.eof())
                {
                    char c;
                    for (; c != '#' && !stream.eof(); stream >> c)
                        ;
                    if (stream.eof())
                        break;

                    std::string key;
                    stream >> key;

                    size_t size;
                    stream >> size;

                    stream >> c;

                    std::string value(size, 0);
                    stream.read(value.data(), size - 1);

                    this->_values.insert({key, value});
                }
            }

            void clear()
            {
                this->_values.clear();
            }
        };
    } // namespace db

} // namespace core
