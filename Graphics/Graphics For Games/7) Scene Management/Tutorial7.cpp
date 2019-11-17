#include "../../nclgl/window.h"
#include "Renderer.h"

#pragma comment(lib, "nclgl.lib")

int main() {	
	Window w("Scene Management!", 800,600,false);
	if(!w.HasInitialised()) {
		return -1;
	}

	Renderer renderer(w);
	if(!renderer.HasInitialised()) {
		return -1;
	}

	w.LockMouseToWindow(true);
	w.ShowOSPointer(false);

	while(w.UpdateWindow() && !Window::GetKeyboard()->KeyDown(KEYBOARD_ESCAPE)){
		float t = w.GetTimer()->GetTimedMS();
		renderer.UpdateScene(t);
		renderer.RenderScene(t);
	}

	return 0;
}