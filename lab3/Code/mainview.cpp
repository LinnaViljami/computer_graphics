#include "mainview.h"
#include <QDateTime>
#include <QtMath>
#include <cstdlib>


/**
 * @brief MainView::MainView
 *
 * Constructor of MainView
 *
 * @param parent
 */
MainView::MainView(QWidget *parent) : QOpenGLWidget(parent){
    connect(&timer, SIGNAL(timeout()), this, SLOT(update()));

}

/**
 * @brief MainView::~MainView
 *
 * Destructor of MainView
 * This is the last function called, before exit of the program
 * Use this to clean up your variables, buffers etc.
 *
 */
MainView::~MainView() {
    qDebug() << "MainView destructor";
    for(auto& object : objects){
        glDeleteBuffers(1, &object.second.vboId);
        glDeleteVertexArrays(1, &object.second.vaoId);
    }
    makeCurrent();
}

// --- OpenGL initialization

/**
 * @brief MainView::initializeGL
 *
 * Called upon OpenGL initialization
 * Attaches a debugger and calls other init functions
 */
void MainView::initializeGL() {
    qDebug() << ":: Initializing OpenGL";
    initializeOpenGLFunctions();

    connect(&debugLogger, SIGNAL( messageLogged( QOpenGLDebugMessage)),
            this, SLOT(onMessageLogged(QOpenGLDebugMessage)), Qt::DirectConnection);

    if (debugLogger.initialize()) {
        qDebug() << ":: Logging initialized";
        debugLogger.startLogging(QOpenGLDebugLogger::SynchronousLogging);
    }


    QString glVersion;
    glVersion = reinterpret_cast<const char*>(glGetString(GL_VERSION));
    qDebug() << ":: Using OpenGL" << qPrintable(glVersion);

    glEnable(GL_DEPTH_TEST);

    glEnable(GL_CULL_FACE);

    glDepthFunc(GL_LEQUAL);

    // Set the color to be used by glClear. This is, effectively, the background color.
    glClearColor(0.2f, 0.5f, 0.7f, 0.0f);
    createShaderPrograms(Shader::PHONG);
    initializeObject(SceneObject::Goat, ImportedObjectType::cat);
    currentTextureType = TextureType::Diff;
    loadTexture(getTextureFileName(currentTextureType), textureLocation);
}

// Adds and links a vertex shader and a fragment shader, based on which ShaderType
// is passed as parameter.
void MainView::createShaderPrograms(Shader::ShadingMode shadingMode) {
    phongShader.init();
    normalShader.init();
    setShadingMode(shadingMode);
}

QString MainView::getTextureFileName(TextureType texture)
{
    QString fileName;
    int width = 512;
    int height = 1024;
    switch (texture) {
    case NoTexture:
        fileName = "no texture";
        break;
    case Diff:
        fileName = ":/textures/cat_diff.png";
        break;
    case Norm:
        fileName = ":/textures/cat_norm.png";
        break;
    case Spec:
        fileName = ":/textures/cat_spec.png";
        break;
    case Rug:
        fileName = ":/textures/rug_logo.png";
        width = 1024;
        break;
    }
    return fileName;
}


void MainView::initializeObject(SceneObject objectId, ImportedObjectType type) {


    objects[objectId] = ImportedObject(type);
    ImportedObject* object = &objects.at(objectId);
    glGenBuffers(1, &object->vboId);
    glGenVertexArrays(1, &object->vaoId);

    glBindVertexArray(object->vaoId);
    glBindBuffer(GL_ARRAY_BUFFER, object->vboId);

    glBufferData(GL_ARRAY_BUFFER, object->vertices.size()*sizeof(vertex3d), object->vertices.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

    GLintptr coordinatPtrIndex = 0*sizeof(float);
    GLintptr colorPtrIndex = offsetof(vertex3d, normalX);
    GLintptr texturePtrIndex = offsetof(vertex3d, textureX);

    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,sizeof(vertex3d), (GLvoid*)(coordinatPtrIndex));
    glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,sizeof(vertex3d), (GLvoid*)(colorPtrIndex));
    glVertexAttribPointer(2,2,GL_FLOAT, GL_FALSE, sizeof(vertex3d), (GLvoid*)(texturePtrIndex));

    // generate textures
    glGenTextures(1, &textureLocation);

}


void MainView::setDataToUniform(SceneObject objectId)
{
    ImportedObject& object = objects.at(objectId);
    switch (currentShader->type()) {
    case Shader::PHONG:
        phongShader.setUniformData(object.getModelTransformationMatrix(),
                                   perspectiveTransformationMatrix,
                                   object.rotationMatrix.normalMatrix(),
                                   object.getMaterialVector(),
                                   getLightPosition(),
                                   getLightColor(),
                                   useTextures);
        break;
    case Shader::NORMAL:
        normalShader.setUniformData(object.getModelTransformationMatrix(),
                                   perspectiveTransformationMatrix,
                                   object.rotationMatrix.normalMatrix());
        break;
    case Shader::GOURAUD:
        qDebug("gouraud shader not implemented");
        break;
    }
}

QVector3D MainView::getLightPosition()
{
    return {
        0.0f,
        100.0f,
        0.0f
    };
}

QVector3D MainView::getLightColor()
{
    return {
        1.0f,
        1.0f,
        1.0f
    };
}

void MainView::loadTexture(QString fileName, GLuint texturePtr)
{

        glBindTexture(GL_TEXTURE_2D, texturePtr);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

        // Push image data to texture.
        QImage image(fileName);
        QVector<quint8> imageData = imageToBytes(image);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, image.width(), image.height(),
                     0, GL_RGBA, GL_UNSIGNED_BYTE, imageData.data());




}


void MainView::paintObject(SceneObject objectId)
{
    ImportedObject& object = objects.at(objectId);
    qDebug("Paint object called");
    useTextures = true;
    object.setTranslation(0, -3, -10);

    GLint * textureUniformLocation = currentShader->getTextureBufferLocation();
    currentTextureType = TextureType::Diff;
    if(textureUniformLocation != nullptr && currentTextureType != TextureType::NoTexture){
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D,textureLocation);
        glUniform1i(*textureUniformLocation, 0);

    }
    setDataToUniform(objectId);
    glBindVertexArray(object.vaoId);
    glDrawArrays(GL_TRIANGLES, 0, object.vertices.size());
}


// --- OpenGL drawing

/**
 * @brief MainView::paintGL
 *
 * Actual function used for drawing to the screen
 *
 */
void MainView::paintGL() {
    currentShader->bind();
    // Clear the screen before rendering
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    paintObject(SceneObject::Goat);
    currentShader->release();
}

/**
 * @brief MainView::resizeGL
 *
 * Called upon resizing of the screen
 *
 * @param newWidth
 * @param newHeight
 */
void MainView::resizeGL(int newWidth, int newHeight) {
    qDebug() << "resize called.";
    perspectiveTransformationMatrix.setToIdentity();
    perspectiveTransformationMatrix.perspective(60, ((float)newWidth)/newHeight, 0.2f, 20.0f);
}

// --- Public interface

void MainView::setRotation(int rotateX, int rotateY, int rotateZ) {
    for (auto& object : objects){
        object.second.setRotation(rotateX, rotateY, rotateZ);
    }

    update();
}

void MainView::setScale(int scale) {
    for (auto& object : objects){
        object.second.setScale(scale);
    }
    update();
}

void MainView::setShadingMode(Shader::ShadingMode shading) {
    qDebug() << "Changed shading to" << shading;
    switch (shading) {
    case Shader::PHONG:
        currentShader = &phongShader;
        break;
    case Shader::NORMAL:
        currentShader = &normalShader;
        break;
    case Shader::GOURAUD:
        qDebug("gouraud shader not implemented");
        break;
    }
}

// --- Private helpers ---

/**
 * @brief MainView::onMessageLogged
 *
 * OpenGL logging function, do not change
 *
 * @param Message
 */
void MainView::onMessageLogged( QOpenGLDebugMessage Message ) {
    qDebug() << " → Log:" << Message;
}
