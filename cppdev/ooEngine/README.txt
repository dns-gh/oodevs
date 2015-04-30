# Usage
1] add a folder next to the ooEngine.sln file named externallibs
2] The externallibs folder should contains the SDL2 lib under the folder named SDL2-2.0.3
(see https://www.libsdl.org/download-2.0.php and choose a *devel-2.0.3-VC* lib)
The path to the lib should look like that : dev\ooEngine\externallibs\SDL2-2.0.3
Then add the SDL2.dll to the folder containing the exe for execution or the PATH to the dll into the environement project setting
using : PATH=%PATH%;$(ProjectDir)\..\externallibs\SDL2-2.0.3\lib\x64;$(ProjectDir)\..\externallibs\SDL2_image-2.0.0\lib\x64
3] The folder should also contain the SDL2 Image lib : https://www.libsdl.org/projects/SDL_image/
Pick  SDL2_image-devel-2.0.0-VC and follow the same steps as for the SDL2 lib

# Info
Early dev is actually available under x64 platform only (Debug or Release mode)