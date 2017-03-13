#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QGLWidget>
//#include <QTimer>
#include <QWheelEvent>
#include <QMouseEvent>
//#include <QMatrix4x4>

#define WHEEL_DELTA 1.0
#define ZOOM_THRESHOLD -12

class GLWidget : public QGLWidget
{
    Q_OBJECT
public:
    explicit GLWidget(QWidget *parent = 0);

    void initializeGL();
    void paintGL();
    void resizeGL(int w, int h);


    void down();
    void up();
    void left();
    void right();

    QString modelPath;
    QByteArray temp;
    char *c_str;

public slots:
    void setXRotation(int angle);
    void setYRotation(int angle);
    void setZRotation(int angle);


signals:
    void xRotationChanged(int angle);
    void yRotationChanged(int angle);
    void zRotationChanged(int angle);



protected:
    void wheelEvent(QWheelEvent *event);
    void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void mouseMoveEvent(QMouseEvent *event) Q_DECL_OVERRIDE;

private:
//    QTimer timer;

    float camDelta[3];
    float lookAtDelta[3];

    int m_xRot;
    int m_yRot;
    int m_zRot;
    QPoint m_lastPos;
    float lx;
    float lz;
    float ly;
//    float angle;

};

#endif // GLWIDGET_H
