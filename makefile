CFLAGS=-Wall -O2 -Wno-unused
INCLUDES=-I include
OBJ_DIR=obj
BIN_DIR=bin
SRC_DIR=src
LIB=libDivaS.a

# -----------------------------------------------
# Detect available system libraries
# -----------------------------------------------
LIBGPP   := $(shell echo "int main(int argc,char** argv) {return 0;}" > obj/tmp.c 2>/dev/null && gcc obj/tmp.c -o obj/tmp -lg++     > /dev/null 2>&1 && echo -lg++)
LIBSTDCPP:= $(shell echo "int main(int argc, char** argv) {return 0;}" > obj/tmp.c 2>/dev/null && gcc obj/tmp.c -o obj/tmp -lstdc++ > /dev/null 2>&1 && echo -lstdc++)
$(shell rm obj/tmp.c obj/tmp)

# -----------------------------------------------
# Detect location of DivaS library
# -----------------------------------------------
LIBDIVA=$(shell test -f ../$(LIB) && echo ../$(LIB))
ifeq (,${LIBDIVA})
LIBDIVA=$(shell test -f ../../lib/$(LIB) && echo ../../lib/$(LIB))
endif
ifeq (,${LIBDIVA})
LIBDIVA=-lDivaS
endif

# -----------------------------------------------
#  Libraries to be used
# -----------------------------------------------
LIBS=$(LIBDIVA) -lpthread $(LIBGPP) $(LIBSTDCPP)

# -----------------------------------------------
#  Extra includes to be used
# -----------------------------------------------
EXTRA_INCLUDES=$(shell test -f /usr/include/string.h && echo -include /usr/include/string.h)
EXTRA_INCLUDES+=$(shell test -f /usr/include/strings.h && echo -include /usr/include/strings.h)
EXTRA_INCLUDES+=$(shell test -f /usr/include/memory.h && echo -include /usr/include/memory.h)

# -------------------------------------------------------------------------

TARGET=$(BIN_DIR)/bluebox
SRC=$(SRC_DIR)/main.c

# -------------------------------------------------------------------------

$(TARGET): $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRC))
	$(CC) $(CFLAGS) $(INCLUDES) $^ $(LIBS) -o $(TARGET)

$(OBJ_DIR)/%.o:$(SRC_DIR)/%.c
	$(CC) $(EXTRA_INCLUDES) -c $(CFLAGS) $(INCLUDES) $< -o $@

$(OBJ_DIR)/%.o:$(SRC_DIR)/%.cpp
	$(CC) $(EXTRA_INCLUDES) -c $(CFLAGS) $(INCLUDES) $< -o $@

# -------------------------------------------------------------------------

clean:
	@rm -f $(OBJ_DIR)/*.o $(TARGET) $(OBJ_DIR)/.depend $(OBJ_DIR)/tmp.o $(OBJ_DIR)/tmp

# -------------------------------------------------------------------------

depend:
	@$(CC) $(INCLUDES) $(CFLAGS) -M $(SRC) | \
          sed -e "s/^.*:/$(OBJ_DIR)\/&/" - > $(OBJ_DIR)/.depend

-include $(OBJ_DIR)/.depend
