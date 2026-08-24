#include "handlers/scene_renderer.h"
#include "raylib-cpp.hpp"
#include "data/point.h"

namespace {

void drawGlyph(char symbol, int row, int col) {
    if (symbol == EMPTY_CELL_SYMBOL) return;
    char text[2] = {symbol, '\0'};
    Vector2 pos{static_cast<float>(col * CELL_PIXEL_SIZE), static_cast<float>(row * CELL_PIXEL_SIZE)};
    DrawTextEx(GetFontDefault(), text, pos, static_cast<float>(FONT_POINT_SIZE), 1.0f, WHITE);
}

} // namespace

void SceneRenderer::render(const Board& board, const std::array<Mine, NUM_OF_MINES>& mines) {
    for (int row = 0; row < BOARD_HEIGHT; row++) {
        for (int col = 0; col < BOARD_WIDTH; col++) {
            drawGlyph(board.getBoardCell(Point(row, col)), row, col);
        }
    }

    for (const auto& mine : mines) {
        if (mine.isVisible() && !mine.isDestroyed()) {
            Point p = mine.getPosition();
            drawGlyph(MINE_SYMBOL, p.getX(), p.getY());
        }
    }
}
