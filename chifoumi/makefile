
COMPILER = gcc
ARGS = -Wall -Wextra -std=c11 -O3

all: base_client base_server q5_client q5_server q9_client q9_server q10_client q10_server q11_client q11_server q12_client q12_server

common.o: common.c
	$(COMPILER) $(ARGS) -c common.c

base_client: common.o base_client.o
	$(COMPILER) $(ARGS) common.o base_client.o -o base_client

base_server: common.o base_server.o
	$(COMPILER) $(ARGS) common.o base_server.o -o base_server

base_client.o: base_client.c
	$(COMPILER) $(ARGS) -c base_client.c

base_server.o: base_server.c
	$(COMPILER) $(ARGS) -c base_server.c

q5_client: common.o q5_client.o
	$(COMPILER) $(ARGS) common.o q5_client.o -o q5_client

q5_server: common.o q5_server.o
	$(COMPILER) $(ARGS) common.o q5_server.o -o q5_server

q5_client.o: q5_client.c
	$(COMPILER) $(ARGS) -c q5_client.c

q5_server.o: q5_server.c
	$(COMPILER) $(ARGS) -c q5_server.c

q9_client: common.o q9_client.o
	$(COMPILER) $(ARGS) common.o q9_client.o -o q9_client

q9_server: common.o q9_server.o
	$(COMPILER) $(ARGS) common.o q9_server.o -o q9_server

q9_client.o: q9_client.c
	$(COMPILER) $(ARGS) -c q9_client.c

q9_server.o: q9_server.c
	$(COMPILER) $(ARGS) -c q9_server.c

q10_client: common.o q10_client.o
	$(COMPILER) $(ARGS) common.o q10_client.o -o q10_client

q10_server: common.o q10_server.o
	$(COMPILER) $(ARGS) common.o q10_server.o -o q10_server

q10_client.o: q10_client.c
	$(COMPILER) $(ARGS) -c q10_client.c

q10_server.o: q10_server.c
	$(COMPILER) $(ARGS) -c q10_server.c

q11_client: common.o q11_client.o
	$(COMPILER) $(ARGS) common.o q11_client.o -o q11_client

q11_server: common.o q11_server.o
	$(COMPILER) $(ARGS) common.o q11_server.o -o q11_server

q11_client.o: q11_client.c
	$(COMPILER) $(ARGS) -c q11_client.c

q11_server.o: q11_server.c
	$(COMPILER) $(ARGS) -c q11_server.c

q12_client: common.o q12_client.o
	$(COMPILER) $(ARGS) common.o q12_client.o -o q12_client

q12_server: common.o q12_server.o
	$(COMPILER) $(ARGS) common.o q12_server.o -o q12_server

q12_client.o: q12_client.c
	$(COMPILER) $(ARGS) -c q12_client.c

q12_server.o: q12_server.c
	$(COMPILER) $(ARGS) -c q12_server.c

clean:
	rm -f *_client *_server *.o