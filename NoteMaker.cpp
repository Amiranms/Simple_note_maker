#include "Application.h"

int main()
{
	Application::app().open();
	Application::app().run();
	Application::app().close();

	return 0;
}