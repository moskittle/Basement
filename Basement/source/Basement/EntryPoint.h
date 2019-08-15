#pragma once

#ifdef BM_PLATFORM_WINDOWS

extern Basement::Application* Basement::CreateApplication();

int main(int argc, char** argv)
{
	Basement::Log::Init();

	auto gpBackyardApp = Basement::CreateApplication();
	gpBackyardApp->Run();
	delete gpBackyardApp;

	return 0;
}

#endif // BM_PLATFORM_WINDOWS