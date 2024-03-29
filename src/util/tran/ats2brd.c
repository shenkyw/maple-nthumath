/*-------------------------------------------------------*/
/* util/transbrd.c					 */
/*-------------------------------------------------------*/
/* target : Maple Sob 2.36 至 Maple 3.02 看板轉換	 */
/*          .BOARDS => .BRD				 */
/* create :   /  /  					 */
/* update : 98/06/14					 */
/* author : ernie@micro8.ee.nthu.edu.tw			 */
/* modify : itoc.bbs@bbs.tnfsh.tn.edu.tw		 */
/*-------------------------------------------------------*/
/* syntax : transbrd [target_board]			 */
/*-------------------------------------------------------*/

#if 0

   1. 修改 struct boardheader 及 transbrd()
       (boardheader 兩版定義的字串長度不一，請自行換成數字)
   2. 投票不轉換
   3. 進板畫面 copy
   4. 如有需要請 chmod 644 `find PATH -perm 600`
   5. 開 gem 目錄 gem/target_board/? 但不轉換 gem
   6. 不會更新 bshm，使用後請自行更新
   7. 轉換後請手動設看板權限

   ps. Use on ur own risk.

#endif


#include "ats.h"


static time_t
trans_hdr_chrono(filename)
  char *filename;
{
  char time_str[11];

  /* M.1087654321.A 或 M.987654321.A */
  str_ncpy(time_str, filename + 2, filename[2] == '1' ? 11 : 10);

  return (time_t) atoi(time_str);
}


static void
trans_hdr_stamp(folder, t, hdr, fpath)
  char *folder;
  time_t t;
  HDR *hdr;
  char *fpath;
{
  FILE *fp;
  char *fname, *family;
  int rc;

  fname = fpath;
  while (rc = *folder++)
  {
    *fname++ = rc;
    if (rc == '/')
      family = fname;
  }
  fname = family + 1;
  *fname++ = '/';
  *fname++ = 'A';

  for (;;)
  {
    *family = radix32[t & 31];
    archiv32(t, fname);

    if (fp = fopen(fpath, "r"))
    {
      fclose(fp);
      t++;
    }
    else
    {
      memset(hdr, 0, sizeof(HDR));
      hdr->chrono = t;
      str_stamp(hdr->date, &hdr->chrono);
      strcpy(hdr->xname, --fname);
      break;
    }
  }
}
/* ----------------------------------------------------- */
/* →文章權限考慮 by Shenk						 */
/*		參考原src/so/lock_post.c 約於25~36是重點	*/
/* ----------------------------------------------------- */
static int
is_locked(lock_type)
 char lock_type;
{
  /*int lock_level;*/
  char lock_level[9] = "plsabcde";
  int i;

  for(i = 0 ; i < 8 ; i++) {
    if(lock_type == lock_level[i]) return 1/*lock_level = i + 1*/;
  }

  return 0;
}

/* ----------------------------------------------------- */
/* →看板權限考慮 by Shenk						 */
/*		參考原src/maple/board.c 1135~1180 	 */
/*		   原src/include/perm.h 	*/
/* ----------------------------------------------------- */
#define ATS_PERM_ENTERSC       0400000
#define ATS_PERM_SECRET       01000000

/* ----------------------------------------------------- */
/* →合併推文 by Shenk						 */
/* ----------------------------------------------------- */
int
f_cp_suggest(src, dst, mode)
  char *src, *dst;
  int mode;			/* O_EXCL / O_APPEND / O_TRUNC */
{
  int fsrc, fsuggest, fdst, ret;
  char suggest[AFNLEN-2+8];

  str_cat(suggest,src,".suggest");

  ret = 0;

  if ((fsrc = open(src, O_RDONLY)) >= 0 & (fsuggest = open(suggest, O_RDONLY)) >= 0)
  {
    ret = -1;

    if ((fdst = open(dst, O_WRONLY | O_CREAT | mode, 0600)) >= 0)
    {
      char pool[BLK_SIZ];

      src = pool;
      do
      {
	ret = read(fsrc, src, BLK_SIZ);
	if (ret <= 0)
	  break;
      } while (write(fdst, src, ret) > 0);

      do
      {
	ret = read(fsuggest, src, BLK_SIZ);
	if (ret <= 0)
	  break;
      } while (write(fdst, src, ret) > 0);

      close(fdst);
    }
    close(fsrc);
    close(fsuggest);
  }
  return ret;
}

/* ----------------------------------------------------- */
/* →備份招牌 by Shenk						 */
/* ----------------------------------------------------- */

static void
gem_add(board, document /*owner, nick*/)
  char *board, *document /**owner, *nick*/;
{
  //int cc;
  //char *str;
  char folder[64], fpath[64];
  HDR hdr;
  FILE *fp;

  /* 寫入文章內容 */

  gem_fpath(folder, board, FN_DIR);

  if (fp = fdopen(hdr_stamp(folder, 'A', &hdr, fpath), "w"))
  {
    /*fprintf(fp, "發信人: %.50s 看板: %s\n", FROM, board);
    fprintf(fp, "標  題: %.70s\n", SUBJECT);
    fclose(fp);*/
	fprintf(fp,"%s\n",document);
	fprintf(fp,"%s\n",&document[80]);
	fprintf(fp,"%s\n",&document[160]);
	fprintf(fp,"%s\n",&document[240]);
    fclose(fp);
  }

  /* 造 HDR */

  hdr.xmode = 0;

  /* Thor.980825: 防止字串太長蓋過頭 */
  str_ncpy(hdr.owner, "轉換精靈", sizeof(hdr.owner));
  str_ncpy(hdr.nick, "", sizeof(hdr.nick));
  //str_stamp(hdr.date, &datevalue);	/* 依 DATE: 欄位的日期，與 hdr.chrono 不同步 */
  str_ncpy(hdr.title, ">>>原樹大招風 - 看板小招牌", sizeof(hdr.title));

  rec_bot(folder, &hdr, sizeof(HDR));

  //update_btime(board);

  //HISadd(MSGID, board, hdr.xname);
}
/* ----------------------------------------------------- */
/* 轉換主程式						 */
/* ----------------------------------------------------- */


static void
transbrd(bh)
  boardheader *bh;
{
  static time_t stamp = 0;

  int fd;
  char index[64], folder[64], buf[64], fpath[64];
  fileheader fh;
  HDR hdr;
  BRD newboard;
  time_t chrono;

  printf("轉換 %s 看板\n", bh->brdname);

  brd_fpath(buf, bh->brdname, NULL);
  if (dashd(buf))
  {
    printf("%s 已經有此看板\n", bh->brdname);
    return;
  }

  if (!stamp)
    time(&stamp);

  /* 轉換 .BRD */

  memset(&newboard, 0, sizeof(newboard));
  str_ncpy(newboard.brdname, bh->brdname, sizeof(newboard.brdname));
  str_ncpy(newboard.class, bh->title, sizeof(newboard.class));
  str_ncpy(newboard.title, &(bh->title[7]), sizeof(newboard.title));//→版標題不含類別

  str_ncpy(newboard.BM, bh->BM, sizeof(newboard.BM));
  newboard.bstamp = stamp++;
  newboard.battr = BRD_NOTRAN; /* 預設不轉信 */
  /*=====→看板權限=====*/
  if((bh->level & ATS_PERM_ENTERSC) || (bh->level & ATS_PERM_SECRET))
    newboard.battr += (BRD_NOSTAT+BRD_NOVOTE);
  newboard.readlevel = ( bh->level & ATS_PERM_ENTERSC ) ? PERM_BOARD : 0;
  newboard.readlevel = ( bh->level & ATS_PERM_SECRET ) ? PERM_SYSOP : 0;
  /*=====→更改完畢=====*/
  newboard.postlevel = PERM_POST;

  rec_add(FN_BRD, &newboard, sizeof(newboard));		/* 別忘了用 brd2gem.c 來轉換 Class */

  /* 開新目錄 */

  sprintf(fpath, "gem/brd/%s", newboard.brdname);
  mak_dirs(fpath);
  mak_dirs(fpath + 4);
  gem_add(newboard.brdname,bh->document);

  /* 轉換進板畫面 */

  sprintf(buf, OLD_BBSHOME "/boards/%s/notes", bh->brdname);

  if (dashf(buf))
  {
    brd_fpath(fpath, newboard.brdname, FN_NOTE);
    f_cp(buf, fpath, O_TRUNC);
  }

  /* 轉換文章 */

  sprintf(index, OLD_BBSHOME "/boards/%s/.DIR", bh->brdname);	/* 舊的 .DIR */
  brd_fpath(folder, newboard.brdname, ".DIR");			/* 新的 .DIR */

  if ((fd = open(index, O_RDONLY)) >= 0)
  {
    while (read(fd, &fh, sizeof(fh)) == sizeof(fh))
    {
      sprintf(buf, OLD_BBSHOME "/boards/%s/%s", bh->brdname, fh.filename);
      if (dashf(buf))	/* 文章檔案在才做轉換 */
      {
	/* 轉換文章 .DIR */
	memset(&hdr, 0, sizeof(HDR));
	chrono = trans_hdr_chrono(fh.filename);
	trans_hdr_stamp(folder, chrono, &hdr, fpath);
	str_ncpy(hdr.owner, fh.owner, sizeof(hdr.owner));
	str_ansi(hdr.title, fh.title, sizeof(hdr.title));
	/*=====→文章權限=====*/
	hdr.xmode = (fh.filemode & 0x2) ? POST_MARKED : 0;
	hdr.xmode += is_locked(fh.report) ? POST_RESTRICT : 0;
	/*=====→更改完畢=====*/
	/*=====→文章分數=====*/
	if(fh.goodpost)
	{
	  hdr.score = fh.goodpost;
	  hdr.xmode += POST_SCORE;
	}
	/*=====→更改完畢=====*/
	rec_add(folder, &hdr, sizeof(HDR));

	/* 拷貝檔案 */
	(fh.goodpost) ? f_cp_suggest(buf, fpath, O_TRUNC) : f_cp(buf, fpath, O_TRUNC);//→合併推文
      }
    }
    close(fd);
  }
}


int
main(argc, argv)
  int argc;
  char *argv[];
{
  int fd;
  int count=0;
  boardheader bh;

  /* argc == 1 轉全部板 */
  /* argc == 2 轉某特定板 */

  if (argc > 2)
  {
    printf("Usage: %s [target_board]\n", argv[0]);
    exit(-1);
  }

  chdir(BBSHOME);

  if (!dashf(FN_BOARD))
  {
    printf("ERROR! Can't open " FN_BOARD "\n");
    exit(-1);
  }
  if (!dashd(OLD_BBSHOME "/boards"))
  {
    printf("ERROR! Can't open " OLD_BBSHOME "/boards\n");
    exit(-1);
  }

  if ((fd = open(FN_BOARD, O_RDONLY)) >= 0)
  {
    while (read(fd, &bh, sizeof(bh)) == sizeof(bh))
    {
      if (argc == 1)
      {
	transbrd(&bh);
        count++;
      }
      else if (!strcmp(bh.brdname, argv[1]))
      {
	transbrd(&bh);
        count++;
	exit(1);
      }
    }
    close(fd);
  }

  printf("\n\n總共轉換 %d 個看板\n\n",count);
  exit(0);
}
