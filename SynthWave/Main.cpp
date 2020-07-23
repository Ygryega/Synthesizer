#define FTYPE double
#include"Instruments.h"
#include<cstdlib>
#include<windows.h>
#include<Mmsystem.h>
#include<stdio.h>
#include "Common.h"

vector<wstring> devices = CSoundDevice<short>::Enumerate();

CSoundDevice<short> sound(devices[0], 44100, 1, 8, 512);


DWORD tempWord;
DWORD tempVel;

void CALLBACK midiCallback(HMIDIIN handle, UINT uMsg, DWORD dwInstance, DWORD dwParam1, DWORD dwParam2)
{

	wcout << "dwInstance is " << dwInstance << endl;
	wcout << "Handle is " << handle << endl;
	wcout << "Bytes of the midi message is " << dwParam1 << endl; //dwParam1 is the bytes of the MIDI Message packed into an unsigned long
	wcout << "Velocity is " << HIWORD(dwParam1) << endl; //velocity
	wcout << "Key ID is " << LOWORD(dwParam1) << endl; //keyID
	wcout << "Midi Data is " << HIBYTE(tempWord) << endl; //velocity
	wcout << "Key Status is " << LOBYTE(tempWord) << endl; //keyID
	wcout << "Time Stamp of keypress is " << dwParam2 << endl; //dwParam2 is the timestamp of key press
	wcout << "uMsg is " << uMsg << endl;
	wcout << "-----" << endl;

	double dTimeNow = sound.GetTime();
	tempWord = LOWORD(dwParam1);
	tempVel = HIWORD(dwParam1);


	DWORD newTemp = tempWord;
	//short nKeyState = LOWORD(dwParam1);

	muxNotes.lock();
	auto noteFound = find_if(vecNotes.begin(), vecNotes.end(), [&newTemp](synthesizer::SNote const& item) { return item.active == true; });
	//auto noteFound = vecNotes.begin();

	if (noteFound == vecNotes.begin())//begin
	{
		bool noteActive = false;
		// Note not found in vector
		for (auto it = vecNotes.begin(); it != vecNotes.end(); ++it)
		{
			if (it->id == tempWord)
			{
				noteActive = true;
			}
		}
		if (noteActive == false)
		{
			if (HIWORD(dwParam1) != 0)
			{
				//synthesizer::SKnob k;
				//if (tempWord == 432) { k.id = tempWord; k.active = true; vecKnobs.emplace_back(k); }
				//else if (tempWord == 688) { k.id = tempWord; k.active = true; vecKnobs.emplace_back(k); }
				//else if (tempWord == 944) { k.id = tempWord; k.active = true; vecKnobs.emplace_back(k); }
				//else if (tempWord == 1200) { k.id = tempWord; k.active = true; vecKnobs.emplace_back(k); }
				//else if (tempWord == 3090) { k.id = tempWord; k.active = true; vecKnobs.emplace_back(k); }
				//else if (tempWord == 1712) { k.id = tempWord; k.active = true; vecKnobs.emplace_back(k); }
				//else if (tempWord == 1968) { k.id = tempWord; k.active = true; vecKnobs.emplace_back(k); }
				//else if (tempWord == 2226) { k.id = tempWord; k.active = true; vecKnobs.emplace_back(k); }
				//// Key has been pressed so create a new note
				////double temp = double(dwParam1);
				//else
				//{
					synthesizer::SNote n;
					n.id = HIBYTE(tempWord);// LOWORD(dwParam1);
					n.on = dTimeNow;
					n.channel = 2;
					n.active = true;
					//n.volume = tempVel;
					// Add note to vector
					vecNotes.emplace_back(n);
				//}
			}
			else
			{
				// Note not in vector, but key has been released...
				// ...nothing to do
				//vecNotes.clear();
				tempWord = NULL;
			}
		}
	}
	else
	{
		// Note exists in vector
		if (HIWORD(dwParam1) != 0)
		{
			// Key is still held, so do nothing
			if (noteFound->off > noteFound->on)
			{
				// Key has been pressed again during release phase
				noteFound->on = dTimeNow;
				//noteFound->active = true;
			}
		}
		else
		{
			// Key has been released, so switch off
			if (noteFound->off < noteFound->on)
			{
				noteFound->off = dTimeNow;
				noteFound->active = false;
			}

		}
	}
	muxNotes.unlock();

}

int main()
{
	MIDIINCAPS      mic;
	MIDIOUTCAPS	outcaps;

	unsigned long result;
	HMIDIIN      inHandle;
	HMIDIOUT    outHandle;

	unsigned long    iNumDevs, i;
	iNumDevs = midiInGetNumDevs();  /* Get the number of MIDI In devices in this computer */
	int nMidiDevices = midiOutGetNumDevs();
	//wcout << "Out Devices : " << nMidiDevices << endl;
	for (int x = 0; x < nMidiDevices; x++)
	{
		/* Get info about the next device */
		if (!midiOutGetDevCaps(x, &outcaps, sizeof(MIDIINCAPS)))
		{
			wcout << "OutDevice ID [" << x << "]: " << outcaps.szPname << endl;
		}
	}



	//Current loop Used to determine at which point in the buffer we are 
	size_t Current_i = 0;

	Start();

	ImGui::CreateContext();
	ImGuiSDL::Initialize( renderer, WIN_WIDTH, WIN_HEIGHT);

	GenerateData();

	bool run = true;


	for (auto d : devices)
	{
		wcout << "Device found: " << d << endl;
	}
	cout << endl;

	// Create sound machine!!

	// Link noise function with sound machine
	sound.SetExternalFunction(MakeNoise);

	char keyboard[129];
	memset(keyboard, ' ', 127);
	keyboard[128] = '\0';

	auto clock_old_time = chrono::high_resolution_clock::now();
	auto clock_real_time = chrono::high_resolution_clock::now();
	double dElapsedTime = 0.0;

	cout << "Press R to Reset and change wave type" << endl;
	cout << "Press Q to Reset the Amplitude" << endl << endl;

	cout << "Press A to decrease aplitude" << endl;
	cout << "Press D to increase apliture" << endl;
	cout << "Press W to Increas HZ" << endl;
	cout << "Press S to Decrease HZ" << endl;
	int Channel = 1;
	
	if (midiOutOpen(&outHandle, 1, NULL, 0, NULL) == MMSYSERR_NOERROR)
	{
		std::cout << "Opened midi" << std::endl;
	}

	while (run)
	{
		for (int k = 0; k < 16; k++)
		{
			if (GetAsyncKeyState('1') & 0x8000)
			{
				Channel = 1;
			}
			else if (GetAsyncKeyState('2') & 0x8000)
			{
				Channel = 2;
			}
			else if (GetAsyncKeyState('3') & 0x8000)
			{
				Channel = 3;
			}
			else if (GetAsyncKeyState('4') & 0x8000)
			{
				Channel = 4;
			}
			if (GetAsyncKeyState('R') & 0x8000)
			{
				vecNotes.clear();

			}
			if (GetAsyncKeyState('Q') & 0x8000)
			{
				IncreaseAmplitude = 0.001;
				IncreaseHZ = 5.0f;
			}
			if (GetAsyncKeyState('W') & 0x8000)
			{
				IncreaseHZ += 0.01;
			}
			else if (GetAsyncKeyState('S') & 0x8000)
			{
				IncreaseHZ -= 0.01;
			}

			//short nKeyState = GetAsyncKeyState((unsigned char)("ZSXCFVGBNJMK\xbcL\xbe\xbf"[k]));
			double dTimeNow = sound.GetTime();
			// Check if note already exists in currently playing notes
			result = midiInOpen(&inHandle, 0, (DWORD)midiCallback, NULL, CALLBACK_FUNCTION);
			if (GetAsyncKeyState('P') & 0x8000)
			{
				midiOutShortMsg(outHandle, 0x00403C90);
			}
			else
			{
				//midiOutShortMsg(outHandle, 0x00003C80);
			}
			for (auto i = vecNotes.begin(); i != vecNotes.end(); ++i)
			{
				if (tempWord != 0)
				{
					if (tempWord == i->id)
					{
						if (i->off > i->on)
						{
							// Key has been pressed again during release phase
							i->on = dTimeNow;
							//matchFound = false;
							//noteFound->active = true;

							//i->volume = tempVel;
						}
					}
				}
				else
				{
					// Key has been released, so switch off
					if (i->off < i->on)
					{
						i->off = dTimeNow;
						//i->active = false;
					}
				}
			}
			//result = midiInOpen(&inHandle, 0, 0, 0, 0);
			midiInStart(inHandle);
		}

		wcout << "\rNotes: " << vecNotes.size() << "    ";
		wcout << "Amplitude: " << IncreaseAmplitude << "    ";
		wcout << "HZ: " << IncreaseHZ << "    ";

		//this_thread::sleep_for(5ms);

		ImGuiIO& io = ImGui::GetIO();

		int wheel = 0;

		SDL_Event e;
		while (SDL_PollEvent(&e))
		{
			if (e.type == SDL_QUIT) run = false;
			else if (e.type == SDL_WINDOWEVENT)
			{
				if (e.window.event == SDL_WINDOWEVENT_SIZE_CHANGED)
				{
					io.DisplaySize.x = static_cast<float>(e.window.data1);
					io.DisplaySize.y = static_cast<float>(e.window.data2);
				}
			}
			else if (e.type == SDL_MOUSEWHEEL)
			{
				wheel = e.wheel.y;
			}
			// Check keyboard escape key press
			if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE)
			{
				run = false;
			}
		}
	
		if (Current_i >= 512)
		{
			Current_i = 0;
		}

		constexpr float sampling_freq = 44100;
		const float t = Current_i / sampling_freq;
		x_data[Current_i] = t;
		wcout << dMixedOutput << endl;
		

		//setting the current point depending if is 0 or less
		if (dMixedOutput == 0)
		{
			y_data1[Current_i] = y_data1[Current_i - 1];
			Current_i++;
		}
		else
		{
			y_data1[Current_i] = dMixedOutput;
			Current_i++;
		}
		/*else if (dMixedOutput <= dMixedOutput + 10 && dMixedOutput >= dMixedOutput + 10)
		{
			y_data1[Current_i] = y_data1[Current_i - 1];
		}*/
		/*else if (dMixedOutput > y_data1[Current_i - 1] + 1.5 || dMixedOutput < y_data1[Current_i -1] - 1.5)
		{
			y_data1[Current_i] = y_data1[Current_i - 1];
		}*/
		
		//y_data2[Current_i] = dMixedOutput * 2;
		//y_data3[Current_i] = dMixedOutput / 2;

		int mouseX, mouseY;
		const int buttons = SDL_GetMouseState(&mouseX, &mouseY);

		// Setup low-level inputs (e.g. on Win32, GetKeyboardState(), 
		// or write to those fields from your Windows message loop handlers, etc.)
		io.DeltaTime = 1.0f / 60.0f;
		io.MousePos = ImVec2(static_cast<float>(mouseX), static_cast<float>(mouseY));
		io.MouseDown[0] = buttons & SDL_BUTTON(SDL_BUTTON_LEFT);
		io.MouseDown[1] = buttons & SDL_BUTTON(SDL_BUTTON_RIGHT);
		io.MouseWheel = static_cast<float>(wheel);

		ImGui::NewFrame();
			
		DrawPlotGraph();
		static bool checkBox = false;
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderClear(renderer);

		static float SlideAmplitude = IncreaseAmplitude;

		ImGui::SliderFloat("Amplitude", &SlideAmplitude, 0.001f, 1.0f);
		IncreaseAmplitude = SlideAmplitude;

		static float SliderHz = IncreaseHZ;
		ImGui::SliderFloat("Hz", &SliderHz, 5.0f, 500);
		IncreaseHZ = SliderHz;

		//Creating valuse to insert in to the drop down window 
		const char* wave_types[] = { "Square", "Triangle", "Piano", "Sine" };
		//where the chosen value is stored
		static const char* current_wave = NULL;

		//Creating the drop down window 
		if (ImGui::BeginCombo("Wave Type", current_wave))
		{
			//iterating trough all of the winodw choices
			for (int idx = 0; idx < IM_ARRAYSIZE(wave_types); ++idx)
			{
				//if its selected 
				bool is_selected = (current_wave == wave_types[idx]);
				if (ImGui::Selectable(wave_types[idx], is_selected))
				{
					current_wave = wave_types[idx];
					Channel = idx + 1;
				}
				if(is_selected)
				{
					ImGui::SetItemDefaultFocus();
				}
			}
			ImGui::EndCombo();
		}

		ImGui::Render();
		ImGuiSDL::Render(ImGui::GetDrawData());

		SDL_RenderPresent(renderer);
	}

	Exit();

	return 0;
}