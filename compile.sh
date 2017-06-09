rm -rf build
mkdir build
cd build
cmake -G"Visual Studio 14 2015 Win64" -L ..
cd ..
cp cars.obj build/cars.obj
cp PixelShader.hlsl build/PixelShader.hlsl
cp VertexShader.hlsl build/VertexShader.hlsl