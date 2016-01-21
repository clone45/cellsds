function on_load()

	local block_count
	local i
	local j

	grid = {}

	for block_count=1,8 do
		grid[block_count] = {}
		for i=1,16 do
 			grid[block_count][i] = {}
 			for j=1,16 do
 				grid[block_count][i][j] = WHITE
 			end
 		end
	end

	-- set globals
	old_column = 1
	selected_sound = 1
	drag_color = WHITE

	display_instructions()
end

function redraw()

	local i
	local j

	for i=1,16 do
		for j=1,16 do
			set_cell(i,j,grid[BLOCK][i][j])
		end
	end

	display_instructions()
end

function display_instructions()
	display_text("Change Sound: Hold [LEFT] and touch a cell",1,1)
	display_text("Clear all cells: Press [Y]",1,12)
end

function pad_newpress_left()
	set_all_cells(WHITE)
	-- todo: set column/row to selected sound if stylus not held
end

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

	local i
	local j

	set_all_cells(WHITE)

	for i=1,16 do
		for j=1,16 do
			grid[BLOCK][i][j] = WHITE
		end
	end
end

function stylus_newpress()

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

	-- draw sequencer notes

	else
 		if grid[BLOCK][X][Y] == WHITE then
 			grid[BLOCK][X][Y] = MEDIUM_GRAY
 		else
 			grid[BLOCK][X][Y] = WHITE
 		end
 	
 		set_cell(X,Y,grid[BLOCK][X][Y])

 		drag_color = grid[BLOCK][X][Y]
	end
end

function stylus_drag()
	
	-- select sound

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

	-- paint sequencer cells
		
	else
		grid[BLOCK][X][Y] = drag_color
		set_cell(X,Y,drag_color)
	end
end

function clock()

	local i
	local j

	if ACTIVE and PAD_HELD_LEFT == 0 and PAD_HELD_RIGHT == 0 then

		-- restore old line
		for i=1,16 do
			set_cell(old_column,i,grid[BLOCK][old_column][i])
		end

		-- display the sequencer line
		set_column(SEQUENCER_STEP_16,LIGHT_GRAY)
	end

	old_column = SEQUENCER_STEP_16

	-- play any notes beneath the sequencer line
	for i=1,16 do
		if grid[BLOCK][SEQUENCER_STEP_16][i] == MEDIUM_GRAY then
			j = 17 - i
			play_note(j,16)
			midi_note(j,16,2) 
			if ACTIVE then set_cell(SEQUENCER_STEP_16,i,LIGHT_BLUE) end
		end
	end

	
end

function save_snapshot()

	local i
	local j
	local block_number

	file = io.open(FILENAME,"w")

	-- write out grid
	for block_number=1,8 do
		for i=1,16 do
			for j=1,16 do
				if grid[block_number][i][j] ~= WHITE then
					-- write out line that looks like: grid[15][10] = 4
					file:write( "grid[" .. block_number .. "][" .. i .. "][" .. j .. "] = " .. grid[block_number][i][j] .. "\n");
				end
			end
		end
	end

	-- write out instrument selection

	file:write("load_sound(" .. selected_sound .. ")\n");
	file:write("selected_sound = " .. selected_sound .. "\n");

	file:close() 

end

function load_snapshot()

	local block_count
	local i
	local j

	for block_count=1,8 do
		for i=1,16 do
 			for j=1,16 do
 				grid[block_count][i][j] = WHITE
 			end
 		end
	end

	dofile(FILENAME)
end