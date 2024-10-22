#include "AxonEngine.h"
#include <win/wintransport.h>

using namespace std;

int main()
{
	cout << "Hello CMake." << endl;

	Axon::Windows::WinUDPSocket("localhost", 4444);

	return 0;
}
