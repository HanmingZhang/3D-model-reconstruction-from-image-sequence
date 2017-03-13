#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    makeTmpDir = new QProcess(this);
    PrcVisualSFM = new QProcess(this);
    PrcMeshlabTrimAndTransform = new QProcess(this);
    PrcPoissonRecon = new QProcess(this);
    PrcPly2Mesh = new QProcess(this);
    moveFiles = new QProcess(this);
    rmTmpDir = new QProcess(this);


    prcGif = new QMovie("loader.gif");


    env = QProcessEnvironment::systemEnvironment();
    env.insert("TMPDIR", "/tmp");   // Add an environment variable
    env.insert("PATH", env.value("PATH") + "/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin:/usr/games:/usr/local/games");
    env.insert("PATH", env.value("PATH") + ":/home/hanming/vsfm/bin");
    env.insert("PATH", env.value("PATH") + ":/home/hanming/MeshLab/meshlab/src/distrib");
    env.insert("PATH", env.value("PATH") + ":/home/hanming/PoissonRecon/Bin/Linux");

    env.insert("LD_LIBRARY_PATH", env.value("LD_LIBRARY_PATH") + ":/home/hanming/vsfm/bin");
    env.insert("LD_LIBRARY_PATH", env.value("LD_LIBRARY_PATH") + ":/home/hanming/MeshLab/meshlab/src/distrib");
    env.insert("LD_LIBRARY_PATH", env.value("LD_LIBRARY_PATH") + ":/home/hanming/PoissonRecon/Bin/Linux");





    qDebug()<<qgetenv("PATH");
    qDebug()<<qgetenv("LD_LIBRARY_PATH");


    qputenv("PATH",qgetenv("PATH")+":/home/hanming/vsfm/bin");
    qputenv("PATH",qgetenv("PATH")+":/home/hanming/MeshLab/meshlab/src/distrib");
    qputenv("PATH",qgetenv("PATH")+":/home/hanming/PoissonRecon/Bin");
    qputenv("LD_LIBRARY_PATH",qgetenv("LD_LIBRARY_PATH")+":/home/hanming/vsfm/bin");
    qputenv("LD_LIBRARY_PATH",qgetenv("LD_LIBRARY_PATH")+":/home/hanming/MeshLab/meshlab/src/distrib");
    qputenv("LD_LIBRARY_PATH",qgetenv("LD_LIBRARY_PATH")+":/home/hanming/PoissonRecon/Bin/Linux");

    qDebug()<<qgetenv("PATH");
    qDebug()<<qgetenv("LD_LIBRARY_PATH");



    connect(ui->fileDialogBtn,SIGNAL(clicked(bool)),this,SLOT(fileDialogBtnClicked()));
    connect(ui->saveDialogBtn,SIGNAL(clicked(bool)),this,SLOT(saveDialogBtnClicked()));
    connect(ui->fileOpenDialogBtn,SIGNAL(clicked(bool)),this,SLOT(fileOpenDialogBtnClicked()));
    connect(ui->fileOpenBtn,SIGNAL(clicked(bool)),this,SLOT(fileOpenBtnClicked()));
    connect(ui->fileTrimScriptBtn,SIGNAL(clicked(bool)),this,SLOT(fileTrimScriptBtnClicked()));


    connect(ui->BtnDown,SIGNAL(clicked(bool)),this,SLOT(downMove()));
    connect(ui->BtnUp,SIGNAL(clicked(bool)),this,SLOT(upMove()));
    connect(ui->BtnLeft,SIGNAL(clicked(bool)),this,SLOT(leftMove()));
    connect(ui->BtnRight,SIGNAL(clicked(bool)),this,SLOT(rightMove()));


    connect(ui->runBtn,SIGNAL(clicked(bool)),this,SLOT(runBtnClicked()));




    connect(PrcVisualSFM, SIGNAL(finished(int,QProcess::ExitStatus)), this, SLOT(TrimAndTransform()));
    connect(PrcMeshlabTrimAndTransform,SIGNAL(finished(int,QProcess::ExitStatus)),this,SLOT(poissonRecon()));
//  connect(PrcPly2Mesh,SIGNAL(finished(int,QProcess::ExitStatus)),this,SLOT(debugAndAlert()));
    connect(PrcPoissonRecon,SIGNAL(finished(int,QProcess::ExitStatus)),this,SLOT(ply2mesh()));
    connect(PrcPly2Mesh,SIGNAL(finished(int,QProcess::ExitStatus)),this,SLOT(reconComplete()));

    //connect command line Error handle
    connect(PrcVisualSFM, SIGNAL(error(QProcess::ProcessError)), this, SLOT(handleError(QProcess::ProcessError)));
    connect(PrcMeshlabTrimAndTransform, SIGNAL(error(QProcess::ProcessError)), this, SLOT(handleError(QProcess::ProcessError)));
    connect(PrcPoissonRecon, SIGNAL(error(QProcess::ProcessError)), this, SLOT(handleError(QProcess::ProcessError)));
    connect(PrcPly2Mesh, SIGNAL(error(QProcess::ProcessError)), this, SLOT(handleError(QProcess::ProcessError)));
    connect(moveFiles, SIGNAL(error(QProcess::ProcessError)), this, SLOT(handleError(QProcess::ProcessError)));

    trimScript = "";

}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::fileDialogBtnClicked(){

    QFileDialog SrcDialog(this);
    SrcDialog.setViewMode(QFileDialog::Detail);
    SrcDialog.setOption(QFileDialog::ShowDirsOnly,true);
    SrcDialog.setFileMode(QFileDialog::Directory);
    if (SrcDialog.exec()){
        ui->txtSrcAddress->setText(SrcDialog.selectedFiles()[0]);
    }
}

void MainWindow::saveDialogBtnClicked(){

    QFileDialog SaveDialog(this);
    SaveDialog.setViewMode(QFileDialog::Detail);
    SaveDialog.setOption(QFileDialog::ShowDirsOnly,true);
    SaveDialog.setFileMode(QFileDialog::Directory);
    if (SaveDialog.exec()){
        ui->txtDesAddress->setText(SaveDialog.selectedFiles()[0]);
    }
}

void MainWindow::fileOpenDialogBtnClicked(){

    QFileDialog fileOpenDialog(this);
    fileOpenDialog.setViewMode(QFileDialog::Detail);
    if (fileOpenDialog.exec()){
        ui->txtOpenFileAddress->setText(fileOpenDialog.selectedFiles()[0]);
    }
}



void MainWindow::fileTrimScriptBtnClicked(){

    QFileDialog fileTrimScriptDialog(this);
    fileTrimScriptDialog.setViewMode(QFileDialog::Detail);
    if (fileTrimScriptDialog.exec()){
        ui->txtTrimScriptFile->setText(fileTrimScriptDialog.selectedFiles()[0]);
    }
}



void MainWindow::runBtnClicked(){


    srcPath = ui->txtSrcAddress->toPlainText();
    destPath = ui->txtDesAddress->toPlainText();
    trimScript = ui->txtTrimScriptFile->toPlainText();

    if (srcPath == "" || destPath == "" ){
        msgBox.setText("Error! Please enter path!");
        msgBox.exec();
    }
    else {
        // Make tmp dir
        program = "mkdir";
        arguments.clear();
        arguments << "/home/hanming/Desktop/tmp";
        makeTmpDir->start(program,arguments);



        // Run VisualSFM here!
        ui->statusLabel->clear();
        ui->statusLabel->setText("Running VisualSFM now...");

        ui->processLabel->setMovie(prcGif);
        prcGif->start();

        ui->runBtn->setEnabled(false);


        PrcVisualSFM->setProcessEnvironment(env);
        PrcVisualSFM->setWorkingDirectory("/home/hanming/vsfm/bin/");
        PrcVisualSFM->setEnvironment(QStringList()<<"/home/hanming/vsfm/bin/");

        QDir::setCurrent("/home/hanming/vsfm/bin/");

        program = "./VisualSFM";
        arguments.clear();
        arguments << "sfm+pmvs";
        arguments << srcPath;
        arguments << "/home/hanming/Desktop/tmp/result.nvm";

        qDebug() << PrcVisualSFM->workingDirectory();


        PrcVisualSFM->start(program,arguments);


    }
}

void MainWindow::TrimAndTransform(){
    // Run Trim And Transform here!

    ui->statusLabel->clear();
    ui->statusLabel->setText("Running MeshLab now...");


    PrcMeshlabTrimAndTransform->setProcessEnvironment(env);
    PrcMeshlabTrimAndTransform->setWorkingDirectory("/home/hanming/MeshLab/meshlab/src/distrib");

    PrcMeshlabTrimAndTransform->setEnvironment(QStringList()<<"/home/hanming/MeshLab/meshlab/src/distrib");

    QDir::setCurrent("/home/hanming/MeshLab/meshlab/src/distrib");


    program = "./meshlabserver";
    arguments.clear();
    arguments << "-i";
    arguments << "/home/hanming/Desktop/tmp/result.nvm.cmvs/00/models/option-0000.ply";
    arguments << "-o";
    arguments << "/home/hanming/Desktop/tmp/result.ply";
    arguments << "-s";
    if(trimScript != ""){
        arguments << trimScript;
    }
    else{
        arguments << "GeneralScript.mlx";
    }
    arguments << "-om";
    arguments << "vc";
    arguments << "vn";

    PrcMeshlabTrimAndTransform->start(program,arguments);

}

void MainWindow::poissonRecon(){
    // Run Possion Reconstruction here!

    ui->statusLabel->clear();
    ui->statusLabel->setText("Running Poisson Recon now...");


    PrcPoissonRecon->setProcessEnvironment(env);
    PrcPoissonRecon->setWorkingDirectory("/home/hanming/PoissonRecon/Bin/Linux");

    PrcPoissonRecon->setEnvironment(QStringList()<<"/home/hanming/PoissonRecon/Bin/Linux");


    QDir::setCurrent("/home/hanming/PoissonRecon/Bin/Linux");

    program = "./PoissonRecon";
    arguments.clear();
    arguments << "--in";
    arguments << "/home/hanming/Desktop/tmp/result.ply";
    arguments << "--out";
    arguments << "/home/hanming/Desktop/tmp/result.screened.color.ply";
    arguments << "--depth";
    arguments << "10";
    arguments << "--color";
    arguments << "16";
    arguments << "--density";

    PrcPoissonRecon->start(program,arguments);

}

void MainWindow::ply2mesh(){
    // Run ply to mesh here!
    ui->statusLabel->clear();
    ui->statusLabel->setText("Running ply to mesh now...");


    PrcPly2Mesh->setProcessEnvironment(env);
    PrcPly2Mesh->setWorkingDirectory("/home/hanming/Desktop");

    PrcPly2Mesh->setEnvironment(QStringList()<<"/usr/bin");

    QDir::setCurrent("/home/hanming/Desktop");


    program = "perl";
    arguments.clear();
    arguments << "ply2mesh";
//  arguments << "/home/hanming/Desktop/tmp/test1.screened.color.ply > /home/hanming/Desktop/tmp/test1_ascii.ply";

//  qDebug() << arguments;

//  PrcPly2Mesh->setStandardOutputFile("prcPly2MeshOutput");
//  PrcPly2Mesh->setStandardErrorFile("prcPly2MeshErr");


    PrcPly2Mesh->start(program,arguments);



//  PrcPly2Mesh->start("perl /home/hanming/Desktop/ply2mesh /home/hanming/Desktop/tmp/test1.screened.color.ply > /home/hanming/Desktop/tmp/test1_ascii.ply");
}


void MainWindow::reconComplete(){
    // Move the file to the Destination floder
    program = "mv";
    arguments.clear();
    arguments << "/home/hanming/Desktop/tmp/result_ascii.ply";
    arguments << destPath;

    moveFiles->start(program,arguments);
    moveFiles->waitForFinished();


    // Remove Tmp dir

//    program = "rm";
//    arguments.clear();
//    arguments << "-r";
//    arguments << "/home/hanming/Desktop/tmp";

//    rmTmpDir->start(program,arguments);


    ui->runBtn->setEnabled(true);
    prcGif->stop();
    ui->processLabel->clear();
    ui->statusLabel->setText("Recon has Completed!");

    // update openGL scene
    ui->openglScene->modelPath = destPath + "/result_ascii.ply";

    ui->openglScene->initializeGL();
    ui->openglScene->paintGL();

    msgBox.setText("Finished!");
    msgBox.exec();
}


void MainWindow::handleError(QProcess::ProcessError error){

    switch(error) {
       case QProcess::FailedToStart:
        qDebug() << "Failed to start, may be due to insufficient permissions";
        break;
       case QProcess::Crashed:
        qDebug() << "Program crashed.";
        break;
       case QProcess::Timedout:
        qDebug() << "Timeout.";
        break;
       case QProcess::WriteError:
        qDebug() << "WriteError";
        break;
       case QProcess::ReadError:
        qDebug() << "ReadError";
        break;
       case QProcess::UnknownError:
        qDebug() << "UnknownError";
        break;
    }
}

void MainWindow::debugAndAlert(){

    msgBox.setText("This step done!");
    msgBox.exec();

}


void MainWindow::fileOpenBtnClicked(){
    ui->openglScene->modelPath = ui->txtOpenFileAddress->toPlainText();

    ui->openglScene->initializeGL();
    ui->openglScene->paintGL();

    msgBox.setText("File has opened!");
    msgBox.exec();


}


void MainWindow::upMove(){

    ui->openglScene->up();

}

void MainWindow::downMove(){

    ui->openglScene->down();

}

void MainWindow::leftMove(){

    ui->openglScene->left();

}

void MainWindow::rightMove(){

    ui->openglScene->right();

}

