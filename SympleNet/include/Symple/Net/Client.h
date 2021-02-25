#pragma once

#include "Symple/Net/Connection.h"

namespace Symple::Net
{
	template<typename T>
	class Client
	{
	private:
		ThreadSafeQueue<OwnedMessage<T>> m_RecievedMessages;
	protected:
		asio::io_context m_Context;
		std::thread m_ContextThread;

		std::unique_ptr<Connection<T>> m_Connection;
	public:
		Client() = default;

		virtual ~Client()
		{ Disconnect(); }

		bool Connect(std::string_view host, uint16_t port)
		{
			try
			{
				asio::ip::tcp::resolver resolver(m_Context);
				asio::ip::tcp::resolver::results_type endpoints = resolver.resolve(host, std::to_string(port));

				m_Connection = std::make_unique<Connection<T>>(Connection<T>::Owner::Client,
					m_Context, asio::ip::tcp::socket(m_Context), m_RecievedMessages);

				m_Connection->ConnectToServer(endpoints);
				m_ContextThread = std::thread([this]() { m_Context.run(); });

				return true;
			}
			catch (std::exception &e)
			{
				std::cerr << "Client exception: " << e.what() << '\n';
				return false;
			}
		}

		void Disconnect()
		{
			if (IsConnected())
				m_Connection->Disconnect();

			m_Context.stop();
			if (m_ContextThread.joinable())
				m_ContextThread.join();

			m_Connection.release();
		}

		bool IsConnected() const
		{ return m_Connection && m_Connection->IsConnected(); }

		void Disconnect() const
		{
			if (IsConnected())
				m_Connection->Disconnect();
		}

		void Send(const Message<T> &msg)
		{
			if (IsConnected())
				m_Connection->Send(msg);
		}

		ThreadSafeQueue<OwnedMessage<T>> &IncomingMessages()
		{ return m_RecievedMessages; }

		const ThreadSafeQueue<OwnedMessage<T>> &IncomingMessages() const
		{ return m_RecievedMessages; }

		void Update(bool wait = true, size_t maxMessages = -1)
		{
			if (wait)
				m_RecievedMessages.Wait();

			size_t msgCount = 0;
			while (msgCount < maxMessages && !m_RecievedMessages.IsEmpty())
			{
				auto msg = m_RecievedMessages.PopFront();
				OnMessageRecieve(msg.Message);
				msgCount++;
			}
		}
	protected:
		virtual bool OnConnect()
		{ return true; }

		virtual void OnDisconnect()
		{}

		virtual void OnMessageRecieve(Message<T> &msg)
		{}
	};
}