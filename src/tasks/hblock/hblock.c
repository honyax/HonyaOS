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

    RECT ball;
    rect.x = 100;
    rect.y = 100;
    rect.w = 4;
    rect.h = 4;
    _sc_win_draw_rect(win_handle, &rect, COL_WHITE);

    for (;;) {
        _sc_sleep(50);
    }
}


/*
TODO:
15:30 ★ボードを作る
15:40 ★ボールを生成する
15:50 ☆ボールに速度を持たせる
16:00 ☆ボールを速度方向に毎フレーム動かす
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