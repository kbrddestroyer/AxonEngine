#pragma once

#include <iostream>
#include <server.h>

namespace Axon
{
	// Network engine global namespace

	namespace Error
	{
		enum AxonErrorCode
		{
			INTERNAL_ERROR = 1
		};
	}

	class AxonError
	{
	protected:
		Error::AxonErrorCode reason;
	public:
		AxonError(Error::AxonErrorCode = Error::AxonErrorCode::INTERNAL_ERROR);

		Error::AxonErrorCode err();
	};

}