#ifndef AURORA_NETWORKMANAGER_H
#define AURORA_NETWORKMANAGER_H

#include <malloc.h>
#include <pspkernel.h>
#include <pspctrl.h>
#include <psputility.h>
#include <string.h>

#include <Aurora/Graphics/RenderManager.h>

namespace Aurora
{
	namespace System
	{
		class NetworkManager
		{
		private:

			static NetworkManager m_NetworkManager;

			char local_address[32];
			bool connectionInitialized;

		public:

			static NetworkManager* Instance();

			void Init();

			bool InitializeConnection();
			bool ShutDownConnection();

			bool GetFile(const char *url, const char *filepath);
		};
	}
}

#endif

