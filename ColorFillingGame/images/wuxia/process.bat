@echo off
setlocal enabledelayedexpansion

set count=1
for %%f in (*.png) do (
    set "newname=!count!.png"
    ren "%%f" "!newname!"
    set /a count+=1
)

echo Renaming completed.
endlocal
pause