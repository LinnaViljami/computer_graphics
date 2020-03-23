#include "mainview.h"

#include <QDebug>

// Triggered by pressing a key
void MainView::keyPressEvent(QKeyEvent *ev)
{
    switch(ev->key()) {
    case 'W':
        MainView::movingForwards = true;
        break;
    case 'S':
        MainView::movingBackwards = true;
        break;
    case 'A':
        MainView::movingLeft = true;
        break;
    case 'D':
        MainView::movingRight = true;
        break;
    default:
        // ev->key() is an integer. For alpha numeric characters keys it equivalent with the char value ('A' == 65, '1' == 49)
        // Alternatively, you could use Qt Key enums, see http://doc.qt.io/qt-5/qt.html#Key-enum
        qDebug() << ev->key() << "pressed";
        break;
    }

    // Used to update the screen after changes
    update();
}

// Triggered by releasing a key
void MainView::keyReleaseEvent(QKeyEvent *ev)
{
    switch(ev->key()) {
    case 'W':
        MainView::movingForwards = false;
        break;
    case 'S':
        MainView::movingBackwards = false;
        break;
    case 'A':
        MainView::movingLeft = false;
        break;
    case 'D':
        MainView::movingRight= false;
        break;
    default:
        qDebug() << ev->key() << "released";
        break;
    }

    update();
}

// Triggered by clicking two subsequent times on any mouse button
// It also fires two mousePress and mouseRelease events!
void MainView::mouseDoubleClickEvent(QMouseEvent *ev)
{
    qDebug() << "Mouse double clicked:" << ev->button();

    update();
}

// Triggered when moving the mouse inside the window (only when the mouse is clicked!)
void MainView::mouseMoveEvent(QMouseEvent *ev)
{
    mouseX = ev->x();
    mouseY = ev->y();

    if (!previousMouseCoordinatesKnown) {
        previousMouseX = mouseX;
        previousMouseY = mouseY;
        previousMouseCoordinatesKnown = true;
    }

    yaw += (mouseX - previousMouseX) * 0.10f;
    pitch += (mouseY - previousMouseY) * 0.10f;
    if (pitch > 89.0f) {
        pitch = 89.0f;
    }
    if (pitch < -89.0f) {
        pitch = -89.0f;
    }

    previousMouseX = mouseX;
    previousMouseY = mouseY;

    qDebug() << "yaw: " << yaw << ", pitch: " << pitch;

    update();
}

// Triggered when pressing any mouse button
void MainView::mousePressEvent(QMouseEvent *ev)
{
    qDebug() << "Mouse button pressed:" << ev->button();

    update();
    // Do not remove the line below, clicking must focus on this widget!
    this->setFocus();
}

// Triggered when releasing any mouse button
void MainView::mouseReleaseEvent(QMouseEvent *ev)
{
    qDebug() << "Mouse button released" << ev->button();
    previousMouseCoordinatesKnown = false;

    update();
}

// Triggered when clicking scrolling with the scroll wheel on the mouse
void MainView::wheelEvent(QWheelEvent *ev)
{
    // Implement something
    qDebug() << "Mouse wheel:" << ev->delta();

    update();
}
