#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

// --- Functions that listen for widget events
// forewards to the mainview

void MainWindow::on_ResetRotationButton_clicked(bool checked) {
    Q_UNUSED(checked);
    ui->RotationDialX->setValue(0);
    ui->RotationDialY->setValue(0);
    ui->RotationDialZ->setValue(0);
    ui->mainView->setRotationToAllObjects(0, 0, 0);
}

void MainWindow::on_RotationDialX_sliderMoved(int value) {
    ui->mainView->setRotationToAllObjects(value,
                              ui->RotationDialY->value(),
                              ui->RotationDialZ->value());
}

void MainWindow::on_RotationDialY_sliderMoved(int value) {
    ui->mainView->setRotationToAllObjects(ui->RotationDialX->value(),
                              value,
                              ui->RotationDialZ->value());
}

void MainWindow::on_RotationDialZ_sliderMoved(int value) {
    ui->mainView->setRotationToAllObjects(ui->RotationDialX->value(),
                              ui->RotationDialY->value(),
                              value);
}

void MainWindow::on_ResetScaleButton_clicked(bool checked) {
    Q_UNUSED(checked);
    ui->ScaleSlider->setValue(100);
    ui->mainView->setScale(100);
}

void MainWindow::on_ScaleSlider_sliderMoved(int value) {
    ui->mainView->setScale(value);
}

void MainWindow::on_PhongButton_toggled(bool checked) {
    if (checked) {
        ui->mainView->setShadingMode(Shader::PHONG);
        ui->mainView->update();
    }
}

void MainWindow::on_NormalButton_toggled(bool checked) {
    if (checked) {
        ui->mainView->setShadingMode(Shader::NORMAL);
        ui->mainView->update();
    }
}

void MainWindow::on_GouraudButton_toggled(bool checked) {
    if (checked) {
        ui->mainView->setShadingMode(Shader::GOURAUD);
        ui->mainView->update();
    }
}

