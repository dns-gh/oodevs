@echo off
set NODE=%~dp0../bin/nodejs
set COFFEE=%NODE%\node.exe "%NODE%\node_modules\coffee-script\bin\coffee"
pushd "%~dp0..\node\www\js"
start /B %COFFEE% -wclj login.js        utils.coffee login.coffee
start /B %COFFEE% -wclj nodes.js        utils.coffee nodes.coffee
start /B %COFFEE% -wclj packages.js     utils.coffee packages.coffee
start /B %COFFEE% -wclj sessions.js     utils.coffee sessions.coffee
start /B %COFFEE% -wclj update_login.js utils.coffee update_login.coffee
start /B %COFFEE% -wclj uploads.js      utils.coffee uploads.coffee
start /B %COFFEE% -wclj users.js        utils.coffee users.coffee
start /B %COFFEE% -wclj utils.js        utils.coffee
