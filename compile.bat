@echo off
call setenv-vars.bat

echo !!!!!!!!!!!!!!!!!!!!SIENA UTILS RECOMPILER!!!!!!!!!!!!!!!!!

cd %SIENAUTILS%

echo Deleting Old Class files
del *.class

echo Recompiling Package
javac *.java

echo !!!!!!!!!!!!!!!!!!!!!!!FRAX RECOMPILER!!!!!!!!!!!!!!!!!!!!!!!!!
cd %FRAX

echo Deleting Old Class files
del *.class

del %FRAX%\plugs\*.class
del %FRAX%\plugs\bscw\*.class
del %FRAX%\plugs\dir\*.class
del %FRAX%\plugs\gen\*.class
del %FRAX%\plugs\html\*.class
del %FRAX%\plugs\img\*.class
del %FRAX%\plugs\xml\*.class

del %FRAX%\protocols\bscw\*.class
del %FRAX%\protocols\http\*.class
del %FRAX%\protocols\lfs\*.class
del %FRAX%\protocols\*.class

del %FRAX%\gui\*.class
del %FRAX%\fraxExceptions\*.class

echo Recompiling Package
javac *.java
javac %FRAX%\plugs\*.java
javac %FRAX%\plugs\bscw\*.java
javac %FRAX%\plugs\dir\*.java
javac %FRAX%\plugs\gen\*.java
javac %FRAX%\plugs\html\*.java
javac %FRAX%\plugs\img\*.java
javac %FRAX%\plugs\xml\*.java

javac %FRAX%\protocols\bscw\*.java
javac %FRAX%\protocols\http\*.java
javac %FRAX%\protocols\lfs\*.java
javac %FRAX%\protocols\*.java

javac %FRAX%\gui\*.java
javac %FRAX%\fraxExceptions\*.java

echo !!!!!!!!!!!!!!!!!!!!!!!!!AUTHENTICATOR RECOMPILING!!!!!!!!!!!!!!

cd %AUTH%
echo Deleting Old Class files
del *.class

echo Recompiling package
javac *.java

echo !!!!!!!!!!!!!!!!!!!!!!!!!DATASERVER RECOMPILING!!!!!!!!!!!!!!!!!!

cd %DATASERVER%

echo Deleting Old Class files
del *.class

echo Recompiling package
javac *.java

echo !!!!!!!!!!!!!!!!!!!!!!!!!VEM RECOMPILING!!!!!!!!!!!!!!!!!!!!!!!!!!

cd %VEM%

echo Deleting Old Class files
del *.class

echo Recompiling package
javac *.java

echo !!!!!!!!!!!!!!!!!!!!!!!!THEME MANAGER RECOMPILING!!!!!!!!!!!!!!!!!

cd %EVENT_TRACER%

echo Deleting Old Class files
del *.class

echo Recompiling package
javac *.java

cd %CHIME%
echo Done..........