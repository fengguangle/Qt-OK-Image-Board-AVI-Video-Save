#include "mainwindow.h"
#include "ui_mainwindow.h"

bool savevideo=false;
bool cvmode=false;
bool normalmode=false;
char* filebeg;
char* filecon;
char* fileend;
QByteArray ba1;
QByteArray ba2;
QByteArray ba3;

long tnum=0;
long snum=0;
QImage *rimg;
MainWindow* _mainWindow;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    _mainWindow=this;
    ui->setupUi(this);
    ui->comboBox_videotype->addItem("MJPG");
    ui->comboBox_videotype->addItem("MPG4");
    ui->comboBox_videotype->setCurrentIndex(0);
}

MainWindow::~MainWindow()
{
    delete ui;
}

bool MainWindow::open()
{
    MLONG index = -1;
    m_board = okOpenBoard(&index);
    if (m_board) {
        okSetCaptureParam(m_board, CAPTURE_BUFRGBFORMAT, FORM_RGB888);
        okSetCaptureParam(m_board, CAPTURE_BUFBLOCKSIZE, 0);

        okSetSeqCallback(m_board, beginCapture, process, endCapture);
        okCaptureTo(m_board, BUFFER, 0, -1);
        log("save:"+QString("%1").arg(savevideo));

        return true;
    }else {
        return false;
    }
}

void MainWindow::stop(){
    okStopCapture(m_board);
    okCloseBoard(m_board);
}

void MainWindow::updateimage(){
    QPixmap pimg;
    pimg.convertFromImage(*rimg);
    ui->label->setPixmap(pimg);
    ui->lineEdit_getimgnum->setText(QString("%1").arg(tnum));
    ui->lineEdit_saveimgnum->setText(QString("%1").arg(snum));
}

void MainWindow::UpdateFilePath(){
    filepath.clear();
    filepath=QString("%1,%2,%3,%4").arg("image.avi")
            .arg(ui->comboBox_videotype->currentText())
            .arg(ui->spinBox_quality->value()).arg("beg");
    ba1 = filepath.toLatin1();
    filebeg = ba1.data();

    filepath.clear();
    filepath=QString("%1,%2,%3,%4").arg("image.avi")
            .arg(ui->comboBox_videotype->currentText())
            .arg(ui->spinBox_quality->value()).arg("con");
    ba2 = filepath.toLatin1();
    filecon = ba2.data();

    filepath.clear();
    filepath=QString("%1,%2,%3,%4").arg("image.avi")
            .arg(ui->comboBox_videotype->currentText())
            .arg(ui->spinBox_quality->value()).arg("end");
    ba3 = filepath.toLatin1();
    fileend = ba3.data();
    log(QString("filebeg: %1").arg(filebeg));
    log(QString("filecon: %1").arg(filecon));
    log(QString("fileend: %1").arg(fileend));
}

BOOL CALLBACK beginCapture(HANDLE board) {
    RECT rect;
    SetRect(&rect, IMAGE_LEFT, IMAGE_TOP, IMAGE_RIGHT, IMAGE_BOTTOM);
    okSetTargetRect(board, BUFFER, &rect);

    if (savevideo)
    {
        okSaveImageFile(board, filebeg, 0, BUFFER, 0, 1);
        snum=1;
    }
    return 1;
}

BOOL CALLBACK process(HANDLE board, long frameIndex)
{
    short width, height;
    long stride;
    LPBYTE currentImage = (LPBYTE)okGetTargetInfo(board,BUFFER,frameIndex,
                                                 &width, &height, &stride);
    rimg=new QImage(currentImage, width, height, QImage::Format_RGB888);
    //QImage image = QImage(currentImage, width, height, QImage::Format_RGB888);
    _mainWindow->updateimage();

    if (savevideo)
    {
        if(snum==0)
        {
             okSaveImageFile(board, filebeg, snum, BUFFER, 0, 1);
        }
        else{
            okSaveImageFile(board, filecon, snum, BUFFER, frameIndex, 1);
        }
        snum++;
    }
    else if(snum!=0){
        okSaveImageFile(board, fileend, 0, BUFFER, 0, 1);
        snum=0;
    }

    tnum++;
    delete rimg;

    return 1;
}

BOOL CALLBACK endCapture(HANDLE board)
{
    okSetSeqCallback(board, NULL, NULL, NULL);
    okStopCapture(board);

    if (savevideo&&snum!=0)
    {
        okSaveImageFile(board, fileend, 0, BUFFER, 0, 1);
    }
    return 1;
}

void MainWindow::on_pushButton_start_clicked()
{
    if(open())
    {
        log("开启成功");
    }else log("开启失败");
}

void MainWindow::on_pushButton_stop_clicked()
{
    stop();
    log("停止");
}

void MainWindow::log(QString text)
{
    ui->textEdit->append(text);
}

void MainWindow::on_pushButton_saveavi_clicked()
{
    savevideo=true;
    log("save:"+QString("%1").arg(savevideo));
}

void MainWindow::on_pushButton_saveavi_2_clicked()
{
    savevideo=false;
    log("save:"+QString("%1").arg(savevideo));
}

void MainWindow::on_spinBox_quality_valueChanged(const QString &arg1)
{
    UpdateFilePath();
}

void MainWindow::on_comboBox_videotype_currentIndexChanged(int index)
{
    UpdateFilePath();
}
