#ifndef CLIENT_CONTROLLER_H
#define CLIENT_CONTROLLER_H

#include <almaz/net/client.h>
#include <almaz/misc/singleton.h>
#include <almaz/logger/logger.h>

#include "client_message_executor.h"
#include "common/signal_sender.h"

using namespace almaz::net;
using namespace almaz;

class ClientController
        : public misc::Singleton<ClientController>
{
    friend class misc::Singleton<ClientController>;
    using NetClient = TcpClient<eShooterTypes, ShooterClientMsgExec>;
    using NetClientPtr = std::unique_ptr<NetClient>;
    using NamePassType = std::pair<std::string, std::string>;

public:

    //!
    //! \brief init
    //! \param address
    //! \param port
    //! \return
    //!
    bool init(std::string address,
              unsigned short port,
              ClientInfo clientInfo){

        netClient_ = std::make_unique<NetClient>(clientInfo,
                                                 address,
                                                 port
                                                 );

        future_ = std::async(std::launch::async, [this](){

            ALMAZ_LOCAL_LOG1(logger::info) << "Net client started";
            return netClient_->run();
        });

        initComplete_ = true;
        return initComplete_;
    }

    //!
    //! \brief run
    //!
    void run(){

        if(!initComplete_)
            return;

        ALMAZ_LOCAL_LOG1(logger::info) << "Client run..";

        while(!stop_.load()){

//            if (GetAsyncKeyState((unsigned short)'W') & 0x8000){
//                SignalSender::instance().forwardSignal();
//            }

//            if (GetAsyncKeyState((unsigned short)'S') & 0x8000){
//                SignalSender::instance().backSignal();
//            }

//            if (GetAsyncKeyState((unsigned short)'D') & 0x8000){
//                SignalSender::instance().rightSignal();
//            }

//            if (GetAsyncKeyState((unsigned short)'A') & 0x8000){
//                SignalSender::instance().leftSignal();
//            }

            std::this_thread::sleep_for(std::chrono::milliseconds(30));
        }
    }

private:

    ClientController()
        : stop_(false)
        , initComplete_(false){}

    ~ClientController(){

        netClient_->stop();

        // waiting for finish..
        while(future_.wait_for(std::chrono::seconds(0))
              != std::future_status::ready){
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }

        try{
            if(future_.valid()){
                future_.get();
            }
        }
        catch(const std::exception& ex){
            ALMAZ_LOCAL_LOG1(almaz::logger::error) << ex.what();
        }
    }

private:

    NetClientPtr netClient_;
    std::atomic_bool stop_;
    bool initComplete_;
    std::future<void> future_;

};

#endif
