default: probe griffin.so
probe: probe.c
	gcc $< -o $@
griffin.so: griffin.c
	gcc -fPIC -shared $< -o $@ -lsporth -lsoundpipe -lpthread 
clean:
	rm -rf probe griffin.so
