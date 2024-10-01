#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QLabel>
#include <vector>
#include <Box2D/Box2D.h>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void updateWorld();

private:
    Ui::MainWindow *ui;
    QTimer *timer;
    float currentX;
    float currentY;
    std::vector<b2World*> worlds;

    void setupBox2D();
    void setupBox2D(float x, int index, int labelIndex);

};

#endif // MAINWINDOW_H
