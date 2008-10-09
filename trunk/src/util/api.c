#include "bbs.h"

//Print in markup style
//todo:cover text into legal format
//ex: ">" => "&gt;"
void
m(tag,text)
	char *tag;
	char *text;
{
	printf("<%s>%s</%s>\n",tag,text,tag);
}


//1-Exist 0-not exist
void
user_exists(username)
	char *username;
{
	chdir(BBSHOME);
	char folder[64];
	usr_fpath(folder, username, NULL);

	printf("%i",dashd(folder));
}

int
auth(userid, passwd)
	char *userid;
	char *passwd;
{
	char fpath[64];
	ACCT acct;

	chdir(BBSHOME);

	if (*userid && *passwd && strlen(userid) <= IDLEN && strlen(passwd) <= PSWDLEN)
	{
		usr_fpath(fpath, userid, FN_ACCT);

		if (!rec_get(fpath, &acct, sizeof(ACCT), 0) &&
		  !(acct.userlevel & (PERM_DENYLOGIN | PERM_PURGE)) &&
		  !chkpasswd(acct.passwd, passwd))
			return 1;
	}
	return 0;
}

void
info(userid)
	char *userid;
{
	char fpath[64];
	ACCT acct;

	chdir(BBSHOME);
	usr_fpath(fpath, userid, FN_ACCT);
	if(rec_get(fpath, &acct, sizeof(ACCT), 0))
		return;

	printf("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n");
	printf("<user>\n");
	m("email",acct.email);
	m("authed",(acct.userlevel & PERM_VALID)? "True" : "False");
	m("sysop",(acct.userlevel & PERM_SYSOP)? "True" : "False");
	printf("</user>");
}

int
main(argc, argv)
  int argc;
  char *argv[];
{
	if (str_cmp("user_exist",argv[1]) == 0){
		user_exists(argv[2]);return 0;}

	if (str_cmp("auth",argv[1]) == 0){
		printf("%i",auth(argv[2],argv[3]));return 0;}

	if (str_cmp("info",argv[1]) == 0){
		info(argv[2]);return 0;}
}
