#ifndef AURORA_LOGGER_H
#define AURORA_LOGGER_H

namespace Aurora
{
	namespace Utils
	{

		class Logger
		{
		private:

			static Logger m_Logger;

		public:

			static Logger* Instance()
			{
				return &m_Logger;
			}

			void LogMessage(const char *message, ...);

		};
	}
}

#endif
