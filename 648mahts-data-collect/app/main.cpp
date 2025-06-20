#include "mainService.h"
#include <memory>

std::shared_ptr<mainService> g_service = std::make_shared<mainService>();


int main(int argc,char *argv[])
{
	g_service->init();

	while(1)
		sleep(2);
}
