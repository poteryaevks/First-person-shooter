#include "shooter.h"
#include "local_controller.h"
#include "net_controller.h"

#include <almaz/misc/program_controll.h>
#include <almaz/logger/logger.h>

#include <experimental/filesystem>

po::variables_map initProgramOptions(const int &argc, char **argv);

int main(int argc, char* argv[])
{
    // ..получаем параметры запуска..
    try{
        options = initProgramOptions(argc, argv);

        if(options.empty())
            return 0;
    }
    catch(const std::exception& ex){

        std::cout << ""
                  << ex.what()
                  << '\n'
                  << USAGE_DESC
                  << '\n';

        return eErrorTypes::UNRECOGNISED_OPTION;
    }
    catch(...){
        std::cout << USAGE_DESC;
        return eErrorTypes::UNRECOGNISED_OPTION;
    }

    auto localController = std::make_unique<LocalController>();
//    localController->init(options["port"].as<int>());

    Shooter shooter;
    shooter.init(std::move(localController));
    std::cerr << "dsds\n";
    try{
        auto res = shooter.Start();
    }
    catch(const std::exception& ex){
        ALMAZ_LOCAL_LOG1(almaz::logger::error) << ex.what();
    }
    catch(...){
        ALMAZ_LOCAL_LOG1(almaz::logger::error) << "Error";
    }

    return 0;
}

po::variables_map initProgramOptions(const int &argc, char **argv)
{
    po::variables_map vm;

    // Declare the supported options.
    po::options_description desc("Program options");

    desc.add_options()
            ("help", "produce help message")
            ("port", po::value<int>()->default_value(8001), "Tcp port");

    po::store(po::parse_command_line(argc, argv, desc), vm);
    po::notify(vm);

    if (vm.count("help")) {
        std::cout << desc << std::endl;
        return po::variables_map();
    }

    return vm;
}



