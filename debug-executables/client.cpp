/*
* Just a test executable
* I'll use it in dockertest process
* 
* Falls in infinite loop and sends UDP packages every second
* Can be stopped by keyboard interrupt (Ctrl+C)
* 
* By: KbrdDestroyer
*/

#include <AxonEngine.h>

using namespace Axon;

int main()
{
	Axon::Client((char*) "localhost", 10243);
	return 0;
}
