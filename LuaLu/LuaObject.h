#pragma once
extern "C"
{
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}
#include "stdafx.h"
#define CONFIG "config.ini"
//缓冲画区是正常画区宽高的多少倍
#define SAVEMUTIL 20

//区域结构
struct Area
{
	LPCSTR key;
	int x;
	int y;
	int w;
	int h;
};
struct Color
{
	int r;
	int g;
	int b;
};

class LuaObject
{
public:
	LuaObject(void);
	~LuaObject(void);
	//执行脚本文件
	void Run(LPCSTR file);
	//取得Lua堆栈内变量个数
	int StackLen() { return lua_gettop(L); }
	static void StackShow();
	void keyDown();
	void keyUp();
	void mouseDown(LPCSTR,int,int);
	void mouseUp(LPCSTR,int,int);
	void mouseHover(int,int);
	void Start(HWND h,HDC);
	int config_W,config_H,config_WD,config_HD;
	LPCSTR config_Script,config_Title;
	bool beginPaint;
	HBITMAP* paintTemp,* canvasTemp,* bgTemp,* saveTemp;
	static HDC hdc,canvasDC,bgDC,saveDC;
	static HWND hWnd;
	static HINSTANCE hInst;
	static LPSTR keyText;		// 保存键盘按键的数组
	static LuaObject* thiss;

	int INPUTBOX;
	int IDCINP;
	int IDCSHOW;
	char inpTxt[5000];
	LPCSTR inpTitle;
	LPCSTR inpHint;

	//Lua Function
	static int msg(lua_State* L);
	static int cinp(lua_State* L);
	static int setTitle(lua_State* L);
	static int drawTxt(lua_State* L);
	static int drawImage(lua_State* L);
	static int drawBox(lua_State* L);
	static int drawLine(lua_State* L);
	static int flush(lua_State* L);
	static int clear(lua_State* L);
	static int setMaskColor(lua_State* L);
	static int setBG(lua_State* L);
	static int save(lua_State* L);
	static int restore(lua_State* L);
	static int csleep(lua_State* L);
private:
	lua_State* L;
	char str[255];
	void loadConfig();
	static void showErr(LPCSTR errInfo);
	static HFONT font,oldFont;
	static Area* getArea(lua_State* L);
	static Color* getColor(lua_State* L);
	static Area* lastSaveA;
	static Color* lastTrC;
};

