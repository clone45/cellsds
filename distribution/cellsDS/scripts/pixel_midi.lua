function on_load()
	old_x = 1
	old_y = 1
end

function stylus_newpress()

	set_cell(old_x,old_y,WHITE)

	old_x = X
	old_y = Y

	set_cell(X,Y,DARK_BLUE)
	set_pan(X)	
	play_note(17-Y,16)
	midi_note(17-Y,127,2) -- cell, volume, duration (in sequencer steps)
	-- send_midi_note_on(1,64,127) -- channel, note, velocity
end

