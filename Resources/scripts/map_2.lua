message = "I'm testing!!!!"

-- Delay = -1 means wait for the end of this action, 0 means start next action at once, other numbers mean how many minutes should wait
EnterCinematic = {
	{ObjectType = "GameRole", ObjectName = "doll", EventName = "say", EventData = "say_shock", Delay = 1},
	{ObjectType = "GameRole", ObjectName = "girl", EventName = "say", EventData = "say_shock", Delay = -1},
	-- {ObjectType = "GameRole", ObjectName = "girl", EventName = "turn_around", Delay = -1},
	{ObjectType = "GameRole", ObjectName = "girl", EventName = "say", EventData = "say_cry", Delay = -1},
}
