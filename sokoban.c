#include <gb/gb.h>
#include <string.h>
#include "tile.h"
#include "map.c"

/* プレイヤー */
typedef struct
{
  int x;
  int y;
} Player;
Player player;

/* ボタン押下情報 */
char button;
char pre_button;

/* プロトタイプ宣言 */
void disp_title();   // ゲームタイトル画面表示
void game_start();   // ゲーム開始処理
void init();         // ゲーム初期化処理
void update();       // ゲーム更新処理
void disp_clear();   // ゲームクリア画面表示
char* get_tile(int); // BGタイル番号配列取得処理

/* メイン処理 */
void main(void)
{
  // 初期処理（背景有効化、BGタイルの設定）
  SHOW_BKG;
  set_bkg_data(0, sizeof(tile)/16, tile);
  // タイトル画面の表示
  disp_title();
  // ゲームスタート
  game_start();
}

/* タイトル画面表示 */
void disp_title()
{
  // 乱数シードを保持
  UWORD seed = DIV_REG;

  // タイトル画面の表示
  for (int y = 0; y < MAP_HEIGHT*2; y++) {
    for (int x = 0; x < MAP_WIDTH*2; x++) {
      if (title_map[y][x] == BLOCK_TILE_NO) {
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

  // 乱数初期化
  // ※ボタン押下までの時間を使用してランダム調整
  seed |= (UWORD)DIV_REG << 8;
  initrand(seed);

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
  for (int y = 0; y < MAP_HEIGHT; y++) {
    for (int x = 0; x < MAP_WIDTH; x++) {
      // 番号に紐づくオブジェクトを描画
      set_bkg_tiles(x*2, y*2, 2, 2, get_tile(map[y][x]));
      // プレイヤータイルの場合、変数に設定
      if (map[y][x] == PLAYER_TILE_NO) {
        player.x = x;
        player.y = y;
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
      if (button & J_UP   ) player_pos[1]-=1;
      if (button & J_DOWN ) player_pos[1]+=1;
      if (button & J_LEFT ) player_pos[0]-=1;
      if (button & J_RIGHT) player_pos[0]+=1;
      // 以下の場合、移動処理を行わない
      // ① プレイヤーの移動後位置がマップ領域を超えている場合
      if (player_pos[0] < 0 || MAP_WIDTH-1 < player_pos[0]
          || player_pos[1] < 0 || MAP_HEIGHT-1 < player_pos[1]) {
        continue;
      }
      // ② プレイヤーの移動後位置がブロックの場合
      if (map[player_pos[1]][player_pos[0]] == BLOCK_TILE_NO) {
        continue;
      }

      // プレイヤーの移動後位置がボックスの場合
      if (map[player_pos[1]][player_pos[0]] == BOX_TILE_NO) {
        // ボックスの移動後位置を求める
        box_pos[0] = player_pos[0];
        box_pos[1] = player_pos[1];
        if (button & J_UP   ) box_pos[1]-=1;
        if (button & J_DOWN ) box_pos[1]+=1;
        if (button & J_LEFT ) box_pos[0]-=1;
        if (button & J_RIGHT) box_pos[0]+=1;
        // 以下の場合、移動処理を行わない
        // ① ボックスの移動後位置がマップ領域を超えている場合
        if (box_pos[0] < 0 || MAP_WIDTH-1 < box_pos[0]
            || box_pos[1] < 0 || MAP_HEIGHT-1 < box_pos[1]) {
          continue;
        }
        // ② ボックスの移動後位置がブロックの場合
        if (map[box_pos[1]][box_pos[0]] == BLOCK_TILE_NO) {
          continue;
        }
        // ボックス移動処理
        if (map[box_pos[1]][box_pos[0]] == POINT_TILE_NO) {
          // 移動後位置がポイントの場合、ボックスとポイントを消す
          map[player_pos[1]][player_pos[0]] = BLANK_TILE_NO;
          map[box_pos[1]][box_pos[0]]       = BLANK_TILE_NO;
          set_bkg_tiles(player_pos[0]*2, player_pos[1]*2, 2, 2, blank_tile);
          set_bkg_tiles(box_pos[0]*2, box_pos[1]*2, 2, 2, blank_tile);
        } else {
          // ボックスを移動させる
          map[player_pos[1]][player_pos[0]] = BLANK_TILE_NO;
          map[box_pos[1]][box_pos[0]]       = BOX_TILE_NO;
          set_bkg_tiles(player_pos[0]*2, player_pos[1]*2, 2, 2, blank_tile);
          set_bkg_tiles(box_pos[0]*2, box_pos[1]*2, 2, 2, box_tile);
        }
      }

      // プレイヤー移動処理
      // 現在位置がプレイヤーのタイル番号の場合、白背景に設定
      if (map[player.y][player.x] == PLAYER_TILE_NO) {
        map[player.y][player.x] = BLANK_TILE_NO;
      }
      // 元いた場所をマップ指定のオブジェクトに戻す
      set_bkg_tiles(player.x*2, player.y*2, 2, 2, get_tile(map[player.y][player.x]));   
      // プレイヤーを移動させる
      player.x = player_pos[0];
      player.y = player_pos[1];
      set_bkg_tiles(player.x*2, player.y*2, 2, 2, player_tile);

      // マップにボックスが１つも無ければクリア
      is_clear = 1;
      for (int y = 0; y < MAP_HEIGHT; y++) {
        for (int x = 0; x < MAP_WIDTH; x++) {
          if (map[y][x] == BOX_TILE_NO) {
            is_clear = 0;
          }
        }
      }
      if (is_clear == 1) {
        break;
      }
    }
    // 押下ボタンを保持
    pre_button = button;
  }

  // 以下、ループから抜けた後の処理
  // クリア処理
  if (is_clear == 1) {
    disp_clear();
    return;
  }
  // リトライ処理
  if (is_retly == 1) {
    game_start();
    return;
  }
}

/* ゲームクリア画面表示 */
void disp_clear()
{
  // クリア文字の表示
  for (int y = 0; y < MAP_HEIGHT*2; y++) {
    for (int x = 0; x < MAP_WIDTH*2; x++) {
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
      // スタートボタンかAボタン押下で次処理へ
      if ((button & J_START) || (button & J_A)) {
        break;
      }
    }
    pre_button = button;
  }
  // ゲーム再スタート
  game_start();
}
