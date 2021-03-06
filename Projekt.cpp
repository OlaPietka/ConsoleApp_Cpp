#define M_PI 3.14159265358979323846

#include <stdio.h>
#include <tchar.h>
#include <iostream>
#include <Windows.h>
#include <conio.h>
#include <cmath>
#include <math.h>

using namespace std;

struct program_params{ // Struktura o nazwie program_params zawierajaca typy danych o nazwach: shape_w, shape_h, shape_angle i display_char.
	int shape_w, shape_h, shape_angle; // shape_w = szerokosc figury, shape_h = wysokosc figury, shape_angle = kat pomiedzy ramieniem a ramą.
	char display_char; // shape_char = znak z ktorego zbudowana jest figura.
};

SIZE get_console_size() // Struktura typu SIZE o nazwie "get_console_size".
{
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
	int columns = csbi.srWindow.Right - csbi.srWindow.Left + 1; // Pobiera liczbe kolumn.
	int rows = csbi.srWindow.Bottom - csbi.srWindow.Top + 1; //Pobiera liczbe wierszy.

	return { columns, rows };
}

void set_cursor(short x, short y) // Funkcja o nazwie "set_cursor" ktora ustawia kursor w wybranym miejscu.  
{
	COORD cursor = { x, y }; // Struktura COORD określająca nową pozycję kursora.
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cursor);  // Funkcja o nazwie "SetConsoleCursorPosition", ustawia pozycję kursora dla określonej konsoli.
}

void point (char c, int x, int y) // Funkcja o nazwie "point" wypisuje podany znak o podanych koordynatach.
{
	SIZE size = get_console_size(); 
	if (x < 0 || y < 0 || x > size.cx - 1 || y > size.cy - 1 )  // Jesli wyswietlany znak wyjdzie poza linie, ma przestac rysowac.
		return;

	set_cursor(x, y);
	cout << c;
}

void line (char c, int x0, int y0, int x1, int y1) // Funkcja o nazwie "line" ktora zawiera algorytm bresenhama, algorytm tworzy linie zaczynajaca sie w punkcie (x0,y0) i konczy w punkcie (x1,y1). 
{
	int dx = abs(x1 - x0);
	int dy = -abs(y1 - y0);

	int mx = x0 < x1 ? 1 : -1; 
	int my = y0 < y1 ? 1 : -1; 

	int x = x0;
	int y = y0;

	int err = dx + dy;

	while(true) {
		point(c, x, y);

		if (x == x1 && y == y1) 
			break;

		int e2 = 2 * err;
		if (e2 >= dy) { 
			err += dy; 
			x += mx;
		}
		if (e2 <= dx) { 
			err += dx; 
			y += my; 
		}
	}
}

void draw (program_params config, int x, int y, int zoom) // Funkcja o nazwie "draw" ktora rysuje figure. 
{	
	int w = config.shape_w + zoom * 2; // w = szerokosc figury.
	int h = config.shape_h + zoom; // h = wysokosc figury.
	int a = h / cos(config.shape_angle * M_PI / 180); // a = dlugosc ramienia figury wiloczna z funkcji trygonometrycznych.
	int dy = h / 2; // dy = polowa wysokosci.
	int dx = (int)sqrt(a * a - dy * dy); // dx = odleglosc miedzy punktem "x" (srodek figury) a koncem figury.
	
	line(config.display_char, x, y, x + dx, y - dy); // Funkcja line rysujaca górne ramie figury.
	line(config.display_char, x, y, x + dx, y + dy); // Funkcja line rysujaca dolne ramie figury.
	line(config.display_char, x + dx, y - dy, x + dx - w, y - dy); // Funkcja line rysujaca górną ramę figury.
	line(config.display_char, x + dx, y + dy, x + dx - w, y + dy); // Funkcja line rysujaca dolną ramę figury.
}

void tutorial() // Funkcja o nazwie "tutorial" wypisujaca mozliwosci programu.
{

	do {
		system("cls");

		cout << "-------------OLA-PIETKA----------------" << endl;
		cout << "1. Program umozliwia wyswitlenie figury w kstalcie litery 'M'," << endl << "ulozonej z wybranego znaku." << endl;
		cout << "2. Figure mozna przesuwac za pomoca strzelk." << endl;
		cout << "3. Figure mozna zwiekszac i zmniejszac za pomoca klawiszy '+' lub '-'." << endl;
		cout << "4. Jezeli chcesz zakonczyc program, wcisnij 'ESC'" << endl;
		cout << "---------------------------------------" << endl;
		cout << "Po zapoznaniu sie z instrukcja, wcisnij ENTER..." << endl;
	} while (!cin.ignore()); // Pętla do while działa do momentu naciśniecia przycisku "ENTER".

	system("cls"); // Czysci ekran.
}

program_params load_config() // Funkcja o nazwie 'load_config' typu program_params pobierająca dane od użytkownika.
{
	int option;

	tutorial();

	program_params config = { 0 };
	cout << "Podaj znak z ktorego bedzie ulozona figura: ";
	config.display_char = _getch();
	cout << config.display_char << endl << endl;

	cout << "OPCJONALNIE: " << endl;
	cout << "Podaj kat nachyleina ramy do ramienia, wysokosc oraz szerokosc figury." << endl;
	cout << "Jezeli chcesz uzyc tej opcji, kliknij '1', jezeli nie, wpisz " << endl << "co kolwiek a zostana podane wartosci domyslne." << endl;
	cin >> option;

	if (option == 1)
	{
		do {
			cout << "Podaj kat nahylenia (miedzy 30 a 60): ";
			cin >> config.shape_angle;
		} while (config.shape_angle < 30 || config.shape_angle > 60);

		do {
			cout << endl << "Podaj wyskosc figury (miedzy 5 a 15): ";
			cin >> config.shape_h;
		} while (config.shape_h < 5 || config.shape_h > 15);

		do {
			cout << endl << "Podaj szerokosc figury (miedzy 15 a 30): ";
			cin >> config.shape_w;
		} while (config.shape_w < 15 || config.shape_w > 30);
	}
	else
	{
		config.shape_w = 15; 
		config.shape_h = 6;
		config.shape_angle = 8;
	}

	system("cls");
	return config;
}

bool shape_in_bounds(int x, int y, int zoom, SIZE size, program_params config)  // Funkcja o nazwie "shape_in_bounds" typu bool która zatrzymuje figure miedzy krawędziami konsoli.
{
	int w = config.shape_w + zoom * 2; // w = szerokosc figury.
	int h = config.shape_h + zoom; // h = wysokosc figury.
	int a = h / cos(config.shape_angle * M_PI / 180); // a = dlugosc ramienia figury wiloczna z funkcji trygonometrycznych.
	int dy = h / 2; // dy = polowa wysokosci.
	int dx = (int)sqrt(a * a - dy * dy); // dx = odleglosc miedzy punktem "x" (srodek figury) a koncem figury.

	if (!(x + dx < size.cx && x + dx - w >= 0)) // Ograniczenie okna na x.
		return false;
	if (!(y + dy < size.cy && y - dy >= 0)) // Ograniczenie okna na y.
		return false;

	return true;
}

void start (program_params config) 
{
	SIZE size = get_console_size();
	int x = size.cx / 2, y = size.cy / 2; // Ustawia kursor na srodku konsoli.
	int zoom = 0;
	char key;

	do {
		int tx = x; // Zmienne pomocnicze
		int ty = y;
		int tz = zoom;

		draw(config, x, y, zoom); 

		key = _getch();
		if (key == -32) {
			key = _getch();
			if (key == 42)
				key = _getch();


			if (key == 80) ty++; // Nacisniecie strzalki w dół.
			else if (key == 72) ty--; // Nacisniecie strzalki w góre.
			else if (key == 77) tx++; // Nacisniecie strzalki w prawo.
			else if (key == 75) tx--; // Nacisniecie strzalki w lewo.
		}

		system("cls"); // Czysci ekran.
		
		if (key == '+') tz++; // Powiekszanie figury.
		else if (key == '-') tz--; // Zmniejszanie figury.

		if (tz < 4 - config.shape_h) // Ograniczenie dla zmniejszania figury.
		{
			tz++;
		}

		if (shape_in_bounds(tx, ty, tz, size, config)) 
		{
			x = tx;
			y = ty;
			zoom = tz;
		}
	} while (key != 27); // Pętla działa dopóki nie zostanie nacisniety klawisz ESC.
}

int main()
{
	program_params config = load_config();
	start(config);
}