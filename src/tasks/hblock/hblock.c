#include "../../syscall/define.h"
#include "../../lib/define.h"

#define BOARD_WIDTH     480
#define BOARD_HEIGHT    320
#define PLAYER_POS_Y    280
#define PLAYER_WIDTH    80
#define PLAYER_HEIGHT   10

RECT board;
RECT ball;
VECTOR p;
VECTOR v;
RECT player;
int player_pos_x;
short *win_pos;

void get_mouse_pos()
{
    int mouse_pos = _sc_get_mouse_pos();
    int mouse_pos_x = (mouse_pos >> 16) & 0xFFFF;

    // ボードの範囲外の場合はボードの範囲内に収める
    if (mouse_pos_x < win_pos[0]) {
        player_pos_x = 0;
    } else if (mouse_pos_x > win_pos[0] + BOARD_WIDTH - PLAYER_WIDTH) {
        player_pos_x = BOARD_WIDTH - PLAYER_WIDTH;
    } else {
        player_pos_x = mouse_pos_x - win_pos[0];
    }
}

// 水平方向に衝突したか
int is_hit_horizontal()
{
    // 壁に衝突した = X座標がボードの範囲外になった
    return p.x > BOARD_WIDTH * 100 || p.x < 0;
}

int is_hit_vertical()
{
    // 壁に衝突した = Y座標がボードの範囲外になった
    return p.y > BOARD_HEIGHT * 100 || p.y < 0;
}

void main()
{
    board.x = 100;
    board.y = 100;
    board.w = BOARD_WIDTH;
    board.h = BOARD_HEIGHT;
    int win_handle = _sc_win_create(&board);

    // TODO: win_handleがWINDOWのポインタになっており、先頭からウィンドウのX座標、Y座標が2byteずつで格納されている。
    // ここでは一旦これを使う。
    win_pos = (short *) win_handle;

    // 位置と速度は、ドットを100倍したものにする
    p.x = 100 * 100;
    p.y = 100 * 100;
    v.x = 3 * 100;
    v.y = -3 * 100;

    ball.x = p.x / 100;
    ball.y = p.y / 100;
    ball.w = 4;
    ball.h = 4;
    _sc_win_draw_rect(win_handle, &ball, COL_WHITE);

    player.x = 0;
    player.y = PLAYER_POS_Y;
    player.w = PLAYER_WIDTH;
    player.h = PLAYER_HEIGHT;
    _sc_win_draw_rect(win_handle, &player, COL_CYAN);

    for (int i = 0; ; i++) {

        // マウスの位置を検出
        get_mouse_pos();
        // 本のプレイヤーの位置を黒く描画
        _sc_win_draw_rect(win_handle, &player, COL_BLACK);
        // プレイヤーの位置をマウスに合わせて更新
        player.x = player_pos_x;
        _sc_win_draw_rect(win_handle, &player, COL_CYAN);

        // ボールの速度分、位置を変更
        p.x += v.x;
        p.y += v.y;

        // 壁にあたった場合は反射
        if (is_hit_horizontal()) {
            v.x = -v.x;
        }
        if (is_hit_vertical()) {
            v.y = -v.y;
        }

        // 直前のボールの位置を黒く塗りつぶして、新しい位置を白く描画
        _sc_win_draw_rect(win_handle, &ball, COL_BLACK);
        ball.x = p.x / 100;
        ball.y = p.y / 100;
        _sc_win_draw_rect(win_handle, &ball, COL_WHITE);
        _sc_sleep(50);
    }
}


/*
TODO:
15:30 ★ボードを作る
15:40 ★ボールを生成する
15:50 ★ボールに速度を持たせる
16:00 ★ボールを速度方向に毎フレーム動かす
16:10 ★ボールが壁にあたったら速度を反射する
16:20 ☆自キャラのバーを作る
16:30 ☆マウスの位置を検知する
16:40 ☆マウスの位置に合わせてバーを動かす
16:50 ☆バーに当たったら反射させる
17:00 ☆バーに当たった位置によって反射方向を調整する
17:10 ☆ブロックを一つ作る
17:20 ☆ブロックを並べて複数作る
17:30 ☆ブロックの接触判定を作る
17:40 ☆ブロックにあたったらブロックを消滅させる
17:50 ☆ボールの速度を徐々に上げる

*/