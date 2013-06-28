@echo off
pushd %~dp0
set ROOT=%~dp0..
echo --node %ROOT%/node/bin/node.exe --proxy %ROOT%/proxy/bin/proxy.exe --node_root %ROOT%/node/www --simulation %ROOT%/out/vc100_x64/sword/simulation_app.exe --replayer %ROOT%/out/vc100_x64/sword/replayer_app.exe --timeline %ROOT%/run/vc100_x64/timeline_server.exe  --user "admin@masagroup.net" --display "Admin" --password "admin"
pause
