#ifndef CLIENT_MESSAGE_EXEC_H
#define CLIENT_MESSAGE_EXEC_H

#include <almaz/logger/logger.h>
#include <almaz/net/netcommon.h>
#include <almaz/net/protocol.h>
#include <almaz/net/message_queue.h>

#include "common/signal_sender.h"
#include "common/shooter_protocol.h"

using namespace  almaz;

using namespace almaz::net;

template <
        class MessageType
        >
class ShooterClientMsgExec
{
    using Message = IMessage<MessageType>;
    using MessageQueueType = almaz::net::LockfreeQueue<Message*>;
    using STtime = std::chrono::time_point<std::chrono::system_clock>;

public:

    ShooterClientMsgExec(ClientInfo clientInfo)
        : clientInfo_(clientInfo),
          stop_(false),
          messageCounter_(0),
          leftPushed_(false),
          rightPushed_(false),
          forwardPushed_(false),
          backPushed_(false)
    {}

    ~ShooterClientMsgExec(){

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

        SignalSender::instance().leftSignal.connect([this](){
            leftPushed_ = true;
        });

        SignalSender::instance().rightSignal.connect([this](){
            rightPushed_ = true;
        });

        SignalSender::instance().forwardSignal.connect([this](){
            forwardPushed_ = true;
        });

        SignalSender::instance().backSignal.connect([this](){
            backPushed_ = true;
        });

        runFuture_ = std::async(std::launch::async,
                                &ShooterClientMsgExec::messagesExec,
                                this);
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
    bool addIn(Message* const& message) {
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

    void messagesExec(){

        Message* inputMessage;

        while(!stop_.load()){

            if(leftPushed_){

                leftPushed_ = false;
                auto outputMessage = Message::create();
                outputMessage->header.typeId = MessageType::MOVE;
                *outputMessage << std::string("a");
                addOut(outputMessage);
                start_ = std::chrono::high_resolution_clock::now();
            }

            if(rightPushed_){

                rightPushed_ = false;
                auto outputMessage = Message::create();
                outputMessage->header.typeId = MessageType::MOVE;
                *outputMessage << std::string("d");
                addOut(outputMessage);
                start_ = std::chrono::high_resolution_clock::now();
            }

            if(backPushed_){

                backPushed_ = false;
                auto outputMessage = Message::create();
                outputMessage->header.typeId = MessageType::MOVE;
                *outputMessage << std::string("s");
                addOut(outputMessage);
                start_ = std::chrono::high_resolution_clock::now();
            }


            if(forwardPushed_){

                forwardPushed_ = false;
                auto outputMessage = Message::create();
                outputMessage->header.typeId = MessageType::MOVE;
                *outputMessage << std::string("w");
                addOut(outputMessage);
                start_ = std::chrono::high_resolution_clock::now();
            }

            if(!getNextIn(inputMessage)){
                continue;
            }

            switch (inputMessage->header.typeId)
            {
            case MessageType::MOVE:
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
    //! \brief moveBackResponse
    //!
    void moveResponse(Message& message){

        auto finish = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> duration = finish - start_;

        std::cout << (boost::format("Duration: %f sec.\n")
                      % duration.count()).str();

    }

private:

    MessageQueueType outputQueue_;
    MessageQueueType inputQueue_;
    std::future<void> runFuture_;
    std::future<void> pingFuture_;
    bool autorizationComplete_ {false};
    ClientInfo clientInfo_;
    std::atomic_bool stop_;
    uint32 messageCounter_;
    STtime start_;

    bool leftPushed_;
    bool rightPushed_;
    bool forwardPushed_;
    bool backPushed_;

};


#endif

