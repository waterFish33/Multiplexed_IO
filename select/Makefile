all:select client
client:client.cpp
	g++ -o $@ $^
select:select.cpp
	g++ -o $@ $^
.PHONY:clean
clean:
	rm -rf select client