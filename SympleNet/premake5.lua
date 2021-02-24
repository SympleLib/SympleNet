project "SympleNet"
	kind "StaticLib"
	language "C++"
	cppdialect "C++Latest"
	
	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")
	
	defines
	{
		"NOMINMAX",
	}
	
	includedirs
	{
		"include",
		
		"vendor/asio-1.18.1/include",
	}
	
	files
	{
		"include/**.h",
		"include/**.hpp",
	}
	
	filter "configurations:Debug"
		defines "SY_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "SY_RELEASE"
		runtime "Release"
		optimize "on"