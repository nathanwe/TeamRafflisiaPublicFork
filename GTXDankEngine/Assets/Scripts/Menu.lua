--menu.lua
showMenu = true


function Init()
	--InitImgui()

end

function Update(dt)
	if showMenu then
		BeginImgui("Sound")
		local mutePressed = ButtonImgui("Mute")
		local unmutePressed = ButtonImgui("Unmute")
		if mutePressed then
			local AudioEventTable = {}
			AudioEventTable["type"] = 2
			SendAudioEvent(AudioEventTable)
		end
		if unmutePressed then
			local AudioEventTable = {}
			AudioEventTable["type"] = 3
			SendAudioEvent(AudioEventTable)
		end
		EndImgui()
	end
end



function HandleEvent(thingsToEffect, eventData)
	if eventData.type == 1 then
		showMenu = not showMenu;
	end
end

function HandleEventPerEntity(e, categoriesToEffect, eventData)

end

function Destroy()

end