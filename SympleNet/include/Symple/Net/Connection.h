#pragma once

#include "Symple/Net/OwnedMessage.h"

namespace Symple::Net
{
	template<typename T>
	class Connection final: public std::enable_shared_from_this<Connection<T>>
	{
	private:
		
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