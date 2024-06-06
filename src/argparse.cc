#include "argparse.hh"

Argparse::Argparse(int argc, int expected_count, std::string info_string) {
    
    // argc is the number of arguments that the user has provided.
    this->argc = argc;

    // this counter is maintained to iterate over the arguments once during the
    // initialization phase.
    this->init_counter = 0;

    // we need this value to allocate memory to all the parser arguments.
    this->expected_count = expected_count;

    // information of the program has to be passed as an init string.
    this->info = info_string;
}

void Argparse::printHelpArgs() {
    std::cout << this->info << "\n" << std::endl;
    std::cout << "\t This program understands the following arguments:"
            << std::endl;
    
    // assert(this->init_counter >= 0 &&
    //         this->init_counter < this->expected_count);
    for(int i = 0 ; i < this->expected_count; i++) 
        std::cout << "\t\t" << this->all_args[i].param << " "
                << this->all_args[i].alt_param << "\t"
                << this->all_args[i].help << " "
                << this->all_args[i].options << std::endl;
    
    std::cout << std::endl;
    // it is expected that the program will exit here.
}

int Argparse::allocArgs(int expected_count) {
    // allocate space for all the args. the provided args will always be less
    // then the total expected_count args.

    this->all_args = new args[expected_count];
    this->all_args_counter = expected_count;
    return 1;
}

int Argparse::initArgs(std::string param, std::string alt_param,
        std::string help, std::string options) {
    // this method should be identical to the python's version of argparse
    // options can be optional. '-' should NOT be provided as param/alt_param
    // strings.

    // all_args has to be allocated at this point.
    assert(this->init_counter >= 0 &&
            this->init_counter < this->expected_count);

    // write the expected names and alt names for all the parameters. the only
    // field missing here is the data field. this will be filled in setArgs().
    this->all_args[this->init_counter].param = param;
    this->all_args[this->init_counter].alt_param = alt_param;
    this->all_args[this->init_counter].help = help;
    this->all_args[this->init_counter].data = "";
    this->all_args[this->init_counter].options = options;

    // increment the expected counter. the data field will be filled up by
    // iterating over each of the elements.
    this->init_counter++;

    return 1;
    }

int Argparse::setArgs(char *argv[]) {
    // this function parses all the arguments passed by the user.
    // for each parameter defined by -xx, there has to be some data except for
    // the parameter -v. All the parameters are already setup by the initArgs()
    // method. This method places the values in the correct place.

    int all_args_counter = 0;

    // for each of the arguments, we parse and store the param/data pair into
    // all_args. we have to split the raw args into separate strings.
    for(int i = 1 ; i < this->argc ; i++) {

        // convert to std::string for better processing.
        std::string str = argv[i];

        // see if this is a value or a parameter
        if(str.at(0) == '-' && !isdigit(str.at(1))) {
            // this is a parameter
            // iterate over the allocated args to find the exact element.
            for(int j = 0 ; j < this->expected_count ; j++) {
                if(this->all_args[j].param == str || 
                        this->all_args[j].alt_param == str) {
                    all_args_counter = j;
                    break;
                }
            }
            if(str == "--verbose" || str == "-v") {
                // there is no data for this string. this is a special case.
                this->all_args[all_args_counter++].data = "1";
            }
            if(str == "--help" || str == "-h") {
                // there is no data for this string. this is a special case.
                this->all_args[all_args_counter++].data = "1";
            }
        }
        else {
            // this is a value. this will not be encountered for --help or
            // --verbose. howevr we have to check if its actually a value or
            // not. this has to be done whenevr we call for this param.
            this->all_args[all_args_counter++].data = str;
        }
    }
    return 1;
}

void Argparse::printAllArgs() {
    // this is a debugging method to test the correctness of the struct
    for(int i = 0 ; i < this->all_args_counter ; i++) {
        std::cout << i << " " << all_args[i].param << " " <<
                all_args[i].alt_param << " " << all_args[i].help << " " <<
                all_args[i].data << "#" << std::endl;
    }
}

std::string Argparse::getArgs(std::string param) {
    // ideally, we needed a dictionary-type data structure to store this value.
    for(int i = 0; i < this->expected_count; i++) {
        // compare each string
        if(all_args[i].param == param || all_args[i].alt_param == param)
            return all_args[i].data;
    }

    // return something such that we can distinguish from -v
    return "param not found!";
}

bool Argparse::insertArgs(std::string param, std::string data) {
    // ideally, we needed a dictionary-type data structure to store this value.
    for(int i = 0; i < this->expected_count; i++) {
        // compare each string
        if(all_args[i].param == param || all_args[i].alt_param == param) {
            all_args[i].data = data;

            // return true if the param was found
            return true;
        }
    }

    // the param that the user was trying to set was not found.
    return false;
}

std::string Argparse::getArgOptions(std::string param) {
    // ideally, we needed a dictionary-type data structure to store this value.
    for(int i = 0; i < this->expected_count; i++) {
        // compare each string
        if(all_args[i].param == param || all_args[i].alt_param == param)
            return all_args[i].options;
    }

    // return something such that we can distinguish from -v
    return "param not found!";
}