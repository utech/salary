@echo off
echo   --- qmake for SalaryNetwork ---
cd src
qmake salaryNetwork.pro
cd ..

echo   --- qmake for ConfigSalary ---
cd src/configSalary
qmake
cd ../..

echo   --- qmake for tke parts done Ok ---