#include <iostream>

#include <Vroom/Core/Application.h>
#include <Vroom/Core/GameLayer.h>

#include "UILayer.h"
#include "HeightFieldScene.h"

int main(int argc, char** argv)
{
    vrm::Application app{argc, argv};
	
	app.pushLayer<UILayer>();

	app.getGameLayer().loadScene<HeightFieldScene>();

	app.run();

	return 0;
}