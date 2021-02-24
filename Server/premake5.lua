project "Server"
	kind "ConsoleApp"
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
		"src",
		
		"%{wks.location}/SympleNet/include",
		"%{wks.location}/SympleNet/vendor/asio-1.18.1/include",
	}
	
	files
	{
		"src/**.h",
		"src/**.hpp",
		
		"src/**.c",
		"src/**.cpp",
	}
	
	filter "configurations:Debug"
		defines "SY_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "SY_RELEASE"
		runtime "Release"
		optimize "on"