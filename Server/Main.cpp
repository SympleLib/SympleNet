#include <Symple/Net/Server.h>

enum class ServerMessage: uint32_t
{
	Accept,
	Deny,
	Ping,
	Message,
	MessageAll,
};

class Server: public Symple::Net::Server<ServerMessage>
{
public:
	Server(uint16_t port)
		: Symple::Net::Server<ServerMessage>(port) {}
protected:
	virtual bool OnClientConnect(std::shared_ptr<Symple::Net::Connection<ServerMessage>> client) override
	{ return true; }

	virtual void OnClientDisconnect(std::shared_ptr<Symple::Net::Connection<ServerMessage>> client) override
	{ }

	virtual void OnMessageRecieve(std::shared_ptr<Symple::Net::Connection<ServerMessage>> client, Symple::Net::Message<ServerMessage> &msg) override
	{ }
};

int main()
{
	Server server(42069);
	server.Start();

	while (true)
		server.Update();

	system("pause");
}