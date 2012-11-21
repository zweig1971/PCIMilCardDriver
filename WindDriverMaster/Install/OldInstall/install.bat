@echo off


call wdreg remove
copy a:\windrvr.sys e:\winnt\system32\drivers\
copy a:\PCIMilTreiber.dll e:\winnt\system32\
call wdreg install