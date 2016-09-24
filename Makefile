default: probe griffin.so

probe: probe.c
	gcc $< -o $@

%.o: %.c
	$(CC) -fPIC -c -o $@ $<

griffin.so: griffin.c main.o
	gcc -fPIC -shared $^ -o $@ -lsporth -lsoundpipe -lpthread 


install: griffin.so
	mkdir -p /usr/local/share/sporth/plugins
	install griffin.so /usr/local/share/sporth/plugins

clean:
	rm -rf probe griffin.so main.o
