speed = 2
cycleTime = 2
timers = {}
directions = {}
thingsToWiggle = {}

function Init()
	GetEntiysByCategory(4)
	if thingsToWiggle[0] == nil then
		print("in init thingsToWiggle hasnt loaded ")
	end
	for index,e in pairs(thingsToWiggle) do
		timers[index] = index
		directions[index] = (index % 2) * 2 -1
	end
end

function AddEntityToList(e)
	table.insert(thingsToWiggle, e)
end


function Update(dt)
	for index,e in pairs(thingsToWiggle) do
		timers[index] = timers[index] + dt
			if timers[index] > cycleTime then
				timers[index] = timers[index] - cycleTime
				directions[index] = directions[index] * -1
			end
		AddToVQS(e, 0, speed*directions[index]*dt, 0)
	end
end