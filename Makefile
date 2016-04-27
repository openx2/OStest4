hdrs = myipc.h
opts = -g -c
c_src = smoker.c myipc.c
c_obj = smoker.o myipc.o
p_src = provider.c myipc.c
p_obj = provider.o myipc.o

all: provider smoker

smoker: $(c_obj)
	gcc $(c_obj) -o smoker
smoker.o: $(c_src) $(hdrs)
	gcc $(opts) $(c_src)

provider: $(p_obj)
	gcc $(p_obj) -o provider
provider.o: $(p_src) $(hdrs)
	gcc $(opts) $(p_src)

clean:
	rm smoker provider *.o
