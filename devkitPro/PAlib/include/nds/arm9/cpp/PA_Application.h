#pragma once

/////////////////////////////////////////////////////////////////
// Simple application abstraction layer for PAlib C++ programs //
/////////////////////////////////////////////////////////////////

/*! \file PA_Application.h
    \brief Simple application abstraction layer for PAlib C++ programs
*/

namespace PA{
	/// Simple application abstraction layer for PAlib C++ programs
	class Application{
	protected:
		// Methods to be implemented by the user

		/// Initialization function
		virtual void init();
		/// Update function
		virtual bool update();
		/// Render function
		virtual void render();
		/// Cleanup function (optional)
		virtual void cleanup(){};

	public:
		/// Runs the application.
		inline void run(){
			// Call the init function
			init();

			for(;;){
				// Render at each VBlank
				PA_WaitForVBlank();
				render();

				// Update the program
				if(!update())
					break;
			}

			// Call the cleanup function
			cleanup();
		}
	};
};
