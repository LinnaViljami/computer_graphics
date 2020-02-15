#include "mainview.h"
#include <QDateTime>
/**
 * @brief MainView::MainView
 *
 * Constructor of MainView
 *
 * @param parent
 */
MainView::MainView(QWidget *parent) : QOpenGLWidget(parent) {
    qDebug() << "MainView constructor";

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
    glDeleteBuffers(1, &this->_cube.vbo_id);
    glDeleteVertexArrays(1, &this->_cube.vao_id);
    glDeleteBuffers(1, &this->_pyramid.vbo_id);
    glDeleteVertexArrays(1, &this->_pyramid.vao_id);
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

    // Default is GL_LESS
    glDepthFunc(GL_LEQUAL);

    // Set the color to be used by glClear. This is, effectively, the background color.
    glClearColor(0.2f, 0.5f, 0.7f, 0.0f);

    createShaderProgram();

    const char *translationUniformName;
    translationUniformName = "modelTranslation";

    _translationUniformLocation = shaderProgram.uniformLocation(translationUniformName);
    _transformationUniformLocation = shaderProgram.uniformLocation("modelTransformation");
    if (_translationUniformLocation == -1 || _transformationUniformLocation == -1) {
        // Did not find uniform
        qDebug() << "Failed to find uniform in createShaderProgram()";
    }

    initializeCube();
    initializePyramid();
}

void MainView::createShaderProgram() {
    // Create shader program
    shaderProgram.addShaderFromSourceFile(QOpenGLShader::Vertex,
                                           ":/shaders/vertshader.glsl");
    shaderProgram.addShaderFromSourceFile(QOpenGLShader::Fragment,
                                           ":/shaders/fragshader.glsl");
    shaderProgram.link();
}

// --- OpenGL drawing

/**
 * @brief MainView::paintGL
 *
 * Actual function used for drawing to the screen
 *
 */
void MainView::paintGL() {
    shaderProgram.bind();

    // Clear the screen before rendering
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


    // --- PAINT CUBE ---
    QMatrix4x4 cubeTranslation = {
            1, 0, 0, 0,
            0, 1, 0, 0,
            0, 0, 1, 0,
            2, 0, -6, 1,
    };

    // l = -4, r = 4, b = -4, t = 4, n = -1
    // alpha = 60
    // f = - (t - b) / tan(alpha) = -25
    QMatrix4x4 cubeTransformation = {
        0.25f, 0, 0, 0,
        0, 0.25f, 0, 0,
        0, 0, -1.0833f, -2.0833f,
        0, 0, -1, 0,
    };

    glUniformMatrix4fv(_translationUniformLocation, 1, GL_FALSE, cubeTranslation.data());
    glUniformMatrix4fv(_transformationUniformLocation, 1, GL_FALSE, cubeTransformation.data());

    glBindVertexArray(this->_cube.vao_id);
    glDrawArrays(GL_TRIANGLES, 0, 36);


    // --- PAINT PYRAMID ---
//    QMatrix4x4 pyramidTranslation = {
//            1, 0, 0, 0,
//            0, 1, 0, 0,
//            0, 0, 1, 0,
//            0, 0, 0, 1,
//    };

//    glUniformMatrix4fv(_translationUniformLocation, 1, GL_FALSE, pyramidTranslation.data());

//    glBindVertexArray(this->_pyramid.vao_id);
//    glDrawArrays(GL_TRIANGLES, 0, 18);

    shaderProgram.release();
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
    // TODO: Update projection to fit the new aspect ratio
    Q_UNUSED(newWidth)
    Q_UNUSED(newHeight)
}

// --- Public interface

void MainView::setRotation(int rotateX, int rotateY, int rotateZ) {
    qDebug() << "Rotation changed to (" << rotateX << "," << rotateY << "," << rotateZ << ")";
    Q_UNIMPLEMENTED();
}

void MainView::setScale(int scale) {
    qDebug() << "Scale changed to " << scale;
    Q_UNIMPLEMENTED();
}

void MainView::setShadingMode(ShadingMode shading) {
    qDebug() << "Changed shading to" << shading;
    Q_UNIMPLEMENTED();
}

// --- Private helpers

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

void MainView::initializeCube()
{
    this->_cube = Cube();
    glGenBuffers(1, &this->_cube.vbo_id);
    glGenVertexArrays(1, &this->_cube.vao_id);


    std::vector<vertex_3d>::iterator buffer_data_pointer = this->_cube.get_triangles()->begin();
    void * buffer_void_pointer = &* buffer_data_pointer;
    glBindVertexArray(this->_cube.vao_id);
    glBindBuffer(GL_ARRAY_BUFFER, this->_cube.vbo_id);
    glBufferData(GL_ARRAY_BUFFER,36*sizeof(vertex_3d), buffer_void_pointer, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);


    GLintptr coordinate_ptr_index = 0*sizeof(float);
    GLintptr color_ptr_index = 3*sizeof(float);

    glVertexAttribPointer(
                0,                              // Index of the attribute defined by glEnableVertexAttribArray
                3,                              // Number of elements per vertex
                GL_FLOAT,                       // Type of the elements
                GL_FALSE,
                sizeof(vertex_3d),              // Offset between different vertices
                (GLvoid*)(coordinate_ptr_index) // Offset from the start of this vertex
    );
    glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,sizeof(vertex_3d), (GLvoid*)(color_ptr_index));
}

void MainView::initializePyramid()
{
    this->_pyramid = pyramid();
    glGenBuffers(1, &this->_pyramid.vbo_id);
    glGenVertexArrays(1, &this->_pyramid.vao_id);


    std::vector<vertex_3d>::iterator buffer_data_pointer = this->_pyramid.get_triangles()->begin();
    void * buffer_void_pointer = &* buffer_data_pointer;
    glBindVertexArray(this->_pyramid.vao_id);
    glBindBuffer(GL_ARRAY_BUFFER, this->_pyramid.vbo_id);
    glBufferData(GL_ARRAY_BUFFER,18*sizeof(vertex_3d), buffer_void_pointer, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);


    GLintptr coordinate_ptr_index = 0*sizeof(float);
    GLintptr color_ptr_index = 3*sizeof(float);

    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,sizeof(vertex_3d), (GLvoid*)(coordinate_ptr_index));
    glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,sizeof(vertex_3d), (GLvoid*)(color_ptr_index));

}
