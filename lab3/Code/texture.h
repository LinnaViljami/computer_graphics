#ifndef TEXTURE_H
#define TEXTURE_H

#include <QObject>
#include <memory>


class Texture : public QObject
{
    Q_OBJECT
    enum TextureType {
        diff, norm, spec, rug};
    Q_ENUM(TextureType)
public:
    QString fileName = ":/textures/cat_diff.png";
    int width = 512;
    int height = 1024;
    QString mapTextureTypeToString(TextureType type);
    Texture();
private:
};

#endif // TEXTURE_H
