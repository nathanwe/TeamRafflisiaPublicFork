--menu.lua
showMenu = true
BGMMuted = false
SFXMuted = false
BGMVolumeLocation = nil
SFXVolumeLocation = nil


function Init()
	BGMVolumeLocation, SFXVolumeLocation =GetSoundVolumes();
end

function Update(dt)
	if showMenu then
		BeginImgui("Sound")
		if BGMMuted then
			local unmutePressed = ButtonImgui("Unmute BGM", 100, 50)
			if unmutePressed then
				local AudioEventTable = {}
				AudioEventTable["type"] = 3
				SendAudioEvent(AudioEventTable)
				BGMMuted = false
			end
		else
			local mutePressed = ButtonImgui("Mute BGM", 100, 50)
			if mutePressed then
				local AudioEventTable = {}
				AudioEventTable["type"] = 2
				SendAudioEvent(AudioEventTable)
				BGMMuted = true
			end
		end
		IntSliderImgui("BGM Volume", BGMVolumeLocation, 1, 10)
		if SFXMuted then
			local unmutePressed = ButtonImgui("Unmute SFX", 100, 50)
			if unmutePressed then
				local AudioEventTable = {}
				AudioEventTable["type"] = 11
				SendAudioEvent(AudioEventTable)
				SFXMuted = false
			end
		else
			local mutePressed = ButtonImgui("Mute SFX", 100, 50)
			if mutePressed then
				local AudioEventTable = {}
				AudioEventTable["type"] = 10
				SendAudioEvent(AudioEventTable)
				SFXMuted = true
			end
		end
		IntSliderImgui("SFX Volume", SFXVolumeLocation, 1, 10)
		local playSfxPressed = ButtonImgui("SFX test", 100, 50);
		if playSfxPressed then
			local AudioEventTable = {}
			AudioEventTable["type"] = 9
			AudioEventTable["stringData1"] = "SaberRelay.mp3"
			AudioEventTable["floatData1"] = 1.0
			AudioEventTable["floatData2"] = 1.0
			AudioEventTable["floatData3"] = 1.0
			AudioEventTable["floatData4"] = 1.0
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