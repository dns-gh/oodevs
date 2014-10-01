@echo off
sc failure "sword cloud" reset= 1000000 actions= "" actions= "" actions= ""
taskkill /f /im cloud_server.exe
sc failure "sword cloud" reset= 1000000 actions= restart/10000 actions= restart/10000 actions= restart/10000
net start "sword cloud"
