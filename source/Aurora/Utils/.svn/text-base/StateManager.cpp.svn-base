#include <pspkernel.h>
#include <stdio.h>
#include <string.h>
#include <pspgu.h>
#include <pspgum.h>
#include <pspdisplay.h>

//gamestate/engine includes
#include <Aurora/Utils/StateManager.h>
#include <Aurora/Utils/GameState.h>

namespace Aurora
{
	namespace Utils
	{

		void StateManager::Init()
		{
			m_running = true;
		}

		void StateManager::Enter()
		{

		}

		void StateManager::CleanUp()
		{

		}

		void StateManager::ChangeState(CGameState* state)
		{
			// cleanup the current state
			if ( !states.empty() ) {
				states.back()->CleanUp();
				states.pop_back();
			}

			// store and init the new state
			states.push_back(state);
			states.back()->Enter();
		}

		void StateManager::PushState(CGameState* state)
		{
			// pause current state
			if ( !states.empty() ) {
				states.back()->Pause();
			}

			// store and init the new state
			states.push_back(state);
			states.back()->Enter();
		}

		void StateManager::PopState()
		{
			// cleanup the current state
			if ( !states.empty() ) {
				states.back()->CleanUp();
				states.pop_back();
			}

			// resume previous state
			if ( !states.empty() ) {
				states.back()->Resume();
			}
		}


		void StateManager::HandleEvents()
		{
			states.back()->HandleEvents(this);
		}

		void StateManager::Update()
		{
			states.back()->Update(this);
		}

		void StateManager::Draw()
		{
			states.back()->Draw(this);
		}

	}
}

