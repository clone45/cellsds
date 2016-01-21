function on_load()

	recorder_x = {}
	recorder_y = {}
	state = 0
	counter = 0
	old_x = 0
	old_y = 0

	local i

	for i=1,32 do
		recorder_x[i] = 0
		recorder_y[i] = 0
	end

	redraw()	
end

function redraw()
	set_all_cells(WHITE)
	set_column(4,LIGHT_GRAY)
	set_column(8,LIGHT_GRAY)
	set_column(12,LIGHT_GRAY)
end

function pad_released_y()
	redraw()
end

function pad_released_left()
	redraw()
end

function stylus_newpress()

	local i

	-- choose sound

	if PAD_HELD_LEFT == 1 then

		-- draw crosshair
		set_all_cells(WHITE)
		set_column(X,LIGHT_GRAY)
		set_row(Y,LIGHT_GRAY)

		-- load and play new sound (8,16 is note number, volume)	
		load_sound(X + ((Y-1)*16))
		play_note(8,16);

	elseif PAD_HELD_Y == 1 then

		-- draw crosshair
		set_all_cells(WHITE)
		set_column(X,LIGHT_GRAY)
		set_pan(X)
	end
end

function stylus_drag()
	if PAD_HELD_Y == 1 then

		-- draw crosshair
		set_all_cells(WHITE)
		set_column(X,LIGHT_GRAY)
		set_pan(X)
	end
end

function clock()

	local playsound = 0

	if (STYLUS_HELD == 1 and PAD_HELD_LEFT ~= 1) or PAD_HELD_RIGHT == 1 then

		if (SEQUENCER_STEP_16 % 2 == 1) and (Y == 1) then playsound = 1 end
		if (SEQUENCER_STEP_16 % 4 == 1) and (Y == 2) then playsound = 1 end
		if (SEQUENCER_STEP_16 % 8 == 1) and (Y == 3) then playsound = 1 end
		if (SEQUENCER_STEP_16 % 16 == 1) and (Y == 4) then playsound = 1 end
		if (SEQUENCER_STEP_16 % 32 == 1) and (Y == 5) then playsound = 1 end
	end

	-- clear old note indicator
	if old_y ~= 0 then
		if old_x == 4 or old_x == 8 or old_x == 12 then
			set_cell(old_x,old_y,LIGHT_GRAY)
		else
			set_cell(old_x,old_y,WHITE)
		end
	end

	if playsound == 1 then
		play_note(X,100)
		set_cell(X,Y,DARK_BLUE)
		state = 1
	else
		set_cell(X,Y,LIGHT_GRAY)
		state = 0
	end

	old_x = X
	old_y = Y
end