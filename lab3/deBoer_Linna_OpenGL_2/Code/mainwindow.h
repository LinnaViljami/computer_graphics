#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

    Ui::MainWindow *ui;

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_ResetRotationButton_clicked(bool checked);
    void on_RotationDialX_sliderMoved(int value);
    void on_RotationDialY_sliderMoved(int value);
    void on_RotationDialZ_sliderMoved(int value);

    void on_ResetScaleButton_clicked(bool checked);
    void on_ScaleSlider_sliderMoved(int value);

    void on_PhongButton_toggled(bool checked);
    void on_PhongSpinBox_updated(int value);
    void on_NormalButton_toggled(bool checked);
    void on_GouraudButton_toggled(bool checked);
    void on_phongSpinBox_valueChanged(int arg1);
};

#endif // MAINWINDOW_H
