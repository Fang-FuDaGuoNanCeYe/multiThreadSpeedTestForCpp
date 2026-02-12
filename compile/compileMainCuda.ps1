cd ..
if ($?) { nvcc mainCuda.cu -o mainCuda } ; if ($?) { .\mainCuda }
echo "Eng: Compile and run completed, press Enter to exit"
echo "Chn: 编译运行完成，按回车退出"
Pause