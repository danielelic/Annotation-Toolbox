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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtWidgets>
#include <QSettings>

#include <camera.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_pushButtonSavePic_clicked();

    void on_toolButtonPathOni_clicked();

    void on_toolButtonPathPics_clicked();

    void on_actionAbout_triggered();

    void on_actionAbout_Qt_triggered();

    void on_pushButtonStart_clicked();

    void on_pushButtonExit_clicked();

    void FrameReady(cv::Mat FrameImage);

    void on_pushButtonNext_clicked();

    void on_pushButtonCancel_clicked();

private:
    Ui::MainWindow *ui;
    Camera *cam = NULL;

    enum class State {
        Stop = 0,
        Start,
        Setup,
        Acquisition,
        Process
    };
    State state{State::Stop};

    QString m_sSettingsFile;

    void loadSettings();
    void saveSettings();
};

#endif // MAINWINDOW_H
