#include<iostream>
#include<string>
#include<cassert>

#include<stdlib.h>

class Argparse {
    private:
        struct args {
            std::string param;
            std::string alt_param;
            std::string data;
            std::string help;
            std::string options;
        };
        int argc;
        int init_counter;
        int expected_count;
        int all_args_counter;
        struct args* all_args;
        std::string info;
    public:
        Argparse(int argc, int expected_count, std::string info_string);
        int allocArgs(int expected_count);
        int initArgs(std::string param, std::string alt_param,
                std::string help, std::string options = "");
        int setArgs(char* argv[]);
        std::string getArgs(std::string param);
        void printAllArgs();
        void printHelpArgs();
        bool insertArgs(std::string param, std::string data);
        std::string getArgOptions(std::string param);
};