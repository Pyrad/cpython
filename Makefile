CC	:=	gcc
INC	:=	-I/home/pyrad/procs/python2.7.14/include/python2.7
LIB	:=	-L/home/pyrad/procs/python2.7.14/lib

SOFLAGS	:=	-fPIC -shared

TARGET	:=	spam
SRC1	:=	spammodule.c
OUT1	:=	spam.so

PYUSER	:=	pyspamuser
SRC2	:=	pyspamuser.c
OUT2	:=	pyspamuser.so

$(TARGET)	:	$(SRC1)
	$(CC) $(SOFLAGS) $(INC) $(LIB) $(SRC1) -o $(OUT1)

$(PYUSER)	:	$(SRC2)
	$(CC) $(SOFLAGS) $(INC) $(LIB) $(SRC2) -o $(OUT2)

all	:
	$(CC) $(SOFLAGS) $(INC) $(LIB) $(SRC1) -o $(OUT1)
	$(CC) $(SOFLAGS) $(INC) $(LIB) $(SRC2) -o $(OUT2)


clean	:
	rm -rf $(OUT1)
	rm -rf $(OUT2)
