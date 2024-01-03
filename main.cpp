#include <SDL.h>
#include <GL/glew.h>

int main(int argc, char** args) {
	// Initialize SDL
	if (SDL_Init(SDL_INIT_TIMER | SDL_INIT_AUDIO | SDL_INIT_VIDEO | SDL_INIT_EVENTS) < 0) {
		SDL_Log("Failed to initialize SDL: %s", SDL_GetError());
		return 1;
	}

	// Initialize OpenGL
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);

	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);

	// Initialize SDL Window
	SDL_Window* window = SDL_CreateWindow("Manic Mouse", 100, 100, 1280, 720, SDL_WINDOW_OPENGL);
	if (!window)
	{
		SDL_Log("Failed to create window: %s", SDL_GetError());
		return false;
	}

	// Initialize OpenGL context
	SDL_GLContext gl_context = SDL_GL_CreateContext(window);

	// Initialize GLEW
	glewExperimental = GL_TRUE;
	GLenum glew_error = glewInit();
	if (glew_error != GLEW_OK)
	{
		SDL_Log("Failed to initialize GLEW: %s", glewGetErrorString(glew_error));
		return false;
	}

	// Run game loop
	bool is_running = true;
	Uint32 tick_count = 0;
	while (is_running) {
		SDL_Event event;

		while (SDL_PollEvent(&event) != 0) {
			switch (event.type) {
			case SDL_QUIT:
				is_running = false;
				break;
			}
		}

		float delta_time_seconds = (SDL_GetTicks() - tick_count) / 1000.0f;
		if (delta_time_seconds > 0.05f) {
			delta_time_seconds = 0.05f;
		}

		tick_count = SDL_GetTicks();
	}

	// Shutdown SDL
	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
}
