# Welcome to BIGOS
Advanced game engine and rendering technology almost as good as Polish traditional food. Currently supports DirectX11 (Vulkan and DirectX12 support is in development).

## Demos
<p>Phong lightning demo</p>
<p align="center">
  <img src="https://github.com/MBahyrycz/BIGOS/blob/main/Resources/Demos/phongDemo.png?raw=true" />
</p>

<p>PBR rendering demo</p>
<p align="center">
  <img src="https://github.com/MBahyrycz/BIGOS/blob/main/Resources/Demos/pbrDemo.png?raw=true" />
</p>

## Build
Use Win-GenProjects.bat in "scripts" folder to genetate default Visual Studio 2019 solution

If you are using older version of MS Visual Studio, you need to change Win-GenProjects.bat script:

```lua  
pushd ..\
call ThirdParty\premake\bin\premake5.exe vs2019
PAUSE
```
Possible builds vs2005-2019

## Getting Started
1. Go to Sandbox\src\SandboxApp.cpp
2. Uncomment one of following lines to see demo you are intrested in
```lua  
//PushLayer(new PhongDemoLayer());
//PushLayer(new PBRDemoLayer());
```
3. Enjoy the view

## See our progress
* [Road Map](https://trello.com/b/gB7Valt5/bigos-goals)
