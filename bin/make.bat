SolutionGenerator.exe -bp -make

copy /Y "..\lib\SDL3\lib\x64\*.dll" "..\ide\Core\"
copy /Y "..\lib\SDL3\lib\x64\*.dll" "..\ide\Engine\"
copy /Y "..\lib\SDL3\lib\x64\*.dll" "..\ide\Render\"
copy /Y "..\lib\SDL3\lib\x64\*.dll" "..\ide\Utils\"
copy /Y "..\lib\SDL3\lib\x64\*.dll" "..\ide\Demo\"

PAUSE