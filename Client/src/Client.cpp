#define SY_NET_ENABLE_LOGGING
#define SY_NET_SHOW_VALIDATION
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
protected:
	virtual void OnMessageRecieve(Symple::Net::Message<NetworkMessage> &msg) override
	{
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
public:
	void Ping()
	{
		std::cout << "Pinging server...\n";

		Symple::Net::Message<NetworkMessage> msg;
		msg.Header.Id = NetworkMessage::Ping;

		std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
		msg << now;
		Send(msg);
	}

	void MessageAll()
	{
		std::cout << "Messaging all...\n";

		Symple::Net::Message<NetworkMessage> msg;
		msg.Header.Id = NetworkMessage::MessageAll;
		Send(msg);
	}
};

Client s_Client;
volatile bool s_ShouldQuit = false;

void HandleInput()
{
	bool keys[3] {};
	bool oldKeys[3] {};

	while (!s_ShouldQuit)
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
			s_Client.Ping();
		if (keys[1] && !oldKeys[1])
			s_Client.MessageAll();
		if (keys[2] && !oldKeys[2])
			s_ShouldQuit = true;

		for (int i = 0; i < 3; i++)
			oldKeys[i] = keys[i];
	}
}

int main()
{
	s_Client.Connect("127.0.0.1", 42069);

	std::thread inputThread(HandleInput);

	const std::string_view test = "Test!";
	for (const char &c : test)
		printf("Char: %c, Value: %i\n", c, c);

	while (!s_ShouldQuit)
	{
		if (s_Client.IsConnected())
			s_Client.Update();
		else
			s_ShouldQuit = true;
	}
	
	if (inputThread.joinable())
		inputThread.join();
	system("pause");
}