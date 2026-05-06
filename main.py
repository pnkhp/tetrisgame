import pygame
import random
import sys
import os

# ─── Constants ────────────────────────────────────────────────────────────────
BOARD_COLS = 13   # playable columns (excluding walls)
BOARD_ROWS = 18   # playable rows   (excluding floor)
CELL = 32         # pixel size of one cell

BOARD_W = (BOARD_COLS + 2) * CELL          # board width  (with walls)
PANEL_W = 260                               # right-side panel
WINDOW_W = BOARD_W + PANEL_W
WINDOW_H = (BOARD_ROWS + 1) * CELL         # board height (with floor)

FPS        = 60
DROP_START = 45    # frames between auto-drops (easy speed)
DROP_MIN   = 4

HS_FILE = "highscore.txt"

# ─── Colors ───────────────────────────────────────────────────────────────────
BG        = (12,  12,  20)
WALL_C    = (50,  55,  75)
GRID_C    = (25,  27,  40)
TEXT_C    = (210, 215, 235)
DIM_C     = (90,  95, 115)
PANEL_BG  = (18,  20,  32)
ACCENT    = (80, 160, 255)

BLOCK_COLORS = {
    'I': ((0,   230, 230),  (0,   180, 180)),
    'O': ((255, 215,   0),  (200, 165,   0)),
    'T': ((180,  60, 230),  (130,  30, 180)),
    'S': ((50,  220,  90),  (30,  170,  60)),
    'Z': ((230,  60,  60),  (180,  30,  30)),
    'J': ((60,  120, 230),  (30,   80, 180)),
    'L': ((240, 140,  40),  (190,  95,  20)),
}

# ─── Tetromino shapes (4×4) ───────────────────────────────────────────────────
SHAPES = {
    'I': [[' ','I',' ',' '],
          [' ','I',' ',' '],
          [' ','I',' ',' '],
          [' ','I',' ',' ']],

    'O': [[' ',' ',' ',' '],
          [' ','O','O',' '],
          [' ','O','O',' '],
          [' ',' ',' ',' ']],

    'I2':[[' ',' ',' ',' '],   # horizontal I
          ['I','I','I','I'],
          [' ',' ',' ',' '],
          [' ',' ',' ',' ']],

    'T': [[' ',' ',' ',' '],
          [' ','T',' ',' '],
          ['T','T','T',' '],
          [' ',' ',' ',' ']],

    'S': [[' ',' ',' ',' '],
          [' ','S','S',' '],
          ['S','S',' ',' '],
          [' ',' ',' ',' ']],

    'Z': [[' ',' ',' ',' '],
          ['Z','Z',' ',' '],
          [' ','Z','Z',' '],
          [' ',' ',' ',' ']],

    'J': [[' ',' ',' ',' '],
          ['J',' ',' ',' '],
          ['J','J','J',' '],
          [' ',' ',' ',' ']],

    'L': [[' ',' ',' ',' '],
          [' ',' ','L',' '],
          ['L','L','L',' '],
          [' ',' ',' ',' ']],
}
SHAPE_KEYS = ['I', 'O', 'I2', 'T', 'S', 'Z', 'J', 'L']

# ─── Helpers ──────────────────────────────────────────────────────────────────
def load_high_score():
    try:
        with open(HS_FILE) as f:
            return int(f.read().strip())
    except Exception:
        return 0

def save_high_score(hs):
    with open(HS_FILE, 'w') as f:
        f.write(str(hs))

def calc_score(lines):
    if lines <= 0:
        return 0
    pts = 36
    for _ in range(1, lines):
        pts *= 3
    return pts

def rotate_matrix(m):
    n = len(m)
    rotated = [[' '] * n for _ in range(n)]
    for i in range(n):
        for j in range(n):
            rotated[j][n - 1 - i] = m[i][j]
    return rotated

# ─── Game class ───────────────────────────────────────────────────────────────
class Tetris:
    def __init__(self):
        self.board = self._empty_board()
        self.score = 0
        self.high_score = load_high_score()
        self.total_lines = 0
        self.paused = False
        self.game_over = False
        self.drop_limit = DROP_START
        self.move_timer = 0
        self.flash_rows = []      # rows being cleared (animation)
        self.flash_timer = 0
        self.combo_text = ''
        self.combo_timer = 0
        self.cx = 4
        self.cy = 0
        self.piece = None
        self._spawn()

    def _empty_board(self):
        # Board cells: ' ' empty, '#' wall, letter = locked piece
        board = []
        for i in range(BOARD_ROWS + 1):
            row = []
            for j in range(BOARD_COLS + 2):
                if i == BOARD_ROWS or j == 0 or j == BOARD_COLS + 1:
                    row.append('#')
                else:
                    row.append(' ')
            board.append(row)
        return board

    def _spawn(self):
        key = random.choice(SHAPE_KEYS)
        self.piece = [row[:] for row in SHAPES[key]]
        self.cx = 4
        self.cy = 0
        if not self._can_move(0, 0):
            self.game_over = True

    def _cells(self, piece=None, cx=None, cy=None):
        if piece is None: piece = self.piece
        if cx    is None: cx    = self.cx
        if cy    is None: cy    = self.cy
        result = []
        for i, row in enumerate(piece):
            for j, c in enumerate(row):
                if c != ' ':
                    result.append((cy + i, cx + j, c))
        return result

    def _can_move(self, dx, dy, piece=None):
        for r, c, _ in self._cells(piece, self.cx + dx, self.cy + dy):
            if c < 1 or c >= BOARD_COLS + 1 or r >= BOARD_ROWS:
                return False
            if self.board[r][c] != ' ':
                return False
        return True

    def move(self, dx):
        if not self.game_over and not self.paused:
            if self._can_move(dx, 0):
                self.cx += dx

    def rotate(self):
        if self.game_over or self.paused:
            return
        rotated = rotate_matrix(self.piece)
        if self._can_move(0, 0, rotated):
            self.piece = rotated
        else:
            # Wall-kick: try shifting ±1
            for kick in [1, -1, 2, -2]:
                old_cx = self.cx
                self.cx += kick
                if self._can_move(0, 0, rotated):
                    self.piece = rotated
                    return
                self.cx = old_cx

    def soft_drop(self):
        if not self.game_over and not self.paused:
            if self._can_move(0, 1):
                self.cy += 1

    def hard_drop(self):
        if self.game_over or self.paused:
            return
        while self._can_move(0, 1):
            self.cy += 1
        self._lock()

    def _lock(self):
        for r, c, ch in self._cells():
            self.board[r][c] = ch
        lines = self._find_full_lines()
        if lines:
            self.flash_rows = lines
            self.flash_timer = 12   # frames to flash
        else:
            self._spawn()

    def _find_full_lines(self):
        full = []
        for i in range(BOARD_ROWS):
            if all(self.board[i][j] != ' ' for j in range(1, BOARD_COLS + 1)):
                full.append(i)
        return full

    def _clear_lines(self, rows):
        pts = calc_score(len(rows))
        self.score += pts
        self.total_lines += len(rows)
        if self.score > self.high_score:
            self.high_score = self.score
            
        names = {1: '1 HÀNG  +36đ', 2: '2 HÀNG  +108đ',
                 3: '3 HÀNG  +324đ', 4: '4 HÀNG  +972đ'}
        self.combo_text = names.get(len(rows), f'{len(rows)} HÀNG!')
        self.combo_timer = 90

        for r in sorted(rows, reverse=True):
            del self.board[r]
            
        for _ in range(len(rows)):
            new_row = [' '] * (BOARD_COLS + 2)
            new_row[0] = '#'
            new_row[-1] = '#'
            self.board.insert(0, new_row)

        self.drop_limit -= len(rows)
        if self.drop_limit < DROP_MIN:
            self.drop_limit = DROP_MIN

    def update(self):
        if self.game_over or self.paused:
            return
        # Flash animation
        if self.flash_rows:
            self.flash_timer -= 1
            if self.flash_timer <= 0:
                self._clear_lines(self.flash_rows)
                self.flash_rows = []
                self._spawn()
            return

        self.move_timer += 1
        if self.move_timer >= self.drop_limit:
            self.move_timer = 0
            if self._can_move(0, 1):
                self.cy += 1
            else:
                self._lock()

        if self.combo_timer > 0:
            self.combo_timer -= 1

    def ghost_y(self):
        gy = self.cy
        while self._can_move(0, gy - self.cy + 1):
            gy += 1
        return gy

# ─── Renderer ─────────────────────────────────────────────────────────────────
class Renderer:
    def __init__(self, screen):
        self.screen = screen
        pygame.font.init()
        self.font_lg  = self._load_font(22, bold=True)
        self.font_md  = self._load_font(16)
        self.font_sm  = self._load_font(13)
        self.font_xl  = self._load_font(32, bold=True)

    def _load_font(self, size, bold=False):
        try:
            return pygame.font.SysFont('consolas', size, bold=bold)
        except Exception:
            return pygame.font.SysFont('monospace', size, bold=bold)

    def _cell_rect(self, col, row):
        return pygame.Rect(col * CELL, row * CELL, CELL, CELL)

    def _draw_block(self, surf, rect, color_top, color_bot, alpha=255):
        s = pygame.Surface((rect.w, rect.h), pygame.SRCALPHA)
        # Main face
        pygame.draw.rect(s, (*color_top, alpha), (1, 1, rect.w-2, rect.h-2), border_radius=3)
        # Highlight top-left
        pygame.draw.line(s, (255,255,255,min(alpha, 120)), (2,2), (rect.w-3,2))
        pygame.draw.line(s, (255,255,255,min(alpha, 80)),  (2,2), (2,rect.h-3))
        # Shadow bottom-right
        pygame.draw.line(s, (*color_bot, alpha), (2,rect.h-2), (rect.w-2,rect.h-2))
        pygame.draw.line(s, (*color_bot, alpha), (rect.w-2,2), (rect.w-2,rect.h-2))
        surf.blit(s, rect.topleft)

    def draw(self, game):
        self.screen.fill(BG)
        self._draw_grid(game)
        self._draw_board(game)
        self._draw_ghost(game)
        self._draw_piece(game)
        self._draw_walls(game)
        self._draw_panel(game)
        if game.game_over:
            self._draw_game_over(game)
        elif game.paused:
            self._draw_pause()
        pygame.display.flip()

    def _draw_grid(self, game):
        for r in range(BOARD_ROWS):
            for c in range(1, BOARD_COLS + 1):
                rect = self._cell_rect(c, r)
                pygame.draw.rect(self.screen, GRID_C, rect, 1)

    def _draw_walls(self, game):
        # Left wall
        pygame.draw.rect(self.screen, WALL_C, (0, 0, CELL, WINDOW_H))
        # Right wall
        pygame.draw.rect(self.screen, WALL_C, (BOARD_W - CELL, 0, CELL, WINDOW_H))
        # Floor
        pygame.draw.rect(self.screen, WALL_C, (0, BOARD_ROWS * CELL, BOARD_W, CELL))
        # Bright edge lines
        pygame.draw.line(self.screen, ACCENT, (CELL, 0), (CELL, WINDOW_H), 2)
        pygame.draw.line(self.screen, ACCENT, (BOARD_W - CELL - 1, 0), (BOARD_W - CELL - 1, WINDOW_H), 2)
        pygame.draw.line(self.screen, ACCENT, (CELL, BOARD_ROWS * CELL), (BOARD_W - CELL, BOARD_ROWS * CELL), 2)

    def _draw_board(self, game):
        flash_on = (game.flash_timer % 4) < 2  # blink
        for r in range(BOARD_ROWS):
            for c in range(1, BOARD_COLS + 1):
                ch = game.board[r][c]
                if ch != ' ':
                    rect = self._cell_rect(c, r)
                    if r in game.flash_rows:
                        col = (255, 255, 255) if flash_on else (180, 180, 180)
                        self._draw_block(self.screen, rect, col, (150,150,150))
                    else:
                        ct, cb = BLOCK_COLORS.get(ch, ((150,150,150),(100,100,100)))
                        self._draw_block(self.screen, rect, ct, cb)

    def _draw_ghost(self, game):
        if game.game_over or not game.piece:
            return
        gy = game.ghost_y()
        if gy == game.cy:
            return
        for i, row in enumerate(game.piece):
            for j, ch in enumerate(row):
                if ch != ' ':
                    rect = self._cell_rect(game.cx + j, gy + i)
                    s = pygame.Surface((CELL, CELL), pygame.SRCALPHA)
                    ct, _ = BLOCK_COLORS.get(ch, ((150,150,150),(100,100,100)))
                    pygame.draw.rect(s, (*ct, 45), (1,1,CELL-2,CELL-2), border_radius=3)
                    pygame.draw.rect(s, (*ct, 80), (1,1,CELL-2,CELL-2), border_radius=3, width=1)
                    self.screen.blit(s, rect.topleft)

    def _draw_piece(self, game):
        if game.game_over or not game.piece:
            return
        for i, row in enumerate(game.piece):
            for j, ch in enumerate(row):
                if ch != ' ':
                    rect = self._cell_rect(game.cx + j, game.cy + i)
                    ct, cb = BLOCK_COLORS.get(ch, ((150,150,150),(100,100,100)))
                    self._draw_block(self.screen, rect, ct, cb)

    # ── Panel ──────────────────────────────────────────────────────────────────
    def _draw_panel(self, game):
        px = BOARD_W
        panel = pygame.Rect(px, 0, PANEL_W, WINDOW_H)
        pygame.draw.rect(self.screen, PANEL_BG, panel)
        pygame.draw.line(self.screen, ACCENT, (px, 0), (px, WINDOW_H), 2)

        pad = 16
        y = 14

        # Title
        t = self.font_xl.render('TETRIS', True, ACCENT)
        self.screen.blit(t, (px + (PANEL_W - t.get_width()) // 2, y))
        y += t.get_height() + 6

        pygame.draw.line(self.screen, WALL_C, (px + pad, y), (px + PANEL_W - pad, y))
        y += 12

        # Score section
        def label_val(label, val, color=TEXT_C):
            lbl = self.font_sm.render(label, True, DIM_C)
            self.screen.blit(lbl, (px + pad, y))
            vt = self.font_lg.render(str(val), True, color)
            self.screen.blit(vt, (px + PANEL_W - pad - vt.get_width(), y))
            return lbl.get_height() + 6

        y += label_val('KỶ LỤC', game.high_score, (255, 90, 90))
        y += label_val('ĐIỂM',   game.score,      (255, 215, 0))
        y += label_val('HÀNG',   game.total_lines, (90, 220, 90))

        pygame.draw.line(self.screen, WALL_C, (px + pad, y), (px + PANEL_W - pad, y))
        y += 12

        # Combo popup
        if game.combo_timer > 0:
            alpha = min(255, game.combo_timer * 8)
            s = self.font_md.render(game.combo_text, True, (255, 230, 80))
            s.set_alpha(alpha)
            self.screen.blit(s, (px + (PANEL_W - s.get_width()) // 2, y))
        y += 28

        pygame.draw.line(self.screen, WALL_C, (px + pad, y), (px + PANEL_W - pad, y))
        y += 14

        # Controls
        ctrl_title = self.font_md.render('ĐIỀU KHIỂN', True, ACCENT)
        self.screen.blit(ctrl_title, (px + (PANEL_W - ctrl_title.get_width()) // 2, y))
        y += ctrl_title.get_height() + 8

        controls = [
            ('← / A',   'Trái'),
            ('→ / D',   'Phải'),
            ('↑ / W',   'Xoay'),
            ('↓ / X',   'Rơi nhanh'),
            ('Space',   'Rơi ngay'),
            ('P',       'Tạm dừng'),
            ('Q / Esc', 'Thoát'),
        ]
        for key, desc in controls:
            ks = self.font_sm.render(key, True, (140, 190, 255))
            ds = self.font_sm.render(desc, True, DIM_C)
            self.screen.blit(ks, (px + pad, y))
            self.screen.blit(ds, (px + pad + 80, y))
            y += ks.get_height() + 4

        # Speed indicator
        y += 6
        pygame.draw.line(self.screen, WALL_C, (px + pad, y), (px + PANEL_W - pad, y))
        y += 10
        level = max(1, (DROP_START - game.drop_limit) // 4 + 1)
        lbl = self.font_sm.render(f'LEVEL  {level}', True, ACCENT)
        self.screen.blit(lbl, (px + (PANEL_W - lbl.get_width()) // 2, y))
        y += lbl.get_height() + 6
        bar_w = PANEL_W - pad * 2
        filled = int(bar_w * min(1.0, (level - 1) / 10))
        pygame.draw.rect(self.screen, WALL_C, (px + pad, y, bar_w, 8), border_radius=4)
        if filled > 0:
            pygame.draw.rect(self.screen, ACCENT, (px + pad, y, filled, 8), border_radius=4)

    def _draw_game_over(self, game):
        overlay = pygame.Surface((WINDOW_W, WINDOW_H), pygame.SRCALPHA)
        overlay.fill((0, 0, 0, 170))
        self.screen.blit(overlay, (0, 0))

        bw, bh = 320, 200
        bx = (WINDOW_W - bw) // 2
        by = (WINDOW_H - bh) // 2
        box = pygame.Surface((bw, bh), pygame.SRCALPHA)
        pygame.draw.rect(box, (18, 20, 40, 240), (0,0,bw,bh), border_radius=12)
        pygame.draw.rect(box, ACCENT, (0,0,bw,bh), 2, border_radius=12)
        self.screen.blit(box, (bx, by))

        lines = [
            (self.font_xl, 'GAME OVER', (255, 80, 80)),
            (self.font_md, f'KỶ LỤC  : {game.high_score}', (255, 90, 90)),
            (self.font_md, f'ĐIỂM    : {game.score}',      (255, 215, 0)),
            (self.font_md, f'SỐ HÀNG : {game.total_lines}',(90, 220, 90)),
            (self.font_sm, 'Nhấn R để chơi lại | Q để thoát', DIM_C),
        ]
        ty = by + 20
        for font, text, color in lines:
            s = font.render(text, True, color)
            self.screen.blit(s, (bx + (bw - s.get_width()) // 2, ty))
            ty += s.get_height() + 10

    def _draw_pause(self):
        overlay = pygame.Surface((BOARD_W, WINDOW_H), pygame.SRCALPHA)
        overlay.fill((0, 0, 0, 130))
        self.screen.blit(overlay, (0, 0))
        t = self.font_xl.render('TẠM DỪNG', True, (255, 230, 80))
        self.screen.blit(t, ((BOARD_W - t.get_width()) // 2,
                              (WINDOW_H - t.get_height()) // 2))


# ─── Main ─────────────────────────────────────────────────────────────────────
def main():
    pygame.init()
    screen = pygame.display.set_mode((WINDOW_W, WINDOW_H))
    pygame.display.set_caption('Tetris')
    clock = pygame.time.Clock()

    game     = Tetris()
    renderer = Renderer(screen)

    # Key-repeat for smooth movement
    pygame.key.set_repeat(170, 55)

    running = True
    while running:
        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                running = False

            elif event.type == pygame.KEYDOWN:
                k = event.key
                if game.game_over:
                    if k in (pygame.K_r,):
                        save_high_score(game.high_score)
                        game = Tetris()
                    elif k in (pygame.K_q, pygame.K_ESCAPE):
                        running = False
                else:
                    if k in (pygame.K_LEFT,  pygame.K_a): game.move(-1)
                    elif k in (pygame.K_RIGHT, pygame.K_d): game.move(1)
                    elif k in (pygame.K_UP,   pygame.K_w): game.rotate()
                    elif k in (pygame.K_DOWN,  pygame.K_x): game.soft_drop()
                    elif k == pygame.K_SPACE:               game.hard_drop()
                    elif k == pygame.K_p:                   game.paused = not game.paused
                    elif k in (pygame.K_q, pygame.K_ESCAPE): running = False

        game.update()
        renderer.draw(game)
        clock.tick(FPS)

    save_high_score(game.high_score)
    pygame.quit()
    sys.exit()


if __name__ == '__main__':
    main()
