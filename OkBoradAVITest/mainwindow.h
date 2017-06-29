#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <windows.h>
#include "Okapi32.h"
#include <QGraphicsView>
#include <QGraphicsPixmapItem>
#include <QDebug>

const int IMAGE_LEFT = 0;
const int IMAGE_TOP = 0;
const int IMAGE_RIGHT = 1024;
const int IMAGE_BOTTOM = 768;
const int REC_X = 200;
const int REC_Y = 100;
const int REC_WIDTH = 576;
const int REC_HEIGHT = 576;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    bool open();
    void stop();
    void updateimage();

private slots:
    void on_pushButton_start_clicked();

    void on_pushButton_stop_clicked();

    void on_pushButton_saveavi_clicked();

    void on_pushButton_saveavi_2_clicked();

    void on_spinBox_quality_valueChanged(const QString &arg1);

    void on_comboBox_videotype_currentIndexChanged(int index);

private:
    Ui::MainWindow *ui;
    HANDLE m_board;
    QString filepath;

    void log(QString text);
    void UpdateFilePath();
};

BOOL CALLBACK beginCapture(HANDLE board);
BOOL CALLBACK process(HANDLE board, long frameIndex);
BOOL CALLBACK endCapture(HANDLE board);

#endif // MAINWINDOW_H
