// Supporting functions

int get_engine_id(lua_State *L)
{
   int engine_id;
   
   lua_getglobal(L, "ENGINE_ID");
	engine_id = lua_tonumber(L, -1);
	lua_pop(L, 1);

   return(engine_id);
}

int draw_cell(int x, int y, int color)
{
   int width = 13;
   int height = 10;
   int screen = 0;
   
   if ((x >= 1) && (x <= 16) && (y >= 1) && (y <= 16) && (color >= 0) && (color <= MAX_COLORS))
   {
		x = ((x-1)*CELL_WIDTH)+GRID_OFFSET_X+1;
		y = ((y-1)*CELL_HEIGHT)+GRID_OFFSET_Y+1;

		// draw8bitRectEx(((x-1)*CELL_WIDTH)+GRID_OFFSET_X+1,((y-1)*CELL_HEIGHT)+GRID_OFFSET_Y+1,(CELL_WIDTH - 1),(CELL_HEIGHT - 1),0,color);  

		// Draw top line
		u16 lastX = x + width;
		u16 i;
		
		for (i = x; i < lastX; i++)
		{
			PA_Put8bitPixel(screen, i, y, color);
		}
	
		// Calculate DMA copy values - we need to deal with even values only
		u16 dmaX = (x + 1) & ~1;
		s32 dmaWidth = width;
		bool drawRight = false;
	
		// Did we convert to an even x value?
		if (dmaX != x)
		{
			// Reduce the width to compensate
			dmaWidth--;
	
			// Remember that we need to draw an extra pixel
			drawRight = true;
		}
	
		// Pre-emptive bitshift to save calculations
		dmaX = dmaX >> 1;
		dmaWidth = (dmaWidth & 1 ? dmaWidth - 1 : dmaWidth) >> 1;
	
		// Calculate last line to draw
		u16 lastY = y + height;
		lastX = x + width - 1;
	
		// Precalculate line values for loop
		u16* line0 = PA_DrawBg[screen] + dmaX + (y << 7);
		u16* linei = line0 + (1 << 7);
	
		// Loop through all lines
		for (i = y + 1; i < lastY; i++)
		{
	
			// Perform DMA copy
			if (dmaWidth > 0) DMA_Copy(line0, linei, dmaWidth, DMA_16NOW);
	
			// Fill left gap
			if (x & 1) PA_Put8bitPixel(screen, x, i, color);
	
			// Fill right gap
			if ((width & 1) || (drawRight)) PA_Put8bitPixel(screen, lastX, i, color);
	
			// Move to next line
			linei += (1 << 7);
		}
	}	
	return(0);
}

// Callback functions

void _set_cell(int engine_id, int x, int y, int color)
{
	if (engines[engine_id]->active)
	{
		if (grid[x-1][y-1] != color) // don't bother drawing a cell if it's already the color
		{
			draw_cell(x,y,color);
			grid[x-1][y-1] = color;
		}		
	} 
}


void _set_all_cells(int engine_id, int color)
{
	int x,y;
	
	if (engines[engine_id]->active)
	{	
	   // clear the screen
		for (x=1;x<=16;x++)
		{
		   for (y=1;y<=16;y++)
		   {
		      _set_cell(engine_id,x,y,color);
			}		
		}  
	}	 
}

void _set_column(int engine_id, int column, int color)
{
   int y;

	if (column >= 1 && column <= 16)
	{
		if (engines[engine_id]->active)
		{	
		   for (y=1;y<=16;y++)
		   {
		      _set_cell(engine_id,column,y,color);
			}
		}				
	}
}

void _set_row(int engine_id, int row, int color)
{   
   int x;

	if (row >= 1 && row <= 16)
	{		
		if (engines[engine_id]->active)
		{		
		   for (x=1;x<=16;x++)
		   {
		      _set_cell(engine_id,x,row,color);
			}
		}				
	}
}

void _play_note(int engine_id, int note, int volume)
{
   int multisample_part = 0;

	if (engines[engine_id]->voice[multisample_part].is_playable())
	{
		if (volume > 16){ 
		   volume = 127;
		}
		else {
		   volume = (8 * volume) - 1;
		}
			
		if (volume > 127) volume = 127; 
		if (volume < 0) volume = 0;
				
		engines[engine_id]->play_note(note,volume);
	   // engines[engine_id]->voice[multisample_part].play((scale[note]/2),volume);
	}
}

void _play_frequency(int engine_id, int frequency, int volume)
{
   int multisample_part = 0;

	if (engines[engine_id]->voice[multisample_part].is_playable())
	{
		if (volume > 16){ 
		   volume = 127;
		}
		else {
		   volume = (8 * volume) - 1;
		}
			
		if (volume > 127) volume = 127; 
		if (volume < 0) volume = 0;
				
		engines[engine_id]->play_frequency(frequency,volume);
	}
}

void _set_block(int block)
{  
   int engine_id;
      
   if (block > 0 && block < 9)
   {
		for (engine_id = 0; engine_id < NUMBER_OF_ENGINES; engine_id++)
		{	
			engines[engine_id]->set_block_en(block);
		}
	}
}

void _set_global_variable(string variable_name, int variable_value)
{  
   int engine_id;
      
	for (engine_id = 0; engine_id < NUMBER_OF_ENGINES; engine_id++)
	{	
		engines[engine_id]->_set_lua_global((char*)variable_name.c_str(), variable_value);
	}
}	

void _display_text(int engine_id, string text_to_display, int x, int y)
{  
	if (engines[engine_id]->active)
	{
		// PA_OutputText(TOP_SCREEN,x,y,(char*)text_to_display.c_str());
		PA_8bitText(TOP_SCREEN, x, y, 255, 90,(char*)text_to_display.c_str(),WHITE,1,0,100);
	}	
}

void _load_sound(int engine_id, int sound_slot_number)
{
	char filename[100];
	int multisample_part = 0;
	
	sprintf(filename,"/cellsDS/sounds/sound%d.raw",sound_slot_number);
	engines[engine_id]->voice[multisample_part].load(filename);
}

void _set_pan(int engine_id, int pan)
{
 	int multisample_part = 0;	 
	int converted_pan;
	
	// (0=left, 64=center, 127=right) 

	pan = 17 - pan;
	
	if (pan >= 1 && pan <= 16)
	{
		converted_pan = (int)(((pan-1)/15.0) * 127);	
		engines[engine_id]->voice[multisample_part].set_pan(converted_pan);
	} 
}

void _midi_note(int engine_id, int cell, int velocity, int duration)
{
   int note;
   int channel;
   int cell_and_channel;

	channel = engine_id + 1;
	
	if (midi_module.get_status() == MIDI_STATUS_CONNECTED)
	{
		if ((cell >= 1) && (cell <= 16) && (duration > 0) && (velocity >= 1) && (velocity <= 16))
		{
		   velocity = (velocity * 8) - 1;
		   
			note = midi_table[cell];
	
			// add note to midi_notes map, which keeps track of all active notes
			
			// 10000 110 = cell 16, channel 6
			
			cell_and_channel = (cell << 3) | channel;
			midi_notes[cell_and_channel] = duration;
			
			// send the midi note
			midi_module.send_note_on(channel, note, velocity);
		}	
	}	
}

int _clear_top_screen(int engine_id)
{
	if (engines[engine_id]->active) PA_Clear8bitBg(TOP_SCREEN);	
	
	return(0);
}

int set_scale_frequency(lua_State *L)
{
   int scale_index = luaL_checknumber(L, 1);
   int frequency = luaL_checknumber(L, 2);
   
   if (scale_index >= 1 && scale_index <= 16) scale[scale_index - 1] = frequency;
	
	return(0);
}

int get_scale_frequency(lua_State *L)
{
   int scale_index = luaL_checknumber(L, 1);
   
	if (scale_index >= 1 && scale_index <= 16) lua_pushnumber(L, scale[scale_index - 1]); // return frequency for scale index

	return 1; // number of return values
}

int send_midi_note_on(lua_State *L)
{
   if (midi_module.get_status() == MIDI_STATUS_CONNECTED) midi_module.send_note_on(luaL_checknumber(L, 1),luaL_checknumber(L, 2),luaL_checknumber(L, 3)); // channel, note, volume
	return(0);
}

int send_midi_note_off(lua_State *L)
{
   if (midi_module.get_status() == MIDI_STATUS_CONNECTED) midi_module.send_note_off(luaL_checknumber(L, 1),luaL_checknumber(L, 2)); // channel, note
   return(0);
}

int send_midi_message(lua_State *L)
{
   if (midi_module.get_status() == MIDI_STATUS_CONNECTED) midi_module.send(luaL_checknumber(L, 1),luaL_checknumber(L, 2),luaL_checknumber(L, 3)); // message, data1, data2
   return(0);
}

// ================= actual callbacks ========================

size_t len;

// These callbacks are repeated for each engine so that I don't have to do a costly reverse-lookup into the Lua namespace to determine the engine_id

int set_block(lua_State *L) { _set_block(luaL_checknumber(L, 1)); return(0); }  // _set_block(block)
int set_global_variable(lua_State *L) { _set_global_variable(luaL_checklstring(L, 1, &len),luaL_checknumber(L, 2)); return(0); }

/*
int load_sound(lua_State *L) { _load_sound(get_engine_id(L), luaL_checknumber(L, 1)); return(0); } // _load_sound(engine_id, sound_slot_number)
int play_note(lua_State *L) { _play_note(get_engine_id(L), luaL_checknumber(L, 1) - 1, luaL_checknumber(L, 2)); return(0);	}  // _play_note(engine_id, note, volume) 
int set_pan(lua_State *L) { _set_pan(get_engine_id(L), luaL_checknumber(L, 1)); return(0); } // _set_pan(engine_id, pan)
int set_all_cells(lua_State *L) {_set_all_cells(get_engine_id(L), luaL_checknumber(L, 1)); return(0); } // _set_all_cells(engine_id, color)
int set_cell(lua_State *L) { _set_cell(get_engine_id(L), luaL_checknumber(L, 1), luaL_checknumber(L, 2), luaL_checknumber(L, 3)); return(0); } // _set_cell(engine_id, x, y, color)
int set_column(lua_State *L) { _set_column(get_engine_id(L), luaL_checknumber(L, 1), luaL_checknumber(L, 2)); return(0); } // _set_column(engine_id, column, color)
int set_row(lua_State *L) { _set_row(get_engine_id(L), luaL_checknumber(L, 1), luaL_checknumber(L, 2)); return(0); } // _set_row(engine_id, row, color)
*/

int load_sound0(lua_State *L) { _load_sound(0, luaL_checknumber(L, 1)); return(0); } // _load_sound(engine_id, sound_slot_number)
int play_note0(lua_State *L) { _play_note(0, luaL_checknumber(L, 1) - 1, luaL_checknumber(L, 2)); return(0);	}  // _play_note(engine_id, note, volume) 
int play_frequency0(lua_State *L) { _play_frequency(0, luaL_checknumber(L, 1), luaL_checknumber(L, 2)); return(0); } // _play_frequency(engine_id, frequency, volume)
int set_pan0(lua_State *L) { _set_pan(0, luaL_checknumber(L, 1)); return(0); } // _set_pan(engine_id, pan)
int set_all_cells0(lua_State *L) {_set_all_cells(0, luaL_checknumber(L, 1)); return(0); } // _set_all_cells(engine_id, color)
int set_cell0(lua_State *L) { _set_cell(0, luaL_checknumber(L, 1), luaL_checknumber(L, 2), luaL_checknumber(L, 3)); return(0); } // _set_cell(engine_id, x, y, color)
int set_column0(lua_State *L) { _set_column(0, luaL_checknumber(L, 1), luaL_checknumber(L, 2)); return(0); } // _set_column(engine_id, column, color)
int set_row0(lua_State *L) { _set_row(0, luaL_checknumber(L, 1), luaL_checknumber(L, 2)); return(0); } // _set_row(engine_id, row, color)
int display_text0(lua_State *L) { _display_text(0,luaL_checklstring(L, 1, &len),luaL_checknumber(L, 2),luaL_checknumber(L, 3)); return(0); }
int midi_note0(lua_State *L) { _midi_note(0, luaL_checknumber(L, 1), luaL_checknumber(L, 2), luaL_checknumber(L, 3)); return(0); } // _midi_note(engine_id, cell, volume, duration)
int clear_top_screen0(lua_State *L) { _clear_top_screen(0); return(0); }

int load_sound1(lua_State *L) { _load_sound(1, luaL_checknumber(L, 1)); return(0); } // _load_sound(engine_id, sound_slot_number)
int play_note1(lua_State *L) { _play_note(1, luaL_checknumber(L, 1) - 1, luaL_checknumber(L, 2)); return(0);	}  // _play_note(engine_id, note, volume) 
int play_frequency1(lua_State *L) { _play_frequency(1, luaL_checknumber(L, 1), luaL_checknumber(L, 2)); return(0); } // _play_frequency(engine_id, frequency, volume)
int set_pan1(lua_State *L) { _set_pan(1, luaL_checknumber(L, 1)); return(0); } // _set_pan(engine_id, pan)
int set_all_cells1(lua_State *L) {_set_all_cells(1, luaL_checknumber(L, 1)); return(0); } // _set_all_cells(engine_id, color)
int set_cell1(lua_State *L) { _set_cell(1, luaL_checknumber(L, 1), luaL_checknumber(L, 2), luaL_checknumber(L, 3)); return(0); } // _set_cell(engine_id, x, y, color)
int set_column1(lua_State *L) { _set_column(1, luaL_checknumber(L, 1), luaL_checknumber(L, 2)); return(0); } // _set_column(engine_id, column, color)
int set_row1(lua_State *L) { _set_row(1, luaL_checknumber(L, 1), luaL_checknumber(L, 2)); return(0); } // _set_row(engine_id, row, color)
int display_text1(lua_State *L) { _display_text(1,luaL_checklstring(L, 1, &len),luaL_checknumber(L, 2),luaL_checknumber(L, 3)); return(0); }
int midi_note1(lua_State *L) { _midi_note(1, luaL_checknumber(L, 1), luaL_checknumber(L, 2), luaL_checknumber(L, 3)); return(0); } // _midi_note(engine_id, cell, volume, duration)
int clear_top_screen1(lua_State *L) { _clear_top_screen(1); return(0); }

int load_sound2(lua_State *L) { _load_sound(2, luaL_checknumber(L, 1)); return(0); } // _load_sound(engine_id, sound_slot_number)
int play_note2(lua_State *L) { _play_note(2, luaL_checknumber(L, 1) - 1, luaL_checknumber(L, 2)); return(0);	}  // _play_note(engine_id, note, volume) 
int play_frequency2(lua_State *L) { _play_frequency(2, luaL_checknumber(L, 1), luaL_checknumber(L, 2)); return(0); } // _play_frequency(engine_id, frequency, volume)
int set_pan2(lua_State *L) { _set_pan(2, luaL_checknumber(L, 1)); return(0); } // _set_pan(engine_id, pan)
int set_all_cells2(lua_State *L) {_set_all_cells(2, luaL_checknumber(L, 1)); return(0); } // _set_all_cells(engine_id, color)
int set_cell2(lua_State *L) { _set_cell(2, luaL_checknumber(L, 1), luaL_checknumber(L, 2), luaL_checknumber(L, 3)); return(0); } // _set_cell(engine_id, x, y, color)
int set_column2(lua_State *L) { _set_column(2, luaL_checknumber(L, 1), luaL_checknumber(L, 2)); return(0); } // _set_column(engine_id, column, color)
int set_row2(lua_State *L) { _set_row(2, luaL_checknumber(L, 1), luaL_checknumber(L, 2)); return(0); } // _set_row(engine_id, row, color)
int display_text2(lua_State *L) { _display_text(2,luaL_checklstring(L, 1, &len),luaL_checknumber(L, 2),luaL_checknumber(L, 3)); return(0); }
int midi_note2(lua_State *L) { _midi_note(2, luaL_checknumber(L, 1), luaL_checknumber(L, 2), luaL_checknumber(L, 3)); return(0); } // _midi_note(engine_id, cell, volume, duration)
int clear_top_screen2(lua_State *L) { _clear_top_screen(2); return(0); }

int load_sound3(lua_State *L) { _load_sound(3, luaL_checknumber(L, 1)); return(0); } // _load_sound(engine_id, sound_slot_number)
int play_note3(lua_State *L) { _play_note(3, luaL_checknumber(L, 1) - 1, luaL_checknumber(L, 2)); return(0);	}  // _play_note(engine_id, note, volume) 
int play_frequency3(lua_State *L) { _play_frequency(3, luaL_checknumber(L, 1), luaL_checknumber(L, 2)); return(0); } // _play_frequency(engine_id, frequency, volume)
int set_pan3(lua_State *L) { _set_pan(3, luaL_checknumber(L, 1)); return(0); } // _set_pan(engine_id, pan)
int set_all_cells3(lua_State *L) {_set_all_cells(3, luaL_checknumber(L, 1)); return(0); } // _set_all_cells(engine_id, color)
int set_cell3(lua_State *L) { _set_cell(3, luaL_checknumber(L, 1), luaL_checknumber(L, 2), luaL_checknumber(L, 3)); return(0); } // _set_cell(engine_id, x, y, color)
int set_column3(lua_State *L) { _set_column(3, luaL_checknumber(L, 1), luaL_checknumber(L, 2)); return(0); } // _set_column(engine_id, column, color)
int set_row3(lua_State *L) { _set_row(3, luaL_checknumber(L, 1), luaL_checknumber(L, 2)); return(0); } // _set_row(engine_id, row, color)
int display_text3(lua_State *L) { _display_text(3,luaL_checklstring(L, 1, &len),luaL_checknumber(L, 2),luaL_checknumber(L, 3)); return(0); }
int midi_note3(lua_State *L) { _midi_note(3, luaL_checknumber(L, 1), luaL_checknumber(L, 2), luaL_checknumber(L, 3)); return(0); } // _midi_note(engine_id, cell, volume, duration)
int clear_top_screen3(lua_State *L) { _clear_top_screen(3); return(0); }

int load_sound4(lua_State *L) { _load_sound(4, luaL_checknumber(L, 1)); return(0); } // _load_sound(engine_id, sound_slot_number)
int play_note4(lua_State *L) { _play_note(4, luaL_checknumber(L, 1) - 1, luaL_checknumber(L, 2)); return(0);	}  // _play_note(engine_id, note, volume) 
int play_frequency4(lua_State *L) { _play_frequency(4, luaL_checknumber(L, 1), luaL_checknumber(L, 2)); return(0); } // _play_note(engine_id, frequency, volume)
int set_pan4(lua_State *L) { _set_pan(4, luaL_checknumber(L, 1)); return(0); } // _set_pan(engine_id, pan)
int set_all_cells4(lua_State *L) {_set_all_cells(4, luaL_checknumber(L, 1)); return(0); } // _set_all_cells(engine_id, color)
int set_cell4(lua_State *L) { _set_cell(4, luaL_checknumber(L, 1), luaL_checknumber(L, 2), luaL_checknumber(L, 3)); return(0); } // _set_cell(engine_id, x, y, color)
int set_column4(lua_State *L) { _set_column(4, luaL_checknumber(L, 1), luaL_checknumber(L, 2)); return(0); } // _set_column(engine_id, column, color)
int set_row4(lua_State *L) { _set_row(4, luaL_checknumber(L, 1), luaL_checknumber(L, 2)); return(0); } // _set_row(engine_id, row, color)
int display_text4(lua_State *L) { _display_text(4,luaL_checklstring(L, 1, &len),luaL_checknumber(L, 2),luaL_checknumber(L, 3)); return(0); }
int midi_note4(lua_State *L) { _midi_note(4, luaL_checknumber(L, 1), luaL_checknumber(L, 2), luaL_checknumber(L, 3)); return(0); } // _midi_note(engine_id, cell, volume, duration)
int clear_top_screen4(lua_State *L) { _clear_top_screen(4); return(0); }

int load_sound5(lua_State *L) { _load_sound(5, luaL_checknumber(L, 1)); return(0); } // _load_sound(engine_id, sound_slot_number)
int play_note5(lua_State *L) { _play_note(5, luaL_checknumber(L, 1) - 1, luaL_checknumber(L, 2)); return(0);	}  // _play_note(engine_id, note, volume) 
int play_frequency5(lua_State *L) { _play_frequency(5, luaL_checknumber(L, 1), luaL_checknumber(L, 2)); return(0); } // _play_frequency(engine_id, frequency, volume)
int set_pan5(lua_State *L) { _set_pan(5, luaL_checknumber(L, 1)); return(0); } // _set_pan(engine_id, pan)
int set_all_cells5(lua_State *L) {_set_all_cells(5, luaL_checknumber(L, 1)); return(0); } // _set_all_cells(engine_id, color)
int set_cell5(lua_State *L) { _set_cell(5, luaL_checknumber(L, 1), luaL_checknumber(L, 2), luaL_checknumber(L, 3)); return(0); } // _set_cell(engine_id, x, y, color)
int set_column5(lua_State *L) { _set_column(5, luaL_checknumber(L, 1), luaL_checknumber(L, 2)); return(0); } // _set_column(engine_id, column, color)
int set_row5(lua_State *L) { _set_row(5, luaL_checknumber(L, 1), luaL_checknumber(L, 2)); return(0); } // _set_row(engine_id, row, color)
int display_text5(lua_State *L) { _display_text(5,luaL_checklstring(L, 1, &len),luaL_checknumber(L, 2),luaL_checknumber(L, 3)); return(0); }
int midi_note5(lua_State *L) { _midi_note(5, luaL_checknumber(L, 1), luaL_checknumber(L, 2), luaL_checknumber(L, 3)); return(0); } // _midi_note(engine_id, cell, volume, duration)
int clear_top_screen5(lua_State *L) { _clear_top_screen(5); return(0); }

