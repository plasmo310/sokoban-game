#include <stdio.h>
#include <rand.h>
#include "tile.h"

/* マップ情報（10x9） */
const int MAP_WIDTH = 10;
const int MAP_HEIGHT = 9;
// マップ配列
unsigned int map[9][10];
// 初期化用デフォルトマップ
unsigned int default_map[9][10] = 
{
  {1,1,1,1,1,1,1,1,1,1},
  {1,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,1},
  {1,1,1,1,1,1,1,1,1,1},
};
// タイトル画面表示用マップ
unsigned int title_map[18][20] = 
{
  {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
  {1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1},
  {1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1},
  {1,1,0,1,0,0,1,0,0,0,1,0,0,1,1,1,1,0,1,1},
  {1,1,0,0,1,0,1,0,1,1,1,1,0,0,0,0,1,0,1,1},
  {1,1,0,0,0,0,1,0,1,0,0,1,0,0,0,0,1,0,1,1},
  {1,1,0,0,0,1,0,0,0,0,0,1,0,0,0,0,1,0,1,1},
  {1,1,0,1,1,0,0,0,0,1,1,0,0,1,1,1,1,0,1,1},
  {1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1},
  {1,1,0,0,0,0,0,0,1,0,1,0,0,0,0,0,0,0,1,1},
  {1,1,0,0,0,1,0,1,1,0,1,0,1,1,0,1,0,0,1,1},
  {1,1,0,0,0,1,0,1,0,0,0,0,0,0,0,1,0,0,1,1},
  {1,1,0,0,0,1,0,1,0,0,0,0,0,0,0,1,0,0,1,1},
  {1,1,0,0,1,0,0,0,1,0,0,0,1,1,1,0,0,0,1,1},
  {1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1},
  {1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1},
  {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
  {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
};
// クリア文字表示用マップ
unsigned int clear_map[18][20] = 
{
  {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
  {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
  {1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1},
  {1,1,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,1,1},
  {1,1,1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,1,1},
  {1,1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1},
  {1,1,0,0,1,0,0,1,0,0,1,0,0,0,0,0,0,0,1,1},
  {1,1,0,1,0,0,0,1,0,0,1,0,0,0,0,0,0,0,1,1},
  {1,1,0,0,0,0,0,1,0,0,1,0,0,0,0,0,0,0,1,1},
  {1,1,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,1},
  {1,1,0,0,0,0,0,1,1,1,0,0,1,1,1,1,1,0,1,1},
  {1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,1,1},
  {1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1,1},
  {1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1,1},
  {1,1,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,1,1},
  {1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1},
  {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
  {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
};

/* 移動方向 */
const int LEFT_MOVE  = 0;
const int RIGHT_MOVE = 1;
const int UP_MOVE    = 2;
const int DOWN_MOVE  = 3;

/* ループ回数制限用 */
const int MAX_LOOP_COUNT = 50;
int loop_count = 0;

/* プロトタイプ宣言 */
void generate_map();                   // マップ配列自動生成処理
void set_map_tiles(int[], int[], int); // マップのタイル設定処理
int randint(int, int);                 // 乱数生成処理

/* マップ配列生成処理 */
void generate_map()
{
    // デフォルトのマップ情報を設定
    for (int i = 0; i < MAP_HEIGHT; i++) { // Y方向の配列数
        for (int j = 0; j < MAP_WIDTH; j++) { // X方向の配列数
            map[i][j] = default_map[i][j];
        }
    }
    // マップ生成のための変数定義
    int point_pos[2] = {0, 0};     // ポイント位置
    int last_walk_pos[2] = {0, 0}; // 最後の歩行ポイント位置
    // １つ目のポイントをランダムに決めてマップを生成
    point_pos[0] = randint(1, 8);
    point_pos[1] = randint(1, 7);
    set_map_tiles(point_pos, last_walk_pos, 5);
    // ２つ目のポイントを最後の歩行ポイント位置としてマップを生成
    point_pos[0] = last_walk_pos[0];
    point_pos[1] = last_walk_pos[1];
    set_map_tiles(point_pos, last_walk_pos, 3);
    // 最後の歩行ポイント位置をプレイヤー位置とする
    map[last_walk_pos[1]][last_walk_pos[0]] = PLAYER_TILE_NO;

    // 生成されたマップを整形
    for(int j = 0; j < MAP_HEIGHT; j++) { // Y方向の配列数
        for (int i = 0; i < MAP_WIDTH; i++) { // X方向の配列数
            if (map[j][i] == BLANK_TILE_NO) {
                // ブランクタイルの場合、２分の１の確率でブロックタイルに設定
                if (randint(0, 1) == 0) {
                    map[j][i] = BLOCK_TILE_NO;
                }
            } else if (map[j][i] == WALK_TILE_NO) {
                // 歩行経路のタイルの場合、ブランクタイルに設定
                map[j][i] = BLANK_TILE_NO;
            }
        }
    }
}

/* マップのタイル設定処理 */
void set_map_tiles(int point_pos[2], int last_walk_pos[2], int move_count)
{
    // 移動方向格納用
    int move_vec = -1;     // 移動方向
    int pre_move_vec = -1; // 一つ前の移動方向
    // 位置格納用
    int box_pos[] = {0, 0};      // ボックス位置
    int walk_pos[] = {0, 0};     // 歩行ポイント位置
    int pre_box_pos[] = {0, 0};  // 一つ前のボックス位置
    int pre_walk_pos[] = {0, 0}; // 一つ前の歩行ポイント位置
    int walk2_pos[] = {0, 0};    // 歩行ポイント位置２（経路用）
    int walk3_pos[] = {0, 0};    // 歩行ポイント位置３（経路用）

    /* ポイント位置を基準にマップの自動生成を行う*/
    // ポイント位置を基準とする
    pre_box_pos[0] = point_pos[0];
    pre_box_pos[1] = point_pos[1];

    // ループ回数初期化
    loop_count = 0;
    // ボックスと歩行ポイントをポイントの直線上に並べて置く
    while (1) {
        // ４方向をランダムで決める
        move_vec = randint(0, 3);
        if (move_vec == LEFT_MOVE) {
            box_pos[0]  = pre_box_pos[0] - 1;
            box_pos[1]  = pre_box_pos[1];
            walk_pos[0] = pre_box_pos[0] - 2;
            walk_pos[1] = pre_box_pos[1];
        } else if (move_vec == RIGHT_MOVE) {
            box_pos[0]  = pre_box_pos[0] + 1;
            box_pos[1]  = pre_box_pos[1];
            walk_pos[0] = pre_box_pos[0] + 2;
            walk_pos[1] = pre_box_pos[1];
        } else if (move_vec == UP_MOVE) {
            box_pos[0]  = pre_box_pos[0];
            box_pos[1]  = pre_box_pos[1] - 1;
            walk_pos[0] = pre_box_pos[0];
            walk_pos[1] = pre_box_pos[1] - 2;
        } else if (move_vec == DOWN_MOVE) {
            box_pos[0]  = pre_box_pos[0];
            box_pos[1]  = pre_box_pos[1] + 1;
            walk_pos[0] = pre_box_pos[0];
            walk_pos[1] = pre_box_pos[1] + 2;
        }
        // 歩行ポイント（２マス先）が置ける範囲なら置く
        // マップの範囲 かつ ブランクか歩行経路のタイル
        if (0 < walk_pos[0] && walk_pos[0] < MAP_WIDTH-1 && 0 < walk_pos[1] && walk_pos[1] < MAP_HEIGHT-1
        && (map[walk_pos[1]][walk_pos[0]] == BLANK_TILE_NO || map[walk_pos[1]][walk_pos[0]] == WALK_TILE_NO)
        && (map[box_pos[1]][box_pos[0]]   == BLANK_TILE_NO || map[box_pos[1]][box_pos[0]]   == WALK_TILE_NO)) {
            // 移動した方向を保持
            pre_move_vec = move_vec;
            break;
        }
        // ループ回数が最大数を超えたら処理終了
        loop_count++;
        if (loop_count >= MAX_LOOP_COUNT) {
            // 最終的な位置はポイント位置として返却
            last_walk_pos[0] = point_pos[0];
            last_walk_pos[1] = point_pos[1];
            return;
        }
    }
    // ポイント、ボックス、歩行ポイントを設定
    map[point_pos[1]][point_pos[0]] = POINT_TILE_NO;
    map[box_pos[1]][box_pos[0]] = BOX_TILE_NO;
    map[walk_pos[1]][walk_pos[0]] = WALK_TILE_NO;

    // ボックスを指定回数動かしながら歩行経路を埋める
    for (int i = 0; i < move_count; i++) {
        // 最後のボックス位置を基準とする
        pre_box_pos[0] = box_pos[0];
        pre_box_pos[1] = box_pos[1];
        pre_walk_pos[0] = walk_pos[0];
        pre_walk_pos[1] = walk_pos[1];
        // ループ回数初期化
        loop_count = 0;
        while (1) {
            // ４方向をランダムで決める
            move_vec = randint(0, 3);
            if (move_vec == LEFT_MOVE) {
                box_pos[0]  = pre_box_pos[0] - 1;
                box_pos[1]  = pre_box_pos[1];
                walk_pos[0] = pre_box_pos[0] - 2;
                walk_pos[1] = pre_box_pos[1];
            } else if (move_vec == RIGHT_MOVE) {
                box_pos[0]  = pre_box_pos[0] + 1;
                box_pos[1]  = pre_box_pos[1];
                walk_pos[0] = pre_box_pos[0] + 2;
                walk_pos[1] = pre_box_pos[1];
            } else if (move_vec == UP_MOVE) {
                box_pos[0]  = pre_box_pos[0];
                box_pos[1]  = pre_box_pos[1] - 1;
                walk_pos[0] = pre_box_pos[0];
                walk_pos[1] = pre_box_pos[1] - 2;
            } else if (move_vec == DOWN_MOVE) {
                box_pos[0]  = pre_box_pos[0];
                box_pos[1]  = pre_box_pos[1] + 1;
                walk_pos[0] = pre_box_pos[0];
                walk_pos[1] = pre_box_pos[1] + 2;
            }
            // 歩行ポイント（２マス先）が置ける範囲なら置く
            // マップの範囲 かつ ブランクか歩行経路のタイル
            if (0 < walk_pos[0] && walk_pos[0] < MAP_WIDTH-1 && 0 < walk_pos[1] && walk_pos[1] < MAP_HEIGHT-1
            && (map[walk_pos[1]][walk_pos[0]] == BLANK_TILE_NO || map[walk_pos[1]][walk_pos[0]] == WALK_TILE_NO)
            && (map[box_pos[1]][box_pos[0]]   == BLANK_TILE_NO || map[box_pos[1]][box_pos[0]]   == WALK_TILE_NO)) {
                // ボックスの移動方向が変わった場合、周囲に歩行経路を設定
                walk2_pos[0] = -1;
                walk2_pos[1] = -1;
                walk3_pos[0] = -1;
                walk3_pos[1] = -1;
                if (pre_move_vec == LEFT_MOVE && move_vec == UP_MOVE) {
                    walk2_pos[0] = walk_pos[0] - 1;
                    walk3_pos[0] = walk_pos[0] - 1;
                    walk2_pos[1] = walk_pos[1];
                    walk3_pos[1] = walk_pos[1] + 1;
                } else if (pre_move_vec == LEFT_MOVE && move_vec == DOWN_MOVE) {
                    walk2_pos[0] = walk_pos[0] - 1;
                    walk3_pos[0] = walk_pos[0] - 1;
                    walk2_pos[1] = walk_pos[1];
                    walk3_pos[1] = walk_pos[1] - 1;
                } else if (pre_move_vec == RIGHT_MOVE && move_vec == UP_MOVE) {
                    walk2_pos[0] = walk_pos[0] + 1;
                    walk3_pos[0] = walk_pos[0] + 1;
                    walk2_pos[1] = walk_pos[1];
                    walk3_pos[1] = walk_pos[1] + 1;
                } else if (pre_move_vec == RIGHT_MOVE && move_vec == DOWN_MOVE) {
                    walk2_pos[0] = walk_pos[0] + 1;
                    walk3_pos[0] = walk_pos[0] + 1;
                    walk2_pos[1] = walk_pos[1];
                    walk3_pos[1] = walk_pos[1] - 1;
                } else if (pre_move_vec == UP_MOVE && move_vec == LEFT_MOVE) {
                    walk2_pos[0] = walk_pos[0];
                    walk3_pos[0] = walk_pos[0] + 1;
                    walk2_pos[1] = walk_pos[1] - 1;
                    walk3_pos[1] = walk_pos[1] - 1;
                } else if (pre_move_vec == UP_MOVE && move_vec == RIGHT_MOVE) {
                    walk2_pos[0] = walk_pos[0];
                    walk3_pos[0] = walk_pos[0] - 1;
                    walk2_pos[1] = walk_pos[1] - 1;
                    walk3_pos[1] = walk_pos[1] - 1;
                } else if (pre_move_vec == DOWN_MOVE && move_vec == LEFT_MOVE) {
                    walk2_pos[0] = walk_pos[0];
                    walk3_pos[0] = walk_pos[0] + 1;
                    walk2_pos[1] = walk_pos[1] + 1;
                    walk3_pos[1] = walk_pos[1] + 1;
                } else if (pre_move_vec == DOWN_MOVE && move_vec == RIGHT_MOVE) {
                    walk2_pos[0] = walk_pos[0];
                    walk3_pos[0] = walk_pos[0] - 1;
                    walk2_pos[1] = walk_pos[1] + 1;
                    walk3_pos[1] = walk_pos[1] + 1;
                }
                // 上記で設定された場合
                if (walk2_pos[0] != -1 && walk3_pos[0] != -1) {
                    // ブランクタイル以外の場合、やり直し
                    if (map[walk2_pos[1]][walk2_pos[0]] != BLANK_TILE_NO 
                        || map[walk3_pos[1]][walk3_pos[0]] != BLANK_TILE_NO) {
                        loop_count++;
                        if (loop_count >= MAX_LOOP_COUNT) {
                            last_walk_pos[0] = pre_walk_pos[0];
                            last_walk_pos[1] = pre_walk_pos[1];
                            return;
                        }
                        continue;
                    }
                    // 歩行経路を設定
                    map[walk2_pos[1]][walk2_pos[0]] = WALK_TILE_NO;
                    map[walk3_pos[1]][walk3_pos[0]] = WALK_TILE_NO;
                }
                // 移動した方向を保持
                pre_move_vec = move_vec;
                break;
            }
            // ループ回数が最大数を超えたら処理終了
            loop_count++;
            if (loop_count >= MAX_LOOP_COUNT) {
                // 最終的な位置は最後の歩行ポイントとして返却
                last_walk_pos[0] = pre_walk_pos[0];
                last_walk_pos[1] = pre_walk_pos[1];
                return;
            }
        }
        // ボックス、歩行ポイントを設定
        map[box_pos[1]][box_pos[0]] = BOX_TILE_NO;
        map[pre_box_pos[1]][pre_box_pos[0]] = WALK_TILE_NO;
        map[walk_pos[1]][walk_pos[0]] = WALK_TILE_NO;
    }

    // 最後の歩行ポイントを設定
    last_walk_pos[0] = walk_pos[0];
    last_walk_pos[1] = walk_pos[1];
}

// 乱数を範囲指定で生成
int randint(int min,int max)
{
    // 計算式；min + rand()%(max-min+1)
    // ※シード値によりrand()が負の値になる場合がある
    int random = rand()%(max-min+1);
    if (random < min || random > max) {
        random *= -1; // 符号を反転
    }
    random += min;
	return random;
}