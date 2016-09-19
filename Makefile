default: probe griffin.so

probe: probe.c
	gcc $< -o $@

%.o: %.c
	$(CC) -fPIC -c -o $@ $<

griffin.so: griffin.c main.o
	gcc -fPIC -shared $^ -o $@ -lsporth -lsoundpipe -lpthread 
clean:
	rm -rf probe griffin.so main.o
