#pragma once

#include "../Core/App/AApp.hpp"
#include "../Utility/StringOperations.hpp"
#include "EchoApp.hpp"
#include "Core/Bot.hpp"
#include <vector>
#include <string>
#include <fstream>
#include <algorithm>
#include "Utility/StringOperations.hpp"
#include "Utility/CommandParser.hpp"
#include "Utility/RecourceLoader.hpp"
#include <random>
#include <map>


class RusAccentRecource: public util::ARecource {
    public:
    std::vector<std::string> recource;

    virtual ~RusAccentRecource(){}

    virtual void Load() {
        auto path = core::GetBot()->config->data_path + "rus.txt";
        std::ifstream fin(path);

        if(!fin)
            throw std::runtime_error(std::string("No recource file found: ") + path);

        int count;
        fin >> count;

        recource.resize(count);
        std::getline(fin, recource[0]);
        for (size_t i = 0; i < count; i++){
            std::getline(fin, recource[i]);
            recource[i] = util::strip(recource[i]);
        }
    }

    const std::string& operator[](int i) const{
        return recource[i];
    }
    auto size() const {
        return recource.size();
    }
    const std::string& GetRandom(std::mt19937& device){
        std::uniform_int_distribution<int> dist(0, size());

        return this->operator[](dist(device));
    }
};

namespace rus
{

    class RusAccent : public core::AApp
    {
        std::mt19937 _random;
        RusAccentRecource _recource;

        enum GameType{
            NONE,       // no game
            SINGLE,     // user should rewrite with corret vowel
            CHECK,      // correct or no
            RANDOM      // same as in exam
        } gt;

        std::string _answer = "", _question = "", _word = "";

        std::vector<std::pair<int, std::string>> _mistakes;
        int _mistake_count = 0;

        void _lower(std::string&s);
    public:
        RusAccent() : core::AApp(core::AApp::NullArg{}) {}
        RusAccent(bool) : core::AApp(core::AApp::NullArg{}) {}
        RusAccent(core::SessionAppData data);
        virtual ~RusAccent() {}

        virtual void OnMessage(const std::string &message);
        virtual core::appid_t AppId() { return StaticAppId(); }

        void Answer(const std::string& str);
        void NextQuestion();
        void ShowQuestion();
        std::string GetWord();

        void AddWordCount(const std::string& name, int delta = 1);

        static core::appid_t StaticAppId() { return "app.rus.accent"; }
    protected:
        virtual void OnLoad();
        virtual void OnClose();

    private:
        util::CommandParser _commands;
    };
}


rus::RusAccent::RusAccent(core::SessionAppData data) : core::AApp(core::AApp::NullArg{})
{
    this->Load(data);
}

void rus::RusAccent::OnMessage(const std::string &message)
{
    if(message[0] == '!') {
        this->_commands.Run(message);
    } else {
        if(this->gt == rus::RusAccent::NONE)
            this->SendMessage("Не знаю, что сказать :/");
        else
            this->Answer(util::strip(message));
    }
}

void rus::RusAccent::OnLoad()
{
    using namespace std;
    
    this->_random = std::mt19937(std::random_device()());

    this->_recource = util::Recources.Get<RusAccentRecource>();
    
    this->_commands.Bind("помощь", [this](const std::string& s){
        std::string message = "Команды: \n"
                            "\t!главная\t- запуск стартового прилжения\n"
                            "\t!о      \t- вывод ответа на вопрос\n"
                            "\t!помощь \t- вывод помощи\n"
                            "\t!список \t- вывод всех слов\n"
                            "\t!игра [=1/2/0] - выбор режима игры\n"
                            "\t   1 - вы должны переписать слово, с правильно выделенной ударной гласной\n"
                            "\t   2 - правильно или нет (ответы да/нет)\n";
                            // "\t   3 - как на егэ\n"
                            "\t   0 - нет игры\n"
                            "\t!вопрос \t- показать ответ\n";

        this->SendMessage(message);
    });
    this->_commands.Bind("о", [this](const std::string& s){
        this->Answer(s);
    });
    this->_commands.Bind("список", [this](const std::string& s){
        string ans = "";

        for(auto &word: this->_recource.recource){
            ans += word;
            ans += '\n';
        }

        this->SendMessage(ans);
    });
    this->_commands.Bind("игра", [this](const std::string& s1){
        auto s = util::strip_alpha(s1);
        if(s == "1" or s == "")
            this->gt = rus::RusAccent::SINGLE;
        else if(s == "2")
            this->gt = rus::RusAccent::CHECK;
        else if(s == "0")
            this->gt = rus::RusAccent::NONE;
        // else if(s == "3")
        //     this->gt = rus::RusAccent::RANDOM;

        this->NextQuestion();
        this->ShowQuestion();

    });

    this->_commands.Bind("вопрос", [this](const std::string& s){this->ShowQuestion();});
    this->_commands.Bind("главная", [this](const std::string& s){
        this->SendMessage("Запуск приложения...");
        this->SetContext(AApp::AppSwitchContext("app.rus.select"));
    });

    this->_commands.SetDefaultDelegate([this](const std::string& s){
        this->SendMessage("Неизвестная команда, для вывода помощи напиши !помощь");
    });
     
    this->SendMessage("Добро пожаловать в тренировку ударений, для вывода списка команд пиши: !помощь");
}
void rus::RusAccent::OnClose()
{
}
void rus::RusAccent::ShowQuestion(){
    this->SendMessage(util::concatenate_params("Вопрос: \n", this->_question));
}

void rus::RusAccent::NextQuestion(){
    switch (this->gt)
    {
    case rus::RusAccent::NONE:
        this->_question = this->_word = this->_answer = "";
        break;
    case rus::RusAccent::SINGLE:
        this->_question = this->_word = this->_answer = this->GetWord();
        this->_lower(this->_question);
        break;
    case rus::RusAccent::CHECK:
        {
            bool correct = this->_random()%2;

            if(correct==0){
                this->_answer = "да";
                this-> _word = this->_question = this->GetWord();
            }else{
                this->_answer = "нет";
                this-> _word = this->_question = this->GetWord();

                auto q = this->_question;

                const std::string glasnie[] = {
                    "я","и","ю","ы","а","о","э","у","е","ё"
                };
                const std::string hi_glasnie[] = {
                    "Я","И","Ю","Ы","А","О","Э","У","Е","Ё"
                };


                do{
                    this->_lower(q);
                    
                    std::vector<std::pair<int, int>> v;
                    size_t i = 0;
                    size_t lpar_beg = q.find('(');

                    for(auto& line: glasnie){
                        size_t begin = q.find(line);
                        while (begin != std::string::npos && begin < lpar_beg)
                        {
                            v.push_back({begin, i});
                            begin = q.find(line, begin+1);
                        }
                        i++;
                    }
                    std::sort(v.begin(), v.end());

                    std::uniform_int_distribution<int> dist(0, v.size());

                    int id = dist(this->_random);
                    q.replace(v[id].first, glasnie[v[id].second].length(), hi_glasnie[v[id].second]);
                } while (this->_question != q);

                this->_question = q;
            }
        }
        break;
    case rus::RusAccent::RANDOM:
        {
            
        }
        break;   
    default:
        break;
    }

    if(this->_word.find('(')!=std::string::npos)
        this->_word.erase(this->_word.find('('));
}
void rus::RusAccent::Answer(const std::string& str){
    bool correct = 0;
    switch (this->gt)
    {
    case rus::RusAccent::NONE:
        this->Answer("Не было вопросов.");
        return;
        break;
    case rus::RusAccent::SINGLE:
        correct = util::strip_alpha(str) == this->_answer;
        break;
    case rus::RusAccent::CHECK:
        correct = util::strip_alpha(str) == this->_answer;
        break;
    case rus::RusAccent::RANDOM:
        {
            auto line = util::strip_alpha(str);
            std::sort(line.begin(), line.end());

            correct = line == this->_answer;
        }
        break;   
    default:
        break;
    }

    if(correct){
        this->SendMessage("*Правильно\\!*", 1);

        this->AddWordCount(this->_word, -1);
    }
    else{
        if(this->gt == rus::RusAccent::SINGLE)
            this->SendMessage(util::concatenate_params("*Неправильно\\!* Правильный ответ: ||", this->_answer, "||"), 1);
        else
            this->SendMessage(util::concatenate_params("*Неправильно\\!* Правильный ответ: ||", this->_answer, " - ", this->_word , "||"), 1);
        this->AddWordCount(this->_word, 1);
    }
    this->NextQuestion();
    this->ShowQuestion();
}
void rus::RusAccent::_lower(std::string& str){
    static const std::vector<std::pair<std::string, std::string>> maps = { // std::array dont work \:
    {"А", "а"},    {"Б", "б"},    {"В", "в"},    {"Г", "г"},    {"Д", "д"},    {"Е", "е"},    {"Ё", "ё"},    {"Ж", "ж"},
    {"З", "з"},    {"И", "и"},    {"Й", "й"},    {"К", "к"},    {"Л", "л"},    {"М", "м"},    {"Н", "н"},    {"О", "о"},
    {"П", "п"},    {"Р", "р"},    {"С", "с"},    {"Т", "т"},    {"У", "у"},    {"Ф", "ф"},    {"Х", "х"},    {"Ц", "ц"},
    {"Ч", "ч"},    {"Ш", "ш"},    {"Щ", "щ"},    {"Ъ", "ъ"},    {"Ы", "ы"},    {"Ь", "ь"},    {"Э", "э"},    {"Ю", "ю"},    {"Я", "я"}};

    for(auto&[hi, lo] : maps)
        util::replaceAll(str, hi, lo);
}
void rus::RusAccent::AddWordCount(const std::string& name, int delta){
    auto it = std::find_if(this->_mistakes.begin(), this->_mistakes.end(), [&](auto v){
        return v.second == name;
    });

    if(it == this->_mistakes.end()){
        this->_mistakes.push_back({std::max(-5, delta), name});
        if(delta > 0)
            this->_mistake_count++;
    }else{
        it->first = std::max(delta + it->first, -5);
        if(it->first > 0 && it->first - delta <= 0)
            _mistake_count++;
        if(it->first < 0 && it->first - delta >= 0)
            _mistake_count--;
    }

    std::sort(this->_mistakes.begin(), this->_mistakes.end(), [](auto a, auto b){ return a.first < b.first; });
}
std::string rus::RusAccent::GetWord(){
    if(this->_random()%100 < 30 && this->_mistakes.size() > 0 && this->_mistake_count > 0)
        return this->_mistakes.back().second;
    return this->_recource.GetRandom(this->_random);
}