all: hmpweb.wasm

FUNS=["_playerweb_init", "_playerweb_gen", "_playerweb_get_last_rc", "_playerweb_get_data", "_playerweb_play", "_playerweb_set_freq"]

hmpweb.wasm: playerweb.c
	emcc -Os -g --no-entry $^ -o $@ -s EXPORTED_FUNCTIONS='$(FUNS)'

clean:
	rm -f *.wasm
 