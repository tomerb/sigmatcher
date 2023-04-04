CLEANEXTS   = o a
OUTPUTFILE  = libsigmatcher.a

.PHONY: all
all: $(OUTPUTFILE)

$(OUTPUTFILE): crc_matcher.o bloom_filter_matcher.o
	ar ru $@ $^
	ranlib $@

.PHONY: clean
clean:
	for file in $(CLEANEXTS); do rm -f *.$$file; done

crc_matcher.o: crc_matcher.h
bloom_filter_matcher.o: bloom_filter_matcher.h
