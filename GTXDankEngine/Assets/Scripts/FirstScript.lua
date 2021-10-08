dataTable = {}
dataTable[0] = 12345
dataTable[1] = "hello world"
dataTable[2] = 12345
yetAntotherTable = {foo = "bar", baz = "munge"}
dataTable[3] = yetAntotherTable
dataTable[4] = yetAntotherTable.baz
reps = 0;

function Init()
end

function luaHelperFunc( a )
	return a+10
end

function Update(dt)
	if reps < 6 then
		print("[LUA] Update called for the " .. reps .. "'th time\n")
		print("dt = " ..dt .. "\n")
		reps = reps + 1
		print(dataTable[reps])
		a = GetExampleData(1)
		b = GetExampleData(2)
		c = HostFunction(luaHelperFunc(a),b*3)
		print("c is "..c)
	end
end