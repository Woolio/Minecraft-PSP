#include "StateOptions.h"
#include "InputHelper.h"
#include "TextureHelper.h"

// font vars
#define default_size 0.5
#define default_big_size 0.687

#define ENGLISH 1
#define RUSSIAN 2

StateOptions::StateOptions()
{

}

StateOptions::~StateOptions()
{

}

void StateOptions::Init()
{
	//set render manager instance
	mRender = RenderManager::InstancePtr();
	mSystemMgr = SystemManager::Instance();
	mSoundMgr = SoundManager::Instance();

	buttonSprite = new Sprite(TextureHelper::Instance()->GetTexture(TextureHelper::Buttons),0,0,95,12);
    buttonSprite->SetPosition(240,150);
    buttonSprite->Scale(2,2);

    nbuttonSprite = new Sprite(TextureHelper::Instance()->GetTexture(TextureHelper::Buttons),0,24,95,12);
    nbuttonSprite->SetPosition(240,150);
    nbuttonSprite->Scale(2,2);

	sbuttonSprite = new Sprite(TextureHelper::Instance()->GetTexture(TextureHelper::Buttons),0,12,95,12);
    sbuttonSprite->SetPosition(240,150);
    sbuttonSprite->Scale(2,2);

	halfbuttonSprite = new Sprite(TextureHelper::Instance()->GetTexture(TextureHelper::Buttons),0,0,95,12);
	halfbuttonSprite->Scale(1.5,2);
	halfbuttonSprite->SetPosition(240,150);

	halfsbuttonSprite = new Sprite(TextureHelper::Instance()->GetTexture(TextureHelper::Buttons),0,12,95,12);
	halfsbuttonSprite->Scale(1.5,2);
	halfsbuttonSprite->SetPosition(240,150);

	backSprite = new Sprite(TextureHelper::Instance()->GetTexture(TextureHelper::Dirt),0,0,32,32);
	backSprite->Scale(2,2);

	moverSprite = new Sprite(TextureHelper::Instance()->GetTexture(TextureHelper::Buttons),0,60,6,12);
	moverSprite->Scale(2,2);
	smoverSprite = new Sprite(TextureHelper::Instance()->GetTexture(TextureHelper::Buttons),0,60,6,12);
	smoverSprite->Scale(2,2);

	selectPos = 0;
	menuState = 0;

	//controls
	controlPos = 0;
	controlStart = 0;
	controlEnd = 0;

	//key states
	chooseKeyState = false;
	configChanged = false;
	currentKey = -1;
	newKey = -1;

	currentAnalogPos = 0;
}

void StateOptions::Enter()
{
	mRender->SetOrtho(0,0,0,0,0,0);
	selectPos = 0;
}

void StateOptions::CleanUp()
{
	delete buttonSprite;
    delete nbuttonSprite;
	delete sbuttonSprite;
	delete halfbuttonSprite;
	delete halfsbuttonSprite;
	delete backSprite;
	delete moverSprite;
	delete smoverSprite;
}

void StateOptions::Pause()
{

}

void StateOptions::Resume()
{

	menuState = 0;
	mRender->SetOrtho(0,0,0,0,0,0);
}

void StateOptions::HandleEvents(StateManager* sManager)
{
	//update input
	mSystemMgr->InputUpdate();

	switch(menuState)
	{
		case 0:
		{
			//up, down
			if(mSystemMgr->KeyPressed(PSP_CTRL_UP))
			{
				selectPos--;
				if(selectPos < 0)
					selectPos = 2;

				mSoundMgr->PlayMenuSound();
			}

			if(mSystemMgr->KeyPressed(PSP_CTRL_DOWN))
			{
				selectPos++;
				if(selectPos > 2)
					selectPos = 0;

				mSoundMgr->PlayMenuSound();
			}

			if(mSystemMgr->KeyPressed(PSP_CTRL_CIRCLE))
			{
				InputHelper::Instance()->Save();
				sManager->PopState();
			}

			if(mSystemMgr->KeyPressed(PSP_CTRL_CROSS))
			{
				if(selectPos == 0)
				{
					menuState = 1;

					controlPos = 0;
					controlStart = 0;
					controlEnd = 6;

					//reset key state settings
					chooseKeyState = false;
					configChanged = false;
					currentKey = -1;
					newKey = -1;

				}
				if(selectPos == 1)
				{
					menuState = 2;
					currentAnalogPos = 0;
				}
				if(selectPos == 2)
				{
					InputHelper::Instance()->Save();
					sManager->PopState();
				}
			}
		}
		break;
		case 1:
		{
			if(chooseKeyState)//here we set new key for action
			{
				//check each button
				if(mSystemMgr->KeyPressed(PSP_CTRL_UP))
					currentKey = 0;//cross
				if(mSystemMgr->KeyPressed(PSP_CTRL_DOWN))
					currentKey = 1;//cross
				if(mSystemMgr->KeyPressed(PSP_CTRL_LEFT))
					currentKey = 2;//cross
				if(mSystemMgr->KeyPressed(PSP_CTRL_RIGHT))
					currentKey = 3;//cross

				if(mSystemMgr->KeyPressed(PSP_CTRL_TRIANGLE))
					currentKey = 4;//cross
				if(mSystemMgr->KeyPressed(PSP_CTRL_CROSS))
					currentKey = 5;//cross
				if(mSystemMgr->KeyPressed(PSP_CTRL_SQUARE))
					currentKey = 6;//cross
				if(mSystemMgr->KeyPressed(PSP_CTRL_CIRCLE))
					currentKey = 7;//cross

				if(mSystemMgr->KeyPressed(PSP_CTRL_LTRIGGER))
					currentKey = 8;//cross
				if(mSystemMgr->KeyPressed(PSP_CTRL_RTRIGGER))
					currentKey = 9;//cross
				if(mSystemMgr->KeyPressed(PSP_CTRL_SELECT))
					currentKey = 10;//cross
				if(mSystemMgr->KeyPressed(PSP_CTRL_START))
					currentKey = 15;//cross

				if(mSystemMgr->GetAnalogY() > InputHelper::Instance()->analogYup)//analog up
					currentKey = 12;//cross
				if(mSystemMgr->GetAnalogY() < InputHelper::Instance()->analogYdown)//analog down
					currentKey = 11;//cross
				if(mSystemMgr->GetAnalogX() < InputHelper::Instance()->analogXleft)//analog left
					currentKey = 13;//cross
				if(mSystemMgr->GetAnalogX() > InputHelper::Instance()->analogXright)//analog right
					currentKey = 14;//cross

				if(currentKey != -1)
				{
					//chek if it the same key
					if(InputHelper::Instance()->getButtonToAction(controlPos) != currentKey)
						configChanged = true;

					//set key
					InputHelper::Instance()->setButtonToAction(controlPos,currentKey);

					//quit
					chooseKeyState = false;

					mSoundMgr->PlayMenuSound();
				}

			}else
			{
				if(mSystemMgr->KeyPressed(PSP_CTRL_UP))
				{
					controlPos--;

					if(controlPos < controlStart)
					{
						controlStart--;
						controlEnd--;

						if(controlEnd < 6)
						{
							controlStart = 0;
							controlEnd = 6;
						}
					}

					if(controlPos < 0)
					{
						controlPos = 16;
						controlEnd = 16;
						controlStart = 10;
						if(controlStart < 0)
							controlStart = 0;
					}

					mSoundMgr->PlayMenuSound();
				}

				if(mSystemMgr->KeyPressed(PSP_CTRL_DOWN))
				{
					controlPos++;

					if(controlPos == controlEnd)
					{
						controlStart++;
						controlEnd++;
						if(controlEnd > 16)//jest 15 opcji od 0
						{
							controlStart = 10;
							controlEnd = 16;
						}
					}

					if(controlPos > 16)
					{
						controlPos = 0;
						controlStart = 0;
						controlEnd = 6;
					}

					mSoundMgr->PlayMenuSound();
				}

				if(mSystemMgr->KeyPressed(PSP_CTRL_CIRCLE))
				{
					menuState = 0;
				}

				if(mSystemMgr->KeyPressed(PSP_CTRL_CROSS))
				{
					if(controlPos == 16)//back button - exit from this menu
					{
						//save if any key changed
						//if(configChanged)
						//	InputHelper::Instance()->Save();

						menuState = 0;
					}else//start key choosing
					{
						currentKey = -1;
						chooseKeyState = true;
					}
				}
			}
		}
		break;
		case 2:
		{
			if(mSystemMgr->KeyPressed(PSP_CTRL_UP))
			{
				currentAnalogPos--;

				if(currentAnalogPos < 0)
					currentAnalogPos = 4;

				mSoundMgr->PlayMenuSound();
			}

			if(mSystemMgr->KeyPressed(PSP_CTRL_DOWN))
			{
				currentAnalogPos++;
				if(currentAnalogPos > 4)
					currentAnalogPos = 0;

				mSoundMgr->PlayMenuSound();
			}

			if(mSystemMgr->KeyPressed(PSP_CTRL_LEFT))
			{
				//dodatnie
				if(currentAnalogPos == 0 || currentAnalogPos == 3)
				{
					if(currentAnalogPos == 0)
					{
						InputHelper::Instance()->analogYup -= 0.1f;
						if(InputHelper::Instance()->analogYup < 0.1f)
							InputHelper::Instance()->analogYup = 0.1f;
					}else
					{
						InputHelper::Instance()->analogXright -= 0.1f;
						if(InputHelper::Instance()->analogXright < 0.1f)
							InputHelper::Instance()->analogXright = 0.1f;
					}
				}
				//ujemne
				if(currentAnalogPos == 1 || currentAnalogPos == 2)
				{
					if(currentAnalogPos == 1)
					{
						InputHelper::Instance()->analogYdown += 0.1f;
						if(InputHelper::Instance()->analogYdown > -0.1f)
							InputHelper::Instance()->analogYdown = -0.1f;
					}else
					{
						InputHelper::Instance()->analogXleft += 0.1f;
						if(InputHelper::Instance()->analogXleft > -0.1f)
							InputHelper::Instance()->analogXleft = -0.1f;
					}
				}

				mSoundMgr->PlayMenuSound();
			}

			if(mSystemMgr->KeyPressed(PSP_CTRL_RIGHT))
			{
				//dodatnie
				if(currentAnalogPos == 0 || currentAnalogPos == 3)
				{
					if(currentAnalogPos == 0)
					{
						InputHelper::Instance()->analogYup += 0.1f;
						if(InputHelper::Instance()->analogYup > 0.9f)
							InputHelper::Instance()->analogYup = 0.9f;
					}else
					{
						InputHelper::Instance()->analogXright += 0.1f;
						if(InputHelper::Instance()->analogXright > 0.9f)
							InputHelper::Instance()->analogXright = 0.9f;
					}
				}
				//ujemne
				if(currentAnalogPos == 1 || currentAnalogPos == 2)
				{
					if(currentAnalogPos == 1)
					{
						InputHelper::Instance()->analogYdown -= 0.1f;
						if(InputHelper::Instance()->analogYdown < -0.9f)
							InputHelper::Instance()->analogYdown = -0.9f;
					}else
					{
						InputHelper::Instance()->analogXleft -= 0.1f;
						if(InputHelper::Instance()->analogXleft < -0.9f)
							InputHelper::Instance()->analogXleft = -0.9f;
					}
				}

				mSoundMgr->PlayMenuSound();
			}

			if(mSystemMgr->KeyPressed(PSP_CTRL_CIRCLE))
			{
				menuState = 0;
			}

			if(mSystemMgr->KeyPressed(PSP_CTRL_CROSS))
			{
				if(currentAnalogPos == 4)
					menuState = 0;
			}
		}
		break;
	}
}

void StateOptions::Update(StateManager* sManager)
{

}

void StateOptions::Draw(StateManager* sManager)
{
	//start rendering
	mRender->StartFrame(1,1,1);

	switch(menuState)
	{
		case 0:
		{
			sceGuDisable(GU_DEPTH_TEST);
			sceGuEnable(GU_BLEND);
			sceGuColor(GU_COLOR(1,1,1,1.0f));

            for(int x = 0; x < 8; x++)
            {
                for(int y = 0; y < 5; y++)
                {
                    backSprite->SetPosition(x*64,y*64);
                    backSprite->Draw();
                }
            }


			//Controls
			buttonSprite->SetPosition(240,120);
			buttonSprite->Draw();

			//sensity - analog stick
			buttonSprite->SetPosition(240,160);
			buttonSprite->Draw();

			//back
			buttonSprite->SetPosition(240,200);
			buttonSprite->Draw();


			//selected button
			sbuttonSprite->SetPosition(240,(selectPos * 40) + 120);
			sbuttonSprite->Draw();

			sceGuDisable(GU_BLEND);
			sceGuEnable(GU_DEPTH_TEST);

			//draw subtitles on buttons
            if(mRender->GetFontLanguage() == ENGLISH)
            {
                selectPos == 0 ? DrawText(240,129,GU_COLOR(1,1,0,1) ,default_size,"Controls") : DrawText(240,129,GU_COLOR(1,1,1,1) ,default_size,"Controls");
                selectPos == 1 ? DrawText(240,169,GU_COLOR(1,1,0,1) ,default_size,"Analog stick") : DrawText(240,169,GU_COLOR(1,1,1,1) ,default_size,"Analog stick");
                selectPos == 2 ? DrawText(240,209,GU_COLOR(1,1,0,1) ,default_size,"Cancel") : DrawText(240,209,GU_COLOR(1,1,1,1) ,default_size,"Cancel");

                DrawText(240,29,GU_COLOR(1,1,1,1) ,default_size,"Options");
            }
            if(mRender->GetFontLanguage() == RUSSIAN)
            {
                selectPos == 0 ? DrawText(240,129,GU_COLOR(1,1,0,1) ,default_size,"Naznayenie knopok") : DrawText(240,129,GU_COLOR(1,1,1,1) ,default_size,"Naznayenie knopok");
                selectPos == 1 ? DrawText(240,169,GU_COLOR(1,1,0,1) ,default_size,"Nastro~ki stika") : DrawText(240,169,GU_COLOR(1,1,1,1) ,default_size,"Nastro~ki stika");
                selectPos == 2 ? DrawText(240,209,GU_COLOR(1,1,0,1) ,default_size,"Otmena") : DrawText(240,209,GU_COLOR(1,1,1,1) ,default_size,"Otmena");

                DrawText(240,29,GU_COLOR(1,1,1,1) ,default_size,"Nastro~ki");
            }
		}
		break;
		case 1://controls
		{
		    mRender->SetFont(ENGLISH);
			sceGuDisable(GU_DEPTH_TEST);
			sceGuEnable(GU_BLEND);
			sceGuColor(GU_COLOR(1,1,1,1.0f));

            for(int x = 0; x < 8; x++)
            {
                for(int y = 0; y < 5; y++)
                {
                    backSprite->SetPosition(x*64,y*64);
                    backSprite->Draw();
                }
            }


			int starty = 60;
			//something
			for(int i = controlStart;i < controlEnd;i++)
			{
				if(controlPos == i)
				{
					halfsbuttonSprite->SetPosition(160,starty + (i * 30) - (controlStart * 30));
					halfsbuttonSprite->Draw();
				}else
				{
					halfbuttonSprite->SetPosition(160,starty + (i * 30) - (controlStart * 30));
					halfbuttonSprite->Draw();
				}
			}

			//back
			if(controlPos == 16)
			{
				sbuttonSprite->SetPosition(240,260);
				sbuttonSprite->Draw();
			}else
			{
				buttonSprite->SetPosition(240,260);
				buttonSprite->Draw();
			}

			sceGuDisable(GU_BLEND);
			sceGuEnable(GU_DEPTH_TEST);

			//write action names
			starty = 67;
			mRender->SetFontStyle(default_size,0xFFFFFFFF,0,0x00000000);
			for(int i = controlStart;i < controlEnd;i++)
			{
				//action
				controlPos == i ? mRender->SetFontStyle(default_size,GU_COLOR(1,1,0.25f,1),0,0x00000000) : mRender->SetFontStyle(default_size,GU_COLOR(1,1,1,1),0,0x00000000);
				mRender->DebugPrint(250,starty + (i * 30) - (controlStart * 30)+4,InputHelper::Instance()->getActionName(i).c_str());
			}

			mRender->SetFontStyle(default_big_size,0xFFFFFFFF,0,0x00000200);
			starty = 65;
			for(int i = controlStart;i < controlEnd;i++)
			{
				//button assigned to this action
				controlPos == i ? mRender->SetFontStyle(default_size,GU_COLOR(1,1,0.25f,1),0,0x00000200) : mRender->SetFontStyle(default_size,GU_COLOR(1,1,1,1),0,0x00000200);
				if(controlPos == i && chooseKeyState == true)
					mRender->DebugPrint(160,starty + (i * 30) - (controlStart * 30)+4,"...");
				else
					mRender->DebugPrint(160,starty + (i * 30) - (controlStart * 30)+4,InputHelper::Instance()->getButtonName(InputHelper::Instance()->getConnection(i).button).c_str());
			}
			mRender->SetDefaultFont();

            if(mRender->GetFontLanguage() == ENGLISH)
            {
                controlPos == 16 ? DrawText(240,269,GU_COLOR(1,1,0,1) ,default_size,"Cancel") : DrawText(240,269,GU_COLOR(1,1,1,1) ,default_size,"Cancel");
                DrawText(240,29,GU_COLOR(1,1,1,1) ,default_size,"Controls");
            }
            if(mRender->GetFontLanguage() == RUSSIAN)
            {
                controlPos == 16 ? DrawText(240,269,GU_COLOR(1,1,0,1) ,default_size,"Otmena") : DrawText(240,269,GU_COLOR(1,1,1,1) ,default_size,"Otmena");
                DrawText(240,29,GU_COLOR(1,1,1,1) ,default_size,"Naznayenie knopok");
            }
		}
		break;
		case 2://analog stick
		{
			sceGuDisable(GU_DEPTH_TEST);
			sceGuEnable(GU_BLEND);
			sceGuColor(GU_COLOR(1,1,1,1.0f));

            for(int x = 0; x < 8; x++)
            {
                for(int y = 0; y < 5; y++)
                {
                    backSprite->SetPosition(x*64,y*64);
                    backSprite->Draw();
                }
            }

			//something
			nbuttonSprite->SetPosition(240,100);
			nbuttonSprite->Draw();

			nbuttonSprite->SetPosition(240,130);
			nbuttonSprite->Draw();

			nbuttonSprite->SetPosition(240,160);
			nbuttonSprite->Draw();

			nbuttonSprite->SetPosition(240,190);
			nbuttonSprite->Draw();

			//back
			buttonSprite->SetPosition(240,260);
			buttonSprite->Draw();

			if(currentAnalogPos < 4)
			{
				sbuttonSprite->SetPosition(240,100 + (currentAnalogPos * 30));
				sbuttonSprite->Draw();
			}else
			{
				sbuttonSprite->SetPosition(240,260);
				sbuttonSprite->Draw();
			}

			moverSprite->SetPosition(144 +((fabs(InputHelper::Instance()->analogYup) * 10) * 20),100);
			moverSprite->Draw();

			moverSprite->SetPosition(144 +((fabs(InputHelper::Instance()->analogYdown) * 10) * 20),130);
			moverSprite->Draw();

			moverSprite->SetPosition(144 +((fabs(InputHelper::Instance()->analogXleft) * 10) * 20),160);
			moverSprite->Draw();

			moverSprite->SetPosition(144 +((fabs(InputHelper::Instance()->analogXright) * 10) * 20),190);
			moverSprite->Draw();

			sceGuDisable(GU_BLEND);
			sceGuEnable(GU_DEPTH_TEST);

            mRender->SetFontStyle(default_size,0xFFFFFFFF,0,0x00000200);
            if(mRender->GetFontLanguage() == ENGLISH)
            {
                currentAnalogPos == 0 ? mRender->SetFontStyle(default_size,GU_COLOR(1,1,0.25f,1),0,0x00000200) : mRender->SetFontStyle(default_size,GU_COLOR(1,1,1,1),0,0x00000200);
                mRender->DebugPrint(240,109,"Analog up : %d%%",(int)(fabs(InputHelper::Instance()->analogYup) * 100.0f));

                currentAnalogPos == 1 ? mRender->SetFontStyle(default_size,GU_COLOR(1,1,0.25f,1),0,0x00000200) : mRender->SetFontStyle(default_size,GU_COLOR(1,1,1,1),0,0x00000200);
                mRender->DebugPrint(240,139,"Analog down : %d%%",(int)(fabs(InputHelper::Instance()->analogYdown) * 100.0f));

                currentAnalogPos == 2 ? mRender->SetFontStyle(default_size,GU_COLOR(1,1,0.25f,1),0,0x00000200) : mRender->SetFontStyle(default_size,GU_COLOR(1,1,1,1),0,0x00000200);
                mRender->DebugPrint(240,169,"Analog left : %d%%",(int)(fabs(InputHelper::Instance()->analogXleft) * 100.0f));

                currentAnalogPos == 3 ? mRender->SetFontStyle(default_size,GU_COLOR(1,1,0.25f,1),0,0x00000200) : mRender->SetFontStyle(default_size,GU_COLOR(1,1,1,1),0,0x00000200);
                mRender->DebugPrint(240,199,"Analog right : %d%%",(int)(fabs(InputHelper::Instance()->analogXright) * 100.0f));

                currentAnalogPos >= 4 ? mRender->SetFontStyle(default_size,GU_COLOR(1,1,0.25f,1),0,0x00000200) : mRender->SetFontStyle(default_size,GU_COLOR(1,1,1,1),0,0x00000200);
                mRender->DebugPrint(240,269,"Cancel");

                DrawText(240,29,GU_COLOR(1,1,1,1) ,default_size,"Analog stick");
            }
            if(mRender->GetFontLanguage() == RUSSIAN)
            {
                currentAnalogPos == 0 ? mRender->SetFontStyle(default_size,GU_COLOR(1,1,0.25f,1),0,0x00000200) : mRender->SetFontStyle(default_size,GU_COLOR(1,1,1,1),0,0x00000200);
                mRender->DebugPrint(240,109,"Stik vverh : %d%%",(int)(fabs(InputHelper::Instance()->analogYup) * 100.0f));

                currentAnalogPos == 1 ? mRender->SetFontStyle(default_size,GU_COLOR(1,1,0.25f,1),0,0x00000200) : mRender->SetFontStyle(default_size,GU_COLOR(1,1,1,1),0,0x00000200);
                mRender->DebugPrint(240,139,"Stik vniz : %d%%",(int)(fabs(InputHelper::Instance()->analogYdown) * 100.0f));

                currentAnalogPos == 2 ? mRender->SetFontStyle(default_size,GU_COLOR(1,1,0.25f,1),0,0x00000200) : mRender->SetFontStyle(default_size,GU_COLOR(1,1,1,1),0,0x00000200);
                mRender->DebugPrint(240,169,"Stik vlevo : %d%%",(int)(fabs(InputHelper::Instance()->analogXleft) * 100.0f));

                currentAnalogPos == 3 ? mRender->SetFontStyle(default_size,GU_COLOR(1,1,0.25f,1),0,0x00000200) : mRender->SetFontStyle(default_size,GU_COLOR(1,1,1,1),0,0x00000200);
                mRender->DebugPrint(240,199,"Stik vpravo : %d%%",(int)(fabs(InputHelper::Instance()->analogXright) * 100.0f));

                currentAnalogPos >= 4 ? mRender->SetFontStyle(default_size,GU_COLOR(1,1,0.25f,1),0,0x00000200) : mRender->SetFontStyle(default_size,GU_COLOR(1,1,1,1),0,0x00000200);
                mRender->DebugPrint(240,269,"Otmena");

                DrawText(240,29,GU_COLOR(1,1,1,1) ,default_size,"Nastro~ki stika");
            }
		}
		break;
	}

	//end frame
	mRender->EndFrame();
}

void StateOptions::DrawText(int x,int y, unsigned int color, float size, const char *message, ...)
{
    mRender->SetFontStyle(size,color,0,0x00000200|0x00000000);
    mRender->DebugPrint(x,y,message);
}
