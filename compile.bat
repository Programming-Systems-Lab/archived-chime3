@echo off
SET ROOT=C:\pslroot
SET CHIME=%ROOT%\psl\chime
SET FRAX=%CHIME%\frax
SET JARS=%ROOT%\jars\siena-1.1.2.jar;%ROOT%\jars\HTTPClient.jar
SET AUTH=%CHIME%\auth
SET SIENAUTILS=%CHIME%\sienautils
SET DATASERVER=%CHIME%\DataServer
SET SHEN=%ROOT%\jars\hsql.jar;%ROOT%\jars\jdom.jar;%ROOT%\jars\xerces.jar;%ROOT%\jars\collections.jar
SET CLASSPATH=%ROOT%;%JARS%;%FRAX%;%AUTH%;%SIENAUTILS%;%SHEN%;%DATASERVER%;.

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
cd xescii

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

cd %THEME%

echo Deleting Old Class files
del *.class

echo Recompiling package
javac *.java

cd %ROOT%
echo Done..........