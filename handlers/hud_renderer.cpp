#include "handlers/hud_renderer.h"
#include "raylib-cpp.hpp"
#include "constants/constants.h"
#include <string>

namespace {

int countAliveTanks(Player& player) {
    int count = 0;
    for (Tank& t : player.getTankVector()) {
        if (!t.isTankHit()) count++;
    }
    return count;
}

// Dims the whole window so overlaid text stays legible against the busy
// board behind it.
void renderDimOverlay() {
    DrawRectangle(0, 0, WINDOW_WIDTH_PX, WINDOW_HEIGHT_PX, Color{0, 0, 0, 200});
}

void renderCenteredText(const char* text, Color color) {
    Vector2 size = MeasureTextEx(GetFontDefault(), text, static_cast<float>(FONT_POINT_SIZE), 1.0f);
    Vector2 pos{(WINDOW_WIDTH_PX - size.x) / 2.0f, (WINDOW_HEIGHT_PX - size.y) / 2.0f};
    DrawTextEx(GetFontDefault(), text, pos, static_cast<float>(FONT_POINT_SIZE), 1.0f, color);
}

} // namespace

void HudRenderer::renderLives(Player& player1, Player& player2) {
    std::string p1Text = "Player 1 lives: " + std::to_string(countAliveTanks(player1));
    std::string p2Text = "Player 2 lives: " + std::to_string(countAliveTanks(player2));

    DrawTextEx(GetFontDefault(), p1Text.c_str(),
               Vector2{static_cast<float>(HUD_PANEL_X_PX), static_cast<float>(HUD_MARGIN_PX)},
               static_cast<float>(FONT_POINT_SIZE), 1.0f, WHITE);
    DrawTextEx(GetFontDefault(), p2Text.c_str(),
               Vector2{static_cast<float>(HUD_PANEL_X_PX), static_cast<float>(HUD_MARGIN_PX + HUD_LINE_HEIGHT_PX)},
               static_cast<float>(FONT_POINT_SIZE), 1.0f, WHITE);
}

void HudRenderer::renderPauseOverlay() {
    renderDimOverlay();
    renderCenteredText("Game paused. Press ESC to resume or X to return to the main menu.", YELLOW);
}

void HudRenderer::renderEndMessage(const char* text) {
    renderDimOverlay();
    renderCenteredText(text, YELLOW);
}
