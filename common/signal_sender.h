#ifndef SIGNAL_SENDER_H
#define SIGNAL_SENDER_H

#include <almaz/misc/singleton.h>

#include <boost/signals2.hpp>

class SignalSender
        : public almaz::misc::Singleton<SignalSender>
{

    friend class almaz::misc::Singleton<SignalSender>;
    using SignalToSend = boost::signals2::signal<void()>;

public:

    SignalToSend leftSignal;
    SignalToSend rightSignal;
    SignalToSend forwardSignal;
    SignalToSend backSignal;

private:

    SignalSender() {}
    ~SignalSender() {}
};


#endif
