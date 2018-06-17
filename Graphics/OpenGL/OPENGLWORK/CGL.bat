set Source=%1
set Aim=%2
gcc %Source% -lopengl32 -lglu32 -lglut32 -o %Aim%