#pragma once

#ifdef BM_PLATFORM_WINDOWS

extern Basement::Application* Basement::CreateApplication();

int main(int argc, char** argv)
{
	Basement::Log::Init();
	BM_CORE_WARN("Successfully initialized Log");

	auto pApp = Basement::CreateApplication();
	
	pApp->Run();
	
	delete pApp;
}

#endif // BM_PLATFORM_WINDOWS