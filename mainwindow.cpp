#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //creating a window to visualize 3D content & set the scene inside it
    Qt3DExtras::Qt3DWindow *window = new Qt3DExtras::Qt3DWindow();
    //setting background color of the scene
    window->defaultFrameGraph()->setClearColor(QColorConstants::DarkGray);
    //setting root entity of the window
    window->setRootEntity(createScene());

    //creating a widget to act as a container for the window to embed it inside the mainWindow
    QWidget *container = QWidget::createWindowContainer(window);
    container->setMinimumSize(QSize(200, 100));
    QSize screenSize = window->screen()->size();
    container->setMaximumSize(screenSize);

    //Set the container as the central widget of the MainWindow
    setCentralWidget(container);
    camera = window->camera();
    camera->setPosition(QVector3D(0.0f, 0.0f, 75.0f));
    position = camera->position();
    camera->setUpVector(QVector3D(0, 1, 0));
    camera->setViewCenter(QVector3D(0.0f, 0.0f, 0.0f));

    auto cameraController = new Qt3DExtras::QOrbitCameraController(camera);
    cameraController->setCamera(camera);
    cameraController->setLinearSpeed(10.0f);
    cameraController->setLookSpeed(180.0f);

    //----connecting signals to slots----//
    connect(qApp, &QApplication::focusChanged, this, &MainWindow::onFocusChanged);
    connect(camera, &Qt3DRender::QCamera::positionChanged, this, &MainWindow::onPositionChanged);

    //setting focus to main window
    this->setFocusPolicy(Qt::StrongFocus);

    this->setWindowTitle("3D-TunnelSimulation");
}

MainWindow::~MainWindow()
{
    delete ui;
}

Qt3DCore::QEntity *MainWindow::createScene()
{
    auto scene = new Qt3DCore::QEntity;
    //creating Multiple Torus Mesh, Torus Transform and Texture for each mesh
    for(int i=0; i<15; i++){
        Qt3DExtras::QTorusMesh *m_torus = new Qt3DExtras::QTorusMesh(scene);
        auto tunnel = new Qt3DCore::QEntity(scene);
        m_torus->setRadius(6.0f);
        m_torus->setMinorRadius(2.0f);
        m_torus->setRings(200);
        m_torus->setSlices(100);

        Qt3DCore::QTransform *torusTransform = new Qt3DCore::QTransform(scene);
        torusTransform->setScale(1.0f);
        torusTransform->setTranslation(QVector3D(0.0f, 0.0f, -i*5 + 50));

        /*Adding Texture to the Torus Mesh by
          Create a texture image and load data from an image file*/
        auto *texture = new Qt3DRender::QTexture2D(scene);
        texture->setMagnificationFilter(Qt3DRender::QTexture2D::Linear);
        texture->setMinificationFilter(Qt3DRender::QTexture2D::LinearMipMapLinear);
        texture->setWrapMode(Qt3DRender::QTextureWrapMode(Qt3DRender::QTextureWrapMode::MirroredRepeat));

        auto *textureImage = new Qt3DRender::QTextureImage();
        textureImage->setSource(QUrl::fromLocalFile(":/Texture/Texture/image.png"));  // Replace with your image path
        texture->addTextureImage(textureImage);

        auto *textureMaterial = new Qt3DExtras::QTextureMaterial(scene);
        textureMaterial->setTexture(texture);

        tunnel->addComponent(m_torus);
        tunnel->addComponent(torusTransform);
        tunnel->addComponent(textureMaterial);
        m_torus->setEnabled(true);
    }
    return scene;
}

void MainWindow::keyPressEvent(QKeyEvent *e)
{
    // A switch statement to function differently once certain keyboard buttons are pressed
    switch(e->key()){

        case Qt::Key_W:
            // "Move Forward" by decrementing in z position
            if(position.z()-0.1 >= 0.1)
                position.setZ(position.z()-0.1);
            break;

        case Qt::Key_S:
            // "Move Backward" by incrementing in Z position
            position.setZ(position.z()+0.1);
            break;

        case Qt::Key_A:
            // "Move Left" by decrementing in X position
            position.setX(position.x()-0.1);
            break;
        case Qt::Key_D:
            // "Move Right" by incrementing in X position
            position.setX(position.x()+0.1);
            break;

        case Qt::Key_Q:
            // "Move Up" by incrementing in Y position
            position.setY(position.y()+0.1);
            break;

        case Qt::Key_E:
            // "Move Down" by decrementing in Y position
            position.setY(position.y()-0.1);
            break;

        case Qt::Key_Escape:
            // Restore the original start position of the tunnel
            position.setX(0.0f);
            position.setY(0.0f);
            position.setZ(75.0f);
    }
    camera->setPosition(position);
}

void MainWindow::onFocusChanged()
{
    //Slot to set the focus back to main window if slot focusChanged is invoked
    this->setFocus();
}

void MainWindow::onPositionChanged()
{
    /*Slot to set the camera new position to the QVector position,
       to keep track if mouse wheel event changed the position*/
    position.setX(camera->position().x());
    position.setY(camera->position().y());
    position.setZ(camera->position().z());
}
