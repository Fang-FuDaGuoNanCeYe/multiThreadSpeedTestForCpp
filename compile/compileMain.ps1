cd ..
echo "Eng: It is suggested that the compile code should be checked, especially when you aren't using exactly tbb12"
echo "Chn: 我建议您检查下这里的编译语句，毕竟不是每台电脑都在用tbb 12。"
if ($?) { g++ -std=c++23 main.cpp -o main -ltbb12 -D__WXMSW__ -D_WINDOWS -D_UNICODE -DUNICODE -mthreads -lstdc++exp } ; if ($?) { .\main}
echo "Eng: Compile and run completed, press Enter to exit"
echo "Chn: 编译运行完成，按回车退出"
Pause