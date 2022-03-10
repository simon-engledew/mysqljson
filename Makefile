dist/mysqljson.linux.tar.gz: dist/mysqljson
	tar cvzf $@ $<

dist/libmysqljson.linux.tar.gz: dist/libmysqljson.so
	tar cvzf $@ $<

.docker/libmysqljson.iid: $(wildcard src/sql/*)
	docker build --target build --build-arg TARGET=library --iidfile $@ .

.docker/mysqljson.iid: $(wildcard src/sql/*)
	docker build --iidfile $@ .

dist/mysqljson: .docker/mysqljson.iid
	docker run --entrypoint cat --rm "$$(cat $<)" /usr/bin/mysqljson > $@

dist/libmysqljson.so: .docker/libmysqljson.iid
	docker run --entrypoint cat --rm "$$(cat $<)" sql/libmysqljson.so > $@
