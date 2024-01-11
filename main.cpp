#include "Project/Game/GameManager.h"

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_  HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd) {
	GameManager* gameManager = new GameManager();
	gameManager->run();
	delete gameManager;
	return 0;
}