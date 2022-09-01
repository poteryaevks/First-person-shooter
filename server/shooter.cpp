#include "shooter.h"

#define OLC_PGE_APPLICATION
#include "third_party/olcPixelGameEngine/olcPixelGameEngine.h"

#include <chrono>
#include <cmath>

bool Shooter::OnUserCreate()
{
    // Called once at the start, so create things here
    return true;
}

bool Shooter::OnUserUpdate(float fElapsedTime)
{
    // called once per frame
    for (int x = 0; x < ScreenWidth(); x++)
        for (int y = 0; y < ScreenHeight(); y++)
            Draw(x, y, olc::Pixel(rand() % 255, rand() % 255, rand()% 255));
    return true;
}

bool Shooter::init(ControllerPtr controller) noexcept
{
    if(controller == nullptr)
        return false;

    controller_ = std::move(controller);

    // init console

    map_.push_back(L"################");
    map_.push_back(L"#..............#");
    map_.push_back(L"#..............#");
    map_.push_back(L"#..#..###..#...#");
    map_.push_back(L"#..#....#..#...#");
    map_.push_back(L"#..#....#..#...#");
    map_.push_back(L"#..#....#......#");
    map_.push_back(L"#..#....#......#");
    map_.push_back(L"#..#....#####..#");
    map_.push_back(L"#..#........#..#");
    map_.push_back(L"#..#........#..#");
    map_.push_back(L"#..#...##...#..#");
    map_.push_back(L"#......#.......#");
    map_.push_back(L"#..#####....#..#");
    map_.push_back(L"#..............#");
    map_.push_back(L"################");

    initComplete_ = true;

    return initComplete_;
}

void Shooter::stop()
{
    stopped_.store(true);
}

void Shooter::exec()
{
    //    if(!initComplete_)
    //        return;

    //    auto tp1 = std::chrono::system_clock::now();
    //    auto tp2 = std::chrono::system_clock::now();

    //    while (!stopped_.load()){

    ////        std::this_thread::sleep_for(std::chrono::milliseconds(20));

    //        tp2 = std::chrono::system_clock::now();
    //        std::chrono::duration<float> elapsedTime = tp2 - tp1;

    //        tp1 = tp2;
    //        auto fElapsedTime = elapsedTime.count();

    //        // clear screen
    //        for (size_t i = 0; i < nScreenWidth * nScreenHeight; i++)
    //            screen_[i] = ' ';

    //        // input
    //        if(controller_->leftPressed()){

    //            personAngle_ -= personSpeed_ * 0.7;
    //            if (personAngle_ > 2 * M_PI)
    //                personAngle_ -= 2 * M_PI;

    //            if (personAngle_ < -2 * M_PI)
    //                personAngle_ += 2 * M_PI;
    //        }

    //        if(controller_->rightPressed()){

    //            personAngle_ += personSpeed_ * 0.7;
    //            if (personAngle_ > 2 * M_PI)
    //                personAngle_ -= 2 * M_PI;

    //            if (personAngle_ < -2 * M_PI)
    //                personAngle_ += 2 * M_PI;
    //        }

    //        if(controller_->forwardPressed()){

    //            personX_ += personSpeed_ * std::sin(personAngle_);
    //            personY_ -= personSpeed_ * std::cos(personAngle_);
    //        }

    //        if(controller_->backPressed()){

    //            personX_ -= personSpeed_ * std::sin(personAngle_);
    //            personY_ += personSpeed_ * std::cos(personAngle_);
    //        }


    //        // ray casting
    //        for (size_t column = 0; column < nScreenWidth; column++){  // nScreenWidth - угол обзора

    //            // расстояние до стены
    //            auto dist = getDistance(std::make_pair(personX_, personY_),
    //                                    personAngle_ + (((double)column - 45) * M_PI / 180),
    //                                    map_);

    //            // высота стены
    //            double wallHeight = nScreenHeight / dist;

    //            double wallUp = (nScreenHeight - wallHeight) / 2;
    //            double wallDown = nScreenHeight - ((nScreenHeight - wallHeight) / 2);

    //            for (size_t line = 0; line < nScreenHeight; line++){

    //                short nShade;

    //                // потолок
    //                if (line < wallUp)
    //                    nShade = ' ';


    //                else if (line > wallDown){

    //                    // пол
    //                    float b = 1 - (((double)line - nScreenHeight / 2) / ((double)nScreenHeight / 2));

    //                    if (b < 0.25)		nShade = '#';
    //                    else if (b < 0.5)	nShade = 'x';
    //                    else if (b < 0.75)	nShade = '-';
    //                    else if (b < 0.9)	nShade = '.';
    //                    else				nShade = ' ';
    //                }
    //                else{
    //                    // стены
    //                    if (dist <= depth / 4)			nShade = 0x2588;
    //                    else if (dist < depth / 3)		nShade = 0x2593;
    //                    else if (dist < depth / 2)		nShade = 0x2592;
    //                    else if (dist < depth)			nShade = 0x2591;
    //                    else							nShade = ' ';
    //                }

    //                screen_[line * nScreenWidth + column] = nShade;
    //            }
    //        }

    //        // stats
    //        swprintf_s(screen_, 45,
    //                   L"X: %3.2f, Y: %3.2f, Angle: %d, FPS: %3.2f",
    //                   personX_,
    //                   personY_,
    //                   (int)(personAngle_ * 180 / M_PI),
    //                   1 / fElapsedTime
    //                   );

    //        // map
    //        for (size_t line = 0; line < map_.size(); line++)
    //            for (size_t symbol = 0; symbol < map_[line].size(); symbol++)
    //                screen_[((line + 1) * nScreenWidth) + symbol] = map_[line][symbol];

    //        // person
    //        screen_[(int)personY_ * nScreenWidth + (int)personX_] = 'P';

    //        // output
    //        screen_[nScreenWidth * nScreenHeight - 1] = '\0';
    //        WriteConsoleOutputCharacter(hConsole_, screen_, nScreenWidth * nScreenHeight, { 0, 0 }, &dwBytesWritten); //
    //    }
}


double Shooter::getDistance(std::pair<double, double> pos, const double& angle, const Map& map)
{
    double distance{ 0 };
    double step{ 0.1 };
    wchar_t currentChar{ map[(int)pos.second][(int)pos.first] };

    while (currentChar != '#'){

        distance += step;

        pos.first += step * std::sin(angle);
        pos.second -= step * std::cos(angle);

        currentChar = map[(int)pos.second][(int)pos.first];
    }

    return distance;
}

