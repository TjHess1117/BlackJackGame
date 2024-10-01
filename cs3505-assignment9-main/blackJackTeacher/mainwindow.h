#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "PlayingWindow.h"
#include "model.h"
QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(Model &model, QWidget *parent = nullptr);
    ~MainWindow();
signals:
    void SetLevel(int level);
public slots:
    void levelClicked(int level);
    void hitSlot();

private:
    Ui::MainWindow *ui;
    PlayingWindow* levelOneWindow;
    Model* model;
    void setupConnections(Model& model,Ui::MainWindow *ui);
    void setupWindowDisplay();
};
#endif // MAINWINDOW_H
