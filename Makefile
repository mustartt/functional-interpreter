interp: interp.py
	pyinstaller --onefile interpreter/interp.py
	mv interpreter/build/interp.exe ./

clean:
	rm -rf interpreter/build
	rm -rf interpreter/dist
	rm interpreter/interp.spec