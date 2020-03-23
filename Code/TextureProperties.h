#ifndef TEXTUREPROPERTIES_H
#define TEXTUREPROPERTIES_H

#include <qstring.h>

Q_OBJECT

enum TextureType {
    diff, norm, spec, rug};

 = new QMap<QString, TextureType>;

struct TextureProperties {
  QString fileName = ":/textures/cat_diff.png";
  int width = 512;
  int height = 1024;
};

#endif // TEXTUREPROPERTIES_H
