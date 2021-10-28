--menu.lua
showMenu = true
muted = false


function Init()
	--InitImgui()

end

function Update(dt)
	if showMenu then
		BeginImgui("Sound")
		if muted then
		
			local unmutePressed = ButtonImgui("Unmute BGM")
			if unmutePressed then
				local AudioEventTable = {}
				AudioEventTable["type"] = 3
				SendAudioEvent(AudioEventTable)
				muted = false
			end
		else
			local mutePressed = ButtonImgui("Mute BGM")
			if mutePressed then
				local AudioEventTable = {}
				AudioEventTable["type"] = 2
				SendAudioEvent(AudioEventTable)
				muted = true
			end
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