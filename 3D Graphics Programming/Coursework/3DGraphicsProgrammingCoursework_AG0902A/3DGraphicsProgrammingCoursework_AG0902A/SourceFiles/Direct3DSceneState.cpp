#include "..\\HeaderFiles\\Direct3DSceneState.h"

namespace MyD3D10Code
{
	Direct3DSceneStates::Direct3DSceneStates():
	m_ScenePaused(false),
	m_SceneMinimized(false),
	m_SceneMaximized(false),
	m_SceneHasBeenResized(false)
	{
	}

	Direct3DSceneStates::~Direct3DSceneStates()
	{
	}

	void Direct3DSceneStates::SetStateToPaused()
	{
		m_ScenePaused = true;
	}

	void Direct3DSceneStates::SetStateToUnPaused()
	{
		m_ScenePaused = false;
	}

	void Direct3DSceneStates::SetStateToMinimized()
	{
		m_SceneMinimized = true;
	}

	void Direct3DSceneStates::SetStateToUnMinimized()
	{
		m_SceneMinimized = false;
	}

	void Direct3DSceneStates::SetStateToMaximized()
	{
		m_SceneMaximized = true;
	}

	void Direct3DSceneStates::SetStateToUnMaximized()
	{
		m_SceneMaximized = false;
	}

	void Direct3DSceneStates::SetStateToResized()
	{
		m_SceneHasBeenResized = true;
	}

	void Direct3DSceneStates::SetStateToResizeComplete()
	{
		m_SceneHasBeenResized = false;
	}

	bool Direct3DSceneStates::Paused()
	{
		return m_ScenePaused;
	}
	
	bool Direct3DSceneStates::Minimized()
	{
		return m_SceneMinimized;
	}

	bool Direct3DSceneStates::Maximized()
	{
		return m_SceneMaximized;
	}
	
	bool Direct3DSceneStates::Resizing()
	{
		return m_SceneHasBeenResized;
	}
}