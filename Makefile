interp: main
	pyinstaller --onedir --add-data "lang/*.lang;lang" interpreter/main.py

test:
	python -m unittest

clean:
	rm -rf interpreter/build
	rm -rf interpreter/dist
	rm interpreter/interp.spec