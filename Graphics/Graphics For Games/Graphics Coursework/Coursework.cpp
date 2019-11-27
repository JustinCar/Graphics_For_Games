#pragma comment(lib, "nclgl.lib")

#include "../../nclgl/window.h"
#include "Renderer.h"

int main() {
	Window w("Advanced Graphics for Games Coursework!!", 800, 600, false);
	if (!w.HasInitialised()) {
		return -1;
	}

	Renderer renderer(w);
	if (!renderer.HasInitialised()) {
		return -1;
	}

	w.LockMouseToWindow(true);
	w.ShowOSPointer(false);

	while (w.UpdateWindow() && !Window::GetKeyboard()->KeyDown(KEYBOARD_ESCAPE)) {

	
		if (Window::GetKeyboard()->KeyDown(KEYBOARD_R)) {
			renderer.reloadShaders();
		}
		float time = w.GetTimer()->GetTimedMS();

		cout << "FPS: "<< 1 / (time / 1000) << std::endl;


		renderer.UpdateScene(time);
		renderer.RenderScene(w.GetTimer()->GetMS());
	}

	return 0;
}