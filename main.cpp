#include "grid.hpp"

#include <iostream>

#include <cstdint>
#include <cstdlib>
#include <ctime>

#include "windows.h"

BOOL WINAPI consoleHandler(DWORD signal) {
    if (signal == CTRL_C_EVENT)
        ExitProcess(0);

    return TRUE;
}

int main() {
	// Inicjalizacja
	SetConsoleOutputCP(CP_UTF8);
	SetConsoleCtrlHandler(consoleHandler, TRUE);
	srand(time(NULL));

	// Zapytaj użytkownika o rozmiar planszy
	uint16_t w, h;
	do {
		std::cout << "Ile kolumn? ";
		std::cin >> w;
	} while (w < 1);
	do {
		std::cout << "Ile wierszy? ";
		std::cin >> h;
	} while (h < 1);
	int bombPercent;
	do {
		std::cout << "Ile % bomb (0-100)? ";
		std::cin >> bombPercent;
	} while (h < 1);

	// Stwórz planszę i ją wypełnij
	Grid grid{(uint8_t)w, (uint8_t)h};
	grid.FillRandom(bombPercent);
	
	// Główna logika gry
	bool playing = true;
	while (true) {
		std::cout << grid << std::endl;
		
		char choice;
		do {
			std::cout << "Flaga (F) czy odkrycie (O)? ";
			std::cin >> choice;
		} while (choice != 'F' && choice != 'O' && choice != 'f' && choice != 'o');
		
		if (choice == 'O' || choice == 'o') {
			for (;;) {
				int x, y;
				do {
					std::cout << "Gdzie odkryć (kolumna i wiersz, np. 8 3)? ";
					std::cin >> x >> y;
				} while (x <= 0 || x > w || y <= 0 || y > h);
				
				auto &cell = grid.At(x - 1, y - 1);
				if (cell.IsVisible()) {
					std::cout << "Ta komórka jest już odkryta!" << std::endl;
				
				} else if (cell.HasFlag()) {
					std::cout << "Ta komórka ma ustawioną flagę!" << std::endl;
				
				} else if (cell.GetValue() == GridCell::BOMB) {
					grid.UncoverAll();
					std::cout << grid << std::endl;
					std::cout << "!!! PRZEGRANA !!!" << std::endl;
					playing = false;
					break;
				
				} else {
					grid.FloodFillUncover(x - 1, y - 1);
					break;
				}
			}
		
		} else if (choice == 'F' || choice == 'f') {
			int x, y;
			do {
				std::cout << "Gdzie zmienić flagę (kolumna i wiersz, np. 8 3)? ";
				std::cin >> x >> y;
			} while (x <= 0 || x > w || y <= 0 || y > h);
			
			auto &cell = grid.At(x - 1, y - 1);
			std::cout << cell.HasFlag() << std::endl;
			cell.SetFlag(!cell.HasFlag());
		}
		
		if (!playing) break;
		
		if (grid.IsAllSafe()) {
			grid.UncoverAll();
			std::cout << grid << std::endl;
			std::cout << "!!! WYGRANA !!!" << std::endl;
			break;
		}
	};
	Sleep(2000);
	
	return 0;
}