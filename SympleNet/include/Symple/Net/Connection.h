#pragma once

#include "Symple/Net/OwnedMessage.h"
#include "Symple/Net/ThreadSafeQueue.h"

namespace Symple::Net
{
	template<typename T>
	class Connection final: public std::enable_shared_from_this<Connection<T>>
	{
	private:
		asio::ip::tcp::socket m_Scoket;
		asio::io_context &m_Context;

		ThreadSafeQueue<Message<T>> m_MessagesToSend;
		ThreadSafeQueue<OwnedMessage<T>> &m_RecievedMessages;
	public:
		Connection()
		{}

		~Connection()
		{}

		bool Connect();
		bool Disconnect();
		bool IsConnected() const;

		bool Send(const Message<T> &msg);
	};
}