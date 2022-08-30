#include "client_controller.h"

#include <almaz/misc/program_controll.h>
#include <almaz/logger/logger.h>

using namespace boost;

//!
//! \brief initProgramOptions
//! \param argc
//! \param argv
//! \return
//!
boost::program_options::variables_map initProgramOptions(const int &argc, char **argv);


void clientExec(const std::string& address,
                unsigned short port,
                std::string username,
                std::string password
                );

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

    static uint32 clientId{ 0 };
    ClientInfo clientInfo { "client1", "1", clientId++ };

    auto initSucceed = ClientController::instance().init(
                options["host"].as<std::string>(),
            options["port"].as<unsigned short>(),
            clientInfo
            );

    if(!initSucceed){

        ALMAZ_LOCAL_LOG1(logger::fatal) << "Unable to init client controller";
        return -1;
    }

    ALMAZ_LOCAL_LOG1(logger::info) << "Client controller is started";
    ClientController::instance().run();

    ALMAZ_LOCAL_LOG1(logger::info) << "Client controller is stopped";
    return 0;
}



po::variables_map initProgramOptions(const int &argc, char **argv)
{
    po::variables_map vm;

    // Declare the supported options.
    po::options_description desc("Program options");

    desc.add_options()
            ("help", "produce help message")
            ("host", po::value<std::string>(), "Tcp host")
            ("port", po::value<unsigned short>()->default_value(8001), "Tcp port");


    po::store(po::parse_command_line(argc, argv, desc), vm);
    po::notify(vm);

    if (vm.count("help")) {
        std::cout << desc << std::endl;
        return po::variables_map();
    }

    if (!vm.count("host")){
        std::cout << "Host was not set. " << std::endl
                  << USAGE_DESC << std::endl
                  << desc << std::endl;
        return po::variables_map();
    }

    return vm;
}


