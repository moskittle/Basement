#pragma once

#ifdef BM_PLATFORM_WINDOWS

extern Basement::Application* Basement::CreateApplication();

int main(int argc, char** argv)
{
	Basement::Log::Init();
	BM_CORE_WARN("Successfully initialized Log");
	BM_CORE_INFO("Welcome to Basement Engine!");

	auto gpBackyardApp = Basement::CreateApplication();
	
	gpBackyardApp->Run();
	
	delete gpBackyardApp;
}

#endif // BM_PLATFORM_WINDOWS