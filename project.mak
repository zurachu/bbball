#===============================================================================
# プロジェクト設定
#===============================================================================

#=======================================
# プロジェクト名
#=======================================
PROJECT_NAME := hello

#=======================================
# タイトル
#=======================================
CAPTION := 

#=======================================
# アイコンファイル名
#=======================================
ICON := 

#=======================================
# コンパイラ追加オプション
#=======================================
CINCS := 
CDEFS := 

#=======================================
# 追加ライブラリ
#=======================================
LIBRARIES := 

#=======================================
# 高速 RAM に配置したいオブジェクトファイル名
# オブジェクトを高速 RAM に配置する場合、
# pceAppInit() の先頭で FramObject_Init() を呼んで下さい
#=======================================
FRAM_OBJECTS := 

#=======================================
# 16階調描画関連コードを高速 RAM に配置するかどうか
# 不要な場合コメントアウトして下さい
#=======================================
#USE_LDIRECT_ON_FRAM := true

#=======================================
# 縁取りフォント関連コードを高速 RAM に配置するかどうか
# 不要な場合コメントアウトして下さい
#=======================================
USE_FONT_FUCHI_ON_FRAM := true

#=======================================
# P/ECE include ディレクトリ
# 直接依存関係として用いるわけではないが、
# 依存関係を作成する際に警告が出ないようにするため必要
#=======================================
PIECE_INC_DIR := C:/usr/PIECE/include

