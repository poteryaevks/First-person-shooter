#ifndef I_CONTROLLER_H
#define I_CONTROLLER_H

#include <boost/noncopyable.hpp>


class IController
        : private boost::noncopyable
{

public:

    //!
    //! \brief forwardPressed
    //! \return
    //!
    virtual bool forwardPressed() = 0;

    //!
    //! \brief backPressed
    //! \return
    //!
    virtual bool backPressed() = 0;

    //!
    //! \brief rightPressed
    //! \return
    //!
    virtual bool rightPressed() = 0;

    //!
    //! \brief leftPressed
    //! \return
    //!
    virtual bool leftPressed() = 0;
};





#endif
