#ifndef AURORA_TIMER_H
#define AURORA_TIMER_H

#include <psptypes.h>
#include <stdio.h>
#include <psprtc.h>

namespace Aurora
{
	namespace Utils
	{

		class Timer
		{
			public:
				Timer();
				virtual ~Timer();

				double GetDeltaTime( void );	// Returns the delta time

			private:
				double dt;

				u64 timeNow;
				u64 timeLastAsk;
				u32 tickResolution;
		};

	}
}

#endif /* TIMER_H_ */
