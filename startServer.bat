SET ROOT=C:\pslroot
SET CHIME=%ROOT%\psl\chime
SET FRAX=%CHIME%\frax
SET JARS=%ROOT%\jars\siena-1.1.2.jar;%ROOT%\jars\HTTPClient.jar
SET AUTH=%CHIME%\auth
SET SIENAUTILS=%CHIME%\sienautils
SET DATASERVER=%CHIME%\DataServer
SET SHEN=%ROOT%\jars\hsql.jar;%ROOT%\jars\jdom.jar;%ROOT%\jars\xerces.jar;%ROOT%\jars\collections.jar
SET CLASSPATH=%ROOT%;%JARS%;%FRAX%;%AUTH%;%SIENAUTILS%;%SHEN%;%DATASERVER%;.

start "Siena Server" /min java siena.StartServer -port 1234
sleep 2
start "Siena Subscriber" java psl.chime.sienautils.Subscriber senp://localhost:1234