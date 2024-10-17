#pragma once
#include <regex>
#include <string>
#include <string_view>
#include <map>
#include <functional>
#include <exception>
#include "StringOperations.hpp"
#include <iostream>

namespace util
{
    // const std::string CommandNameRule = ("[A-Za-z0-9_йцукенгшщзхъфывапролджэячсмитьбюЙЦУКЕНГШЩЗХЪФЫВАПРОЛДЖЭЯЧСМИТЬБЮ]+");
    const std::string CommandNameRule = ("([A-Za-z0-9_]|(й)|(ц)|(у)|(к)|(е)|(н)|(г)|(ш)|(щ)|(з)|(х)|(ъ)|(ф)|(ы)|(в)|(а)|(п)|(р)|(о)|(л)|(д)|(ж)|(э)|(я)|(ч)|(с)|(м)|(и)|(т)|(ь)|(б)|(ю)|(Й)|(Ц)|(У)|(К)|(Е)|(Н)|(Г)|(Ш)|(Щ)|(З)|(Х)|(Ъ)|(Ф)|(Ы)|(В)|(А)|(П)|(Р)|(О)|(Л)|(Д)|(Ж)|(Э)|(Я)|(Ч)|(С)|(М)|(И)|(Т)|(Ь)|(Б)|(Ю))+");
    const std::regex CommandNamePattern = std::regex(CommandNameRule);
    const std::regex CommandPattern = std::regex(std::string("^\\!") + CommandNameRule);
        
    bool ValidCommand(const std::string& str){
        return std::regex_search(str, CommandNamePattern);
    }


    class CommandParser{
        public:

        // function that called when command entered
        // Return: void
        // Arg: const std::string& - message with command, that had been recieved 
        using CommandDelegate = std::function<void(const std::string&)>;

        CommandParser(){
            this->_unknownCommand = CommandParser::_empty_command;
        }

        // binds command with action
        // throws std::invalid_argument if command is invalid
        void Bind(const std::string& command, CommandDelegate&& action){
            if(!ValidCommand(command))
                throw std::invalid_argument(concatenate_params("CommandParser::Bind: invalid command: \"", command, "\""));
            if(!action)
                throw std::invalid_argument(concatenate_params("CommandParser::Bind: invalid action"));            
            
            this->_maps.insert({command, action});
        }

        // invoke function binded with 'command' key 
        void Call(const std::string& command, const std::string& message = ""){
            auto ptr = this->_maps.find(command);

            if(ptr != this->_maps.end())
`                ptr->second(message);
            else
                this->_unknownCommand(message);
        } 

        // removes command
        void Remove(const std::string& command){
            this->_maps.erase(command);
        }

        // extracts and runs command from message
        // if command not found throws std::invalid_argument
        void Run(const std::string message){
            std::string command;
            size_t off;
            if(!_extract_command(message, command, off)){
                throw std::invalid_argument(util::concatenate_params("CommandParser::Run: invalid command: \"", message, "\""));
            }

            std::string msg = "";
            if(message.size() > off)
                msg = message.substr(off);
            Call(command, msg);
        }

        // sets default functions 
        void SetDefaultDelegate(CommandDelegate&& fn){
            this->_unknownCommand = fn;
        }

        [[nodiscard]] auto begin() {return this->_maps.begin();}
        [[nodiscard]] auto begin() const {return this->_maps.begin();}

        [[nodiscard]] auto end() {return this->_maps.end();}
        [[nodiscard]] auto end() const {return this->_maps.end();}
        private:
        bool _extract_command(const std::string& line, std::string& out, size_t& end_pos){
            std::smatch match;

            if(!std::regex_search(line, match, CommandPattern))
                return false;

            out = line.substr(match.position()+1, match.length()-1);
            end_pos = match.position() + match.length()+1;
            return true;
        }
        static void _empty_command(const std::string& s) {}

        std::map<std::string, CommandDelegate> _maps;
        CommandDelegate _unknownCommand;
    };
}