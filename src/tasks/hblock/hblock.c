#include "../../syscall/define.h"
#include "../../lib/define.h"

#define BOARD_WIDTH     480
#define BOARD_HEIGHT    320

void main()
{
    RECT rect;
    rect.x = 100;
    rect.y = 100;
    rect.w = BOARD_WIDTH;
    rect.h = BOARD_HEIGHT;
    int win_handle = _sc_win_create(&rect);

    // 位置と速度は、ドットを100倍したものにする
    VECTOR p;
    VECTOR v;
    p.x = 100 * 100;
    p.y = 100 * 100;
    v.x = 3 * 100;
    v.y = -3 * 100;

    RECT ball;
    ball.x = p.x / 100;
    ball.y = p.y / 100;
    ball.w = 4;
    ball.h = 4;
    _sc_win_draw_rect(win_handle, &ball, COL_WHITE);

    for (int i = 0; ; i++) {
        p.x += v.x;
        p.y += v.y;
        if (p.x > BOARD_WIDTH * 100) {
            p.x = 0;
        } else if (p.x < 0) {
            p.x = BOARD_WIDTH * 100;
        }
        if (p.y > BOARD_HEIGHT * 100) {
            p.y = 0;
        } else if (p.y < 0) {
            p.y = BOARD_HEIGHT * 100;
        }
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
16:10 ☆ボールが壁にあたったら速度を反射する
16:20 ☆自キャラのバーを作る
16:30 ☆マウスの位置を検知する
16:40 ☆マウスの位置に合わせてバーを動かす
16:50 ☆バーに当たったら反射させる
17:00 ☆バーに当たった位置によって反射方向を調整する
17:10 ☆ブロックを一つ作る
17:20 ☆ブロックを並べて複数作る
17:30 ☆ブロックの接触判定を作る
17:40 ☆ブロックにあたったらブロックを消滅させる

*/