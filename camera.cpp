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

void Camera::start(const char* deviceURI) {
    state = State::Start;

    try {
        // initialize OpenNI
        openni::OpenNI::initialize();

        auto ret = device.open(deviceURI);
        if (ret == openni::STATUS_OK) {
            playbackControl = device.getPlaybackControl();
            playbackControl->setRepeatEnabled(true);
        } else {
            throw std::runtime_error("Can not open device.");
        }
        this->acquisition();
    }
    catch (std::exception&) {
        std::cout << openni::OpenNI::getExtendedError() << std::endl;
    }
}

void Camera::stop() {
    state = State::Stop;
    depthStream.destroy();
    device.close();
    openni::OpenNI::shutdown();
}

void Camera::acquisition()
{
    state = State::Acquisition;

    depthStream.create(device, openni::SensorType::SENSOR_DEPTH);
    depthStream.start();

    do {
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
        cv::imshow("depthImage8", depthImage8);

        if (cv::waitKey(1) == 27) {
            qWarning() << "Exit ...";
            break;
        }
    } while (true);
}
