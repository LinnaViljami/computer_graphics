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
    glDeleteBuffers(1, &object.vboId);
    glDeleteVertexArrays(1, &object.vaoId);
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

    // Enable depth buffer
    glEnable(GL_DEPTH_TEST);

    // Enable backface culling
    glEnable(GL_CULL_FACE);
//    glEnable(GL_TEXTURE_2D);
    // Default is GL_LESS
    glDepthFunc(GL_LEQUAL);

    // Set the color to be used by glClear. This is, effectively, the background color.
    glClearColor(0.2f, 0.5f, 0.7f, 0.0f);

    createShaderPrograms(Shader::PHONG);

    initializePerspectiveMatrix();

    initializeObject();
}

// Adds and links a vertex shader and a fragment shader, based on which ShaderType
// is passed as parameter.
void MainView::createShaderPrograms(Shader::ShadingMode shadingMode) {
    phongShader.init();
    normalShader.init();
    gouraudShader.init();
    setShadingMode(shadingMode);
}

TextureProperties MainView::getTextureProperties(TextureType texture)
{
    QString fileName;
    int width = 512;
    int height = 1024;
    switch (texture) {
    case NoTexture:
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
    return TextureProperties{fileName, width, height};
}






void MainView::initializeObject() {

    object = ImportedObject(cat);

    glGenBuffers(1, &this->object.vboId);
    glGenVertexArrays(1, &this->object.vaoId);

    glBindVertexArray(object.vaoId);
    glBindBuffer(GL_ARRAY_BUFFER, this->object.vboId);

    glBufferData(GL_ARRAY_BUFFER, object.vertices.size()*sizeof(vertex3d), object.vertices.data(), GL_STATIC_DRAW);

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


void MainView::setDataToUniform()
{
    switch (currentShader->type()) {
    case Shader::PHONG:
        phongShader.setUniformData(object.getModelTransformationMatrix(),
                                   perspectiveTransformationMatrix,
                                   object.rotationMatrix.normalMatrix(),
                                   object.getMaterialVector(),
                                   object.getMaterialColorVector(),
                                   getLightPosition(),
                                   getLightColor(),
                                   getPhongExponent(),
                                   useTextures);
        break;
    case Shader::NORMAL:
        normalShader.setUniformData(object.getModelTransformationMatrix(),
                                   perspectiveTransformationMatrix,
                                   object.rotationMatrix.normalMatrix());
        break;
    case Shader::GOURAUD:
        gouraudShader.setUniformData(object.getModelTransformationMatrix(),
                                     perspectiveTransformationMatrix,
                                     object.rotationMatrix.normalMatrix(),
                                     object.getMaterialVector(),
                                     object.getMaterialColorVector(),
                                     getLightPosition(),
                                     getLightColor(),
                                     getPhongExponent(),
                                     useTextures);
        break;
    }
}

QVector3D MainView::getLightPosition()
{
    return {
        -4.0f,
        6.0f,
        8.0f
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

void MainView::loadTexture(TextureProperties properties, GLuint texturePtr)
{

        glBindTexture(GL_TEXTURE_2D, texturePtr);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        textureData = imageToBytes(QImage(properties.fileName));
        glTexImage2D(GL_TEXTURE_2D, 0,GL_RGBA8, properties.width,properties.height,0,GL_RGBA, GL_UNSIGNED_BYTE, textureData.data());
        glGenerateMipmap(GL_TEXTURE_2D);




}

float MainView::getPhongExponent()
{
    return phongExponent;
}

void MainView::paintObject()
{
    useTextures = true;

    object.translationMatrix = {
            1, 0, 0, 0,
            0, 1, 0, -3,
            0, 0, 1, -10,
            0, 0, 0, 1,
    };
    GLint * textureUniformLocation = currentShader->getTextureBufferLocation();
    currentTextureType = TextureType::Diff;
    if(textureUniformLocation != nullptr && currentTextureType != TextureType::NoTexture){
        loadTexture(getTextureProperties(currentTextureType), textureLocation);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D,textureLocation);
        glUniform1i(*textureUniformLocation, 0);

    }
    setDataToUniform();
    glBindVertexArray(object.vaoId);
    glDrawArrays(GL_TRIANGLES, 0, object.vertices.size());
}

void MainView::initializePerspectiveMatrix() {
    //projection transformation
    float n = 0.2f;
    float f = 20.0f;
    float t = 1.0f;
    float l = -1.0f;
    float r = 1.0f;
    float b = -1.0f;
    perspectiveTransformationMatrix = {
        (2*n/r-l), 0, (r+l)/(r-l), 0,
        0, (2*n/t-b), (t+b)/(t-b), 0,
        0, 0, (n+f)/(n-f), (2*n*f)/(n-f),
        0, 0, -1, 0,
    };
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



    paintObject();

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
    // This time use .perspective function. For own implementation, see initializePerspectiveMatrix() method.
    qDebug() << "resize called.";
    perspectiveTransformationMatrix.setToIdentity();
    perspectiveTransformationMatrix.perspective(60, ((float)newWidth)/newHeight, 0.2f, 20.0f);
}

// --- Public interface

void MainView::setRotation(int rotateX, int rotateY, int rotateZ) {

    qreal rotx_rad = static_cast<qreal>(rotateX)*2.0*3.141/360.0;
    QMatrix4x4 xRotationMatrix = {
            1 , 0, 0, 0,
            0, static_cast<float>(qCos(rotx_rad)), static_cast<float>(-qSin(rotx_rad)), 0,
            0, static_cast<float>(qSin(rotx_rad)), static_cast<float>(qCos(rotx_rad)), 0,
            0, 0, 0, 1,
    };

    qreal roty_rad = static_cast<qreal>(rotateY)*2.0*3.141/360.0;
    QMatrix4x4 yRotationMatrix = {
            static_cast<float>(qCos(roty_rad)) , 0, static_cast<float>(qSin(roty_rad)), 0,
            0, 1, 0, 0,
            static_cast<float>(-qSin(roty_rad)), 0, static_cast<float>(qCos(roty_rad)), 0,
            0, 0, 0, 1,
    };

    qreal rotz_rad = static_cast<qreal>(rotateZ)*2.0*3.141/360.0;
    QMatrix4x4 zRotationMatrix = {
        static_cast<float>(qCos(rotz_rad)), static_cast<float>(-qSin(rotz_rad)), 0,0,
        static_cast<float>(qSin(rotz_rad)), static_cast<float>(qCos(rotz_rad)), 0, 0,
        0,0,1,0,
        0, 0, 0, 1,
    };

    object.rotationMatrix = xRotationMatrix * yRotationMatrix * zRotationMatrix;

    update();
    qDebug() << "Rotation changed to (" << rotateX << "," << rotateY << "," << rotateZ << ")";
}

void MainView::setScale(int scale) {
    object.scalingMatrix = {
            static_cast<float>(scale)/100 , 0, 0, 0,
            0, static_cast<float>(scale)/100, 0, 0,
            0, 0, static_cast<float>(scale)/100, 0,
            0, 0, 0, 1,
    };
    update();
    qDebug() << "Scale changed to " << scale;
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
        currentShader = &gouraudShader;
        break;
    }
}

void MainView::updatePhongExponentValue(float value)
{
    qDebug() << "Updating Phong exponent to " << value;
    phongExponent = value;
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
