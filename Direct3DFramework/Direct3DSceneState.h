#ifndef DIRECT3D_SCENE_STATE_H
#define DIRECT3D_SCENE_STATE_H

namespace MyD3D10Code
{
	class Direct3DSceneStates
	{
	public:
		Direct3DSceneStates();
		~Direct3DSceneStates();
		void SetStateToPaused();
		void SetStateToUnPaused();
		void SetStateToMinimized();
		void SetStateToUnMinimized();
		void SetStateToMaximized();
		void SetStateToUnMaximized();
		void SetStateToResized();
		void SetStateToResizeComplete();
		bool Paused();
		bool Minimized();
		bool Maximized();
		bool Resizing();

	private:
		bool m_ScenePaused;
		bool m_SceneMinimized;
		bool m_SceneMaximized;
		bool m_SceneHasBeenResized;
	};
}

#endif