call terser .\src\js\scripts.js > .\dist\assets\js\scripts.js
call lessc .\src\styles\combined.less | call  minify --css > .\dist\assets\styles.css
call minify index.html > .\dist\index.html
xcopy .\src\js\scripts.js .\dist\assets\js\scripts.js /Y