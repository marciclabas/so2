run: dev
	@./dev
	@rm dev
	
dev: dev.c
	@gcc -o dev dev.c

entrega:
	cd zeos && make clean && cd ..
	tar zcvf zeos.tar.gz --exclude='./*' zeos README.md

test:
	mkdir test
	cp zeos.tar.gz test
	cd test && tar zxvf zeos.tar.gz
	cd test/zeos && make emul

clean:
	rm -dr test