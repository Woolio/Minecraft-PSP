#ifndef GAMESTATE_H
#define GAMESTATE_H

#include <Aurora/Utils/StateManager.h>

namespace Aurora
{
	namespace Utils
	{

		class CGameState
		{
		public:
			virtual void Init() = 0;
			virtual void Enter() = 0;
			virtual void CleanUp() = 0;

			virtual void Pause() = 0;
			virtual void Resume() = 0;

			virtual void HandleEvents(StateManager* sManager) = 0;
			virtual void Update(StateManager* sManager) = 0;
			virtual void Draw(StateManager* sManager) = 0;

			void ChangeState(StateManager* sManager, CGameState* state)
			{
				sManager->ChangeState(state);
			}


		protected:
			CGameState() { }
		};
	}
}

#endif
