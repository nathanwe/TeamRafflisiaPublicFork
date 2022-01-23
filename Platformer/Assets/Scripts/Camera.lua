--Camera

function Init()
	
	Set3rdPerson(false)
	--SetCameraPitchYaw(0, 3.14)
	--SetCameraOffest(true, 2,2,2)
	SetCameraStaticScene( 30,40,-80 , 0.2, 3.5);
	SetCellShade(false, 1.0)
	SetSunAngle(-28)
end

function Update(dt)
	UpdateCameraOld()
end

function HandleEvent(thingsToEffect, eventData)
end

function HandleEventPerEntity(e, categoriesToEffect, eventData)
end

function Destroy()
end