--������
addArea('main','box0',40,40,20,30);
addArea('main','box1',70,80,20,20);
addArea('main','cl0',42,42,10,10);
addArea('main','Canvas',0,0,100,200);
foreColor = Color(255,255,255);
--�¼�����
function onKeyDown(key)
	if(key=="Enter") then
		msg('��ҹ˼',[[
���ߣ����
��ǰ���¹⣬���ǵ���˪��
��ͷ�����£���ͷ˼���硣
		]]);
	elseif(key=="F1") then
		name = cinp('������Ϣ','�������������֣�');
		setTitle(name..",Lua��ӭ��~");
	elseif(key=="F2") then
		save('Capture.jpg');
	else
		msg('���µļ�',key);
	end
end
function onMouseDown(mtype,x,y)
	msg('����¼�',"���"..mtype.."�������£�����Ϊ ("..x..","..y..")");
end
--��ʼ����
cprint("���~���硣",20,20,foreColor);
cbox(Area('main','box0'),Color(234,112,32),true);
cbox(Area('main','box1'),Color(0,112,44));
clear(Area('main','cl0'));
cline(50,50,100,200,1,Color(0,0,0));
cline(50,50,200,100,3,Color(222,222,1));
cflush();			--����������ˢ����ʾ������
--��������
