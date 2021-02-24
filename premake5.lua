workspace "SympleNet"
	architecture "x86"

	configurations
	{
		"Debug",
		"Release"
	}
	
	flags "MultiProcessorCompile"
	
outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

include "SympleNet"
include "Client"
include "Server"