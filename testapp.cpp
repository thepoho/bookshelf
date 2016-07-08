
#include <wiringPi.h>
#include "slLib.h"
#include "slNetwork.h"
#include "slTestAppNetworkManager.h"
#include "slInputManager.h"

void StartSystems();
void CloseSystems();

void UpdateLights();


//===============================================================================
//===============================================================================
int main()
{
	slLog_Printf(KYEL "START YOUR ENGINES!" KNRM);
	
	StartSystems();
	
	
	slTestAppNetworkManager* pNet = new slTestAppNetworkManager();
	
	if(slNULL != pNet)
	{
	
		pNet->StartUp();
		
		if(pNet->StartServer(2121))
		//if(pNet->StartClient("192.168.1.100",56700))
		{
			
			slU32 startTime		= slTIMER;
			slU32 lastTime		= startTime;
			slU32 elapsedTime	= 0;
			
			while(1)
			{
				slU32 now 	= slTIMER;
				elapsedTime = lastTime - now;
				lastTime	= now;
				
				pNet->BeginFrame();
				
				slInputManager::Get().Update(elapsedTime);
				
				//slLog_Printf("Time %u %u %u %u",slUtil::GetTimeSeconds(),slTIMER,(slTIMER - startTime),elapsedTime);
				
				//if((slTIMER - startTime) > 1000 * 60)
				//	break;
				
				if((slTIMER - startTime) > 10)
				{
					startTime = slTIMER;
					UpdateLights();
				}
				
			
				pNet->EndFrame();
				
				slSLEEP(1);
			}
		}
		else
		{
			slLog_Printf("Unable to start server");
			
		}
		
		pNet->ShutDown();
		
		slDELETE(pNet);
	}
	
	CloseSystems();
	
	printf("\nWayne has gone home\n");

	return 0;
}

#define LIGHTPIN_CLOCK (27)
#define LIGHTPIN_DATA (28)
//#define LIGHTPIN_GND (29)


slU32 m_maxPixels;
slU32 *m_pixelBuffer;

void SetLight(int idx,slColour col)
{
	if(idx < m_maxPixels)
	{
		m_pixelBuffer[idx] = col.colour;
	}
}
void SetUpLights()
{
	m_maxPixels = 49;//32;//50;

	pinMode (LIGHTPIN_CLOCK,OUTPUT);
	pinMode (LIGHTPIN_DATA,OUTPUT);

	digitalWrite(LIGHTPIN_DATA, LOW);
	digitalWrite(LIGHTPIN_CLOCK, LOW);
	
	pullUpDnControl (LIGHTPIN_CLOCK,PUD_DOWN) ;
	pullUpDnControl (LIGHTPIN_DATA,PUD_DOWN) ;
	
	m_pixelBuffer = new slU32[m_maxPixels];
	
	for(int i = 0;i <m_maxPixels;i++)
	{
		m_pixelBuffer[i] = 0x00000000;
	}
	
}

void RollLights()
{
	for(int i = m_maxPixels-1;i >0;i--)
	{
		m_pixelBuffer[i] = m_pixelBuffer[i-1];
	}
	
	slVarPointer vp;
	
	vp.v = m_pixelBuffer;
	vp.ub++;

	*vp.ub++  = (slByte)(slRandf(255.0f))>>3;
	*vp.ub++  = (slByte)(slRandf(255.0f))>>3;
	*vp.ub++  = (slByte)(slRandf(255.0f))>>3;
	
}

void UpdateLights()
{
	int bitHighCount = 0;
	int bitLowCount = 0;
	int num = m_maxPixels;

	for(int i = 0;i <num;i++)
	{
		// Pi is little endian.. ie u32 0x01020304 number is 04030201 in memory
		//slU32 c = 0x01234567;//m_pixelBuffer[i];
		
		slU32 			c = m_pixelBuffer[i];
		slVarPointer	vp;
		vp.ub = (slByte*)&c;
		
		for(int b = 0; b < 3;b++)
		{
			slByte bb = *vp.ub++;

			for(int j = 0 ;j < 8 ;j++)
			//for(slS32 bit = 0x80; bit ; bit >>= 1) 
			{
				slU32 bit = (1<<(7-j));
				
				if( bb & bit)
				{
					bitHighCount++;
					digitalWrite(LIGHTPIN_DATA, HIGH);
				}
				else         
				{	
					bitLowCount++;
					digitalWrite(LIGHTPIN_DATA, LOW);
				}

				digitalWrite(LIGHTPIN_CLOCK, HIGH);
				digitalWrite(LIGHTPIN_CLOCK, LOW);
			}
		}
	}
	//slLog_Printf("BitHigh %d BitLow %d Total %d",bitHighCount,bitLowCount,(bitHighCount+bitLowCount));
	
	slSLEEP(1);
}

void ShutdownLights()
{
	slDELETEB(m_pixelBuffer);
}


//#define PINOUT_ITSY 	(23)
//#define PINOUT_CERIL 	(24)
//#define PINOUT_MIRROR 	(25)

//===============================================================================
//===============================================================================
void StartSystems()
{
	wiringPiSetup();
	slInputManager::Create();
	slInputManager::Get().Startup();
	SetUpLights();
	
	/*
	// Itsy/Bitsy on
	pinMode (PINOUT_ITSY,OUTPUT);
	pullUpDnControl (PINOUT_ITSY,PUD_DOWN) ;
	digitalWrite(PINOUT_ITSY,LOW);

	// Ceril on
	pinMode (PINOUT_CERIL,OUTPUT);
	pullUpDnControl (PINOUT_CERIL,PUD_DOWN) ;
	digitalWrite(PINOUT_CERIL,LOW);
	
	// Mirror on
	pinMode (PINOUT_MIRROR,OUTPUT);
	pullUpDnControl (PINOUT_MIRROR,PUD_DOWN) ;
	digitalWrite(PINOUT_MIRROR,LOW);
	//*/
	
	
}

//===============================================================================
//===============================================================================
void CloseSystems()
{
	ShutdownLights();
	
	slInputManager::Get().Shutdown();
	slInputManager::Destroy();
	
}

