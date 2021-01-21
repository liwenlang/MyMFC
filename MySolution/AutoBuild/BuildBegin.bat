if not exist "%2". mkdir "%2"
xcopy "%1"    "%2" /e /y /d

xcopy "..\..\..\PMBim-Project(Base)\export\PmBase"  "..\..\export\PmBase"       /e /y /d