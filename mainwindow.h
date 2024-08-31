#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <Qt3DCore/QEntity>
#include <Qt3DCore/QTransform>

#include <Qt3DExtras/Qt3DWindow>
#include <Qt3DExtras/QTorusMesh>
#include <Qt3DExtras/QForwardRenderer>
#include <Qt3DExtras/QPhongMaterial>
#include <Qt3DExtras/QDiffuseSpecularMaterial>
#include <Qt3DExtras/QOrbitCameraController>
#include <Qt3DExtras/QTextureMaterial>

#include <Qt3DRender/QCamera>
#include <Qt3DRender/QTexture>

#include <QtGui/QScreen>
#include <QKeyEvent>

QT_BEGIN_NAMESPACE

namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    Qt3DCore::QEntity *createScene();

private:
    Ui::MainWindow *ui;
    void keyPressEvent(QKeyEvent *e) override;
    QVector3D position;
    Qt3DRender::QCamera *camera;
private slots:
    void onFocusChanged();
    void onPositionChanged();
};
#endif // MAINWINDOW_H
