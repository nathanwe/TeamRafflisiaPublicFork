--Sliderthings
local timers = {}
local directions = {}
local imguiControledEntity = -1
local cycleTime = 5
local speed = 5

function SaveSliders( levelnum )
	levelstr = string.format("%i", levelnum)
	SaveIntFloatTableAsJson(timers, "/Assets/Levels/Level" .. levelstr .."SliderTimerSave.json")
	SaveIntFloatTableAsJson(directions, "/Assets/Levels/Level" .. levelstr .."SliderDirectionSave.json")
end

function LoadSliders( levelnum )
	levelstr = string.format("%i", levelnum)
	timers = LoadIntFloatTableFromJson("/Assets/Levels/Level" .. levelstr .."SliderTimerSave.json")
	directions = LoadIntFloatTableFromJson("/Assets/Levels/Level" .. levelstr .."SliderDirectionSave.json")
end

function ClearSliders()
	timers = {}
	directions = {}
end

function DestroySlider(e)
	timers[e] = nil;
	directions[e] = nil
end


function UpdateSlider(dt, e)
	if timers[e] == nil then
		InitSlider(e)
	end

	timers[e] = timers[e] + dt
		if timers[e] > cycleTime then
			timers[e] = timers[e] - cycleTime
			directions[e] = directions[e] * -1
		end
	--AddToVQS(e, 0, 0, speed*directions[e]*dt)
	--UpdatePhysicsCollider()
end

function HandleEventSlider(eventData)
	if eventData.type == 5 then
		DestroySlider(eventData.e1)
	end
	if eventData.type == 16 then
		ImguiText("Slider")
		ImguiControledFloat(0, "timers", timers[eventData.e1])
		ImguiControledFloat(1, "directions", directions[eventData.e1])
		imguiControledEntity = eventData.e1
	end
	if eventData.type == 17 then
		if imguiControledEntity ~= -1 then
			timers[imguiControledEntity] = GetImguiControledFloat(0)
			directions[imguiControledEntity] = GetImguiControledFloat(1)
		imguiControledEntity = -1
		end
	end
end

function HandleEventPerEntitySlider(e, eventData)
	if eventData.type == 7 then
		DeleteEntity(e)
	end
end

--custom functions

function InitSlider(e)
	timers[e] = e
	directions[e] = 1
end