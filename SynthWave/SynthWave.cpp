//// SynthWave.cpp : This file contains the 'main' function. Program execution begins and ends there.
////
//#define FTYPE double
//#include"Instruments.h"
////using namespace synthesizer;
//
//int main()
//{
//	vector<wstring> devices = CSoundDevice<short>::Enumerate();
//
//	for (auto d : devices)
//	{
//		wcout << "Device found: " << d << endl;
//	}
//	cout << endl;
//
//	// Create sound machine!!
//	CSoundDevice<short> sound(devices[0], 44100, 1, 8, 512);
//
//	// Link noise function with sound machine
//	sound.SetExternalFunction(MakeNoise);
//
//	char keyboard[129];
//	memset(keyboard, ' ', 127);
//	keyboard[128] = '\0';
//
//	auto clock_old_time = chrono::high_resolution_clock::now();
//	auto clock_real_time = chrono::high_resolution_clock::now();
//	double dElapsedTime = 0.0;
//
//	cout << "Press R to Reset and change wave type" << endl;
//	cout << "Press Q to Reset the Amplitude" << endl << endl;
//
//	cout << "Press A to decrease aplitude" << endl;
//	cout << "Press D to increase apliture" << endl;
//	cout << "Press W to Increas HZ" << endl;
//	cout << "Press S to Decrease HZ" << endl;
//	int Channel = 1;
//
//	while (1)
//	{
//		for (int k = 0; k < 16; k++)
//		{
//			if (GetAsyncKeyState('1') & 0x8000)
//			{
//				Channel = 1;
//			}
//			else if (GetAsyncKeyState('2') & 0x8000)
//			{
//				Channel = 2;
//			}
//			else if (GetAsyncKeyState('3') & 0x8000)
//			{
//				Channel = 3;
//			}
//			else if (GetAsyncKeyState('4') & 0x8000)
//			{
//				Channel = 4;
//			}
//			if (GetAsyncKeyState('R') & 0x8000)
//			{
//				vecNotes.clear();
//			
//			}
//			if (GetAsyncKeyState('Q') & 0x8000)
//			{
//				IncreaseAmplitude = 0.001;
//				IncreaseHZ = 5.0f;
//			}
//			if (GetAsyncKeyState('W') & 0x8000)
//			{
//				IncreaseHZ += 0.01;
//			}
//			else if (GetAsyncKeyState('S') & 0x8000)
//			{
//				IncreaseHZ -= 0.01;
//			}
//
//			short nKeyState = GetAsyncKeyState((unsigned char)("ZSXCFVGBNJMK\xbcL\xbe\xbf"[k]));
//
//			double dTimeNow = sound.GetTime();
//
//			// Check if note already exists in currently playing notes
//			muxNotes.lock();
//			auto noteFound = find_if(vecNotes.begin(), vecNotes.end(), [&k](synthesizer::SNote const& item) { return item.id == k; });
//			if (noteFound == vecNotes.end())
//			{
//				// Note not found in vector
//
//				if (nKeyState & 0x8000)
//				{
//					// Key has been pressed so create a new note
//					synthesizer::SNote n;
//					n.id = k;
//					n.on = dTimeNow;
//					n.channel = Channel;
//					n.active = true;
//					
//					// Add note to vector
//					vecNotes.emplace_back(n);
//				}
//				else
//				{
//					// Note not in vector, but key has been released...
//					// ...nothing to do
//				}
//			}
//			else
//			{
//				// Note exists in vector
//				if (nKeyState & 0x8000)
//				{
//					// Key is still held, so do nothing
//					if (noteFound->off > noteFound->on)
//					{
//						// Key has been pressed again during release phase
//						noteFound->on = dTimeNow;
//						noteFound->active = true;
//					}
//				}
//				else
//				{
//					// Key has been released, so switch off
//					if (noteFound->off < noteFound->on)
//					{
//						noteFound->off = dTimeNow;
//					}
//				}
//			}
//			muxNotes.unlock();
//		}
//		
//		wcout << "\rNotes: " << vecNotes.size() << "    ";
//		wcout << "Amplitude: " << IncreaseAmplitude << "    ";
//		wcout << "HZ: " << IncreaseHZ << "    ";
//	
//		//this_thread::sleep_for(5ms);
//	}
//
//	return 0;
//}