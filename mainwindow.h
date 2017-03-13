#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QApplication>
#include <QStringList>
#include <QProcess>
#include <QProcessEnvironment>
#include <QMessageBox>
#include <QMovie>

#include <QDebug>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void fileDialogBtnClicked();
    void saveDialogBtnClicked();
    void fileOpenDialogBtnClicked();
    void fileOpenBtnClicked();
    void runBtnClicked();
    void fileTrimScriptBtnClicked();


    void upMove();
    void downMove();
    void leftMove();
    void rightMove();



    void TrimAndTransform();
    void poissonRecon();
    void ply2mesh();
    void reconComplete();
    void handleError(QProcess::ProcessError);
    void debugAndAlert();

private:
    Ui::MainWindow *ui;

    QProcess* makeTmpDir;
    QProcess* PrcVisualSFM;
    QProcess* PrcMeshlabTrimAndTransform;
    QProcess* PrcPoissonRecon;
    QProcess* PrcPly2Mesh;
    QProcess* moveFiles;
    QProcess* rmTmpDir;


    QString program;
    QStringList arguments;
    QMessageBox msgBox;
    QMovie *prcGif;

    QString srcPath;
    QString destPath;
    QString trimScript;


    QProcessEnvironment env;
};

#endif // MAINWINDOW_H
