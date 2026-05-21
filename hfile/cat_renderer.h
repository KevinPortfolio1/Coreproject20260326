#ifndef CAT_RENDERER_H
#define CAT_RENDERER_H

#include <QPainter>
#include "cyber_palette.h"
#include "core_state.h"

class CatRenderer {
public:
    static void drawCyberCat(QPainter &p, const CoreState &state, const CyberPalette &palette);
};

#endif // CAT_RENDERER_H
