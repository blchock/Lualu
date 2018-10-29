--[[

Lua Luncher Libruary

{本文件为程序必须文件，请勿删除或改名，请勿随意修改前面代码，但可以在最后面追加自定义函数}

程序制作：黄程
版本：vol 0.3
最后更新时间：2014-1-26

]]

function Color(r,g,b)
	local c = {r,g,b};
	return c;
end

function Point(x,y)
	local p = {x,y};
	return p;
end

--区域函数

Events = {};--区域对象集

function addArea(page,id,x,y,w,h)	--添加区域对象
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

--区域函数结束

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


-- 在此处添加程序运行前载入的内容


--
