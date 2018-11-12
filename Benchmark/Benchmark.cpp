// Benchmark.cpp: Definiert den Einstiegspunkt für die Konsolenanwendung.
//

#include "stdafx.h"
#include <iostream>
#include <vector>
#include <time.h>  

using namespace std;

class Zug
{
public:
	int feld;

	Zug(int i)
	{
		feld = i;
	}
	Zug()
	{
		feld = -100;
	}
	void ausführen() {
		
	}
	void rückgängig() {
		
	}

};
int countCuts = 0;
int countEval = 0;
int countCuts2 = 0;
int countEval2 = 0;
int verzweigungsgrad = 0;
int bewertungsrange = 1;

Zug besterZugAlphaBeta;
Zug besterZugMiniMax;
int maxTiefe;

int bewerten() {
	return rand() % 100 + 1 - 50;
}

int bewerten(bool sp, bool minimax) {
	if (sp)
		return ((rand() % bewertungsrange*2 + 1) - bewertungsrange);
	else
		return (rand() % 100 + 1 - 50) * -1;
}

bool istEndstellung() {
	return false;
}


std::vector<Zug> generiereMöglicheZüge(bool sp) {
	vector<Zug> zug;
	for (int i = 0; i < verzweigungsgrad; i++) {
			Zug neuZug = Zug(i);
			zug.push_back(neuZug);
	}
	return zug;
}

int negaMax(bool spieler, int tiefe, int alpha, int beta) {
	countEval++;
	if (tiefe == 0 || istEndstellung())
		return bewerten(spieler, false);
	int maxWert = alpha;

	for (Zug zug : generiereMöglicheZüge(spieler)) {
		zug.ausführen();
		int wert = -negaMax(!spieler, tiefe - 1, -beta, -maxWert);
		zug.rückgängig();
		if (wert > maxWert) {
			maxWert = wert;
			if (maxWert >= beta) {
				countCuts++;
				break;
			}
			if (tiefe == maxTiefe)
				besterZugAlphaBeta.feld = zug.feld;
		}
	}
	return maxWert;
}

int negaMiniMax(bool spieler, int tiefe) {
	countEval2++;
	if (tiefe == 0 || istEndstellung())
		return bewerten(spieler, true);
	int maxWert = -bewertungsrange;

	for (Zug zug : generiereMöglicheZüge(spieler)) {
		zug.ausführen();
		int wert = -negaMiniMax(!spieler, tiefe - 1);
		zug.rückgängig();
		if (wert > maxWert) {
			maxWert = wert;
			if (tiefe == maxTiefe)
				besterZugMiniMax.feld = zug.feld;
		}
	}
	return maxWert;
}


void MiniMax(int verzweigungsgradp, int tiefe) {
	verzweigungsgrad = verzweigungsgradp;
	srand(1);
	int bestcalczug = negaMiniMax(true, tiefe);
	if (besterZugMiniMax.feld != -100) {
		cout << "Feld: " << besterZugMiniMax.feld << endl;
	}
	else {
		cout << "Feld: " << bestcalczug << endl;
	}

	cout << endl << "Minimax Evals: " << countEval2 << endl;
}

void AlphaBeta(int verzweigungsgradp, int tiefe, int alpha, int beta) {
	verzweigungsgrad = verzweigungsgradp;
	srand(time(NULL));
	int bestcalczug = negaMax(true, tiefe, alpha, beta);
	if (besterZugAlphaBeta.feld != -100) {
		cout << "Feld: " << besterZugAlphaBeta.feld << endl;
	}
	else {
		//cout << "Feld: " << bestcalczug;
	}
	//cout << " Cuts: " << countCuts << " Evals: " << countEval << endl;
}

int main()
{
	for (int b = 2; b < 1000000; b = b*2) {
		bewertungsrange = b;
		cout << "Bewertungsfenster: " << bewertungsrange;
		int sumCuts = 0;
		int sumEval = 0;
		int iter = 0;
		for (int i = 0; i < 10; i++) {
			AlphaBeta(30, 4, -bewertungsrange, bewertungsrange);
			sumCuts += countCuts;
			sumEval += countEval;
			iter++;
			countCuts = countEval = 0;
		}
		cout << "\t AvgCuts: " << sumCuts / iter << "\t Evals: " << sumEval / iter << endl;
	}
	
	
	int i;
	cin >> i;
}
/*for (int b = 2; b < 1000000; b = b*2) {
		bewertungsrange = b;
		cout << "Bewertungsfenster: " << bewertungsrange;
		int sumCuts = 0;
		int sumEval = 0;
		int iter = 0;
		for (int i = 0; i < 10; i++) {
			AlphaBeta(4, 10, -bewertungsrange, bewertungsrange);
			sumCuts += countCuts;
			sumEval += countEval;
			iter++;
			countCuts = countEval = 0;
		}
		cout << "\t AvgCuts: " << sumCuts / iter << "\t Evals: " << sumEval / iter << endl;
	}*/