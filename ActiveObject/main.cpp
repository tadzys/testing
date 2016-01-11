#include <iostream>
#include <string>

#include "Active.hpp"


class Backgrounder {
public:
	void Save(std::string filename) {
		a.Send([] {
			std::cout << "Saving some data" << std::endl;
		});
	}

	void Print(std::string data) {
		a.Send([] {
			std::cout << "Printing some data: " << std::endl;
		});
	}

private:
	int somePrivateStateAcrossCalls;
	Active a;
};

int main(int argc, char* argv)
{
	Backgrounder bla;
	bla.Save(std::string("Jhony WTF!!!"));

}