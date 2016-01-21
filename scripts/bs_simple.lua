function on_load()
	redraw()
end

function redraw()
	set_all_cells(WHITE)
	set_column(BLOCK,LIGHT_GRAY)	
end

function stylus_newpress()
	if X < 9 then
		set_all_cells(WHITE)
		set_column(X,LIGHT_GRAY)
		set_block(X)
	end
end

function stylus_drag()
	if X < 9 then 
 		set_all_cells(WHITE)
 		set_column(X,LIGHT_GRAY)
 		set_block(X)
	end
end