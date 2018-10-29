--[[

Lua Luncher Libruary

{���ļ�Ϊ��������ļ�������ɾ������������������޸�ǰ����룬�������������׷���Զ��庯��}

�����������Ƴ�
�汾��vol 0.3
������ʱ�䣺2014-1-26

]]

function Color(r,g,b)
	local c = {r,g,b};
	return c;
end

function Point(x,y)
	local p = {x,y};
	return p;
end

--������

Events = {};--�������

function addArea(page,id,x,y,w,h)	--����������
	if(Events[page] == nil) then Events[page]={}; end
	Events[page][id] = {x,y,w,h};
end

function Area(page,id)
	return Events[page][id];
end

function deleteArea(page,id)
	table.remove(Events[page],id);
end

function clearArea(page)
	table.remove(Events[page]);
end

function Event(page,x,y)
	local inArea={};
	for id,area in pairs(Events[page]) do
		if(x>=area[1] and x<=area[1]+area[3] and y>=area[2] and y<=area[2]+area[4]) then table.insert(inArea,id); end
	end
	return inArea;
end

function getPoint(page,id)
	local p={Events[page][id][1],Events[page][id][2]};
	return p;
end

function getSize(page,id)
	local p={Events[page][id][3],Events[page][id][4]};
	return p;
end

--����������

function readData(url)
	local fin = io.open(url, "r");
	data = {};
	i=1;
	repeat
		dat = fin:read();
		if(dat==nil) then break end
		data[i] = dat;
		i = i + 1;
	until dat==nil;
	io.close(fin);
	return data;
end

function readText(url)
	io.input(url);
	return io.read("*all");
end

function writeData(url,data)
	local fout = io.open(url, "w+");
	local key,values=nil,nil;
	for key,values in pairs(data) do
		fout:write(values.."\n");
	end
	fout:flush();
	io.close(fout);
end


-- �ڴ˴���ӳ�������ǰ���������


--
