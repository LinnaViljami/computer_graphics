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
MainView::MainView(QWidget *parent) : QOpenGLWidget(parent) {
    qDebug() << "MainView constructor";

    connect(&timer, SIGNAL(timeout()), this, SLOT(update()));
    _rotation_matrix = {
        1 , 0, 0, 0,
        0, 1, 0, 0,
        0, 0, 1, 0,
        0, 0, 0, 1,
};
    _scaling_matrix = _rotation_matrix;
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
    glDeleteBuffers(1, &_object.vbo_id);
    glDeleteVertexArrays(1, &_object.vao_id);
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

    //QOpenGLFunctions_3_3_Core* pointer_to_functions = (QOpenGLFunctions_3_3_Core*)this->context()->versionFunctions();

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
    setUniformLocation();

    initializePerspectiveMatrix();

    initializeObject();
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

void MainView::setUniformLocation() {
    _transformationUniformLocation = shaderProgram.uniformLocation("transformation");
    _projectionUniformLocation = shaderProgram.uniformLocation("projection");
    if (_transformationUniformLocation == -1 || _projectionUniformLocation == -1) {
        // Did not find uniform
        qDebug() << "Failed to find uniform in setUniformLocation()";
    }
}

void MainView::initializePerspectiveMatrix() {
    //projection transformation
    float n = 0.2f;
    float f = 20.0f;
    float t = 1.0f;
    float l = -1.0f;
    float r = 1.0f;
    float b = -1.0f;
    _perspective_transformation_matrix = {
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
    shaderProgram.bind();

    // Clear the screen before rendering
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


    // --- PAINT CUBE ---
    _cube_translation_matrix = {
            1, 0, 0, 2,
            0, 1, 0, 0,
            0, 0, 1, -6,
            0, 0, 0, 1,
    };

    QMatrix4x4 cube_transformation_matrix = _cube_translation_matrix * _rotation_matrix * _scaling_matrix;
    glUniformMatrix4fv(_transformationUniformLocation, 1, GL_FALSE, cube_transformation_matrix.data());
    glUniformMatrix4fv(_projectionUniformLocation, 1, GL_FALSE, _perspective_transformation_matrix.data());

    glBindVertexArray(this->_cube.vao_id);
    glDrawArrays(GL_TRIANGLES, 0, 36);


    // --- PAINT PYRAMID ---
    _pyramid_translation_matrix = {
            1, 0, 0, -2,
            0, 1, 0, 0,
            0, 0, 1, -6,
            0, 0, 0, 1,
    };

    QMatrix4x4 pyramid_transformation_matrix = _pyramid_translation_matrix * _rotation_matrix * _scaling_matrix;
    glUniformMatrix4fv(_transformationUniformLocation, 1, GL_FALSE, pyramid_transformation_matrix.data());
    glUniformMatrix4fv(_projectionUniformLocation, 1, GL_FALSE, _perspective_transformation_matrix.data());

    glBindVertexArray(this->_pyramid.vao_id);
    glDrawArrays(GL_TRIANGLES, 0, 18);


    // --- PAINT OBJECT ---
    _object_translation_matrix = {
            1, 0, 0, 0,
            0, 1, 0, 0,
            0, 0, 1, -10,
            0, 0, 0, 1,
    };

    QMatrix4x4 objectTransformationMatrix = _object_translation_matrix * _rotation_matrix * _scaling_matrix;
    glUniformMatrix4fv(_transformationUniformLocation, 1, GL_FALSE, objectTransformationMatrix.data());
    glUniformMatrix4fv(_projectionUniformLocation, 1, GL_FALSE, _perspective_transformation_matrix.data());

    glBindVertexArray(_object.vao_id);
    glDrawArrays(GL_TRIANGLES, 0, _object.vertices.size());

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
    // This time use .perspective function. For own implementation, see initializePerspectiveMatrix() method.
    qDebug() << "resize called.";
    _perspective_transformation_matrix.setToIdentity();
    _perspective_transformation_matrix.perspective(60, ((float)newWidth)/newHeight, 0.2f, 20.0f);
}

// --- Public interface

void MainView::setRotation(int rotateX, int rotateY, int rotateZ) {

    qreal rotx_rad = static_cast<qreal>(rotateX)*2.0*3.141/360.0;
    QMatrix4x4 x_rotation_matrix = {
            1 , 0, 0, 0,
            0, static_cast<float>(qCos(rotx_rad)), static_cast<float>(-qSin(rotx_rad)), 0,
            0, static_cast<float>(qSin(rotx_rad)), static_cast<float>(qCos(rotx_rad)), 0,
            0, 0, 0, 1,
    };

    qreal roty_rad = static_cast<qreal>(rotateY)*2.0*3.141/360.0;
    QMatrix4x4 y_rotation_matrix = {
            static_cast<float>(qCos(roty_rad)) , 0, static_cast<float>(qSin(roty_rad)), 0,
            0, 1, 0, 0,
            static_cast<float>(-qSin(roty_rad)), 0, static_cast<float>(qCos(roty_rad)), 0,
            0, 0, 0, 1,
    };

    qreal rotz_rad = static_cast<qreal>(rotateZ)*2.0*3.141/360.0;
    QMatrix4x4 z_rotation_matrix = {
        static_cast<float>(qCos(rotz_rad)), static_cast<float>(-qSin(rotz_rad)), 0,0,
        static_cast<float>(qSin(rotz_rad)), static_cast<float>(qCos(rotz_rad)), 0, 0,
        0,0,1,0,
        0, 0, 0, 1,
    };

    _rotation_matrix = x_rotation_matrix * y_rotation_matrix * z_rotation_matrix;
    update();
    qDebug() << "Rotation changed to (" << rotateX << "," << rotateY << "," << rotateZ << ")";
}

void MainView::setScale(int scale) {
    _scaling_matrix = {
            static_cast<float>(scale)/100 , 0, 0, 0,
            0, static_cast<float>(scale)/100, 0, 0,
            0, 0, static_cast<float>(scale)/100, 0,
            0, 0, 0, 1,
    };
    update();
    qDebug() << "Scale changed to " << scale;
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

    // Generate VBO and VAO
    glGenBuffers(1, &this->_cube.vbo_id);
    glGenVertexArrays(1, &this->_cube.vao_id);


    std::vector<vertex_3d>::iterator buffer_data_pointer = this->_cube.get_triangles()->begin();
    void * buffer_void_pointer = &* buffer_data_pointer;
    glBindVertexArray(this->_cube.vao_id);
    glBindBuffer(GL_ARRAY_BUFFER, this->_cube.vbo_id);
    glBufferData(GL_ARRAY_BUFFER,36*sizeof(vertex_3d), buffer_void_pointer, GL_STATIC_DRAW);

    // Tell OpenGL what attributes to expect; how the data is laid out
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    GLintptr coordinate_ptr_index = 0*sizeof(float);
    GLintptr color_ptr_index = offsetof(vertex_3d, R);

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
    this->_pyramid = Pyramid();
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
    GLintptr color_ptr_index = offsetof(vertex_3d, R);

    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,sizeof(vertex_3d), (GLvoid*)(coordinate_ptr_index));
    glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,sizeof(vertex_3d), (GLvoid*)(color_ptr_index));

}

void MainView::initializeObject() {

    float object_scaling_factor = 0.04f;
    _object = ImportedObject(object_scaling_factor);

    glGenBuffers(1, &this->_object.vbo_id);
    glGenVertexArrays(1, &this->_object.vao_id);

    glBindVertexArray(_object.vao_id);
    glBindBuffer(GL_ARRAY_BUFFER, this->_object.vbo_id);

    glBufferData(GL_ARRAY_BUFFER, _object.vertices.size()*sizeof(vertex_3d), _object.vertices.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    GLintptr coordinate_ptr_index = 0*sizeof(float);
    GLintptr color_ptr_index = offsetof(vertex_3d, R);

    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,sizeof(vertex_3d), (GLvoid*)(coordinate_ptr_index));
    glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,sizeof(vertex_3d), (GLvoid*)(color_ptr_index));
}
