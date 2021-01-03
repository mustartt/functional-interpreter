# pyinstaller --onedir --add-data "lang/*.lang;lang" main.py 

.PHONY: clean clean-win test

clean: 
	rm -r ./interpreter/build/
	rm -r ./interpreter/dist/
	rm ./interpreter/main.spec

clean-win:
	RMDIR /Q/S "interpreter/dist"
	RMDIR /Q/S "interpreter/build"
	del "interpreter/main.spec"

run:
	python interpreter/main.py

build-win:
	cd interpreter && pyinstaller --onedir --add-data "lang/*.lang;lang" main.py 

build:
	cd interpreter && pyinstaller --onedir --add-data "lang/*.lang:lang" main.py 

test: 
	python -m unittest

