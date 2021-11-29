--Camera

function Init()
<<<<<<< Updated upstream
	Set3rdPerson(true)
=======
	Set3rdPerson(false)
	Set3rdPersonDistance(10)
>>>>>>> Stashed changes
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