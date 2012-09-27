@echo off
set PATH=c:\cygwin\bin;%PATH%
cat jquery-1.8.2.js ^
    bootstrap-2.1.1.js ^
    underscore-1.3.3.js ^
    backbone-0.9.2.js ^
    handlebars-1.0.0.beta.6.js ^
    spin.js ^
    jquery.cookie.js ^
    json2.js ^
  | uglifyjs -nc -o ../js/externals.js
