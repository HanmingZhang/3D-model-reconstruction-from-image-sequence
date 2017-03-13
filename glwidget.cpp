#include "glwidget.h"
#include <GL/glut.h>

#include <math.h>

#include "cplyloader.h"




CPLYLoader plyLoader;


GLWidget::GLWidget(QWidget *parent):
    QGLWidget (parent),
    m_xRot(0),
    m_yRot(0),
    m_zRot(0)
{
//    connect(&timer, SIGNAL(timeout()),this,SLOT(updateGL()));
//    timer.start(16);

    camDelta[0]=0;
    camDelta[1]=0;
    camDelta[2]=0;
    lookAtDelta[0]=0;
    lookAtDelta[1]=0;
    lookAtDelta[2]=0;

    modelPath="";

}


static void qNormalizeAngle(int &angle)
{
    while (angle < 0)
        angle += 360 * 16;
    while (angle > 360 * 16)
        angle -= 360 * 16;
}


void GLWidget::setXRotation(int angle)
{
    qNormalizeAngle(angle);
    if (angle != m_xRot) {
        m_xRot = angle;
        emit xRotationChanged(angle);
        update();
    }
}

void GLWidget::setYRotation(int angle)
{
    qNormalizeAngle(angle);
    if (angle != m_yRot) {
        m_yRot = angle;
        emit yRotationChanged(angle);
        update();
    }
}

void GLWidget::setZRotation(int angle)
{
    qNormalizeAngle(angle);
    if (angle != m_zRot) {
        m_zRot = angle;
        emit zRotationChanged(angle);
        update();
    }
}



void GLWidget::initializeGL(){

    lx = 0.0f;
    lz = -1.0f;
    ly = 0.0f;
//    angle = 0.0;


    glClearColor(0.2,0.2,0.2,1);
    glEnable(GL_DEPTH_TEST);

    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHTING);


    // 指定环境光强度（RGBA）
    GLfloat ambientLight[] = {1.0f, 1.0f, 1.0f, 1.0f};
    // 设置光照模型，将ambientLight所指定的RGBA强度值应用到环境光
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientLight);
    // 启用颜色追踪
    glEnable(GL_COLOR_MATERIAL);
    // 设置多边形正面的环境光和散射光材料属性，追踪glColor
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);


    if(modelPath!=""){
        //plyLoader.LoadModel("/home/hanming/Desktop/result_ascii.ply");
        temp = modelPath.toLatin1();
        c_str = temp.data();

        plyLoader.LoadModel(c_str);
    }
}

void GLWidget::paintGL(){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

//  glRotatef(0.5,1,1,1);

    glLoadIdentity();// reset transformation state
    gluLookAt(0+camDelta[0],0+camDelta[1],25+camDelta[2],
            0+lookAtDelta[0]+lx,0+lookAtDelta[1]+ly,0+lookAtDelta[2]+lz,
            0,1,0);


    glTranslatef(0.0, 0.0, -10.0); // apply translation to transformation
    glRotatef(m_xRot / 8.0, 1.0, 0.0, 0.0); // apply rotation on x
    glRotatef(m_yRot / 8.0, 0.0, 1.0, 0.0); // apply rotation on x
    glRotatef(m_zRot / 8.0, 0.0, 0.0, 1.0); // apply rotation on x


//   glColor3f(1,0.6,0);
//   glutSolidTeapot(0.8);

    if(modelPath!=""){
        plyLoader.Draw();
    }

}

void GLWidget::resizeGL(int w, int h){
    glViewport(0,0,w,h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45,(float)w/h,0.01,100.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0+camDelta[0],0+camDelta[1],25+camDelta[2], 0+lookAtDelta[0],0+lookAtDelta[1],0+lookAtDelta[2], 0,1,0);
    //    gluLookAt(0,0,5, 0,0,0, 0,1,0);
}

void GLWidget::wheelEvent(QWheelEvent *event)
{
//    if (camDelta[2]>=ZOOM_THRESHOLD || event->delta()>0) {
//        camDelta[2] += (event->delta() / 8.0 / 15.0) * WHEEL_DELTA;
//    }
//    if (lookAtDelta[2]>=ZOOM_THRESHOLD || event->delta()>0) {
//        lookAtDelta[2] += (event->delta() / 8.0 / 15.0) * WHEEL_DELTA;
//    }

    camDelta[2] += (event->delta() / 8.0 / 15.0) * WHEEL_DELTA;
    lookAtDelta[2] += (event->delta() / 8.0 / 15.0) * WHEEL_DELTA;

    updateGL();
}


void GLWidget::mousePressEvent(QMouseEvent *event)
{
    m_lastPos = event->pos();
}

void GLWidget::mouseMoveEvent(QMouseEvent *event)
{
    int dx = event->x() - m_lastPos.x();
    int dy = event->y() - m_lastPos.y();

    if (event->buttons() & Qt::LeftButton) {
        setXRotation(m_xRot + 8 * dy);
        setYRotation(m_yRot + 8 * dx);
    } else if (event->buttons() & Qt::RightButton) {
        setXRotation(m_xRot + 8 * dy);
        setZRotation(m_zRot + 8 * dx);
    }
    m_lastPos = event->pos();
}

void GLWidget::down(){

    ly += 0.2;
    updateGL();
}

void GLWidget::up(){

    ly -= 0.2;
    updateGL();

}

void GLWidget::left(){

//    angle -= 0.2f;
//    lx = sin(angle);
//    lz = -cos(angle);

    lx += 0.2;
    lz -= 0.2;

    updateGL();
}

void GLWidget::right(){

//    angle += 0.2f;
//    lx = sin(angle);
//    lz = -cos(angle);

    lx -= 0.2;
    lz += 0.2;

    updateGL();

}
