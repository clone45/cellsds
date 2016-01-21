function on_load()

	local i
	local j

	-- define global clock division
	clock_division = 1

	-- define global grid for holding sequencer state
	grid = {}

	for i=1,16 do
		grid[i] = {}
 		for j=1,16 do
 			grid[i][j] = WHITE
 		end
	end

	-- set globals
	old_column = 1
	selected_sound = 1
	drag_color = WHITE

	-- seed random number generator
	math.randomseed(os.time())

	display_instructions()
end

function redraw()
	set_all_cells(WHITE)
	display_instructions()
end

function display_instructions()
	display_text("Change Sound: Hold [LEFT] and touch a cell",1,1)
	display_text("Slower: Press [B]",1,12)
	display_text("Faster: Press [X]",1,24)
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
	set_all_cells(WHITE)
end

function pad_released_right()
	set_all_cells(WHITE)
end

function pad_newpress_b()
	if clock_division < 16 then
		clock_division = clock_division + 1
		set_column(16,WHITE);
		set_cell(16,clock_division,CYAN)
	end
end

function pad_newpress_x()
	if clock_division > 1 then
		clock_division = clock_division - 1
		set_column(16,WHITE);
		set_cell(16,clock_division,CYAN)
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

	end
end

function clock()

	local column
	local row

	local note1
	local note2 

	if math.mod(SEQUENCER_STEP_16,clock_division) == 0 then

 		-- draw a new pair of notes
 		if math.random(4) == 1 then
 			note1 = math.random(1,12)
 			note2 = math.random(2,4) + note1
	
			play_note(note1,16)
			play_note(note2,16)
			midi_note(note1,16,2)
			midi_note(note2,16,2)

			set_row(8,WHITE)
			set_cell(note1,8,CYAN)
			set_cell(note2,8,CYAN)
 		end
	end
end
