#include "input_manager.h"
#include "script_system.h"
#include "cxmath.h"
#include "scene/scene_manager.h"
#include "window.h"


InputManager::InputManager()
:Singleton<InputManager>(),
m_Keys(1024,false),
m_KeyStates(1024,0),
m_FirstMouse(true),
m_MousePos(0,0),
m_FocusView(nullptr),
m_Camera(nullptr)
{

}

InputManager::~InputManager()
{

}

void InputManager::SetCamera(Camera* camera)
{
	m_Camera = camera;
}

void InputManager::RegisterView(View*v)
{
	m_RegisterViews.insert(v);
}
void InputManager::UnRegisterView(View*v)
{
	if (m_FocusView == v)
	{
		m_FocusView = nullptr;
	}
	m_RegisterViews.erase(v);
}
IntPos InputManager::GetMouseIntPos()
{
	IntPos pos{ (int)m_MousePos.x, (int)m_MousePos.y };
	return pos;
}

void InputManager::Init()
{
	
}

void InputManager::MouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
	if (action == GLFW_PRESS) {
		m_Keys[button] = true;

		for (auto* it : m_RegisterViews)
		{
			if (it)
			{
				Bound bound = it->GetViewBounds();

				Pos pos = GetMousePos();
				if (utils::BoundHitTest(bound, pos))
				{
					it->SetPressed(true, button);
					break;
				}
			}
		}
	}

	if (action == GLFW_RELEASE)
	{
		for (auto* it : m_RegisterViews)
		{
			if (it)
			{
				Bound bound = it->GetViewBounds();
				Pos pos = GetMousePos();
				if (it->IsDragStart())
				{
					it->SetDragStart(false);
					it->dragX = -1;
					it->dragY = -1;
					it->OnDragEnd();
					break;
				}
				else if (it->IsPressed())
				{
					it->SetPressed(false, -1);
					if (utils::BoundHitTest(bound, pos))
					{
						it->OnClick(button, (int)pos.x, (int)pos.y);

						m_Keys[button] = false;
						return;
					}

					break;
				}
			}
		}
		if (m_Keys[button])
		{
			for (auto* it : m_RegisterViews)
			{
				if (it)
				{
					IntPos pos = GetMouseIntPos();
					it->OnGlobleClick(pos.x, pos.y);
				}
			}
		}

		if (m_Keys[button] && m_ClickEvents.find(button) != m_ClickEvents.end())
		{
			for (auto& f : m_ClickEvents[button])
			{
				f();
			}
			//    m_ClickEvents.erase(button);
		}

		if (m_KeyUpEvents.find(button) != m_KeyUpEvents.end())
		{
			for (auto& f : m_KeyUpEvents[button])
			{
				f();
			}
		}
		m_Keys[button] = false;
	}

}


void InputManager::KeyCallbackFunc(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, GL_TRUE);
	}

	if (action == GLFW_PRESS) {
		m_Keys[key] = true;
		for (auto* it : m_RegisterViews)
		{
			if (it)
			{
				it->OnKeyDownEvent(key);
			}
		}
	}
	if (action == GLFW_REPEAT)
	{
		for (auto* it : m_RegisterViews)
		{
			if (it)
			{
				it->OnKeyRepeatEvent(key);
			}
		}
	}

	if (action == GLFW_RELEASE)
	{
		if (m_Keys[key] && m_ClickEvents.find(key) != m_ClickEvents.end())
		{
			for (auto& f : m_ClickEvents[key])
			{
				f();
			}
			//     m_ClickEvents.erase(key);
		}

		for (auto* it : m_RegisterViews)
		{
			if (it)
			{
				it->OnKeyUpEvent(key);
			}
		}
		m_Keys[key] = false;
	}

	if (m_Camera != nullptr)
	{
		float deltaTime = WINDOW_INSTANCE->GetDeltaTime();

		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			glfwSetWindowShouldClose(window, true);
		
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
			m_Camera->ProcessKeyboard(CameraMovement::FORWARD, deltaTime);
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
			m_Camera->ProcessKeyboard(CameraMovement::BACKWARD, deltaTime);
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
			m_Camera->ProcessKeyboard(CameraMovement::LEFT, deltaTime);
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
			m_Camera->ProcessKeyboard(CameraMovement::RIGHT, deltaTime);
	}
}




void InputManager::ScrollCallbackFunc(GLFWwindow* window, float xoffset, float yoffset)
{
	if (m_Camera != nullptr)
	{
		m_Camera->ProcessMouseScroll((float)yoffset);
	}
}

void InputManager::MouseCallbackFunc(GLFWwindow* window, float xpos, float ypos)
{
	if (m_FirstMouse)
	{
		m_MousePos.x = xpos;
		m_MousePos.y = ypos;
		m_FirstMouse = false;
	}
	GLfloat xoffset = xpos - m_MousePos.x;
	GLfloat yoffset = m_MousePos.y - ypos;  // Reversed since y-coordinates go from bottom to left
	if (m_Camera != nullptr)
	{
		m_Camera->ProcessMouseMovement(xoffset, yoffset);
	}

	m_MousePos.x = xpos;
	m_MousePos.y = ypos;
	int ixpos = static_cast<int>(xpos);
	int iypos = static_cast<int>(ypos);
	for (auto* it : m_RegisterViews)
	{
		if (it)
		{
			if (it->IsPressed() && it->GetPressedButton() == GLFW_MOUSE_BUTTON_LEFT)
			{
				if (it->dragX == -1 && it->dragY == -1)
				{
					it->dragX = ixpos;
					it->dragY = iypos;
				}

				int dx = ixpos - it->dragX;
				int dy = iypos - it->dragY;
				if (it->CheckDrag(dx, dy))
				{
					it->SetPressed(false, -1);
					it->SetDragStart(true);
					it->dragX = ixpos;
					it->dragY = iypos;
					it->OnDragStart();
					break;
				}
			}
			else if (it->IsDragStart())
			{
				int dx = ixpos - it->dragX;
				int dy = iypos - it->dragY;
				it->OnDragMove(dx, dy);
				it->dragX = ixpos;
				it->dragY = iypos;
				break;
			}
			
			if (utils::BoundHitTest(it->GetViewBounds(), m_MousePos))
			{
				it->OnHover(m_MousePos.x, m_MousePos.y);
			}
			
		}
	}
	//std::cout << "MouseX:" << xpos << "\tMouseY:" << ypos << std::endl;
}
void InputManager::CharacterInputCallback(GLFWwindow* window, unsigned int charcode)
{
	for (auto* it : m_RegisterViews)
	{
		if (it)
		{
			if (it == m_FocusView)
			{
				it->OnCharacterInput(static_cast<uint32_t>(charcode));
				break;
			}
		}
	}
}

void InputManager::RequestFocus(View* view)
{
	if (view == nullptr)
	{
		if (m_FocusView != nullptr)
		{
			m_FocusView->OnFocusChanged(false);
		}
		m_FocusView = nullptr;
	}
	else
	{
		if (m_FocusView != nullptr )
		{
			if (m_FocusView != view)
			{
				m_FocusView->OnFocusChanged(false);
				m_FocusView = view;
				m_FocusView->OnFocusChanged(true);
			}
			else
			{
				m_FocusView->OnFocusChanged(false);
				m_FocusView = nullptr;
			}
		}
		else
		{
			m_FocusView = view;
			m_FocusView->OnFocusChanged(true);
		}
	}
}

Pos InputManager::GetMousePos()
{
	return m_MousePos;
}

void input_manager_init()
{
	INPUT_MANAGER_INSTANCE->Init();
}

void input_manager_deinit()
{
	INPUT_MANAGER_INSTANCE->DeleteSingleton();
}

void luaopen_input_manager(lua_State* L)
{
	script_system_register_function(L, input_manager_init);
	script_system_register_function(L, input_manager_deinit);
}