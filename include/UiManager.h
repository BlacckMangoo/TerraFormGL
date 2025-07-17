class UiManager {

public:
	UiManager() = default;
	~UiManager() = default;
	// Initialize the UI manager
	void init();
	// Update the UI manager
	void update();
	// Render the UI
	void render();
	// Handle input events
	void handleInput(int eventType, int eventData);
};