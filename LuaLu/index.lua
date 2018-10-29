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
