#ifndef LOCAL_CONTROLLER_H
#define LOCAL_CONTROLLER_H

#include "icontroller.h"

//#include <windows.h>

class LocalController final
        : public IController
{

public:

    //!
    //! \brief forwardPressed
    //! \return
    //!
    bool forwardPressed() override{
            return false; //(GetAsyncKeyState((unsigned short)'W') & 0x8000);
    }

    //!
    //! \brief backPressed
    //! \return
    //!
    bool backPressed() override{
        return false; //(GetAsyncKeyState((unsigned short)'S') & 0x8000);
    }

    //!
    //! \brief rightPressed
    //! \return
    //!
    bool rightPressed() override{
        return false; //(GetAsyncKeyState((unsigned short)'D') & 0x8000);
    }

    //!
    //! \brief leftPressed
    //! \return
    //!
    bool leftPressed() override{
        return false; //(GetAsyncKeyState((unsigned short)'A') & 0x8000);
    }
};



#endif
