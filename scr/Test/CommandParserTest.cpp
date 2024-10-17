#include "Utility/TestProc.hpp"
#include "Utility/CommandParser.hpp"
#include <vector>
#include <random>
#include <algorithm>

using namespace std;

const string alph = "12356890_qwertyuiopasdfghjklzxcvbnmQWETYOPASFGHJKLZXCVBNM";
const string ualph = "йцукенгшщзхъфывапролджэячсмитьбюЙЦУКШЩЗХЪФЫВАПРОЛДЖЭЯЧСМИТЬБЮ";

mt19937 rnd;
uniform_int_distribution<int> dist(0, alph.length() + ualph.length()-1);

string get_random(int comm_len){
    string s = "";

        for (int j = 0; j < comm_len; j++)
        {
            int r = dist(rnd);

            if(r < alph.length()){
                s.push_back(alph[r]);
            }else{
                r -= alph.length();
                r -= r % 2;

                s.push_back(ualph[r]);
                s.push_back(ualph[r+1]);
            }
        }
    return s;
}

void Test(){
    util::CommandParser parser;

    vector<string> comms {
        "QWUOPASZXCVBNML",
        "qwertyuiopasdfghjklzxcvbnm",
        "1234567890", 
        "йцукенгшщзхъэжддлорпавыфячсмитьбю",
        "ЙЦУКЕНГШЩЗХЪФЫВАПРОЛДЖЭЯЧСМТЬБЮ",
        "2vij94nvr92843",
        "_"
    };

    const int comm_len = 10;

    for (int i = 0; i < 1000; i++)
    {
        auto s = get_random(comm_len);

        comms.push_back(s);
    }
    
    sort(comms.begin(), comms.end());
    comms.erase(unique(comms.begin(), comms.end()), comms.end());
     
    int called = -2;

    for (size_t i = 0; i < comms.size(); i++)
    {
        parser.Bind(comms[i], [=, &called](const string& str){
            called = i;
        });
    }

    parser.SetDefaultDelegate([&](const string& str){
        called = -1;
    });

    for (size_t i = 0; i < comms.size(); i++)
    {
        called = -2;

        auto s = get_random(comm_len+1);
        parser.Run(std::string("!") + s);
        if (called!=-1)
            throw std::logic_error("Error at auto s = get_random(comm_len+1); called=" + std::to_string(called));
        
        called = -2;

        s = get_random(comm_len-1);
        parser.Run(std::string("!") + s);
        if (called!=-1)
            throw std::logic_error("Error at auto s = get_random(comm_len-1); called=" + std::to_string(called));

        called = -2;

        s = comms[i];
        parser.Run(std::string("!") + s);
        if (called!=i)
            throw std::logic_error(string("Cant find \"") + s + "\" called=" + std::to_string(called));

        
        called = -2;

        s = comms[i] + " " + get_random(comm_len-1);
        parser.Run(std::string("!") + s);
        if (called!=i)
            throw std::logic_error(string("Cant find \"") + s + "\" called=" + std::to_string(called));
    }
    
}

int main(){
    testutil::TestException("command-parser", Test);

    return 0;
}