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

#include "camera.h"

Camera::Camera()
{
}

Camera::~Camera()
{
}

void Camera::start() {
    try {
        // initialize OpenNI
        openni::OpenNI::initialize();
        auto ret = device.open(deviceURI.c_str());

        if (ret != openni::STATUS_OK) {
            throw std::runtime_error("Can not open device.");
        } else {
            this->setup();
        }
    }
    catch (std::exception&) {
        std::cout << openni::OpenNI::getExtendedError()
                  << std::endl;
    }
}

void Camera::stop() {
    depthStream.destroy();
    device.close();
    openni::OpenNI::shutdown();
}

void Camera::setup() {
    device.setImageRegistrationMode(openni::IMAGE_REGISTRATION_DEPTH_TO_COLOR);

    depthStream.create(device, openni::SensorType::SENSOR_DEPTH);
    depthStream.start();

    playbackControl = device.getPlaybackControl();
    playbackControl->setRepeatEnabled(false);
    playbackControl->setSpeed(-1);

    totalNoFrame = playbackControl->getNumberOfFrames(depthStream);
}

void Camera::acquisition() {
    // depth frame
    depthStream.readFrame(&depthFrame);
    if (depthFrame.isValid()) {
        depthImage = cv::Mat(depthFrame.getVideoMode().getResolutionY(),
                             depthFrame.getVideoMode().getResolutionX(),
                             CV_16UC1,
                             (unsigned short*)depthFrame.getData());
        if (depthImage.cols == 640)
            cv::resize(depthImage.clone(), depthImage,
                       cv::Size(depthImage.cols/2,depthImage.rows/2));
    }

    cv::Mat depthImage8;
    const float scaleFactor = 0.05f;
    depthImage.convertTo(depthImage8, CV_8UC1, scaleFactor);

    emit sigFrameReady(depthImage8);
    frameIndex = depthFrame.getFrameIndex();
}

std::string Camera::getDeviceURI() const
{
    return deviceURI;
}

void Camera::setDeviceURI(const std::string &value)
{
    deviceURI = value;
}
int Camera::getTotalNoFrame() const
{
    return totalNoFrame;
}

void Camera::setTotalNoFrame(int value)
{
    totalNoFrame = value;
}
int Camera::getFrameIndex() const
{
    return frameIndex;
}

void Camera::setFrameIndex(int value)
{
    frameIndex = value;
}
