#ifndef GAMEENGINE_H
#define GAMEENGINE_H

#include <vector>

namespace Aurora
{
	namespace Utils
	{

		class CGameState;


		class StateManager
		{
		public:

			void Init();
			void Enter();
			void CleanUp();

			void ChangeState(CGameState* state);
			void PushState(CGameState* state);
			void PopState();

			void HandleEvents();
			void Update();
			void Draw();

			bool Running() {return m_running;}
			void Quit() {m_running = false;}

		private:

			std::vector<CGameState*> states;
			bool m_running;


		};
	}
}

#endif
