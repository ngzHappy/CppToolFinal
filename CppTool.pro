TEMPLATE = subdirs


#.subdir
#Use the specified subdirectory instead of SUBDIRS value.
#.file
#Specify the subproject pro file explicitly. Cannot be used in conjunction with .subdir modifier.
#.depends
#This subproject depends on specified subproject.
#.makefile
#The makefile of subproject. Available only on platforms that use makefiles.
#.target
#Base string used for makefile targets related to this subproject. Available only on platforms that use makefiles.

appEmpertyTest.file=$$PWD/empertyTest/empertyTest.pro

appSetGetUtility.file=$$PWD/setgetutility/setgetutility.pro
appSetGetUtility.depends+=appEmpertyTest

appClassMaker.file=$$PWD/classmaker/classmaker.pro
appClassMaker.depends+=appEmpertyTest

appMemoryTest.file=$$PWD/memoryTest/memory_test.pro
appMemoryTest.depends+=appEmpertyTest

SUBDIRS += \
    appEmpertyTest \
    appSetGetUtility \
    appClassMaker \
    appMemoryTest





