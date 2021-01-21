xcopy "%1"  "%1_Success"       /e /y /d

rem ¿½±´osg
xcopy	"..\..\export\PmBase\cad\DLL\sys21_x64\*.*"	 "..\..\export\PmBase\cad\dll\osgview\x64"		/y	/d	/f
xcopy	"..\..\export\PmBase\cad\DLL\sys21\*.*"	 "..\..\export\PmBase\cad\dll\osgview\x86"		/y	/d	/f

xcopy "..\3.ProjectData\SysData" 			"\\172.16.11.96\OutPut\Last" /e /y /d
xcopy "..\..\export\PmBase"  					"..\..\export\PmBase_Success"       /e /y /d
xcopy "..\..\export\PmBase\cad\dll" 			"\\172.16.11.96\OutPut\Last" /e /y /d
xcopy "..\..\..\PMBim-Project(Kernel)\KernelBase_Project\3.ProjectData" 			"\\172.16.11.96\OutPut\Last" /e /y /d

rem xcopy "\\172.16.11.28\OutPut\Last_CDBZ\Hoops" 			"\\172.16.11.96\OutPut\Last\Hoops" /e /y /d


rem ¿½±´ÒýÇædll
xcopy "\\172.16.11.28\OutPut\Last_CDBZ\osgview" 			"\\172.16.11.96\OutPut\Last\osgview" /e /y /d

rem ¿½±´ÒýÇædll
set EnginePath=E:\Engine
xcopy	"%EnginePath%\6.Release\dll\x64\*.*"	 "\\172.16.11.96\OutPut\Last\osgview\x64"		/e /y /d
xcopy	"%EnginePath%\6.Release\dll\x86\*.*"	 "\\172.16.11.96\OutPut\Last\osgview\x86"		/e /y /d
xcopy	"%EnginePath%\4.ThirdParty\osg\dll\vc14_x64\*.*"	 "\\172.16.11.96\OutPut\Last\osgview\x64"		/e /y /d
xcopy	"%EnginePath%\4.ThirdParty\osg\dll\vc14_x86\*.*"	 "\\172.16.11.96\OutPut\Last\osgview\x86"		/e /y /d
xcopy	"%EnginePath%\3.ProjectData\resource\*.*"	 "\\172.16.11.96\OutPut\Last\osgview\resource"		/e /y /d
rem xcopy	"%EnginePath%\3.ProjectData\resource\*.*"	 "\\172.16.11.96\OutPut\Last\osgview\x86\resource"		/e /y /d
