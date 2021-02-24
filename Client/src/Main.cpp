#include <SympleNet.h>
#include <iostream>
#include <chrono>

std::vector<char> buff(4096);
void GetDataAsync(asio::ip::tcp::socket &socket)
{
	socket.async_read_some(asio::buffer(buff.data(), buff.size()),
		[&](std::error_code err, size_t len)
		{
			if (!err)
			{
				std::cout << "\n\n\tRead " << len << " bytes\n\n";

				for (size_t i = 0; i < len; i++)
					std::cout << buff[i];

				GetDataAsync(socket);
			}
		});
}

int main()
{
	asio::error_code err;
	asio::io_context context;
	asio::io_context::work idleWork(context);
	std::thread contextThread([&]() { context.run(); });

	asio::ip::tcp::endpoint endPoint(asio::ip::make_address("51.38.81.49", err), 80);
	asio::ip::tcp::socket socket(context);
	socket.connect(endPoint, err);

	if (err)
		std::cout << "Faild to connect:\n" << err.message() << '\n';
	else
		std::cout << "Connected!\n";

	if (socket.is_open())
	{
		GetDataAsync(socket);

		std::string request =
			"GET /index.html HTTP/1.1\r\n"
			"Host: example.com\r\n"
			"Connection: close\r\n\r\n";

		socket.write_some(asio::buffer(request.data(), request.size()), err);

		using namespace std::chrono_literals;
		std::this_thread::sleep_for(2s);

		context.stop();
		if (contextThread.joinable())
			contextThread.join();
	}

	system("pause");
}