#include "fileSystem.h"
#include<iostream>

using namespace std;

void main()
{
	cout << "UnixFileSystem beta1.0" << endl;
	cout << "made by PGL.All rights reserved." << endl;
	cout << "welcome to command line" << endl;
	cout << endl;

	
	loadSuper("vm.dat");
	root = iget(0);
	while (!login())
	{
		NULL;
	}
	current = root;
	while (!logout)
	{
		dispatcher();
	}
	
}

