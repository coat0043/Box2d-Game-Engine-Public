
-- Player

local dir = vec3(0,0,0)
local bCanJump = true

function Key(button)
	return string.byte(button)
end

function OnInputEvent(go, deviceType, deviceState, key, canJump)
	bCanJump = canJump
	if(deviceType == 0) then -- Is Keyboard
		if(deviceState == 0) then -- Pressed
			if(key == Key("W")) then -- W
				dir.y = 1		
			end
			if(key == Key("A")) then -- A
				dir.x = -1
			end
			if(key == Key("S")) then -- S
				dir.y = -1
			end
			if(key == Key("D")) then -- D
				dir.x = 1
			end
		end
		if(deviceState == 1) then -- Released
			if(key == Key("W")) then -- W
				dir.y = 0
			end
			if(key == Key("A")) then -- A
				dir.x = 0
			end
			if(key == Key("S")) then -- S
				dir.y = 0
			end
			if(key == Key("D")) then -- D
				dir.x = 0
			end
		end
	end
end

function Tick(go, deltaTime)
	Log( "Testing Lua " .. deltaTime );
	--local speed = 5
	--if(go:GetPhysicsBody() ~= nill) then
	--	if (dir.x ~= 0) then
	--		go:GetPhysicsBody():ApplyForceToCenter(dir:Scale(speed))
	--	end
	--	if (dir.y > 0 and bCanJump) then
	--		go:GetPhysicsBody():ApplyImpulseToCenter(dir:Scale(1))
	--		bCanJump = false -- This is partially working for some reason. TODO: fix it
	--	end
	--else
	--	dir:Normalize()
	--	go.position.x = go.position.x + dir.x * deltaTime * speed
	--	go.position.y = go.position.y + dir.y * deltaTime * speed
	--end
end