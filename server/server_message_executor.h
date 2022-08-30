#ifndef SERVER_MESSAGE_EXEC_H
#define SERVER_MESSAGE_EXEC_H

#include <almaz/logger/logger.h>
#include <almaz/net/netcommon.h>
#include <almaz/net/protocol.h>
#include <almaz/net/message_queue.h>

#include "common/shooter_protocol.h"
#include "common/signal_sender.h"

using namespace almaz::net;
using namespace  almaz;

template <
        class MessageType
        >
class CustomServerMsgExec
{

    using Message = IMessage<MessageType>;
    using MessageQueueType = almaz::net::LockfreeQueue<Message*>;

public:

    CustomServerMsgExec()
        : stop_(false)
    {}

    ~CustomServerMsgExec(){

        stop_.store(true);


        try{
            if(runFuture_.valid())
                runFuture_.get();
        }
        catch(const std::exception& ex){
            ALMAZ_LOCAL_LOG1(logger::error) << (format("Future's error accured. Reason: %s")
                                                % ex.what()).str();

        }
        catch(...){
            ALMAZ_LOCAL_LOG1(logger::error) << (format("Future's error accured")).str();
        }

        Message* inMessage;
        Message* outMessage;

        // ....
        while(outputQueue_.pop(outMessage))
            delete outMessage;

        while(inputQueue_.pop(inMessage))
            delete inMessage;
    }


    //!
    //! \brief run
    //!
    void runQueueExec(){

        runFuture_ = std::async(std::launch::async,
                                &CustomServerMsgExec::messagesExec, this
                                );
    }

    //!
    //! \brief outIsEmpty
    //! \return
    //!
    bool outIsEmpty() {
        return outputQueue_.empty();
    }

    //!
    //! \brief inIsEmpty
    //! \return
    //!
    bool inIsEmpty() {
        return inputQueue_.empty();
    }

    //!
    //! \brief getNextOut
    //! \return
    //!
    bool getNextOut(Message*& message) {
        return outputQueue_.pop(message);
    }

    //!
    //! \brief getNextIn
    //! \return
    //!
    bool getNextIn(Message*& message) {
        return inputQueue_.pop(message);
    }

    //!
    //! \brief addIn
    //! \return
    //!
    bool addIn(Message* const & message) {
        return inputQueue_.push(message);
    }

    //!
    //! \brief addOut
    //! \return
    //!
    bool addOut(Message* const& message) {
        return outputQueue_.push(message);
    }

private:

    //!
    //! \brief messagesExec
    //!
    void messagesExec(){

        Message* inputMessage;

        while(!stop_.load()){

            if(!getNextIn(inputMessage)){
                continue;
            }

            switch (inputMessage->header.typeId)
            {
            case eShooterTypes::MOVE:
            {
                moveResponse(*inputMessage);

                delete inputMessage;
                break;
            }

            default:
                break;
            }
        }
    }

    //!
    //! \brief moveRightResponse
    //!
    void moveResponse(Message& message){

        std::string str;
        message >> str;

        if(str == "w")
            SignalSender::instance().forwardSignal();
        else if(str == "s")
            SignalSender::instance().backSignal();
        else if(str == "a")
            SignalSender::instance().leftSignal();
        else if(str == "d")
            SignalSender::instance().rightSignal();
        else
            return;

        auto outputMessage = Message::create();
        outputMessage->header.typeId = MessageType::MOVE;
        *outputMessage << std::string("ok");
        addOut(outputMessage);
    }

private:

    MessageQueueType outputQueue_;
    MessageQueueType inputQueue_;
    std::future<void> runFuture_;
    std::atomic_bool stop_;
    bool autorizationComplete_ { false };
    ClientInfo clientInfo_;    //!

};

#endif
