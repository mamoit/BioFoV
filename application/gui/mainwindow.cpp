#include "mainwindow.hpp"
#include "ui_mainwindow.h"

/*******************************************************************************
 * Constructors
 ******************************************************************************/
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->actionAbout, SIGNAL(triggered()), this, SLOT(openAbout()));
}

/*******************************************************************************
 * Destructors
 ******************************************************************************/
MainWindow::~MainWindow()
{
    delete ui;
}

/*******************************************************************************
 * Actions
 ******************************************************************************/
void MainWindow::on_actionAdd_Video_File_triggered()
{
    VideoItem * last = NULL;
    QFileDialog getFileDialog(this);
    getFileDialog.setDirectory(QDir::homePath());
    getFileDialog.setFileMode(QFileDialog::ExistingFiles);
    getFileDialog.setAcceptMode(QFileDialog::AcceptOpen);

    QStringList fileNames;
    if(getFileDialog.exec())
    {
        fileNames = getFileDialog.selectedFiles();
        if (fileNames.isEmpty()) {return;} // No file name provided
        foreach (QString fileName, fileNames) {
            last = new VideoItem(fileName);
            ui->videoList->addItem(last);
        }
        if(last != NULL) {
            ui->player->pause();
            ui->player->unload();
            ui->player->loadVid(last->vid);
        }
    } else {
        qDebug("Failed to open FileDialog window");
    }
    return;
}

void MainWindow::openAbout()
{
    About about;
    about.exec();
}

void MainWindow::on_videoList_itemDoubleClicked(QListWidgetItem *item)
{
    VideoItem * vitem = (VideoItem *) item;
    ui->player->pause();
    ui->player->unload();
    ui->player->loadVid(vitem->vid);
}

/*******************************************************************************
 * VideoItem functions
 ******************************************************************************/
VideoItem::VideoItem(QString filename){
    setText(filename);
    vid = new Video(filename.toStdString());
}
