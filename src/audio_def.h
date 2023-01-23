#define XA_LENGTH(x) (((u64)(x) * 75) / 100 * IO_SECT_SIZE) //Centiseconds to sectors in bytes (w)

typedef struct
{
	XA_File file;
	u32 length;
} XA_TrackDef;

static const XA_TrackDef xa_tracks[] = {
	//MENU.XA
	{XA_Menu, XA_LENGTH(11300)}, //XA_GettinFreaky
	{XA_Menu, XA_LENGTH(3800)},  //XA_GameOver
	
	{XA_Songs1, XA_LENGTH(16286)},
	{XA_Songs1, XA_LENGTH(16516)},
	
	{XA_Songs2, XA_LENGTH(31361)},
	{XA_Songs2, XA_LENGTH(13136)},
	
	{XA_Songs3, XA_LENGTH(16943)},
	{XA_Songs3, XA_LENGTH(15085)},
	
	{XA_Songs4, XA_LENGTH(20337)},
	{XA_Songs4, XA_LENGTH(14144)},
	
	{XA_Songs5, XA_LENGTH(13763)},
	{XA_Songs5, XA_LENGTH(14861)},
	
	{XA_Unused1, XA_LENGTH(13136)},
};

static const char *xa_paths[] = {
	"\\MUSIC\\MENU.XA;1",   //XA_Menu
	"\\MUSIC\\SONGS1.XA;1",
	"\\MUSIC\\SONGS2.XA;1",
	"\\MUSIC\\SONGS3.XA;1",
	"\\MUSIC\\SONGS4.XA;1",
	"\\MUSIC\\SONGS5.XA;1",
	"\\MUSIC\\UNUSED1.XA;1",
	NULL,
};

typedef struct
{
	const char *name;
	boolean vocal;
} XA_Mp3;

static const XA_Mp3 xa_mp3s[] = {
	//MENU.XA
	{"freakymenu", false},   //XA_GettinFreaky
	{"gameover", false}, //XA_GameOver
	
	{"weakling", true},
	{"x", true},
	
	{"slaves", true},
	{"cycles", true},
	
	{"broken", true},
	{"boundless", true},
	
	{"conscience", true},
	{"nightmare-before-christmas", true},
	
	{"petrifying-peppermint", true},
	{"execution", true},
	
	{"cycles-legacy", true},
	
	{NULL, false}
};
