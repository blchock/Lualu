========================================================================
Lua Luncher Lua运行器
========================================================================
运行环境及配置事项说明：
1 请安装VC++2012运行库
2 在运行LuaLu.exe前请在程序同目录下手动创建文件config.ini，并写下4行或6行脚本：
第一行 启动脚本文件名
第二行 程序标题
第三行 窗体宽度
第四行 窗体高度
———————————————————————————————————————————————————
第五行 窗体边框总宽（默认为6）    如果显示图片和调用save('*.jpg')截图有黑边代表系统不同的差异，请设置这两行
第六行 窗体边框总高（默认为28）  为合理的数值，如果使用的是Win7操作系统则不需要填写这两行

例如：
index.lua
我的LuaLu程序
250
250
3 同样的在同目录下创建配置文件第一行你填写的index.lua文件，
使用记事本或者SciTE打开，然后书写Lua脚本即可开始您的LuaLu编程之旅。
示例程序(index.lua)请见文档最后附录
4 将LuaLu/Sys.lua 拷贝至可执行程序同目录

========================================================================
函数说明：
========================================================================

[ msg ]			显示对话框，支持返回按下的按钮

[ 语法 ]			int msg(string title, string content [,btn=0, icon=0] );

title				标题
content			显示内容
btn				按钮样式(可选项，数值参照下表)
icon				图标样式(可选项，数值参照下表)
[ 返回 ]			处理结果(-1为参数错误)（数值参照下表）

//按钮值		[ 附加参考表 ]
#define MB_OK									0
#define MB_OKCANCEL					1
#define MB_ABORTRETRYIGNORE		2
#define MB_YESNOCANCEL				3
#define MB_YESNO							4
#define MB_RETRYCANCEL				5
//图标值
#define MB_ICONHAND					16
#define MB_ICONQUESTION			32
#define MB_ICONEXCLAMATION		48
#define MB_ICONASTERISK				64
//返回值
IDABORT			3	选择放弃按钮。
IDCANCEL		2	选择取消按钮。
IDCONTINUE	11	选择继续按钮。
IDIGNORE			5  选择忽略按钮。
IDNO				7	选择否按钮。
IDOK				6	选择确定按钮。
IDRETRY			4	选择重试按钮。
IDTRYAGAIN		10	选择重试按钮。
IDYES				6	选择是按钮。

========================================================================

[ onKeyDown ]	按键按下事件

[ 语法 ]			void onKeyDown(string key);
[ 参数 ]			按下键的字母或名称

========================================================================

[ onKeyUp ]	按键弹起事件

[ 语法 ]			void onKeyUp(string key);
[ 参数 ]			弹起键的字母或名称

========================================================================

[ onMouseDown ] 鼠标单击事件

[ 语法 ]			void onMouseDown(string mtype, int x, int y);
[ 参数 ]			mtype 按下鼠标左/中/右键(见下表)
x					单击处x坐标
y					单击处y坐标

[ 附加参考表 ]
lb					鼠标左键被按​下
mb				鼠标中键被​按下
rb					鼠标右键被​按下

========================================================================

[ onMouseMove ] 鼠标移动事件

[ 语法 ]			void onMouseMove(int x, int y);
[ 参数 ]          x					当前鼠标x坐标
                    y					当前鼠标y坐标

========================================================================

[ onMouseUp ] 鼠标弹起事件

[ 语法 ]			void onMouseUp(string mtype, int x, int y);
[ 参数 ]			mtype 弹起鼠标左/中/右键(见下表)
x					单击处x坐标
y					单击处y坐标

[ 附加参考表 ]
lb					鼠标左键被弹起
mb				鼠标中键被​弹起
rb					鼠标右键被​弹起

========================================================================

[ csleep ]	延时函数

[ 语法 ]			void csleep(int millisecond);
[ 参数 ]			主程序延时指定毫秒数，请谨慎使用本函数，因为在延时过程中程序将停止响应直到延时结束

========================================================================

[ setTitle ]	设置标题函数

[ 语法 ]			void setTitle(string title);
[ 参数 ]			设置标题文字

========================================================================

[ cinp ]	输入对话框

[ 语法 ]			string cinp(string title,string hint);
[ 参数 ]			读取用户输入的数据对话框，返回string格式内容

========================================================================

[ 绘图语法集合 ]

1  颜色类
color = Color(r,g,b);
[ 参数 ] r:红色权重(0~255) g:绿色权重(0~255) b:蓝色权重(0~255)

2  显示文字(字体宽度是12*12点阵，如果是英文则是6*12)
void cprint(string content,int x,int y,table color);
[ 参数 ] content:文字内容 x,y:文字显示绝对坐标 color:文字颜色

3  清除区域
void clear([table area]);
[ 参数 ] 不填为清除全部区域为透明色，指定矩形区域则为将固定区域清除为透明色

4 设置透明色
void setMaskColor(table color);
[ 参数 ] 设置透明色为color，默认为黑色

4  绘制矩形/填充矩形
void cbox(table area,table color,bool isFillMode=false);
[ 参数 ] 绘制以(x,y)为起点，宽度w，高度h，颜色为color的矩形，如果isFillMode为真则绘制实心矩形，否则绘制空心矩形(默认可以不填，默认空心)

5  绘制线条
void cline(int x0,int y0,int x1,int y1,int width,table color);
[ 参数 ] 绘制从点(x0,y0)到点(x1,y1)的宽度为width，颜色为color的直线

6 显示图片
void cimg(string url,int x,int y[,table area || nil]);
[ 参数 ] 以(x,y)为起点绘制url为路径的某个图片,图片支持大部分常用格式(jpg,bmp,gif,png等)
           如果带有后续参数则将裁剪载入的外部图片，显示其中特定区域area图像
		   如果后续参数为nil则将外部图片缓存到缓冲画区save


7 显示（刷新缓冲区到屏幕）
void cflush([table area]);
[ 参数 ] 你所绘制的所有操作都将保存到内存中的缓冲位图上，调用本函数将缓冲区内容绘制到前台，
            如果你的画图函数绘制完成请调用本函数将所有图像显示出来
			如果不指定参数则为刷新全部区域，如果指定area区域则为只刷新指定区域内容

8 保存图片
void save(nil || string file || table area[,table area1]); 
[ 参数 ] 保存某区域（实际显示的区域）到内存，如果还指定了file则保存到文件(截图)，此方法不保存区域用于还原
           内存中为复制还原有一块专门的保存区,当指定了两个区域时会将画布（实际显示出来的画布）上area区域
		   复制到保存区area1区域上，如果以此方式指定参数则area可以只设置起始位置（宽高由area1决定）,如果只有
		   一个参数nil将清空save临时缓冲区内容。

9 还原图片
void restore([table area[,table area1]]);
[ 参数 ] 将内存中存储的区域缓冲图像还原到原位置(自动刷新)，如果指定了area则还原保存区中area区域部分内容
           (需调用cflush(area)显示)，如果还指定了area1区域则还原保存区area内容到area1中(需调用cflush(area1)显示)

10 设置背景缓冲区
void setBG([nil][Area]);
[ 参数 ] 如果不填将所有当前可见区域(canvas)保存到背景，如果填nil将清空背景缓冲区(bg)画布，如果填Area区域将
           绘制当前可见区域(canvas)内Area区域的内容到背景缓冲区(bg)。

========================================================================

[ 区域语法集合 ]

1 区域类
area = Area(string page,string id);
[ 定义 ] area={x,y,w,h} 以x,y为坐标起点，宽度为w，高度为h的矩形区域
[ 说明 ] 本函数可以取得某页某id对应的区域area，如果不存在将返回nil，例如Area('menu','startGame');

2 点类
po = Point(int x,int y);
[ 参数 ] x:点的横坐标,y:点的纵坐标
 
[ 区域语法集合 ]

1 添加区域
  void addArea(string page,string id,int x,int y,int w,int h);
[ 说明 ]  向区域集合内添加一个新的区域。区域集合Events包含有多个页page(相当于一个画面)，
            每个页可以存放多个区域Area，区域由id唯一标识，但区域允许重合，推荐编程时尽量避免重合。

2 删除区域
  void deleteArea(string page,string id);  删除某页内id对应的区域

3 删除全部区域
  void clearArea(string page);                 删除某页page内的全部区域

4 检测区域
  table Event(string page,int mousex,int mousey);
[ 说明 ]检测在某页内鼠标是否在某一个/一组area上方，如果在将返回所有area的id的集合,如果不在则返回空集合

5 取得区域的起点
  Point getPoint(string page,string id);
[ 参数 ] page:某页 id:某区域标识
[ 说明 ] 取得某页内某区域的起点，返回{x,y}的Point对象

6 取得区域大小
  Point getSize(string page,string id);
[ 参数 ] page:某页 id:某区域标识
[ 说明 ] 取得某页内某区域的大小，返回{w,h}的Point对象

========================================================================

程序制作：黄程
版本：vol 0.4
最后更新时间：2014-1-26
补充内容：
1 添加函数void save(string file || table area[,table area1]);  保存某区域到内存或文件
2 添加函数void restore([table area[,table area1]]);  将内存中存储的区域缓冲图像还原到画布
3 添加函数void setTitle(string title); 设置标题文字
4 添加函数string cinp(string title,string hint); 读取用户输入的数据对话框，返回string格式内容
5 添加类：点，及通过区域获得区域的起点和大小方法getPoint，getSize
6 添加onMouseMove响应事件，实时响应鼠标当前所在位置
7 修改了Area类的存储方式，每个Area由页名page和编号id一起决定，每个页(即画面)可以有n个区域（推荐尽量避免重叠）
8 添加画板bgBMP，实现新的绘图模式：及调用cflush时先绘制bgDC再绘制paintDC,可以随时将canvas内某部分或全部内容
   调用setBG(不填/Area)写入bg上，也可以调用setBG(nil)清空bg内容。
9 添加了setMaskColor方法用于设置透明色（掩码色），在文字透明和图片透明上的处理将更方便。

========================================================================

程序制作：黄程
版本：vol 0.3
最后更新时间：2014-1-25
补充内容：
1 添加了区域对象area，并改变了传入参数模式，所有的x,y,w,h都变为了area	(clear  cbox  cimg cflush)
2 改进clear使其支持以指定颜色清除背景
3 改进cflush刷新可指定某一区域，从缓冲区复制到显示位图以及窗体的重绘都将只应用到新的区域（可以更快的绘制），void cflush([area]);
4 添加csleep(毫秒)方法,能使程序休眠指定的毫秒时间
5 添加语法错误提示，当调用LuaLuncher提供的函数出错时会指示出错的函数。

========================================================================
程序制作：黄程
版本：vol 0.2
最后更新时间：2014-1-24
补充内容：
1 完善cimg，实现贴图功能
2 改变绘图模式，取消onPaint事件，改为在任意位置都可以绘图
3 实现双缓冲技术，使用cflush()可以刷新缓冲区位图到窗口。
4 改进cprint，实现字体美化，并确定字体大小为12*12点阵
5 删除线程刷新画面，改为双缓冲后绘制界面请自己调用clear()和cflush()

========================================================================

程序制作：黄程
版本：vol 0.1
最后更新时间：2014-1-23
实现内容：
1 添加事件：onKeyDown,onMouseDown,onPaint分别实现键盘，鼠标及画面刷新事件。
【注：事件调用方式为在自己的主Lua文件中按文档事件说明部分写自定义函数，程序将自动触发该函数并发送信息给你使用】
2 添加msg方法，实现显示对话框或者选择对话框，选择对话框还可以得到用户选择的选项
3 添加绘图函数cprint，cbox，cline，cimg，clear和Color颜色对象


========================================================================

[ 附录 ] [ index.lua例程示范代码 ]

--对象定义
addArea('main','box0',40,40,20,30);
addArea('main','box1',70,80,20,20);
addArea('main','cl0',42,42,10,10);
addArea('main','Canvas',0,0,100,200);
foreColor = Color(255,255,255);
--事件定义
function onKeyDown(key)
	if(key=="Enter") then
		msg('静夜思',[[
作者：李白
床前明月光，疑是地上霜。
举头望明月，低头思故乡。 
		]]);
	elseif(key=="F1") then
		name = cinp('输入信息','请输入您的名字：');
		setTitle(name..",Lua欢迎您~");
	elseif(key=="F2") then
		save('Capture.jpg');
	else
		msg('按下的键',key);
	end
end
function onMouseDown(mtype,x,y)
	msg('鼠标事件',"鼠标"..mtype.."键被按下，坐标为 ("..x..","..y..")");
end
--开始代码
cprint("你好~世界。",20,20,foreColor);
cbox(Area('main','box0'),Color(234,112,32),true);
cbox(Area('main','box1'),Color(0,112,44));
clear(Area('main','cl0'));
cline(50,50,100,200,1,Color(0,0,0));
cline(50,50,200,100,3,Color(222,222,1));
cflush();			--将绘制内容刷新显示到程序
--结束代码


[ 动态区域代码 ]
--[[
所有的绘图函数都在paint画布上绘制，setBG可以将canvas(直接显示的内容)保存到bg背景画布或者清空背景画布
save命令可以保存canvas到save画布，restore可以还原save画布到paint画布，再次调用cflush显示。
]]
--定义变量
wS = 12;	--文字宽高
bC = Color(0,0,0);  --默认文字颜色
setMaskColor(Color(10,10,10));  --设置掩码色(透明识别色)，设置一个不常用的颜色即可，如果画布上存在相同颜色将显示为黑色。
clear();  --以刚设置的掩码色清空画布paint
cimg('img/bg2.jpg',0,0);  --绘制图片（支持透明）
cprint('显示文字',wS*8,wS*5,bC);
addArea('main','sfTxt',wS*8,wS*5,wS*14,wS);  --设置动态文字区域
cflush();  --绘制背景画布bg到canvas，以掩码色透明绘制paint画布到canvas，再将canvas画布显示出来
setBG();  --将canvas直接复制为背景保存到bg画布
function onMouseDown(mtype,x,y)
	clear(Area('main','sfTxt'));  --清空paint上动态文字区域为掩码色
	cprint('你好啊',wS*8,wS*5,bC);  --往paint上动态文字区域写入新的文字
	cflush(Area('main','sfTxt'));  --刷新动态文字区域内容（先绘制bg画布同区域内容到canvas，再以掩码色透明绘制paint上同区域内容到canvas）
end



[ 画布说明 ]
LuaLu画图系统共使用4块画布绘制，每块画布大小都为窗体大小（除了save缓冲画区）
[ paint画布 ] 主画区
用户绘制的所有内容都是基于paint画布进行操作的，因此我们称paint画布为主画区，
主画区的内容需要调用cflush()方法显示出来，可以显示一部分或者全部显示，paint上
的内容可以调用clear()函数清除部分或全部内容，clear将以掩码色清除，所以推荐先
设置掩码色为一个接近黑色的灰度色。
[ canvas画布 ] 显示画区
用户调用cflush后将主画区全部或部分内容绘制到canvas画布上，系统将立刻把canvas
内容显示到主界面上，因此可以称canvas为显示画区。当使用cflush的时候过度频繁地
使用全部绘制方式，系统每次都会清除界面上所有内容再绘制，如果频率过快将产生闪
屏现象，为避免闪屏推荐您实现动画时调用cflush传入需要重绘的部分区域，系统将局
部刷新该区域内容。
[ bg画布 ] 背景画区
当用户调用setBG命令时，当前显示画区canvas上显示的内容将被全部或部分复制到背
景画区bg，当用户在主画区上调用clear清除某区域后，在调用cflush显示的时候该区域
将显示为背景画区该区域的内容。意思是：只要在主画区上绘制完背景后调用cflush();
setBG();两个命令然后继续绘制动态显示的内容，下次要清除动态显示的内容就只要clear
主画区上动态内容的区域，然后绘制新的内容到该区域，最后clfush刷新相同区域就可以
实现背景不变前景改变的显示模式了。如果要清空背景画区只需调用setBG(nil);即可。
[ save画布 ] 缓冲画区
如果您绘制的动画需要暂时保存,系统将提供您界面的宽高20倍大小的缓冲画区给您缓存
足够的画面内容，如果您的界面大小是400*270，我们给您提供的画区大小是8000*5400
如果您的界面大小是600*500，我们给您提供的画区大小将是12000*10000，您可以用此
存储您的地图数据，角色16宫图或者是任何使用的技能，道具动态图片。您可以通过调用
cimg(图片路径,贴图位置x,贴图位置y,nil);函数将本地图片加载到缓冲画区，也可以调用
save(table area,table area1);将显示画区canvas上area区域内容缓存到缓冲画区area1处
，可以调用save(table area); 将显示画区area区域内容缓存到缓冲画区同位置。如果您要
清空缓存画区可以调用save(nil)命令清空缓冲画区全部内容。save指令还允许您将四个画
区的所有画面截图到文件便于您的程序调试，指令：save('*.jpg'); 您在缓冲画区保存的内
容可以调用restore函数提取到主画区上，例如您可以调用：restore(area,area1);将缓冲
画区area部分的内容绘制到主画区area1区域,然后调用cflush(area1);显示该区域内容。
您也可以调用restore(area);命令直接将缓冲区上area区域内容绘制到主画区相同位置。
您甚至可以直接调用restore();指令将上次您用save缓存的区域绘制直接显示出来而不需要
调用cflush();例如：在绘制时save(area);在需要还原时restore();，不过这么做只会保存最
后一次历史记录区域。

[ 画区关系图 ]

                                                       显示器
                                                          ^
                                                           |
                                                canvas显示画区                                > cflush() 操作
                                                ^                  ^
		                                         |                    |
												 |                   \/
                                        paint主画区     bg背景画区
                                                ^
		                                         |                                                     > save() restore() 操作
		                                        \/
                                       save缓冲画区