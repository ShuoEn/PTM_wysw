del /s /q project\obj\*.*
rmdir project\obj
del /s /q project\lst\*.*
rmdir project\lst
del /s /q project\FLASH_RUN\*.*
rmdir /s /q project\FLASH_RUN

del /s /q "project\project_Target 1.Bak"
del /s /q "project\project_Target 1.dep"
del /s /q "project\project.uvgui_*.bak"

del /s /q RemoteSystemsTempFiles
rmdir /s /q RemoteSystemsTempFiles

del .metadata\.log
del .metadata\.lock
del /s /q .metadata\.plugins\org.eclipse.cdt.core
rmdir /s /q .metadata\.plugins\org.eclipse.core.resources\.history\
del /s /q .metadata\.plugins\org.eclipse.epp.usagedata.recording\*.*
del /s /q .metadata\.plugins\org.eclipse.help.ui\scope_sets\*.hist
rmdir /s /q .metadata\.plugins\org.eclipse.ltk.core.refactoring\.refactorings\.workspace\
rmdir /s /q .metadata\.plugins\org.eclipse.ltk.core.refactoring\.refactorings\project\
del /s /q .metadata\.plugins\org.eclipse.cdt.make.core\*.*
del /s /q .metadata\.plugins\org.eclipse.cdt.ui\*.*
del /s /q .metadata\.plugins\org.eclipse.search\*.*
del /s /q .metadata\.plugins\org.eclipse.core.resources\.projects\project\.indexes\*.*
rmdir /s /q .metadata\.plugins\org.eclipse.core.resources\.projects\project\.indexes
del /s /q .metadata\.plugins\org.eclipse.cdt.core\*.pdom

del /s /q .metadata\.plugins\org.eclipse.linuxtools.cdt.libhover\*.* 
rmdir /s /q .metadata\.plugins\org.eclipse.linuxtools.cdt.libhover 


del /s /q user_app_demo\FLASH_RUN\*.*
rmdir /s /q user_app_demo\FLASH_RUN

rem pause