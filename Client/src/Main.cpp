#include <iostream>

#include <Symple/Net/SympleNet.h>

int main()
{
	enum NetworkProtocal: uint32_t
	{
		NET_Connect,
	};

	Symple::Net::Message<NetworkProtocal> msg;
	msg.Header.Id = NET_Connect;

	int playerId = 69;
	struct
	{
		float x, y;
	} pos { 14, 42 };

	msg << playerId << pos;
	std::cout << "PlayerId: " << playerId << ", Pos: { " << pos.x << ", " << pos.y << " }\n";

	playerId = 0;
	pos = { 0, 0 };
	std::cout << "PlayerId: " << playerId << ", Pos: { " << pos.x << ", " << pos.y << " }\n";

	msg >> pos >> playerId;
	std::cout << "PlayerId: " << playerId << ", Pos: { " << pos.x << ", " << pos.y << " }\n";

	system("pause");
}