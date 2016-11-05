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
#include <QtCore>
#include <stdio.h>
#include <iostream>
#include <vector>

#include <QDebug>
#include <OpenNI.h>
#include <opencv2/opencv.hpp>

class Camera : public QObject
{
    Q_OBJECT
public:
    Camera();
    ~Camera();

    void start();
    void stop();
    void acquisition();
    void setup();

    std::string getDeviceURI() const;
    void setDeviceURI(const std::string &value);

    int getTotalNoFrame() const;
    void setTotalNoFrame(int value);

    int getFrameIndex() const;
    void setFrameIndex(int value);

    uint64_t getTimestampFrame();

    cv::Mat getDepthImage() const;
    void setDepthImage(const cv::Mat &value);

signals:
    void sigFrameReady(cv::Mat);

private:
    std::string deviceURI;
    openni::Device device;
    openni::VideoStream depthStream;
    openni::VideoFrameRef depthFrame;

    openni::PlaybackControl *playbackControl;

    cv::Mat depthImage;

    int totalNoFrame = 0;
    int frameIndex = 0;
};

#endif // CAMERA_H
