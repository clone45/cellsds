class Engine
{
   public:

	// Variables
	int engine_id;
	int old_grid_x;
	int old_grid_y;

	Voice voice[16];

	// map<string, int> globals;
	
	lua_State *L;
	bool active;
	string loaded_script;
	string basename;
	
	// Functions
	void load_lua_script(string filename);
	void run(void);
	void display_lua_error(string additional_info);
	void clock();
   void redraw(void);	
	void set_block_en(int block);
	void save(string filename);
	void load(string filename);
	void set_basename(string basename);
   void activate(void);
   void deactivate(void);
	void play_note(int note, int volume);
	void play_frequency(int frequency, int volume);

	void _call_lua(string function);
	void _set_lua_global(const char* variable_name, int value);
	
	void display_stack_length(void);
	
	Engine(int new_x);
	~Engine(void);
};

