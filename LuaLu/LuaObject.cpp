#include "stdafx.h"
#include "LuaObject.h"


HDC LuaObject::hdc = NULL;
HDC LuaObject::canvasDC = NULL;
HDC LuaObject::bgDC = NULL;
HDC LuaObject::saveDC = NULL;
LPSTR LuaObject::keyText = NULL;
HWND LuaObject::hWnd = NULL;
HINSTANCE LuaObject::hInst = NULL;
LuaObject* LuaObject::thiss = NULL;
HFONT LuaObject::font = NULL;
HFONT LuaObject::oldFont = NULL;
Area* LuaObject::lastSaveA = NULL;
Color* LuaObject::lastTrC = NULL;


INT_PTR CALLBACK InputDialog(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		SetWindowText(hDlg,LuaObject::thiss->inpTitle);
		SetWindowText(GetDlgItem(hDlg,LuaObject::thiss->IDCSHOW),LuaObject::thiss->inpHint);
		return (INT_PTR)TRUE;
	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			GetWindowText(GetDlgItem(hDlg,LuaObject::thiss->IDCINP),LuaObject::thiss->inpTxt,sizeof(LuaObject::thiss->inpTxt));
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}

LuaObject::LuaObject()
{ 
	/* 初始化Lua */ 
	L = lua_open();
	/* 载入Lua基本库 */ 
	luaL_openlibs(L);
	lua_register(L, "msg", msg);
	lua_register(L, "cinp", cinp);
	lua_register(L, "setTitle", setTitle);
	lua_register(L, "cprint", drawTxt);
	lua_register(L, "cimg", drawImage);		//有问题
	lua_register(L, "cline", drawLine);
	lua_register(L, "cbox", drawBox);
	lua_register(L, "clear", clear);
	lua_register(L, "setMaskColor", setMaskColor);
	lua_register(L, "setBG", setBG);
	lua_register(L, "cflush", flush);
	lua_register(L, "save", save);
	lua_register(L, "restore", restore);
	lua_register(L, "csleep", csleep);
	loadConfig();
	beginPaint = FALSE;
	font = CreateFont(12,6,0,0,400,FALSE,FALSE,FALSE,DEFAULT_CHARSET,OUT_CHARACTER_PRECIS,CLIP_CHARACTER_PRECIS,DEFAULT_QUALITY,FF_MODERN,"宋体");
	thiss = this;
	lastSaveA = new Area;
	lastTrC = new Color;
	lastTrC->r=0;lastTrC->g=0;lastTrC->b=0;
	paintTemp = NULL;
	canvasTemp = NULL;
	bgTemp = NULL;
	saveTemp = NULL;
}


LuaObject::~LuaObject(void)
{
	/* 清除Lua */ 
	lua_close(L);
	DeleteObject(font);
	delete lastSaveA;
	delete lastTrC;
}

void LuaObject::loadConfig()
{
	luaL_dofile(L,"sys.lua");
	lua_getglobal(L,"readData");
	lua_pushstring(L,CONFIG);
	lua_call(L,1,1);
	//readData返回的是一个（key:value）的table类型 首先要在堆栈上方留一个空位存放临时数据
	lua_pushnil(L);
	//接着调用下一步 lua自动将table释放一组到空位上并形成2个位置存放key和value，-1位置存放的是value
	lua_next(L,-2);
	//取得value，因为key是下标所以我不获取了，但要记住key在-2位置
	config_Script = lua_tostring(L,-1);
	//弹出一个位置，注意：不是添弹出第几个，而是弹出几个！
	lua_pop(L,1);
	//继续读取table数据
	lua_next(L,-2);
	config_Title = lua_tostring(L,-1);
	lua_pop(L,1);
	lua_next(L,-2);
	config_W = (int)lua_tonumber(L,-1);
	lua_pop(L,1);
	lua_next(L,-2);
	config_H = (int)lua_tonumber(L,-1);
	config_WD = 6;
	config_HD = 28;
	if(lua_gettop(L)>3)	//如果还有多余参数则读取为屏幕差
	{
		lua_pop(L,1);
		lua_next(L,-2);
		config_WD = (int)lua_tonumber(L,-1);
		lua_pop(L,1);
		lua_next(L,-2);
		config_HD = (int)lua_tonumber(L,-1);
	}
	lua_pop(L,3);
}

void LuaObject::StackShow()
{
	int len=lua_gettop(thiss->L);
	if(len == 0)
	{
		showErr("Lua堆栈中没有内容。");
		return;
	}
	memset(thiss->str,0,255);
	for(int i = -1 ; i >=-len ; i--)
	{
		sprintf_s(thiss->str,"[%d]:%s",i,lua_typename(thiss->L,lua_type(thiss->L,i)));
		MessageBox(hWnd,lua_tostring(thiss->L,i),thiss->str,MB_OK);
	}
	MessageBox(hWnd,"Lua堆栈遍历完成","Lua Luncher",MB_OK);
}

void LuaObject::Run(LPCSTR file)
{
	luaL_dofile(L, file);
}

void LuaObject::showErr(LPCSTR errInfo)
{
	MessageBox(hWnd,errInfo,"Lua运行器 编译错误",MB_OK|MB_ICONERROR);
}

Area* LuaObject::getArea(lua_State* L)
{
	Area* a = new Area;
	lua_pushnil(L);
	lua_next(L,-2);
	a->x = lua_tointeger(L,-1);
	lua_pop(L,1);
	lua_next(L,-2);
	a->y = lua_tointeger(L,-1);
	lua_pop(L,1);
	lua_next(L,-2);
	a->w = lua_tointeger(L,-1);
	lua_pop(L,1);
	lua_next(L,-2);
	a->h = lua_tointeger(L,-1);
	lua_pop(L,3);
	return a;
}

Color* LuaObject::getColor(lua_State* L)
{
	Color* c = new Color;
	lua_pushnil(L);
	lua_next(L,-2);
	c->r = lua_tointeger(L,-1);
	lua_pop(L,1);
	lua_next(L,-2);
	c->g = lua_tointeger(L,-1);
	lua_pop(L,1);
	lua_next(L,-2);
	c->b = lua_tointeger(L,-1);
	lua_pop(L,3);
	return c;
}
////////////////////////////////////事件////////////////////////////////////
void LuaObject::keyDown()
{
	//触发事件
	lua_getglobal(L,"onKeyDown");
	lua_pushstring(L,keyText);
	lua_pcall(L,1,0,NULL);
	free(keyText);
	keyText = NULL;
}

void LuaObject::keyUp()
{
	//触发事件
	lua_getglobal(L,"onKeyUp");
	lua_pushstring(L,keyText);
	lua_pcall(L,1,0,NULL);
	free(keyText);
	keyText = NULL;
}

void LuaObject::mouseDown(LPCSTR type,int x,int y)
{
	lua_getglobal(L,"onMouseDown");
	lua_pushstring(L,type);
	lua_pushnumber(L,x);
	lua_pushnumber(L,y);
	lua_pcall(L,3,0,NULL);
}

void LuaObject::mouseUp(LPCSTR type, int x, int y)
{
	lua_getglobal(L, "onMouseUp");
	lua_pushstring(L, type);
	lua_pushnumber(L, x);
	lua_pushnumber(L, y);
	lua_pcall(L, 3, 0, NULL);
}

void LuaObject::mouseHover(int x,int y)
{
	lua_getglobal(L,"onMouseMove");
	lua_pushnumber(L,x);
	lua_pushnumber(L,y);
	lua_pcall(L,2,0,NULL);
}

////////////////////////////////////函数////////////////////////////////////
int LuaObject::msg(lua_State* L)
{
	int n = lua_gettop(L);  //返回栈中元素的个数
	if(n<2 || !lua_isstring(L,1) || !lua_isstring(L,2))
	{
		showErr("调用msg函数时传入的参数个数或类型错误！\n正确的语法为：\nint msg(string title, string content [,btn=0, icon=0] );");
		lua_pushnumber(L,-1);
		lua_error(L);
		return 1;
	}
	int btn = MB_OK,icon = NULL;
	if(n == 4)
	{
		btn = lua_tointeger(L,3);
		icon = lua_tointeger(L,4);
	}
	int rst = MessageBox(hWnd,lua_tostring(L,2),lua_tostring(L,1),btn|icon);
	lua_pushnumber(L,rst);
	return 1;
}

int LuaObject::cinp(lua_State* L)
{//string cinp(string title,string hint);
	int n = lua_gettop(L);
	if(n!=2 || !lua_isstring(L,1) || !lua_isstring(L,2))
	{
		showErr("调用cinp函数时传入的参数个数或类型错误！\n正确的语法为：\nstring cinp(string title,string hint);");
		lua_pushstring(L,"error");
		lua_error(L);
		return 1;
	}
	thiss->inpTitle = lua_tostring(L,1);
	thiss->inpHint = lua_tostring(L,2);
	DialogBox(hInst, MAKEINTRESOURCE(thiss->INPUTBOX), hWnd, InputDialog);
	lua_pushstring(L,thiss->inpTxt);
	return 1;
}

int LuaObject::setTitle(lua_State* L)
{
	int n = lua_gettop(L);
	if(n!=1 || !lua_isstring(L,1))
	{
		showErr("调用setTitle函数时传入的参数个数或类型错误！\n正确的语法为：\nvoid setTitle(string title);");
		lua_error(L);
		return 0;
	}
	SetWindowText(hWnd,lua_tostring(L,1));
	return 0;
}

int LuaObject::drawTxt(lua_State* L)
{
	int n = lua_gettop(L);  //返回栈中元素的个数
	if(n!=4 || !lua_isstring(L,1) || !lua_isnumber(L,2) || !lua_isnumber(L,3) || !lua_istable(L,4))
	{
		showErr("调用cprint函数时传入的参数个数或类型错误！\n正确的语法为：\nvoid cprint(string content,int x,int y,table color);");
		lua_error(L);
		return 0;
	}
	LPCSTR o = lua_tostring(L,1);
	int x = lua_tointeger(L,2),y = lua_tointeger(L,3),r,g,b;
	lua_pushnil(L);
	lua_next(L,-2);
	r = lua_tointeger(L,-1);
	lua_pop(L,1);
	lua_next(L,-2);
	g = lua_tointeger(L,-1);
	lua_pop(L,1);
	lua_next(L,-2);
	b = lua_tointeger(L,-1);
	lua_pop(L,6);
	oldFont = (HFONT)SelectObject(hdc,font);
	SetTextColor(hdc,RGB(r,g,b));
	SetBkMode(hdc,TRANSPARENT);
	TextOut(hdc,x,y,o,strlen(o));
	return 0;
}

int LuaObject::drawImage(lua_State* L)
{
	int n = lua_gettop(L);
	//检验传入参数是否匹配
	if((n!=3 && n!=4) || !lua_isstring(L,1) || !lua_isnumber(L,2) || !lua_isnumber(L,3) || (n==4 && !lua_istable(L,4) && !lua_isnil(L,4))) 
	{
		showErr("调用cimg函数时传入的参数个数或类型错误！\n正确的语法为：\nvoid cimg(string url,int x,int y[,table area || nil]);");
		lua_error(L);
		return 0;
	}
	LPCSTR url = lua_tostring(L,1);
	int x=lua_tointeger(L,2),
		y=lua_tointeger(L,3);
	CImage cimg;		//创建位图
	cimg.Load(url);		//读取文件
	if(n==3) cimg.BitBlt(hdc,0,0,cimg.GetWidth(),cimg.GetHeight(),0,0,SRCCOPY);
	//cimg.TransparentBlt(hdc,x,y,cimg.GetWidth(),cimg.GetHeight(),0,0,cimg.GetWidth(),cimg.GetHeight()); 		//将位图绘制到dc上
	else if(n==4)
	{
		if(lua_isnil(L,4)) {cimg.TransparentBlt(saveDC,x,y,cimg.GetWidth(),cimg.GetHeight(),x,y,cimg.GetWidth(),cimg.GetHeight());}	//绘制透明图片PNG
		else
		{
			Area* a = getArea(L);
			//cimg.BitBlt(hdc,x,y,a->w,a->h,a->x,a->y,SRCCOPY);
			cimg.TransparentBlt(hdc,x,y,a->w,a->h,a->x,a->y,a->w,a->h);	//绘制透明图片PNG
			delete a;
		}
	}
	cimg.Destroy();		//销毁位图对象
	delete cimg;
	return 0;
}

int LuaObject::drawBox(lua_State* L)
{
	int n=lua_gettop(L);
	if((n!=2 && n!=3) || !lua_istable(L,1) || !lua_istable(L,2) || (n==3 && !lua_isboolean(L,3))) 
	{
		showErr("调用cbox函数时传入的参数个数或类型错误！\n正确的语法为：\nvoid cbox(table area,table color,bool isFillMode=false);");
		lua_error(L);
		return 0;
	}
	BOOL fillMode = TRUE;
	Color* color = NULL;
	Area* area = NULL;
	COLORREF c = RGB(0,0,0);
	RECT* rt = new RECT;
	if(n==3)
	{
		fillMode = lua_toboolean(L,3);
		lua_pop(L,1);
	}
	color = getColor(L);
	c = RGB(color->r,color->g,color->b);
	area = getArea(L);
	rt->left = area->x;rt->right=area->x+area->w;
	rt->top=area->y;rt->bottom=area->y+area->h;
	if(fillMode)					//填充模式
	{
		HBRUSH brush = CreateSolidBrush(c);
		FillRect(hdc,rt,brush);
		DeleteObject(brush);
	}
	else								//矩形模式
	{
		HPEN hPen,oldPen;
		hPen = ::CreatePen(PS_SOLID,1,c);
		oldPen = (HPEN)::SelectObject(hdc,hPen);
		Rectangle(hdc,rt->left,rt->top,rt->right,rt->bottom);
		SelectObject(hdc,oldPen);
		DeleteObject(hPen);
	}
	delete rt;
	if(color!=NULL) delete color;
	if(area!=NULL) delete area;
	return 0;
}

int LuaObject::drawLine(lua_State* L)
{
	int n=lua_gettop(L);
	if(n!=6 || !lua_isnumber(L,1) || !lua_isnumber(L,2) || !lua_isnumber(L,3) || !lua_isnumber(L,4) || !lua_isnumber(L,5) || !lua_istable(L,6)) 
	{
		showErr("调用cline函数时传入的参数个数或类型错误！\n正确的语法为：\nvoid cline(int x0,int y0,int x1,int y1,int width,table color);");
		lua_error(L);
		return 0;
	}
	int	x0=lua_tointeger(L,1),
		y0=lua_tointeger(L,2),
		x1=lua_tointeger(L,3),
		y1=lua_tointeger(L,4),
		w = lua_tointeger(L,5),r=255,g=255,b=255;

	lua_pop(L,5);
	HPEN hPen,oldPen;
	hPen = ::CreatePen(PS_SOLID,w,RGB(r,g,b));
	oldPen = (HPEN)::SelectObject(hdc,hPen);
	MoveToEx(hdc,x0,y0,NULL);
	LineTo(hdc,x1,y1);
	SelectObject(hdc,oldPen);
	DeleteObject(hPen);
	return 0;
}

int LuaObject::clear(lua_State* L)
{
	int n=lua_gettop(L);
	if(n>2 || (n==1 && !lua_istable(L,1)))
	{
		showErr("调用clear函数时传入的参数个数或类型错误！\n正确的语法为：\nvoid clear([table area]);");
		lua_error(L);
		return 0;
	}
	int x=0,y=0,w=thiss->config_W,h=thiss->config_H;
	Area* area = NULL;
	RECT* rt = new RECT;
	rt->left = x;rt->right=x+w;
	rt->top = y;rt->bottom=y+h;
	if(n==1)
	{
		area = getArea(L);
		rt->left = area->x;rt->right=area->x+area->w;
		rt->top=area->y;rt->bottom=area->y+area->h;
	}
	HBRUSH brush = CreateSolidBrush(RGB(lastTrC->r,lastTrC->g,lastTrC->b));
	FillRect(hdc,rt,brush);
	DeleteObject(brush);
	delete rt;
	if(area!=NULL) delete area;
	return 0;
}

int LuaObject::setMaskColor(lua_State* L)
{
	int n=lua_gettop(L);
	if(n>1 || (n==1 && !lua_istable(L,1)))
	{
		showErr("调用setClearColor函数时传入的参数个数或类型错误！\n正确的语法为：\nvoid setClearColor(table color);");
		lua_error(L);
		return 0;
	}
	delete lastTrC;
	lastTrC=NULL;
	lastTrC = getColor(L);
	return 0;
}


int LuaObject::setBG(lua_State* L)
{
	int n=lua_gettop(L);
	if(n>1 || (n==1 && !lua_istable(L,1) && !lua_isnil(L,1)))
	{
		showErr("调用setBG函数时传入的参数个数或类型错误！\n正确的语法为：\nvoid setBG([nil][Area]);");
		lua_error(L);
		return 0;
	}
	if(lua_isnil(L,1))
	{
		HBRUSH brush = CreateSolidBrush(RGB(lastTrC->r,lastTrC->g,lastTrC->b));
		RECT* allRect = new RECT;
		allRect->left = 0;allRect->top=0;
		allRect->right = thiss->config_W;allRect->bottom = thiss->config_H;
		FillRect(bgDC,allRect,brush);
		delete allRect;
		DeleteObject(brush);
	}
	if(lua_istable(L,1))
	{
		Area* a = getArea(L);
		BitBlt(bgDC,a->x,a->y,a->w,a->h,canvasDC,a->x,a->y,SRCCOPY);
		delete a;
	}
	if(n==0)
	{
		BitBlt(bgDC,0,0,thiss->config_W,thiss->config_H,canvasDC,0,0,SRCCOPY);
	}
	return 0;
}

int LuaObject::flush(lua_State* L)
{
	if(thiss->beginPaint)
	{
		int n=lua_gettop(L);
		if(n>1 || (n==1 && !lua_istable(L,1)))
		{
			showErr("调用cflush函数时传入的参数个数或类型错误！\n正确的语法为：\nvoid cflush([table area]);");
			lua_error(L);
			return 0;
		}
		if(n==0)
		{
			BitBlt(canvasDC,0,0,thiss->config_W,thiss->config_H,bgDC,0,0,SRCCOPY);
			TransparentBlt(canvasDC,0,0,thiss->config_W,thiss->config_H,hdc,0,0,thiss->config_W,thiss->config_H,RGB(lastTrC->r,lastTrC->g,lastTrC->b));	//透明处理
			//BitBlt(canvasDC,0,0,thiss->config_W,thiss->config_H,hdc,0,0,SRCCOPY);
			InvalidateRect(hWnd,NULL,TRUE);
			UpdateWindow(hWnd);
		}else
		{
			Area* a = getArea(L);
			RECT* r = new RECT;
			r->left=a->x;r->right = a->x+a->w;
			r->top=a->y;r->bottom = a->y+a->h;
			BitBlt(canvasDC,a->x,a->y,a->w,a->h,bgDC,a->x,a->y,SRCCOPY);
			//BitBlt(canvasDC,a->x,a->y,a->w,a->h,hdc,a->x,a->y,SRCCOPY);
			TransparentBlt(canvasDC,a->x,a->y,a->w,a->h,hdc,a->x,a->y,a->w,a->h,RGB(lastTrC->r,lastTrC->g,lastTrC->b));	//透明处理
			InvalidateRect(hWnd,r,TRUE);
			UpdateWindow(hWnd);
			delete a;
			delete r;
		}
	}
	return 0;
}

int LuaObject::csleep(lua_State* L)
{
	int n=lua_gettop(L);
	if(n!=1 || !lua_isnumber(L,1))
	{
		showErr("调用csleep函数时传入的参数个数或类型错误！\n正确的语法为：\nvoid csleep(int millisecond);");
		lua_error(L);
		return 0;
	}
	int millisecond = lua_tointeger(L,1);
	Sleep(millisecond);
	return 0;
}

int LuaObject::save(lua_State* L)
{
	int n=lua_gettop(L);
	if(n>2 || (n==1 && !lua_istable(L,1) && !lua_isstring(L,1) && !lua_isnil(L,1)) || (n==2 && (!lua_istable(L,1) || !lua_istable(L,2))))
	{
		showErr("调用save函数时传入的参数个数或类型错误！\n正确的语法为：\nvoid save(nil || string file || table area[,table area1]); ");
		lua_error(L);
		return 0;
	}
	Area* canvasArea = NULL,* destArea = NULL;
	LPCSTR path = NULL;
	if(n==2)
	{
		destArea = getArea(L);
		canvasArea = getArea(L);
	}
	if(n==1 && lua_isstring(L,1))
	{
		path = lua_tostring(L,1);
		lua_pop(L,1);
	}
	if(n==1 && lua_istable(L,1))
	{
		canvasArea = getArea(L);
	}
	if(n==1 && lua_isnil(L,1))
	{
		HBRUSH brush = CreateSolidBrush(RGB(lastTrC->r,lastTrC->g,lastTrC->b));
		RECT* sRect = new RECT;
		sRect->left = 0;sRect->top=0;
		sRect->right = thiss->config_W*SAVEMUTIL;sRect->bottom = thiss->config_H*SAVEMUTIL;
		FillRect(saveDC,sRect,brush);
		delete sRect;
		DeleteObject(brush);
	}
	if(destArea!=NULL)
	{//复制canvas到内存saveBMP中指定位置
		lastSaveA->x = destArea->x;
		lastSaveA->y = destArea->y;
		lastSaveA->w = destArea->w;
		lastSaveA->h = destArea->h;
		BitBlt(saveDC,destArea->x,destArea->y,destArea->w,destArea->h,canvasDC,canvasArea->x,canvasArea->y,SRCCOPY);
	}
	if(n==1 && canvasArea!=NULL)
	{//内存设备对拷
		lastSaveA->x = canvasArea->x;
		lastSaveA->y = canvasArea->y;
		lastSaveA->w = canvasArea->w;
		lastSaveA->h = canvasArea->h;
		BitBlt(saveDC,canvasArea->x,canvasArea->y,canvasArea->w,canvasArea->h,canvasDC,canvasArea->x,canvasArea->y,SRCCOPY);
	}
	if(path!=NULL)
	{
		CImage img;
		char str[256];
		img.Attach(* (thiss->canvasTemp));
		img.Save(path);
		img.Detach();
		sprintf_s(str,"%s_bgBuffer.png",path);
		img.Attach(* (thiss->bgTemp));
		img.Save(str);
		img.Detach();
		sprintf_s(str,"%s_saveBuffer.png",path);
		img.Attach(* (thiss->saveTemp));
		img.Save(str);
		img.Detach();
		sprintf_s(str,"%s_paintBuffer.png",path);
		img.Attach(* (thiss->paintTemp));
		img.Save(str);
		img.Destroy();
		memset(str,0,256);
	}
	if(canvasArea!=NULL) delete canvasArea;
	if(destArea!=NULL) delete destArea;
	return 0;
}

int LuaObject::restore(lua_State* L)
{
	int n=lua_gettop(L);
	if(n>2 || (n==1 && !lua_istable(L,1)) || (n==2 && (!lua_istable(L,1) || !lua_istable(L,2))))
	{
		showErr("调用restore函数时传入的参数个数或类型错误！\n正确的语法为：\nvoid restore([table area[,table area1]]);");
		lua_error(L);
		return 0;
	}
	Area* destArea = NULL,* oriArea = NULL;	//oriArea对应于save画区上位置，destArea对应于主画区位置
	if(n==2) { destArea = getArea(L); oriArea = getArea(L); }
	else if(n==1) {oriArea = getArea(L);}
	if(destArea!=NULL && oriArea!=NULL)
	{
		BitBlt(hdc,destArea->x,destArea->y,oriArea->w,oriArea->h,saveDC,oriArea->x,oriArea->y,SRCCOPY);
	}
	if(destArea==NULL && oriArea!=NULL)
	{
		BitBlt(hdc,oriArea->x,oriArea->y,oriArea->w,oriArea->h,saveDC,oriArea->x,oriArea->y,SRCCOPY);
	}
	if(destArea==NULL && oriArea==NULL)
	{
		BitBlt(hdc,lastSaveA->x,lastSaveA->y,lastSaveA->w,lastSaveA->h,saveDC,lastSaveA->x,lastSaveA->y,SRCCOPY);
		//立即刷新区域
		BitBlt(canvasDC,lastSaveA->x,lastSaveA->y,lastSaveA->w,lastSaveA->h,bgDC,lastSaveA->x,lastSaveA->y,SRCCOPY);
		BitBlt(canvasDC,lastSaveA->x,lastSaveA->y,lastSaveA->w,lastSaveA->h,hdc,lastSaveA->x,lastSaveA->y,SRCCOPY);
		RECT* r = new RECT;
		r->left=lastSaveA->x;r->right = lastSaveA->x+lastSaveA->w;
		r->top=lastSaveA->y;r->bottom = lastSaveA->y+lastSaveA->h;
		InvalidateRect(hWnd,r,TRUE);
		UpdateWindow(hWnd);
		delete r;
	}
	if(destArea!=NULL) delete destArea;
	if(oriArea!=NULL) delete oriArea;
	return 0;
}