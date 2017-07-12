#ifndef INPUTHELPER_H_
#define INPUTHELPER_H_

#include <string>

class InputConection
{
public:
	int key;
	int action;
	int button;

	InputConection()
	{
		key = 0;
		action = 0;
		button = 0;
	}

	InputConection(int k,int a,int b)
	{
		key = k;
		action = a;
		button = b;
	}
};

class InputHelper
{
public:
	InputHelper();
	~InputHelper();

	static InputHelper m_InputHelper;
	static InputHelper* Instance();

	void Init();

	std::string getActionName(int number);
	std::string getButtonName(int number);
	InputConection getConnection(int number);

	void setButtonToAction(int action,int button);
	int getButtonToAction(int action);

	float analogXleft,analogXright;
	float analogYup,analogYdown;

	void Save();
	void Load();

private:

	std::string actionNames[16];
	std::string buttonNames[16];
	InputConection connections[16];
};

#endif
