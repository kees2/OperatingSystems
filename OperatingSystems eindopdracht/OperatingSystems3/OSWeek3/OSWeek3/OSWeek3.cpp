#include "stdafx.h"
#define _CRT_RAND_S
#include <stdlib.h>
#include <windows.h>
#include <stdio.h>
#include <tchar.h>
#include <time.h>
#include <iostream>
using namespace std;

// Parameters die je kunt veranderen om queue te testen
// Als alles lijkt te werken, moet je NBOXES maar eens op 1000 zetten en TICK op 10
const int TICK = 1000;  // de basis-tijdeenheid in miliseconden
const int NBOXES = 1000; // aantal dozen dat een producent maakt en een consument gebruikt
const int BUFLEN = 4;   // de lengte van de queue

						// Heb mijn eigen random() geschreven
						// De standaard rand() geeft voor alle threads dezelfde reeks!
int random(int n)
{
	unsigned int t;
	rand_s(&t);
	return t % n;
}

// Eenvoudig doosje. Niks meer aan doen.
// Bevat alleen de naam van de producent en een volgnummer
class Box
{
private:
	const char *name;
	int number;
public:
	Box(const char *name, int number)
	{
		this->name = name;
		this->number = number;
	}
	// Operator overloading voor de fijnprovers
	friend ostream &operator << (ostream &out, Box *b)
	{
		return out << b->name << "-" << b->number;
	}
};

// De ronde wachtrij
// Die moet je dus thread-safe maken!
class Queue
{
private:
	Box * buffer[BUFLEN];

	// Twee indexen en de lengte bijhouden.
	// Redundant, maar lekker makkelijk!
	int getpos, putpos;
	int count;

	// Deze sync-dingen moet je gaan gebruiken in get() en put()
	CRITICAL_SECTION busy;
	HANDLE CanPut, CanGet;

public:
	Queue()
	{
		// Hier worden ze goed geinitialiseerd
		InitializeCriticalSection(&busy);
		CanGet = CreateEvent(0, 0, 0, 0);
		CanPut = CreateEvent(0, 0, 0, 0);

		getpos = 0;
		putpos = 0;
		count = 0;
		for (int i = 0; i < BUFLEN; i++) buffer[i] = 0;
	}

	// DEZE METHODE MOET BEVEILIGD WORDEN
	Box *get(const char *consumername)
	{
		EnterCriticalSection(&busy);

		while (count < 1) {
			LeaveCriticalSection(&busy);
			WaitForSingleObject(CanGet, INFINITE);
			EnterCriticalSection(&busy);
		}
			
		

		Box *box = buffer[getpos];
		getpos = (getpos + 1) % BUFLEN;
		count--;
		cout << consumername << ": gets " << box << endl;
		LeaveCriticalSection(&busy);
		ResetEvent(CanGet);
		SetEvent(CanPut);
		return box;
	}

	// DEZE METHODE MOET BEVEILIGD WORDEN
	void put(const char *producername, Box *box)
	{
		EnterCriticalSection(&busy);
		while (count >= BUFLEN) {
			LeaveCriticalSection(&busy);
			WaitForSingleObject(CanPut, INFINITE);
			EnterCriticalSection(&busy);
		}
		
		
		cout << producername << ": puts " << box << endl;
		buffer[putpos] = box;
		putpos = (putpos + 1) % BUFLEN;
		count++;
		LeaveCriticalSection(&busy);
		ResetEvent(CanPut);
		SetEvent(CanGet);
	}
};

// Fijne globale variabele: de wachtrij
Queue q;

// De code voor de productie-thread
// Hoef je niks aan te doen
DWORD WINAPI produce(void *arg)
{
	char *name = (char*)arg;
	for (int i = 0; i < NBOXES; i++)
	{
		Sleep(random(TICK));
		Box *box = new Box(name, i);
		q.put(name, box);
	}
	return 0;
}

// De code voor de consumptie-thread
// Hoef je niks aan te doen
DWORD WINAPI consume(void *arg)
{
	char *name = (char*)arg;
	for (int i = 0; i < NBOXES; i++)
	{
		Sleep(random(TICK));
		Box *box = q.get(name);
		delete box;
	}
	return 0;
}

// Main is ook al klaar.
// Loopt bijna zeker vast als je de queue niet verbeterd.
int _tmain(int argc, _TCHAR* argv[])
{
	CreateThread(0, 0, produce, LPVOID("P1"), 0, 0);
	CreateThread(0, 0, produce, LPVOID("P2"), 0, 0);
	CreateThread(0, 0, produce, LPVOID("P3"), 0, 0);
	CreateThread(0, 0, produce, LPVOID("P4"), 0, 0);

	CreateThread(0, 0, consume, LPVOID("C1"), 0, 0);
	CreateThread(0, 0, consume, LPVOID("C2"), 0, 0);
	CreateThread(0, 0, consume, LPVOID("C3"), 0, 0);
	CreateThread(0, 0, consume, LPVOID("C4"), 0, 0);

	// Druk op een toets om af te breken...
	cin.get();
	return 0;
}