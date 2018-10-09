#include <iostream>
#include <time.h>
#include <cstdlib>
#include <fstream>
#include <stdlib.h>
#include <sstream>
using namespace std;
//////////////////////////////
const int ILOSC_KOSTKI = 2;
const int TABLICA_WYS = 20;
const int TABLICA_SZER = 6*2*2;
const int BANDA_WYS = 20;
const int BANDA_SZER = 2;
const int KOLUMNA_SZER = 2;
const int KOLUMNA_WYS = TABLICA_WYS / 2;
const char RANKING[] = "ranking.txt";
////////////////////////////
struct gracz{
	string name;
	int win = 0, los = 0;
};
void LOG(const char* log){
	cout << log << endl;
}
void playerLog(const bool player){
	if (!player) LOG("Tura gracza 1 = x");
	else LOG("Tura gracz 2 = o");
	LOG("Liczba to pole ruchu, 0 to poddanie, -1 zapis i wyjscie, -2 w razie braku ruchu");
}
void LOG(const int log){
	cout << log << endl;
}
void renderDziura(int oczko){
	cout << "|";
	if (oczko == 0) cout << " ";
	else if (oczko == 1) cout << "x";
	else if (oczko == 2) cout << "o";
	cout << "|";
}
void renderLinia(int start,int inkrementacja,int ile){
	int max = ile;
	while (ile){
		cout << start<<" ";
		if (ile == (max / 2) + 1) {
			if (abs(start < 10)) cout << " ";
			cout << "|  |";
		}
		if (abs(start < 10)) cout << " ";
		start += inkrementacja;
		ile--;
	}
	cout << endl;
}
char czytajZnak(int a){
	if (a == 0) return ' ';
	else if (a == 1) return 'x';
	else if (a == 2) return 'o';

}
void renderTablica(int tab[][TABLICA_SZER],int banda[][BANDA_SZER]){
	for (int i = 0; i < TABLICA_WYS; i++){
		for (int j = 0; j < TABLICA_SZER; j++){
			cout << czytajZnak(tab[i][j]);
			if (j % 2 && j>0) cout << " ";
			if (j == (TABLICA_SZER / 2)-1){
				int k = 0;
				cout << "|";
				while (k<2){
					cout << czytajZnak(banda[i][k]);
					k++;
				}
				cout << "|";
			}
		}
		cout << endl;
	}
}
void zapiszGre(gracz Gracze[], int Tab[][TABLICA_SZER], bool Tura, int Ban[][BANDA_SZER],int kostki[]){
	string nazwa;
	LOG("Podaj nazwe pliku");
	cin >> nazwa;
	fstream plik(nazwa, ios::out);
	if (!plik.is_open()){
		LOG("Nie udalo sie otworzyc pliku");
		return;
	}
	plik << Gracze[0].name << '\t' << Gracze[1].name << '\t' << Tura << '\t' << kostki[0] << '\t'<< kostki[1] << '\n';
	for (int i = 0; i < TABLICA_WYS; i++){
		for (int j = 0; j < TABLICA_SZER; j++){
			plik << Tab[i][j]<<' ';
		}
		plik << '\n';
	}
	plik << '\n';
	for (int i = 0; i < BANDA_WYS; i++){
		for (int j = 0; j < BANDA_SZER; j++){
			plik << Ban[i][j]<< ' ';
		}
		plik << '\n';
	}
	plik.close();
}
void wypiszKostki(int kostki[]){
	cout << "Kostki gracza to " << kostki[0] << " i " << kostki[1]<<endl;
}
void render(int tab[][TABLICA_SZER],int banda[][BANDA_SZER],bool Gracz,int kostki[]){
	playerLog(Gracz);
	wypiszKostki(kostki);
	renderLinia(13, 1, 12);
	renderTablica(tab,banda);
	renderLinia(12, -1, 12);
}
int losujKostke(int nr){
	LOG("Nacisni przycisk zeby rzucic kostka");
	getchar();
	int kostka = (rand() % 5) + 1;
	LOG("Wylosowano: ");
	cout << kostka<<endl;
	getchar();
	return kostka;
}
void losowanieTury(bool &Gracz, int kostki[]){
	while (kostki[0] == kostki[1]){
		system("CLS");
		LOG("Gracz pierwszy rzuca koscmi");
			kostki[0] = losujKostke(1);
	
		LOG("Gracz drugi rzuca koscmi");
			kostki[1] = losujKostke(2);
		
		if (kostki[0] > kostki[1]){
			Gracz = false;
			LOG("zaczyna gracz numer 1");
			getchar();
			system("CLS");
			return;
		}
		else if (kostki[0] < kostki[1]){
			Gracz = true;
			LOG("zaczyna gracz numer 2");
			getchar();
			system("CLS");
			return;
		}
	}
}
void intro(){
	LOG("WITAMY W GRZE 'TRYKTRAK' ");
	LOG("NACISNIJ PRZYCISK ZEBY ZACZAC ROZGRYWKE");
	getchar();
}
//sprawdzamy czy pole jest zajete przez 1 wroga czy kilku jesli tylko 1 to zwracamy true
bool sprawdzIlosc(int tab[][TABLICA_SZER],int x,int y,int strona){
	bool jeden = true;
	if (strona == 1){
		if (tab[y + 1][x] > 0) return !jeden;
	}
	else{
		if (tab[y -1][x] > 0) return !jeden;
	}
	return jeden;
}
// podaje koordynaty startowe tablicy podanej kolumny
void podajKoordynaty(int &x, int &y, int &strona,int cel){
	if (cel < 13 && cel >0){
		y = TABLICA_WYS - 1;
		x = (abs(cel - 12) * 2);
		strona = -1;
	}
	else if (cel >= 13 && cel <= 24){
		y = 0;
		x = (cel - 13) * 2;
		strona = 1;
	}
}
// 1 = x
// 2 = 0
//zwracamy 1 gdy idziemy w pole przez nas zajete i mozemy ruszyc
//zwracamy 0 gdy wybrane pole jest puste
//zwracamy -1 gdy mozemy zbic pjon
//zwracamy -2 gdy pole jest zajete przez wroga
int sprawdzTeren(const int cel, const bool player,int tab[][TABLICA_SZER]){
	int x, y;
	int strona; //1 jesli gora -1 jesli dol
	podajKoordynaty(x, y, strona,cel);
	if (tab[y][x] == 0) return 0;
	for (int i = x,l = 0; l < KOLUMNA_SZER; i++,l++){
		for (int j = y, k = 0; k < KOLUMNA_WYS; j+=strona,k++){
			if (player == false && tab[j][i] == 1) return 1;
			else if (player == true && tab[j][i] == 2) return 1;

			else if (player == false && tab[j][i] == 2){
				if (sprawdzIlosc(tab, i, j, strona)) return -1;
				else return -2;
			}
			else if (player == true && tab[j][i] == 1){
				if (sprawdzIlosc(tab, i, j, strona)) return -1;
				else return -2;
			}
		}
	}
	return 0;
}
int ruchDwa(int cel,int zrodlo, int kostki[],int stronaZ, int stronaC){
	if (cel - zrodlo == kostki[0]){
		kostki[0] = 0;
		return 1;
	}
	else if ( cel - zrodlo == kostki[1]){
		kostki[1] = 0;
		return 1;
	}
	else if (cel - zrodlo == (kostki[1] + kostki[0])){
		kostki[1] = 0;
		kostki[0] = 0;
		return 1;
	}
	else if (stronaC != stronaZ){
		if (cel - zrodlo == kostki[1]){
			kostki[1] = 0;
			return 1;
		}
		else if (cel - zrodlo == kostki[0]){
			kostki[0] = 0;
			return 1;
		}
		else if (cel - zrodlo == (kostki[0] + kostki[1])){
			kostki[0] = 0;
			kostki[1] = 0;
			return 1;
		}
	}
	return 0;
}
int ruchJeden(int cel, int zrodlo, int kostki[], int stronaZ, int stronaC){
	if (cel - zrodlo == -kostki[0]){
		kostki[0] = 0;
		return 1;
	}
	else if ( cel - zrodlo == -kostki[1]){
		kostki[1] = 0;
		return 1;
	}
	else if (cel - zrodlo == -(kostki[0] + kostki[1])){
		kostki[0] = 0;
		kostki[1] = 0;
		return 1;
	}
	else if (stronaZ == 1 && cel - zrodlo == -(kostki[0] + kostki[1])){
		kostki[0] = 0;
		kostki[1] = 0;
		return 1;
	}
	else if (stronaC != stronaZ){
		if (cel - zrodlo == -kostki[1]){
			kostki[1] = 0;
			return 1;
		}
		else if (cel - zrodlo == -kostki[0]){
			kostki[0] = 0;
			return 1;
		}
		else if (cel - zrodlo == -(kostki[0] + kostki[1])){
			kostki[0] = 0;
			kostki[1] = 0;
			return 1;
		}
	}
	return 0;
}
//int stronaC = 1 ? -1 : cel > 12;
//sprawdzamy czy oczka pozwalaja na ruch
//1 jesli mozemy ruszyc i 0 jesli nie e
int sprawdzRuch(int cel, int zrodlo, int kostki[],bool player){
	int stronaC = cel > 12 ? 1 : -1;
	int stronaZ = cel > 12 ? 1 : -1;
	if (player == false){
		if (ruchJeden(cel, zrodlo, kostki, stronaZ, stronaC) == 1) return 1;
	}
	///
	else {
		if (ruchDwa(cel, zrodlo, kostki, stronaZ, stronaC) == 1) return 1;
	}
	return 0;
}
void fillLine(const int x, const int ile, int rodzaj, int tab[][TABLICA_SZER]){
	int temp = ile;
	int y = 0;
	while (temp){
		tab[y][x] = rodzaj;
		temp--;
		y++;
	}
	y = TABLICA_WYS-(ile);
	temp = ile;
	if (rodzaj == 1)rodzaj = 2;
	else rodzaj = 1;
	while (temp){
		tab[y][x] = rodzaj;
		temp--;
		y++;
	}
}
void zeruj_T(int tab[][TABLICA_SZER]){
	for (int i = 0; i < TABLICA_WYS; i++){
		for (int j = 0; j < TABLICA_SZER;j++){
			tab[i][j] = 0;
		}
	}
}
void zeruj_B(int tab[][BANDA_SZER]){
	for (int i = 0; i < BANDA_WYS; i++){
		for (int j = 0; j < BANDA_SZER; j++){
			tab[i][j] = 0;
		}
	}
}

void inicjalizujTab(int tab[][TABLICA_SZER]){
	zeruj_T(tab);
	fillLine(0, 5, 2, tab);
	fillLine(4*2, 3, 1, tab);
	//fillLine(6*2, 5, 1, tab);
	//fillLine(11*2, 2, 2, tab);
}
void swap(int celX, int celY, int zrodloX, int zrodloY, int tab[][TABLICA_SZER]){
	int temp = tab[celY][celX];
	tab[celY][celX] = tab[zrodloY][zrodloX];
	tab[zrodloY][zrodloX] = temp;
}
void gdziePrzeniesc(int cel, int &x, int &y, int tab[][TABLICA_SZER],int strona){
	for (int i = x, l = 0; l < KOLUMNA_SZER; i++, l++){
		for (int j = y, k = 0; k < KOLUMNA_WYS; j += strona, k++){
			if (tab[j][i] == 0) {
				x = i;
				y = j;
				return;
			}
		}
	}
}
bool kogoPrzeniesc(int zrodlo, int &x, int &y, int tab[][TABLICA_SZER],bool player){
	int strona = 0;
	podajKoordynaty(x, y, strona, zrodlo);
	for (int i = x, l = 0; l < KOLUMNA_SZER; i++, l++){
		for (int j = y, k = 0; k < KOLUMNA_WYS; j += strona, k++){
			if (strona == 1 && k < KOLUMNA_WYS - 1 && tab[j + 1][i] == 0 && tab[j][i] == 1 && player == false) {
				x = i; 
				y = j;
				return true;
			}
			else if (strona == 1 && k < KOLUMNA_WYS - 1 && tab[j + 1][i] == 0 && tab[j][i] == 2 && player == true) {
				x = i;
				y = j;
				return true;
			}
			else if (strona == -1 && tab[j - 1][i] == 0 && tab[j][i] == 1 && player == false) {
				x = i;
				y = j;
				return true;
			}
			else if (strona == -1 && tab[j - 1][i] == 0 && tab[j][i] == 2 && player == true) {
				x = i;
				y = j;
				return true;
			}
			else if (k == KOLUMNA_WYS - 1 && l<TABLICA_SZER - 1 && tab[j][i] == 1 && player == false && tab[y][x + 1] == 0){
				x = i;
				y = j;
				return true;
			}
			else if (k == KOLUMNA_WYS - 1 && l<TABLICA_SZER - 1 && tab[j][i] == 2 && player == true && tab[y][x + 1] == 0){
				x = i;
				y = j;
				return true;
			}
		}
	}
	return false;
}
void przenies(int xCel, int yCel,int xzrodlo,int yzrodlo,int tab[][TABLICA_SZER]){
	int temp = tab[yCel][xCel];
	tab[yCel][xCel] = tab[yzrodlo][xzrodlo];
	tab[yzrodlo][xzrodlo] = temp;
}
void usunPion(bool player, int ban[][BANDA_SZER]){
	int pjon = player ? 1 : 2;
	for (int i = 0; i < BANDA_WYS; i++){
		for (int j = 0; j < BANDA_SZER; j++){
			if (ban[i][j] == 0) {
				ban[i][j] = pjon;
				return;
			}
		}
	}
}
void zbijaj(int tab[][TABLICA_SZER], int zrodloX, int zrodloY, int celX, int celY, bool player, int ban[][BANDA_SZER]){
	tab[celY][celX] = tab[zrodloY][zrodloX];
	tab[zrodloY][zrodloX] = 0;
	usunPion(player, ban);
}
//zwraca 0 jak po prostu wykona ruch -1 jesli zbijemy pionka x -2 jak pjonka o, 1 jesli nie ma kim ruszyc
int przeniesPion(int cel, int tab[][TABLICA_SZER], int zrodlo, bool player, int ban[][BANDA_SZER]){
	int x, y;
	int zrodloX, zrodloY;
	int strona;
	podajKoordynaty(x, y, strona, cel);
	kogoPrzeniesc(zrodlo, zrodloX, zrodloY, tab,player);
	int wybor = sprawdzTeren(cel,player,tab);
	if (wybor == 0){
		przenies(x, y, zrodloX, zrodloY, tab);
		return 0;
	}
	else if (wybor == 1){
		gdziePrzeniesc(cel, x, y, tab, strona);
		przenies(x, y, zrodloX, zrodloY, tab);
		return 0;
	}
	else if (wybor == -1){
		zbijaj(tab, zrodloX, zrodloY, x, y, player, ban);
	}
	return 1;
}
bool gameWin(bool Gracz, int Tab[][TABLICA_SZER], const int ile){
	int k = 0;
	if (Gracz == false){
		for (int i = TABLICA_WYS / 2; i < TABLICA_WYS; i++){
			for (int j = TABLICA_SZER / 2; j < TABLICA_SZER; j++){
				if (Tab[i][j] == 1) k++;
			}
		}
	}
	else{
		for (int i = 0; i < TABLICA_WYS / 2; i++){
			for (int j = TABLICA_SZER / 2; j < TABLICA_SZER; j++){
				if (Tab[i][j] == 2) k++;
			}
		}
	}
	if (k == ile) return true;
	else return false;
}
//sprawdz teren
//zwracamy 1 gdy idziemy w pole przez nas zajete i mozemy ruszyc
//zwracamy 0 gdy wybrane pole jest puste
//zwracamy -1 gdy mozemy zbic pjon
//zwracamy -2 gdy pole jest zajete przez wroga
//PRZENIES PJON
//zwraca 0 jak po prostu wykona ruch -1 jesli zbijemy pionka x -2 jak pjonka o, 1 jesli nie ma kim ruszyc
void inputSelect(bool &Player, int Kostki[], bool &Gra, bool &Podwojny, int IlePionkow,
	int Ban[][BANDA_SZER], int Tab[][TABLICA_SZER],int temp[],gracz Gracze[],bool &zapisane,bool &skip){
	int Zrodlo, Cel;
	int Xzrodlo, Yzrodlo;
	bool Restart = false;
	while (Kostki[0] + Kostki[1] > 0){
		while (1){
			LOG("skad chcesz wykonac ruch?");
			cin >> Zrodlo;
			if (Zrodlo == 0){
				Gra = false;
				return;
			}
			else if (Zrodlo == -1){
				Gra = false;
				zapiszGre(Gracze, Tab, Player, Ban,Kostki);
				zapisane = true;
				return;
			}
			else if (Zrodlo == -2){
				Player = !Player;
				skip = true;
				return;
			}
			if (kogoPrzeniesc(Zrodlo, Xzrodlo, Yzrodlo, Tab, Player) == true) break;
		}
		while (1){
			LOG("do kod chcesz wykonac ruch?");
			cin >> Cel;
			if (Cel == Zrodlo){
				Restart = true;
				break;
			}
			if ((sprawdzTeren(Cel, Player, Tab) > -2 && sprawdzRuch(Cel, Zrodlo, Kostki, Player) == 1)) break;
		}
		if (Restart == true){
			Restart = false;
			continue;
		}
		system("CLS");
		przeniesPion(Cel, Tab, Zrodlo, Player, Ban);
		render(Tab, Ban, Player, Kostki);
		if (Podwojny == true && Kostki[0] + Kostki[1] == 0){
			Kostki[0] = temp[0];
			Kostki[1] = temp[1];
			Podwojny = false;
			system("CLS");
			render(Tab, Ban, Player, Kostki);
		}
		if (gameWin(Player, Tab, IlePionkow)){
			Gra = false;
			Player = !Player;
			return;
		}
	}
}
void input(int Tab[][TABLICA_SZER], bool &Player, int Kostki[], bool &Start,
	int Ban[][BANDA_SZER], bool &Gra,int IlePionkow,gracz Gracze[],bool &zapisane,bool &skip){
	bool podwojny = false;
	int temp[ILOSC_KOSTKI];
	if (Start == false && Kostki[0] + Kostki[1] == 0 || skip == true){
		for (int i = 0; i < ILOSC_KOSTKI; i++){
			Kostki[i] = losujKostke(int(Player) + 1);
		}
		if (skip == true) skip = false;
	}
	if (Kostki[0] == Kostki[1]){
		podwojny = true;
		temp[0] = Kostki[0];
		temp[1] = Kostki[1];
	}
	if (Start == true) Start = false;
	render(Tab, Ban, Player, Kostki);
	LOG("Wykonaj ruch");
	inputSelect(Player, Kostki, Gra, podwojny, IlePionkow, Ban, Tab, temp,Gracze,zapisane,skip);
	if (Gra == false)return;
	if (Kostki[0] + Kostki[1] == 0) Player = !Player;
	system("cls");
}
bool bandaCheck(const bool Player, int ban[][BANDA_SZER]){
	for (int i = 0; i < BANDA_WYS; i++){
		for (int j = 0; j < BANDA_SZER; j++){
			if (ban[i][j] == 1 && Player == false) return true;
			else if (ban[i][j] == 2 && Player == true) return true;
		}
	}
	return false;
}
int przeniesBanda(int cel, int tab[][TABLICA_SZER], bool Player, int ban[][BANDA_SZER]){
	int x, y;
	int strona;
	podajKoordynaty(x, y, strona, cel);

	for (int i = 0; i < BANDA_WYS; i++){
		for (int j = 0; j < BANDA_SZER; j++){
			if (ban[i][j] == 1 && Player == false)  {
				ban[i][j] = 0;
				break;
			}
			else if (ban[i][j] == 2 && Player == true) {
				ban[i][j] = 0;
				break;
			}
		}
	}
	int wybor = sprawdzTeren(cel, Player, tab);
	if (wybor == 0){
		tab[y][x] = int(Player) + 1;
		return 0;
	}
	else if (wybor == 1){
		gdziePrzeniesc(cel, x, y, tab, strona);
		tab[y][x] = int(Player) + 1;
		return 0;
	}
	else if (wybor == -1){
		tab[y][x] = int(Player) + 1;
		usunPion(Player, ban);
	}
	return 1;
}
int ilePionkow(int Tab[][TABLICA_SZER],int Ban[][BANDA_SZER]){
	int ile = 0;
	for (int i = 0; i < TABLICA_WYS; i++){
		for (int j = 0; j < TABLICA_SZER; j++){
			if (Tab[i][j] == 1) ile++;
		}
	}
	for (int i = 0; i < BANDA_WYS; i++){
		for (int j = 0; j < BANDA_SZER; j++){
			if (Ban[i][j] == 1) ile++;
		}
	}
	return ile;
}
//////////////
void inputBanda(bool &Player, int Ban[][BANDA_SZER], int Tab[][TABLICA_SZER],int Kostki[],bool &Gra,bool Start,gracz Gracze[]){
	int Cel;
	int Xzrodlo, Yzrodlo;
	bool Restart = false;
	if (Start == false){
		for (int i = 0; i < ILOSC_KOSTKI; i++){
			Kostki[i] = losujKostke(int(Player) + 1);
		}
	}
	render(Tab, Ban, Player, Kostki);
	LOG("Wyjdz z bandy(-1 jesli nei mozesz wykoanc ruchu)");
	while (bandaCheck(Player, Ban) && Kostki[0] + Kostki[1] >0){
		while (1){
			LOG("do kod chcesz wykonac ruch?");
			cin >> Cel;
			if (Cel == 0){
				Gra = false;
				return;
			}
			else if (Cel == -1){
				Gra = false;
				zapiszGre(Gracze, Tab, Player, Ban,Kostki);
				return;
			}
			else if (Cel == -2){
				Player = !Player;
				return;
			}
			if ((sprawdzTeren(Cel, Player, Tab) > -2 && sprawdzRuch(Cel, 25, Kostki, Player) == 1) && Player == false) break;
			else if ((sprawdzTeren(Cel, Player, Tab) > -2 && sprawdzRuch(Cel, 0, Kostki, Player) == 1) && Player == true) break;
		}
		system("CLS");
		przeniesBanda(Cel, Tab, Player, Ban);
		render(Tab, Ban, Player, Kostki);
	}
	if(Kostki[0] + Kostki[1] == 0) Player = !Player;
	system("cls");
}
void outro(bool Gracz, gracz Gracze[]){
	int a;
	system("CLS");
	if (Gracz == false){
		LOG("Wygrywa gracz nr 2");
		Gracze[1].win++;
		Gracze[0].los++;
	}
	else{
		LOG("Wygrywa gracz nr 1");
		Gracze[0].win++;
		Gracze[1].los++;
	}
	cin >> a;
	cin.clear();
	system("cls");
}
void wczytajGraczy(gracz Gracze[]){
	while (1){
		LOG("Podaj login gracza 1 = x");
		cin >> Gracze[0].name;
		LOG("Podaj login gracza 2 = o");
		cin >> Gracze[1].name;
		if (Gracze[1].name == Gracze[0].name){
			LOG("zle nazwy graczy");
		}
		else break;
	}
	system("CLS");
}
void wczytajRanking(){
	string gracz;
	float win, los;
	fstream plik(RANKING, ios::in);
	if (!plik.is_open()){
		LOG("NIE UDALO SIE ODCZYTAC PLIKU");
		return;
	}
	while (1){
		plik >> gracz;
		plik >> win;
		plik >> los;
		if (plik.eof()) break;
		cout << "Gracz :" << gracz << " wygral " << win << " meczy i przegral " << los << " a jego skutecznosc to :" <<win / (los + win) << "%" << endl;
	}
	plik.close();
	cin.clear();
	getchar();
}

void sprawdzGracza(gracz &Gracz){
	fstream plik("ranking.txt", ios::in);
	if (!plik.is_open()){
		LOG("Nie udalo sie otworzyc pliku");
		return;
	}
	string gracz;
	int win, los;
	while (plik.good()){
		plik >> gracz;
		plik >> win;
		plik >> los;
		if (gracz == Gracz.name){
			Gracz.win = win;
			Gracz.los = los;
			return;
		}
	}
}
bool wczytajGre(gracz Gracze[], int Tab[][TABLICA_SZER], int Ban[][BANDA_SZER],bool &gracz,int kostki[]){
	string name;
	LOG("PODAJ NAZWE PLIKU DO WCZYTANIA");
	cin >> name;
	fstream plik(name, ios::in);
	if (!plik.good()) {
		LOG("NIE OTWARTO PLIKU");
		return false;
	}
	plik >> Gracze[0].name >> Gracze[1].name>> gracz >> kostki[0] >> kostki[1];
	for (int i = 0; i < TABLICA_WYS; i++){
		for (int j = 0; j < TABLICA_SZER; j++){
			plik >> Tab[i][j];
		}
	}
	plik << '\n';
	for (int i = 0; i < BANDA_WYS; i++){
		for (int j = 0; j < BANDA_SZER; j++){
			plik >> Ban[i][j];
		}
	}
	return true;
}
void options(gracz Gracze[], int Tab[][TABLICA_SZER], int Ban[][BANDA_SZER], bool &gracz, int kostki[],bool &program,bool &game,bool &Wczytane){
	int wybor;
	while (1){
		LOG("1-zacznij gre");
		LOG("2-wczytaj gre");
		LOG("3-wyswietl ranking");
		LOG("4-zakoncz gre");
		cin >> wybor;
		system("CLS");
		switch (wybor)
		{
		case 1:
			return;
		case 3:
			system("CLS");
			wczytajRanking();
			break;
		case 4:
			game = false;
			program = false;
			return;
		case 2:
			if (wczytajGre(Gracze, Tab, Ban, gracz, kostki)){
				system("CLS");
				Wczytane = true;
				return;
			}
			break;
		default:
			cin >> wybor;
			break;
		}
	}
}
void dodajDoRankingu(gracz Gracze[]){
	string Name;
	int Temp[2];
	fstream plik(RANKING, ios::in);
	fstream kopia("temp.txt", ios::out);
	if (!plik.is_open() && !kopia.is_open()){
		LOG("NIE MOZNA OTWORZYC");
		return;
	}
	bool znaleziony[2] = { false, false };
	while (1){
		plik >> Name;
		plik >> Temp[0];
		plik >> Temp[1];
		if (plik.eof()) break;
		if (Name == Gracze[0].name){
			kopia << Gracze[0].name<<'\t';
			kopia << Gracze[0].win << '\t';
			kopia << Gracze[0].los << '\t';
			znaleziony[0] = true;
			kopia << '\n';
		}
		else if (Name == Gracze[1].name){
			kopia << Gracze[1].name << '\t';
			kopia << Gracze[1].win << '\t';
			kopia << Gracze[1].los << '\t';
			znaleziony[1] = true;
			kopia << '\n';
		}
		else{
			kopia << Name << '\t';
			kopia << Temp[0] << '\t';
			kopia << Temp[1] << '\t';
			kopia << '\n';
		}
	}
	for (int i = 0; i < 2; i++){
		if (znaleziony[i] == false){
			kopia << Gracze[i].name << '\t';
			kopia << Gracze[i].win << '\t';
			kopia << Gracze[i].los << '\t';
			kopia << '\n';
		}
	}
	plik.clear();
	plik.close();
	kopia.close();
	remove(RANKING);
	rename("temp.txt", RANKING);
}
int main(){
	///inicjalizacja
	srand(time(NULL));
	bool Start;
	bool Gra; //czas trwania gry
	bool Gracz; //kto gra false to gracz 1 a true to 2 
	bool Program = true;
	int Kostka[ILOSC_KOSTKI]; //kostki ;)
	int Tablica[TABLICA_WYS][TABLICA_SZER];
	int Banda[BANDA_WYS][BANDA_SZER];
	int IlePionkow;
	bool zapisane = false;
	bool Wczytane = false;
	bool skip = false;
	gracz Gracze[2];

	while (Program){
		intro();  //napisy poczatkowe
		options(Gracze, Tablica, Banda, Gracz, Kostka, Program, Gra,Wczytane);
		if (Program == false)break;
		Gra = true;
		if (Wczytane == false){
			wczytajGraczy(Gracze);
			sprawdzGracza(Gracze[0]);
			sprawdzGracza(Gracze[1]);
			zeruj_B(Banda);
			inicjalizujTab(Tablica);
		}
		zapisane = false;
		Start = true;
		IlePionkow = ilePionkow(Tablica,Banda);
		losowanieTury(Gracz, Kostka); // tu mamy rzucanie koscmi o to kto zaczyna

		//////////Gra/////////
		while (Gra){
			if (bandaCheck(Gracz, Banda) == true) inputBanda(Gracz, Banda, Tablica, Kostka, Gra, Start, Gracze);
			else input(Tablica, Gracz, Kostka, Start, Banda, Gra, IlePionkow, Gracze,zapisane,skip);
		}
		if (zapisane == false){
			outro(Gracz, Gracze);
			dodajDoRankingu(Gracze);
		}
		system("CLS");
	}
}