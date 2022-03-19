--Camera
local post = {[0] = 1, [1] = 2}
function Init()
	
	Set3rdPerson(false)
	--SetCameraPitchYaw(0, 3.14)
	--SetCameraOffest(true, 2,2,2)
	SetCameraStaticScene( 0,30,-80 , 0.0, 3.14);
	SetSunAngle(-28)
	SetPostProcess(2)
end

function Update(dt)
	UpdateCameraOld()
end

function HandleEvent(thingsToEffect, eventData)
	if eventData.type == 14 then
		if post[eventData.intData1] == nil then
			SetPostProcess(2)
		else
			SetPostProcess(post[eventData.intData1])
		end
	end
end

function HandleEventPerEntity(e, categoriesToEffect, eventData)
end

function Destroy()
end