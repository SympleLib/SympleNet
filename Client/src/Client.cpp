#include <Symple/Net/Client.h>

enum class NetworkMessage: uint32_t
{
	Accept,
	Deny,
	Ping,
	Message,
	MessageAll,
};

class Client: public Symple::Net::Client<NetworkMessage>
{
private:
	using Connection = Symple::Net::Connection<NetworkMessage>;
public:
	void Ping()
	{
		Symple::Net::Message<NetworkMessage> msg;
		msg.Header.Id = NetworkMessage::Ping;

		std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
		msg << now;
		Send(msg);
	}

	void MessageAll()
	{
		Symple::Net::Message<NetworkMessage> msg;
		msg.Header.Id = NetworkMessage::MessageAll;
		Send(msg);
	}
};

int main()
{
	Client client;
	client.Connect("127.0.0.1", 42069);

	bool keys[3] {};
	bool oldKeys[3] {};

	bool quit = false;
	while (!quit)
	{
		if (client.IsConnected())
		{
			std::memset(keys, false, 3);
			#if defined(_WIN32)
				if (GetForegroundWindow() == GetConsoleWindow())
				{
					keys[0] = GetAsyncKeyState('1') & 0x8000;
					keys[1] = GetAsyncKeyState('2') & 0x8000;
					keys[2] = GetAsyncKeyState('3') & 0x8000;
				}
			#endif

				if (keys[0] && !oldKeys[0])
					client.Ping();
				if (keys[1] && !oldKeys[1])
					client.MessageAll();
				if (keys[2] && !oldKeys[2])
					quit = true;

				for (int i = 0; i < 3; i++)
					oldKeys[i] = keys[i];

			if (!client.IncomingMessages().IsEmpty())
			{
				auto msg = client.IncomingMessages().PopFront().Message;

				switch (msg.Header.Id)
				{
				case NetworkMessage::Accept:
					std::cout << "Connection accepted!\n";
					break;
				case NetworkMessage::Ping:
				{
					std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
					std::chrono::system_clock::time_point then;
					msg >> then;
					std::cout << "Ping: " << std::chrono::duration_cast<std::chrono::milliseconds>(now - then).count() << "ms.\n";
					break;
				}
				case NetworkMessage::Message:
				{
					uint32_t id;
					msg >> id;
					std::cout << "Client #" << id << " says hello!\n";
					break;
				}
				}
			}
		}
		else
			quit = true;
	}

	system("pause");
}