#include "define.h"

static FILEINFO *file_info;
static byte sectors_per_cluster;
static int file_count;

void init_file()
{
    // ファイルシステムヘッダの解析
    byte* fs_pos = (byte*) FILESYSTEM_LOAD;

    sectors_per_cluster = fs_pos[0x0d];
    ushort fat_tbl_head_sector = fs_pos[0x0f] << 8 | fs_pos[0x0e];
    byte fat_tbl_num = fs_pos[0x10];
    ushort sector_per_fat_tbl = fs_pos[0x17] << 8 | fs_pos[0x16];
    int max_file_count = fs_pos[0x12] << 8 | fs_pos[0x11];

#if 0
    // FATテーブルの領域
    char str[64];
    hsprintf(str, "%d, %d, %d, %d", fat_tbl_head_sector, fat_tbl_num, sector_per_fat_tbl, max_file_count);
    println(str);
#endif
    // FATテーブルをスキップした先頭が、ファイルの先頭
    file_info = (FILEINFO *) &(fs_pos[SECT_SIZE * fat_tbl_head_sector + SECT_SIZE * sector_per_fat_tbl * fat_tbl_num]);
    file_count = 0;
    for (int i = 0; i < max_file_count; i++) {
        if (file_info[i].name[0] != 0x00) {
            file_count++;
        }
    }
}

FILEINFO *get_file_info()
{
    return file_info;
}

int get_file_count()
{
    return file_count;
}

FILEINFO *search_file(const char *filename)
{
    char basename[8];
    char extname[3];
    for (int i = 0; i < 8; i++) {
        basename[i] = ' ';
    }
    for (int i = 0; i < 3; i++) {
        extname[i] = ' ';
    }

    // filenameをbasename[8], extname[3]に分割する。
    // 不足する部分はすでに半角空白で埋められているので文字が存在している部分のみでOK.
    // 例）hoge.csの場合
    // "hoge    ", "cs "でそれぞれFILEINFOのname, extと比較する
    // dotindex:4, len:7, namelen:4, extlen:2
    // makefileの場合
    // "makefile", "   "で比較する
    // dotindex:-1, len:8, namelen:8, extlen:0
    int dotindex = hstrindexof(filename, '.');
    int len = hstrlen(filename);
    int namelen = dotindex < 0 ? len : dotindex;
    int extlen = dotindex < 0 ? 0 : len - dotindex - 1;
    for (int i = 0; i < namelen; i++) {
        basename[i] = filename[i];
    }
    for (int i = 0; i < extlen; i++) {
        extname[i] = filename[dotindex + 1 + i];
    }

    for (int i = 0; i < file_count; i++) {
        // ファイル名、拡張子名の両方を比較。
        if (hstrncmp(file_info[i].name, basename, 8) == 0 &&
            hstrncmp(file_info[i].ext, extname, 3) == 0) {
            return &file_info[i];
        }
    }

    return NULL;
}
