
CC = g++ -std=c++14
CFLAGPOST = -g -I ./post -I/usr/include/jsoncpp

LDFLAGS = 	-ljsoncpp -lmicrohttpd -ljsonrpccpp-common -ljsonrpccpp-server -lcurl -ljsonrpccpp-client

CORE_INCS =	post/Core.h post/Person.h post/Post.h post/Comment.h post/Reaction.h	\
		post/Action.h post/Link.h post/Message.h post/JvTime.h post/Tag.h post/OKey.h
CORE_OBJS =	post/Core.o post/Person.o post/Post.o post/Comment.o post/Reaction.o	\
	 	post/Action.o post/Link.o post/JvTime.o post/Tag.o

# rules.
all: 	server/fbserver update/fbupdate search/fbsearch

fbserver.h:		calls_rpc.json
	jsonrpcstub calls_rpc.json --cpp-server=fbServer --cpp-client=fbClient

fbclient.h:		calls_rpc.json
	jsonrpcstub calls_rpc.json --cpp-server=fbServer --cpp-client=fbClient

update/fbupdate.o:		fbclient.h update/fbupdate.cpp $(CORE_INCS)
	$(CC) -c $(CFLAGPOST) -I ./ update/fbupdate.cpp -o update/fbupdate.o

search/fbsearch.o:		fbclient.h search/fbsearch.cpp $(CORE_INCS)
	$(CC) -c $(CFLAGPOST) -I ./ search/fbsearch.cpp -o search/fbsearch.o

server/fbserver.o:		fbserver.h server/fbserver.cpp $(CORE_INCS)
	$(CC) -c $(CFLAGPOST) -I ./ server/fbserver.cpp -o server/fbserver.o

server/fbserver:		$(CORE_OBJS) server/fbserver.o post/JSON_Post.o
	$(CC) -o server/fbserver $(CORE_OBJS) -I ./ server/fbserver.o post/JSON_Post.o $(LDFLAGS)

update/fbupdate: 		$(CORE_OBJS) update/fbupdate.o post/JSON_Post.o
	$(CC) -o update/fbupdate $(CORE_OBJS) -I ./ update/fbupdate.o post/JSON_Post.o $(LDFLAGS)

search/fbsearch:		$(CORE_OBJS) search/fbsearch.o post/JSON_Post.o
	$(CC) -o search/fbsearch $(CORE_OBJS) -I ./ search/fbsearch.o post/JSON_Post.o $(LDFLAGS)

post/Core.o:			post/Core.cpp post/Core.h
	$(CC) -c $(CFLAGPOST) post/Core.cpp -o post/Core.o

post/Tag.o:			post/Tag.cpp post/Tag.h
	$(CC) -c $(CFLAGPOST) post/Tag.cpp -o post/Tag.o

post/Person.o:		post/Person.cpp post/Person.h post/Core.h
	$(CC) -c $(CFLAGPOST) post/Person.cpp -o post/Person.o

post/Post.o:			post/Post.cpp post/Post.h
	$(CC) -c $(CFLAGPOST) post/Post.cpp -o post/Post.o

post/Comment.o:		post/Comment.cpp post/Comment.h
	$(CC) -c $(CFLAGPOST) post/Comment.cpp -o post/Comment.o

post/Reaction.o:		post/Reaction.cpp post/Reaction.h
	$(CC) -c $(CFLAGPOST) post/Reaction.cpp -o post/Reaction.o

post/JvTime.o:		post/JvTime.cpp post/JvTime.h
	$(CC) -c $(CFLAGPOST) post/JvTime.cpp -o post/JvTime.o

post/Link.o:			post/Link.cpp post/Link.h
	$(CC) -c $(CFLAGPOST) post/Link.cpp -o post/Link.o

post/Action.o:		post/Action.cpp post/Action.h
	$(CC) -c $(CFLAGPOST) post/Action.cpp -o post/Action.o

post/JSON_Post.o:		post/JSON_Post.cpp $(CORE_INCS)
	$(CC) -c $(CFLAGPOST) post/JSON_Post.cpp -o post/JSON_Post.o

clean:
	rm -f post/*.o search/*.o update/*.o server/*.o *~ core search/fbsearch server/fbserver update/fbupdate fbserver.h fbclient.h


