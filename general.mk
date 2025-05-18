include config.mk

generate_lib: libcolors.a $(TARGET).a
	ar -t $(TARGET).a

all: generate_lib examples
	@echo

examples:
	gcc $(CFLAGS) $(INCLUDE_FLAGS) $(PATH_EXAMPLES)/hash.c $(LINKER_FLAGS) -o example.$(EXTENSION)

libcolors.a:
	echo "generando librerias estaticas..."
	$(MAKE) -C ./$(PATH_COLORS) -f $(MAKE_NAME)

$(TARGET).a: $(OBJECTS)

	$(ARR) $(ARR_FLAGS) $@ $^
	ranlib $@

debug_c.o: $(PATH_SRC)/debug_c.c
	$(CC) $(CFLAGS) -c $^ -o $@

error_c.o: $(PATH_SRC)/error_c.c
	$(CC) $(CFLAGS) -c $^ -o $@

time_code.o: $(PATH_SRC)/time_code.c
	$(CC) $(CFLAGS) -c $^ -o $@

cleanobj:
	$(RM) $(RMFLAGS) $(OBJECTS) $(OBJECTS_DEBUG)
	$(MAKE) -C ./$(PATH_COLORS) -f $(MAKE_NAME) cleanobj

cleanall: cleanobj
	$(RM) $(RMFLAGS) $(TARGET).o $(TARGET).a \
	$(TARGET_DEBUG).a
	$(MAKE) -C ./$(PATH_COLORS) -f $(MAKE_NAME) cleanall

.SILENT: clean cleanobj cleanall
.IGNORE: cleanobj cleanall
.PHONY:  cleanobj cleanall