call setenv-vars.bat

del ET.*
del DB.*
start "Siena Server" /min java siena.StartServer -port 1234
sleep 2
java psl.chime.sienautils.Subscriber senp://localhost:1234