#include <Basement.h>

class Backyard : public Basement::Application
{
public:
	Backyard()
	{

	}

	~Backyard()
	{

	}
};

Basement::Application* Basement::CreateApplication()
{
	return new Backyard();
}
