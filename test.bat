call setenv-vars.bat

cd %SIENAUTILS%
java Tester senp://localhost:1234 %1 %2
cd %CHIME%