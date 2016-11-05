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

#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    state = State::Start;
}

MainWindow::~MainWindow()
{
    state = State::Stop;
    delete ui;
}

void MainWindow::on_pushButtonSavePic_clicked()
{
    if (cam) {
        ui->statusBar->showMessage("Saving Picture ...", 2000);
        ui->labelFileName->setText(QString("File: %1.tiff")
                                   .arg(QString::number(
                                            cam->getTimestampFrame())));
    }
}

void MainWindow::on_pushButtonStart_clicked()
{
    if (!cam && ui->lineEditPathOni->text() != "" &&
            ui->lineEditPathPics->text() != "") {
        ui->statusBar->showMessage("Ready to start the annotation ...", 2000);
        ui->pushButtonStart->setEnabled(false);
        ui->toolButtonPathOni->setEnabled(false);
        ui->toolButtonPathPics->setEnabled(false);

        cam = new Camera();

        qRegisterMetaType<cv::Mat>("cv::Mat");
        connect(cam, SIGNAL(sigFrameReady(cv::Mat)),
                this, SLOT(FrameReady(cv::Mat)));
        cam->setDeviceURI(ui->lineEditPathOni->text().toStdString());
        cam->start();
        state = State::Setup;
        ui->statusBar->showMessage(ui->lineEditPathOni->text() +
                                   QString(" contains %1 frames.")
                                   .arg(cam->getTotalNoFrame()),
                                   2000);
        state = State::Acquisition;
        cam->acquisition();
        ui->labelFrameCounter->setText(QString::number(cam->getFrameIndex()) +
                                       "/" +
                                       QString::number(cam->getTotalNoFrame()));
    } else {
        ui->statusBar->showMessage(
                    "Please, set *.oni file and picture directory!", 2000);
    }
}

void MainWindow::on_toolButtonPathOni_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this->parentWidget(),
                                                    tr("Open Rec."),
                                                    QDir::homePath(),
                                                    tr("Oni Files (*.oni)"));
    if (fileName.isEmpty())
        return;
    else {
        QFile file(fileName);
        if (!file.open(QIODevice::ReadOnly)) {
            QMessageBox::information(this->parentWidget(),
                                     tr("Unable to open file"),
                                     file.errorString());
            return;
        }
        ui->lineEditPathOni->clear();
        ui->lineEditPathOni->insert(fileName);
    }
}

void MainWindow::on_toolButtonPathPics_clicked()
{
    QString dir =
            QFileDialog::getExistingDirectory(
                this,
                tr("Open Directory"),
                QDir::homePath(),
                QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    ui->lineEditPathPics->setText(dir);
}

void MainWindow::on_actionAbout_triggered()
{
    QString title = "About " + qApp->applicationName();

    QString msg = QString("<p><b>Application Name:</b> %1</p>")
            .arg(qApp->applicationName());
    msg += QString("<p><b>Version:</b> %1</p>")
            .arg(APP_VERSION);
    msg += QString("<p><b>Qt Version:</b> %1</p>")
            .arg(QT_VERSION_STR);

    QMessageBox::about(this,
                       title,
                       msg +
                       tr("<p><b>"APP_NAME"</b> Copyright (C) 2016</p>"
                          "Author:"
                          "<ul type=”disc”>"
                          "<li>Daniele Liciotti</li>"
                          "</ul>"
                          ));

}

void MainWindow::on_actionAbout_Qt_triggered()
{
    QMessageBox::aboutQt(this);
}

void MainWindow::on_pushButtonExit_clicked()
{
    if(cam) {
        cam->stop();
        delete cam;
        cam = NULL;
    }
}

void MainWindow::FrameReady(cv::Mat FrameImage)
{
    cv::Mat frameRGB;
    cv::cvtColor(FrameImage.clone(), frameRGB, CV_GRAY2BGR);
    QImage qimageBGR((uchar*)frameRGB.data,
                     frameRGB.cols,
                     frameRGB.rows,
                     frameRGB.step,
                     QImage::Format_RGB888);
    ui->labelFrame->setPixmap(QPixmap::fromImage(qimageBGR));
}

void MainWindow::on_pushButtonNext_clicked()
{
    cam->acquisition();
    ui->labelFrameCounter->setText(QString::number(cam->getFrameIndex()) +
                                   "/" +
                                   QString::number(cam->getTotalNoFrame()));
}

void MainWindow::on_pushButtonCancel_clicked()
{

}
