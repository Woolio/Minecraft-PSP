#include "InputHelper.h"

InputHelper::InputHelper()
{

}

InputHelper::~InputHelper()
{

}

void InputHelper::Init()
{
	//move
	actionNames[0] = std::string("Forward");
	actionNames[1] = std::string("Back");
	actionNames[2] = std::string("Left");
	actionNames[3] = std::string("Right");

	//rotate
	actionNames[4] = std::string("Rotate up");
	actionNames[5] = std::string("Rotate down");
	actionNames[6] = std::string("Rotate left");
	actionNames[7] = std::string("Rotate right");

	//switch block
	actionNames[8] = std::string("Switch block left");
	actionNames[9] = std::string("Switch block right");
	actionNames[10] = std::string("Switch set up");
	actionNames[11] = std::string("Switch set down");

	actionNames[12] = std::string("Jump");
	actionNames[13] = std::string("Remove block");
	actionNames[14] = std::string("Place block");
	actionNames[15] = std::string("Menu");

	//button names
	buttonNames[0] = std::string("Up");
	buttonNames[1] = std::string("Down");
	buttonNames[2] = std::string("Left");
	buttonNames[3] = std::string("Right");

	buttonNames[4] = std::string("Triangle");
	buttonNames[5] = std::string("Cross");
	buttonNames[6] = std::string("Square");
	buttonNames[7] = std::string("Circle");

	buttonNames[8] = std::string("LTrigger");
	buttonNames[9] = std::string("RTrigger");
	buttonNames[10] = std::string("Select");

	buttonNames[11] = std::string("Analog up");
	buttonNames[12] = std::string("Analog down");
	buttonNames[13] = std::string("Analog left");
	buttonNames[14] = std::string("Analog right");
	buttonNames[15] = std::string("Start");

	//connections
	connections[0] = InputConection(0,0,11);
	connections[1] = InputConection(1,1,12);
	connections[2] = InputConection(2,2,13);
	connections[3] = InputConection(3,3,14);
	connections[4] = InputConection(4,4,4);
	connections[5] = InputConection(5,5,5);
	connections[6] = InputConection(6,6,6);
	connections[7] = InputConection(7,7,7);
	connections[8] = InputConection(8,8,2);
	connections[9] = InputConection(9,9,3);
	connections[10] = InputConection(10,10,0);
	connections[11] = InputConection(11,11,1);
	connections[12] = InputConection(12,12,10);
	connections[13] = InputConection(13,13,8);
	connections[14] = InputConection(14,14,9);
	connections[15] = InputConection(15,15,15);

	//default analog sensitivity
	analogXright = analogYup = 0.4f;
	analogXleft = analogYdown = -0.4f;
}

std::string InputHelper::getActionName(int number)
{
	if(number < 0 || number > 15)
		return "";

	return actionNames[number];
}

std::string InputHelper::getButtonName(int number)
{
	if(number < 0 || number > 15)
		return "";

	return buttonNames[number];
}

InputConection InputHelper::getConnection(int number)
{
	return connections[number];
}

void InputHelper::setButtonToAction(int action,int button)
{
	if(action < 0 || action > 15)
		return;

	connections[action].button = button;
}

int InputHelper::getButtonToAction(int action)
{
	if(action < 0 || action > 15)
		return -1;

	return connections[action].button;
}

void InputHelper::Save()
{
	FILE * pFile;
	pFile = fopen("Config/input.cf","wb");

	if(pFile == NULL)
		return;

	for(int i = 0;i < 16;i++)
	{
		fwrite(&connections[i].key,sizeof(int),1,pFile);
		fwrite(&connections[i].action,sizeof(int),1,pFile);
		fwrite(&connections[i].button,sizeof(int),1,pFile);
	}

	//analog
	fwrite(&analogYup,sizeof(float),1,pFile);
	fwrite(&analogYdown,sizeof(float),1,pFile);
	fwrite(&analogXleft,sizeof(float),1,pFile);
	fwrite(&analogXright,sizeof(float),1,pFile);

	//close file
	fclose(pFile);
}

void InputHelper::Load()
{
	FILE * pFile;
	pFile = fopen("Config/input.cf","rb");

	if(pFile == NULL)
		return;

	for(int i = 0;i < 16;i++)
	{
		InputConection connection;

		fread(&connection.key,sizeof(int),1,pFile);
		fread(&connection.action,sizeof(int),1,pFile);
		fread(&connection.button,sizeof(int),1,pFile);

		connections[i] = connection;
	}

	//analog
	fread(&analogYup,sizeof(float),1,pFile);
	fread(&analogYdown,sizeof(float),1,pFile);
	fread(&analogXleft,sizeof(float),1,pFile);
	fread(&analogXright,sizeof(float),1,pFile);


	//close file
	fclose(pFile);
}

InputHelper InputHelper::m_InputHelper;

InputHelper* InputHelper::Instance()
{
	return &m_InputHelper;
}
