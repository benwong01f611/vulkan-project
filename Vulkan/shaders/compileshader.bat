@echo off
%VULKAN_BIN%\glslc.exe shader.vert -o vert.spv
%VULKAN_BIN%\glslc.exe shader.frag -o frag.spv
echo Done.
pause