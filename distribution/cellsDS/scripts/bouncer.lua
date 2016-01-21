function on_load()

	local i
	local j
	local block_count

	ball = {}

	for block_count=1,8 do
		ball[block_count] = {}
		for i=1,16 do
			ball[block_count][i] = {}
			ball[block_count][i]["height"] = 16
			ball[block_count][i]["direction"] = 0 -- 1=down, -1=up
			ball[block_count][i]["position"] = 0
		end
		
	end

	selected_sound = 1

	display_instructions()
end

function redraw()

	local i
	local j

	set_row(16,WHITE)

	for i=1,16 do
		if ball[BLOCK][i]["height"] ~= 16 then
			if ball[BLOCK][i]["height"] >= 1 then
				for j=1,ball[BLOCK][i]["height"] do
 					set_cell(i,j,WHITE)			
				end
			end
			if ball[BLOCK][i]["height"] <= 15 then
				for j=ball[BLOCK][i]["height"],15 do
 					set_cell(i,j,LIGHT_GRAY)
 				end
			end
		else
			set_column(i,WHITE)
		end
	end

	display_instructions()
end

function display_instructions()
	display_text("Change Sound: Hold [LEFT] and touch a cell",1,1)
	display_text("Clear all cells: Press [Y]",1,12)
	display_text("Create ball: Draw on screen",1,24)
	display_text("Remove ball: Touch cell under gray column",1,36)
end

function pad_newpress_left()
	set_all_cells(WHITE)
end

-- select pan
function pad_newpress_right()
	set_all_cells(WHITE)
end

function pad_released_left()
	-- If the left pad is released, it means that the user has exited from selecting a sound.
	-- so redraw the sequencer grid
	redraw()
end

function pad_released_right()
	-- stop setting pan, redraw sequencer
	redraw()
end

function pad_newpress_y()

	local column

	set_all_cells(WHITE)

	for column=1,16 do
		-- turn off all bouncing balls by putting height at 16 (which means "off")
		ball[BLOCK][column]["height"] = 16 
	end
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
		selected_sound = X + ((Y-1)*16)
		load_sound(selected_sound)
		play_note(8,16);

	-- set pan

	elseif PAD_HELD_RIGHT == 1 then

		set_all_cells(WHITE)
		set_column(X,LIGHT_GRAY)
		set_pan(X)		

	-- change the maximum height of one of the bouncing ball columns

	else
		set_column(X,WHITE)

		ball[BLOCK][X]["height"] = Y;

		if Y < 16 then
			for i=Y,15 do
				set_cell(X,i,LIGHT_GRAY)
			end
			ball[BLOCK][X]["position"] = 15
			ball[BLOCK][X]["direction"] = -1
		end
	end
end

function stylus_drag()
	
	if PAD_HELD_LEFT == 1 then

		-- draw crosshair
		set_all_cells(WHITE)
		set_column(X,LIGHT_GRAY)
		set_row(Y,LIGHT_GRAY)

		-- load and play note
		load_sound(X + ((Y-1)*16))
		play_note(8,16);

	-- if dragging to change PANNING
	elseif PAD_HELD_RIGHT == 1 then

		-- draw crosshair
		set_all_cells(WHITE)
		set_column(X,LIGHT_GRAY)
		set_pan(X)
	else
		set_column(X,WHITE)

		ball[BLOCK][X]["height"] = Y;
		
		if Y < 16 then
			for i=Y,15 do
				set_cell(X,i,LIGHT_GRAY)
			end
			ball[BLOCK][X]["position"] = 15
			ball[BLOCK][X]["direction"] = -1
		end
	end
end

function clock()

	local column

	-- do the bouncing ball action
	for column=1,16 do
		if ball[BLOCK][column]["height"] < 16 then

			-- clear out old cell
			set_cell(column,ball[BLOCK][column]["position"],LIGHT_GRAY)

			-- move bouncing ball
			ball[BLOCK][column]["position"] = ball[BLOCK][column]["position"] + ball[BLOCK][column]["direction"]
			if ball[BLOCK][column]["position"] <= ball[BLOCK][column]["height"] then ball[BLOCK][column]["direction"] = 1 end
			if ball[BLOCK][column]["position"] == 15 then 
				ball[BLOCK][column]["direction"] = -1
				play_note(column,100)
				midi_note(column,16,2)
			end

			-- draw the ball
			if PAD_HELD_LEFT == 0 and PAD_HELD_RIGHT == 0 then
				set_cell(column,ball[BLOCK][column]["position"],LIGHT_BLUE)
			end
		end
	end
	
end

function save_snapshot()

	file = io.open(FILENAME,"w")

	local column
	local block_count

	for block_count=1,8 do
		for column=1,16 do
			file:write( "ball[" .. block_count .. "][" .. column .. "]['height'] = " .. ball[block_count][column]["height"] .. "\n");
			file:write( "ball[" .. block_count .. "][" .. column .. "]['position'] = " .. ball[block_count][column]["position"] .. "\n");
			file:write( "ball[" .. block_count .. "][" .. column .. "]['direction'] = " .. ball[block_count][column]["direction"] .. "\n");
		end
	end

	-- write out instrument selection

	file:write("load_sound(" .. selected_sound .. ")\n");	
	file:write("selected_sound = " .. selected_sound .. "\n");

	file:close() 

end

function load_snapshot()
	dofile(FILENAME)
end