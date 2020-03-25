#include "texture.h"

QString Texture::mapTextureTypeToString(TextureType type)
{
    QString typeString;
    switch (type) {
    case Texture::diff:
        typeString = "cat diff";
        break;
    case Texture::norm:
        typeString = "cat norm";
        break;
    case Texture::spec:
        typeString = "cat spec";
        break;
    case Texture::rug:
        typeString = "rug logo";
        break;

    }
}

Texture::Texture()
{

}
