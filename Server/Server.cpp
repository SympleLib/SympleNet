#include <Symple/Net/Server.h>

enum class NetworkMessage: uint32_t
{
	Accept,
	Deny,
	Ping,
	Message,
	MessageAll,
};

class Server: public Symple::Net::Server<NetworkMessage>
{
private:
	using Connection = Symple::Net::Connection<NetworkMessage>;
public:
	Server(uint16_t port)
		: Symple::Net::Server<NetworkMessage>(port) {}
protected:
	virtual bool OnClientConnect(std::shared_ptr<Connection> client) override
	{
		Symple::Net::Message<NetworkMessage> msg;
		msg.Header.Id = NetworkMessage::Accept;
		client->Send(msg);

		return true;
	}

	virtual void OnClientDisconnect(std::shared_ptr<Connection> client) override
	{
		std::cout << "Client #" << client->GetId() << " disconnected.\n";
	}

	virtual void OnMessageRecieve(std::shared_ptr<Connection> client, Symple::Net::Message<NetworkMessage> &msg) override
	{
		switch (msg.Header.Id)
		{
		case NetworkMessage::Ping:
			std::cout << "Client #" << client->GetId() << " pinged.\n";
			client->Send(msg);
			break;
		case NetworkMessage::MessageAll:
		{
			std::cout << "Client #" << client->GetId() << " says Hello!\n";

			Symple::Net::Message<NetworkMessage> msg;
			msg.Header.Id = NetworkMessage::MessageAll;
			msg << client->GetId();

			MessageAllClients(msg, client);
			break;
		}
		}
	}
};

int main()
{
	Server server(42069);
	server.Start();

	while (true)
		server.Update(false);

	system("pause");
}