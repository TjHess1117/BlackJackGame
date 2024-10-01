#ifndef ANIMATION_H
#define ANIMATION_H

#include <QMainWindow>
#include <QTimer>
#include <QLabel>
#include <Box2D/Box2D.h>
#include "ui_animationWindow.h"

QT_BEGIN_NAMESPACE
namespace Ui { class Animation; }
QT_END_NAMESPACE

class Animation : public QMainWindow
{
    Q_OBJECT

signals:
    void finished();

public:

    Animation(QWidget *parent = nullptr);
    ~Animation();

private slots:
    void updateWorld();


private:
    Ui::AnimationWindow *ui;
    QTimer *timer;
    QVector<b2World*> worlds;
    float currentX;
    float currentY;

    void setupBox2D();
    void setupBox2D(float x, int index, int labelIndex);
};

#endif // ANIMATION_H
