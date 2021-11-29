--Camera

function Init()
	Set3rdPerson(true)
	SetCameraOffest(true, 1,1,1)
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