#ifndef NET_CONTROLLER_H
#define NET_CONTROLLER_H

#include "icontroller.h"
#include "server_message_executor.h"
#include "common/signal_sender.h"

#include <almaz/net/server.h>
#include <almaz/logger/logger.h>

#include <future>

using namespace almaz::net;

class NetController final
    : public IController
{

    using Server = TcpServer<eShooterTypes, CustomServerMsgExec>;

public:
    NetController()
        : leftPushed_(false),
          rightPushed_(false),
          forwardPushed_(false),
          backPushed_(false)
    {
    }

    //!
    //! \brief init
    //!
    bool init(unsigned int port)
    {

        SignalSender::instance().leftSignal.connect([this]()
                                                    { leftPushed_ = true; });

        SignalSender::instance().rightSignal.connect([this]()
                                                     { rightPushed_ = true; });

        SignalSender::instance().forwardSignal.connect([this]()
                                                       { forwardPushed_ = true; });

        SignalSender::instance().backSignal.connect([this]()
                                                    { backPushed_ = true; });

        if (!server_.init(port))
        {
            ALMAZ_LOCAL_LOG1(almaz::logger::error) << "Tcp server is inited..";
            return false;
        }

        future_ = std::async(std::launch::async, [this]()
                             { return server_.run(); });

        return true;
    }

    //!
    //! \brief forwardPressed
    //! \return
    //!
    bool forwardPressed() override
    {

        if (forwardPushed_)
        {
            forwardPushed_ = false;
            return true;
        }

        return false;
    }

    //!
    //! \brief backPressed
    //! \return
    //!
    bool backPressed() override
    {

        if (backPushed_)
        {
            backPushed_ = false;
            return true;
        }

        return false;
    }

    //!
    //! \brief rightPressed
    //! \return
    //!
    bool rightPressed() override
    {

        if (rightPushed_)
        {
            rightPushed_ = false;
            return true;
        }

        return false;
    }

    //!
    //! \brief leftPressed
    //! \return
    //!
    bool leftPressed() override
    {

        if (leftPushed_)
        {
            leftPushed_ = false;
            return true;
        }

        return false;
    }

private:
    std::future<void> future_;
    Server server_;
    bool leftPushed_;
    bool rightPushed_;
    bool forwardPushed_;
    bool backPushed_;
};

#endif
