#ifndef SHOOTER_H
#define SHOOTER_H

#include "icontroller.h"
#include "common/shooter_protocol.h"


#include <almaz/misc/singleton.h>
#include <almaz/logger/logger.h>

#include <vector>
#include <string>
#include <atomic>
#include <future>

#include "third_party/olcPixelGameEngine/olcPixelGameEngine.h"
//#include <windows.h>


class Shooter
        :
        //        public almaz::misc::Singleton<Shooter>
        public olc::PixelGameEngine
{

    friend class almaz::misc::Singleton<Shooter>;

    using ControllerPtr = std::unique_ptr<IController>;
    using Map = std::vector < std::wstring >;

public:

    bool OnUserCreate() override;


    bool OnUserUpdate(float fElapsedTime) override;

    //!
    //! \brief init
    //! \return
    //!
    bool init(ControllerPtr controller) noexcept;

    //!
    //! \brief stop
    //!
    void stop();

    //!
    //! \brief Shooter
    //!
    Shooter()
        :
          personX_(5.4)
        , personY_(5.4)
        , personAngle_(0)
        , personSpeed_(0.1)
        , stopped_(false)
        , initComplete_(false)
    {
        // Create Screen
//        screen_ = new wchar_t[nScreenWidth * nScreenHeight];
    }

    ~Shooter() {

        try{
            if(future_.valid())
                future_.get();
        }
        catch(const std::exception& ex){
            ALMAZ_LOCAL_LOG1(almaz::logger::error) << ex.what();
        }

//        delete[] screen_;
    }

private:

    //!
    //! \brief exec
    //!
    void exec();

    //!
    //! \brief getDistance
    //! \param pos
    //! \param angle
    //! \param map_
    //! \return
    //!
    double getDistance(std::pair<double, double> pos, const double& angle, const Map& map_);



private:

    double personX_;      // x
    double personY_;      // y
    double personAngle_;  // rad
    double personSpeed_;  //

    Map map_;  // map
    wchar_t* screen_;
    std::atomic_bool stopped_;
    std::future<void> future_;
    bool initComplete_;
    ControllerPtr controller_;

    static constexpr int nScreenHeight { 40 };
    static constexpr int nScreenWidth { 90 };
    static constexpr double depth { 16 };
};


#endif
