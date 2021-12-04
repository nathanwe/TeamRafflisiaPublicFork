--gun
local coolDown = 3;
local coolDownTimer = 0;
local offset = {}
offset.x = 0.1
offset.y = -0.1
offset.z = 0
local prevDir = {}
prevDir.x = 0
prevDir.y = 0
prevDir.z = 0

function SaveGuns( levelnum )
end

function LoadGuns( levelnum )
end

function ClearGuns()
end

function DestroyGun(e)
end

function UpdateGun(dt, e)
	--cooling down
	if coolDownTimer >= 0 then
		coolDownTimer = coolDownTimer - dt
	end

	--if then
	local positionx, positiony, positionz = GetCameraPosition()
	local directionx, directiony, directionz = GetCameraDirection()
	
	prevDir.x = directionx
	prevDir.y = directiony
	prevDir.z = directionz

	SetPosition(e, positionx + directionx * (0.3), 
	positiony + directiony * 0.3 + offset.y, 
	positionz + directionz * (0.3))
	SetRotationFromDirection(e, directionx, directiony, directionz)

	--print(directionx)
	--print(directiony)
	--print(directionz)

end

function HandleEventGun(eventData)
	if eventData.type == 16 then
		ImguiText("Gun")
		--imguiControledEntity = eventData.e1
	end
	if (eventData.type == 19 and eventData.stringData1 == "Fire") then
		if coolDownTimer <= 0 then
			print(coolDownTimer)
			coolDownTimer = coolDown
			print("fire")
			print(coolDownTimer)
		end
	end
end

function HandleEventPerEntityGun(e, eventData)
end

--function HandleEventGun(eventData)

--end

function InitGun(e)
end