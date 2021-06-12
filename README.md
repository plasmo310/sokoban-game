# sokoban_game
GBDKで作った倉庫番のサンプルゲームです。
<br>
`sokoban.gb`ファイルをそのままGBエミュレータで実行できます（恐らく）。
<br>
<br>
※ローカルでビルドする場合、GBDKのダウンロードが必要です。
<br>
　ダウンロード後、`GBDK_ROOT`にGBDKインストールパスを指定するか、
<br>
　`make.bat`、`Makefile`に記述されているパスを直接修正してビルドしてください。
<br>
<br>
|フォルダ構造| 内容 |
|----|----|
.|
├── Makefile|Make用ファイル
├── make.bat|Make用ファイル
├── `map.c`|src: マップ関連処理
├── `sokoban.c`|src: メイン処理
├── `sokoban.gb`|ROMファイル
├── sokoban.map|-
├── sokoban.noi|-
└── `tile.h`|src: タイル関連処理

<br>
P.S.
<br>
C言語を綺麗に書くの難しい・・・。
