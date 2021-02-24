#pragma once

#include "Symple/Net/OwnedMessage.h"
#include "Symple/Net/ThreadSafeQueue.h"

namespace Symple::Net
{
	template<typename T>
	class Connection final: public std::enable_shared_from_this<Connection<T>>
	{
	public: enum class Owner;
	private:
		Owner m_Owner;
		uint32_t m_Id = 0;

		asio::ip::tcp::socket m_Scoket;
		asio::io_context &m_AsioContext;

		ThreadSafeQueue<Message<T>> m_MessagesToSend;
		ThreadSafeQueue<OwnedMessage<T>> &m_RecievedMessages;
	public:
		Connection(Owner owner, asio::io_context &asioContext, asio::ip::tcp::socket socket, ThreadSafeQueue<OwnedMessage<T>> &recievedMessages)
			: m_Owner(owner), m_AsioContext(asioContext), m_Scoket(std::move(socket)), m_RecievedMessages(recievedMessages)
		{}

		~Connection()
		{}

		uint32_t GetId() const
		{ return m_Id; }

		void ConnectToClient(uint32_t id = 0)
		{
			if (m_Owner == Owner::Server && IsConnected())
				m_Id = id;
		}

		bool Connect();
		bool Disconnect();
		bool IsConnected() const
		{ return m_Scoket.is_open(); }

		bool Send(const Message<T> &msg);
	public:
		enum class Owner
		{
			Server,
			Client,
		};
	};
}