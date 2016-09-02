// Copyright (C) 2016 Daniele Liciotti

// Author: Daniele Liciotti <danielelic@gmail.com>

// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; version 3 of the License.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301, USA.

#ifndef CAMERA_H
#define CAMERA_H
#include <stdio.h>
#include <iostream>
#include <vector>

#include <QDebug>
#include <OpenNI.h>
#include <opencv2/opencv.hpp>

class Camera
{
public:
    Camera();
    ~Camera();

    void start(const char* deviceURI);
    void stop();
    void acquisition();
private:
    enum class State {
        Stop = 0,
        Start,
        Acquisition,
        Process
    };
    State state{State::Stop};
    openni::Device device;
    openni::PlaybackControl* playbackControl;

    openni::VideoStream depthStream;
    openni::VideoFrameRef depthFrame;
    cv::Mat depthImage;

};

#endif // CAMERA_H
