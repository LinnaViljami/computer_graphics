#include "object.h"
#include <QtMath>

ImportedObject::ImportedObject() : vertices({}), translationMatrix(), rotationMatrix(), textureCoordinates(), scalingMatrix()
{

}

ImportedObjectProperties ImportedObject::getModelProperties(ImportedObjectType modelType, Material material)
{
    ImportedObjectProperties properties;
    switch (modelType) {
    case cat:
        properties.model = Model(":/models/cat.obj");
        properties.scalingFactor = 8.0f;
        break;
    case sphere:
        properties.model = Model(":/models/sphere.obj");
        properties.scalingFactor = 0.04f;
        break;
    case cube:
        break;
    case flat_surface:
        break;
    }
    properties.material = material;
    return properties;
}


ImportedObject::ImportedObject(ImportedObjectType objectType, Material material)
{
    modelProps = getModelProperties(objectType, material);
    QVector<QVector3D> vertexLocations = modelProps.model.getVertices();
    QVector<QVector3D> vertexNormals = modelProps.model.getNormals();
    textureCoordinates = modelProps.model.getTextureCoords();

    std::vector<vertex3d> vertices_init = {};
    for (int i = 0; i < vertexLocations.size(); i++) {
        QVector3D location = vertexLocations.at(i);
        QVector3D normal = vertexNormals.at(i);
        QVector2D textureCoordinate = textureCoordinates.at(i);
        vertices_init.push_back({
            location.x() * modelProps.scalingFactor,
            location.y() * modelProps.scalingFactor,
            location.z() * modelProps.scalingFactor,
            normal.x(),
            normal.y(),
            normal.z(),
            textureCoordinate.x(),
            textureCoordinate.y()
       });
    }

    vertices = vertices_init;
    translationMatrix.setToIdentity();
    rotationMatrix.setToIdentity();
    scalingMatrix.setToIdentity();
}

QMatrix4x4 ImportedObject::getModelTransformationMatrix()
{
    return translationMatrix * rotationMatrix * scalingMatrix;
}

QVector4D ImportedObject::getMaterialVector()
{
    return {
            modelProps.material.ka,
            modelProps.material.kd,
            modelProps.material.ks,
            modelProps.material.phongExponent
        };
}

// This method returns the color of the object. Currently, it returns the same color
// for all object types. This may be
QVector3D ImportedObject::getMaterialColorVector()
{
    return {0.4f, 0.4f, 0.4f};
}

void ImportedObject::setRotation(int rotateX, int rotateY, int rotateZ)
{
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

    rotationMatrix = xRotationMatrix * yRotationMatrix * zRotationMatrix;
}

void ImportedObject::setScale(int scale)
{
    scalingMatrix = {
                static_cast<float>(scale)/100 , 0, 0, 0,
                0, static_cast<float>(scale)/100, 0, 0,
                0, 0, static_cast<float>(scale)/100, 0,
                0, 0, 0, 1,
    };
}

void ImportedObject::setTranslation(float translateX, float translateY, float translateZ)
{
    translationMatrix = {
                1, 0, 0, translateX,
                0, 1, 0, translateY,
                0, 0, 1, translateZ,
                0, 0, 0, 1,
        };

}

