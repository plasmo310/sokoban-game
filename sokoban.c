#include <gb/gb.h>
#include <string.h>
#include "tile.h"
#include "map.c"

/* BGタイル番号配列 */
unsigned char blank_tile[]  = { 0x00, 0x00, 0x00, 0x00 }; // ブランク（白背景）2x2
unsigned char block_tile[]  = { 0x01, 0x01, 0x01, 0x01 }; // ブロック 2x2
unsigned char player_tile[] = { 0x02, 0x04, 0x03, 0x05 }; // プレイヤー 2x2
unsigned char point_tile[]  = { 0x06, 0x08, 0x07, 0x09 }; // ポイント 2x2
unsigned char box_tile[]    = { 0x0a, 0x0c, 0x0b, 0x0d }; // ボックス 2z2
unsigned char blank_single_tile[]  = { 0x00 }; // ブランク（白背景）1x1
unsigned char block_single_tile[]  = { 0x01 }; // ブロック 1x1

/* 変数定義 */
// プレイヤー
typedef struct
{
  int x;
  int y;
} Player;
Player player;

// ボタン押下情報
char button;
char pre_button;

/* プロトタイプ宣言 */
void game_start();   // ゲーム開始処理
void init();         // ゲーム初期化処理
void update();       // ゲーム更新処理
void disp_title();   // ゲームタイトル画面表示
void disp_clear();   // ゲームクリア画面表示
char* get_tile(int); // BGタイル番号配列取得処理

/* メイン処理 */
void main(void)
{
  // 初期処理
  SHOW_BKG; // 背景の有効化
  set_bkg_data(0, sizeof(tile)/16, tile); // BGタイルの設定
  // タイトル画面の表示
  disp_title();
  // ゲームスタート
  game_start();
}

/* ゲーム開始処理 */
void game_start()
{
  init();
  update();
}

/* ゲーム初期化処理 */
void init()
{
  // マップ配列生成
  generate_map();
  
  // マップの描画
  unsigned char *map_tile = NULL;
  for (int y = 0; y < 9; y++) {
    for (int x = 0; x < 10; x++) {
      // 番号に紐づくオブジェクトを描画
      map_tile = get_tile(map[y][x]);
      set_bkg_tiles(x*2, y*2, 2, 2, map_tile);
      // プレイヤーの場合、変数に設定
      if (map_tile == player_tile) {
        player.x = x*2;
        player.y = y*2;
      }
    }
  }
}

/* ゲーム更新処理 */
void update()
{
  int player_pos[2]; // 移動後のプレイヤー位置
  int box_pos[2];    // 移動後のボックス位置
  int is_clear = 0;  // クリアフラグ
  int is_retly = 0;  // リトライフラグ

  while(1) {
    // ボタンの入力検知
    button = joypad();
    if (pre_button != button) {
      // スタートボタン押下でリトライ
      if (button & J_START) {
        is_retly = 1;
        break;
      }

      // プレイヤーの移動後位置を求める
      player_pos[0] = player.x;
      player_pos[1] = player.y;
      if (button & J_UP   ) player_pos[1]-=2;
      if (button & J_DOWN ) player_pos[1]+=2;
      if (button & J_LEFT ) player_pos[0]-=2;
      if (button & J_RIGHT) player_pos[0]+=2;
      // 以下の場合、移動処理を行わない
      // ① プレイヤーの移動後位置がマップ領域を超えている場合
      if (player_pos[0] < 0 || 20-1 < player_pos[0]
          || player_pos[1] < 0 || 18-1 < player_pos[1]) {
        continue;
      }
      // ② プレイヤーの移動後位置がブロックの場合
      if (map[player_pos[1]/2][player_pos[0]/2] == BLOCK_TILE_NO) {
        continue;
      }

      // プレイヤーの移動後位置がボックスの場合
      if (map[player_pos[1]/2][player_pos[0]/2] == BOX_TILE_NO) {
        // ボックスの移動後位置を求める
        box_pos[0] = player_pos[0];
        box_pos[1] = player_pos[1];
        if (button & J_UP   ) box_pos[1]-=2;
        if (button & J_DOWN ) box_pos[1]+=2;
        if (button & J_LEFT ) box_pos[0]-=2;
        if (button & J_RIGHT) box_pos[0]+=2;
        // 以下の場合、移動処理を行わない
        // ① ボックスの移動後位置がマップ領域を超えている場合
        if (box_pos[0] < 0 || 20-1 < box_pos[0]
            || box_pos[1] < 0 || 18-1 < box_pos[1]) {
          continue;
        }
        // ② ボックスの移動後位置がブロックの場合
        if (map[box_pos[1]/2][box_pos[0]/2] == BLOCK_TILE_NO) {
          continue;
        }
        // ボックス移動処理
        // ボックスの移動後位置がポイントの場合
        if (map[box_pos[1]/2][box_pos[0]/2] == POINT_TILE_NO) {
          // ボックスとポイントを消す
          map[player_pos[1]/2][player_pos[0]/2] = BLANK_TILE_NO;
          map[box_pos[1]/2][box_pos[0]/2]       = BLANK_TILE_NO;
          set_bkg_tiles(player_pos[0], player_pos[1], 2, 2, blank_tile);
          set_bkg_tiles(box_pos[0], box_pos[1], 2, 2, blank_tile);
        } else {
          // ボックスを移動させる
          map[player_pos[1]/2][player_pos[0]/2] = BLANK_TILE_NO;
          map[box_pos[1]/2][box_pos[0]/2]       = BOX_TILE_NO;
          set_bkg_tiles(player_pos[0], player_pos[1], 2, 2, blank_tile);
          set_bkg_tiles(box_pos[0], box_pos[1], 2, 2, box_tile);
        }
      }

      // プレイヤー移動処理
      // 現在位置がプレイヤーのタイル番号の場合、白背景に設定
      if (map[player.y/2][player.x/2] == PLAYER_TILE_NO) {
        map[player.y/2][player.x/2] = BLANK_TILE_NO;
      }
      // 元いた場所をマップ指定のオブジェクトに戻す
      set_bkg_tiles(player.x, player.y, 2, 2, get_tile(map[player.y/2][player.x/2]));   
      // プレイヤーを移動させる
      player.x = player_pos[0];
      player.y = player_pos[1];
      set_bkg_tiles(player.x, player.y, 2, 2, player_tile);

      // クリア判定処理
      is_clear = 1;
      for (int y = 0; y < 9; y++) {
        for (int x = 0; x < 10; x++) {
          if (map[y][x] == BOX_TILE_NO) {
            is_clear = 0;
          }
        }
      }
      // ボックスがマップに無ければループから抜ける
      if (is_clear == 1) {
        break;
      }
    }
    // 押下ボタンを保持
    pre_button = button;
  }

  // 以下、ループから抜けた後の処理
  // リトライ処理
  if (is_retly == 1) {
    game_start();
    return;
  }
  // クリア処理
  if (is_clear == 1) {
    disp_clear();
    return;
  }
}

/* タイトル画面表示 */
void disp_title()
{
  // タイトル画面の表示
  for (int y = 0; y < 18; y++) {
    for (int x = 0; x < 20; x++) {
      if (title_map[y][x] == BLOCK_TILE_NO) {
        set_bkg_tiles(x, y, 1, 1, block_single_tile);
      } else {
        set_bkg_tiles(x, y, 1, 1, blank_single_tile);
      }
    }
  }

  // 乱数シードを保持
  UWORD seed = DIV_REG;

  // ボタンの入力検知
  button = joypad();
  pre_button = button;
  while (1) { 
    button = joypad();
    if (pre_button != button) {
      // スタートボタン押下で次処理へ
      if (button & J_START) {
        break;
      }
    }
    pre_button = button;
  }

  // 乱数初期化
  // ※ボタン押下までの時間でランダムに調整
  seed |= (UWORD)DIV_REG << 8;
  initrand(seed);

  // ゲームスタート
  game_start();
}

/* ゲームクリア画面表示 */
void disp_clear()
{
  // クリア文字の表示
  for (int y = 0; y < 18; y++) {
    for (int x = 0; x < 20; x++) {
      if (clear_map[y][x] == BLOCK_TILE_NO) {
        set_bkg_tiles(x, y, 1, 1, block_single_tile);
      } else {
        set_bkg_tiles(x, y, 1, 1, blank_single_tile);
      }
    }
  }
  // ボタンの入力検知
  button = joypad();
  pre_button = button;
  while (1) { 
    button = joypad();
    if (pre_button != button) {
      // スタートボタン押下で次処理へ
      if (button & J_START) {
        break;
      }
    }
    pre_button = button;
  }
  // ゲーム再スタート
  game_start();
}

/* BGタイル番号配列取得処理 */
char* get_tile(int index)
{
  if (index == BLOCK_TILE_NO) {         // 1: ブロック
    return block_tile;
  } else if (index == PLAYER_TILE_NO) { // 2: プレイヤー 
    return player_tile;
  } else if (index == POINT_TILE_NO) {  // 3: ポイント
    return point_tile;
  } else if (index == BOX_TILE_NO) {    // 4: ボックス
    return box_tile;
  } else {                              // その他: 白背景
    return blank_tile;
  }
}
