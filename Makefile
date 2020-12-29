interp: interp.py
	pyinstaller --onefile interpreter/interp.py
	mv interpreter/build/interp.exe ./

test:
	python -m unittest

clean:
	rm -rf interpreter/build
	rm -rf interpreter/dist
	rm interpreter/interp.spec