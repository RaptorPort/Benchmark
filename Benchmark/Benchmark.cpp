// Benchmark.cpp: Definiert den Einstiegspunkt für die Konsolenanwendung.
//

#include "stdafx.h"
#include <iostream>
#include <vector>
#include <time.h>  
#include <chrono>

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
	return ((rand() % (bewertungsrange * 2) + 1) - bewertungsrange);
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
	countEval2 = 0;
	besterZugMiniMax.feld = -100;

	int bestcalczug = negaMiniMax(true, tiefe);
	if (besterZugMiniMax.feld != -100) {
		cout << "Feld: " << besterZugMiniMax.feld << endl;
	}
	else {
		cout << "Feld: " << bestcalczug << endl;
	}

	cout << "Minimax Evals: " << countEval2;
}

void AlphaBeta(int verzweigungsgradp, int tiefe, int alpha, int beta) {
	verzweigungsgrad = verzweigungsgradp;
	countEval = 0;
	countCuts = 0;
	besterZugAlphaBeta.feld = -100;

	int bestcalczug = negaMax(true, tiefe, alpha, beta);
	if (besterZugAlphaBeta.feld != -100) {
		cout << "Feld: " << besterZugAlphaBeta.feld << endl;
	}
	else {
		cout << "Feld: " << bestcalczug;
	}
	cout  << " Evals: " << countEval << " Cuts: " << countCuts;
}

void MiniMaxNoLog(int verzweigungsgradp, int tiefe) {
	verzweigungsgrad = verzweigungsgradp;
	countEval2 = 0;
	besterZugMiniMax.feld = -100;

	int bestcalczug = negaMiniMax(true, tiefe);
	if (besterZugMiniMax.feld != -100) {
		//cout << "Feld: " << besterZugMiniMax.feld << endl;
	}
	else {
		//cout << "Feld: " << bestcalczug << endl;
	}

	//cout << "Minimax Evals: " << countEval2;
}

void AlphaBetaNoLog(int verzweigungsgradp, int tiefe, int alpha, int beta) {
	verzweigungsgrad = verzweigungsgradp;
	countEval = 0;
	countCuts = 0;
	besterZugAlphaBeta.feld = -100;

	int bestcalczug = negaMax(true, tiefe, alpha, beta);
	if (besterZugAlphaBeta.feld != -100) {
		//cout << "Feld: " << besterZugAlphaBeta.feld << endl;
	}
	else {
		//cout << "Feld: " << bestcalczug;
	}
	//cout << " Evals: " << countEval << " Cuts: " << countCuts;
}

void bench() {
	bewertungsrange = 1000;
	srand(time(NULL));
	cout << "Verzweigungsgrad = 31 und Tiefe = 4: " << endl << endl;
	cout << "MiniMax:" << endl;
	auto start = chrono::steady_clock::now();
	MiniMax(31, 4);
	auto end = chrono::steady_clock::now();
	cout << endl << "Zeit in Millisekunden: " << chrono::duration_cast<chrono::milliseconds>(end - start).count() << endl << endl;

	cout << "Alpha-Beta-Suche:" << endl;
	start = chrono::steady_clock::now();
	AlphaBeta(31, 4, -bewertungsrange, bewertungsrange);
	end = chrono::steady_clock::now();
	cout << " %Cut: " << ((double)countEval2 - (double)countEval) / (double)countEval2 << "%";
	cout << endl << "Zeit in Millisekunden: " << chrono::duration_cast<chrono::milliseconds>(end - start).count() << endl << endl;

	/*cout << "-------------------------------------" << endl << endl;
	cout << "Verzweigungsgrad = 31 und Tiefe = 5: " << endl << endl;
	cout << "MiniMax:" << endl;
	start = chrono::steady_clock::now();
	MiniMax(31, 5);
	end = chrono::steady_clock::now();
	cout << endl << "Zeit in Millisekunden: " << chrono::duration_cast<chrono::milliseconds>(end - start).count() << endl << endl;

	cout << "Alpha-Beta-Suche:" << endl;
	start = chrono::steady_clock::now();
	AlphaBeta(31, 5, -bewertungsrange, bewertungsrange);
	end = chrono::steady_clock::now();
	cout << " %Cut: " << ((double)countEval2 - (double)countEval) / (double)countEval2 << "%";
	cout << endl << "Zeit in Millisekunden: " << chrono::duration_cast<chrono::milliseconds>(end - start).count() << endl << endl;*/

	cout << "-------------------------------------" << endl << endl;
	cout << "Verzweigungsgrad = 31 und Tiefe = 6: " << endl << endl;
	cout << "MiniMax:" << endl;
	start = chrono::steady_clock::now();
	MiniMax(31, 6);
	end = chrono::steady_clock::now();
	cout << endl << "Zeit in Millisekunden: " << chrono::duration_cast<chrono::milliseconds>(end - start).count() << endl << endl;

	cout << "Alpha-Beta-Suche:" << endl;
	start = chrono::steady_clock::now();
	AlphaBeta(31, 6, -bewertungsrange, bewertungsrange);
	end = chrono::steady_clock::now();
	cout << " %Cut: " << ((double)countEval2 - (double)countEval) / (double)countEval2 << "%";
	cout << endl << "Zeit in Millisekunden: " << chrono::duration_cast<chrono::milliseconds>(end - start).count() << endl << endl;
}

void benchRange() {
	cout << "Verzweigungsgrad = 31 und Tiefe = 4" << endl;
	for (int b = 2; b < 1000000000; b = b * 2) {
		bewertungsrange = b;
		cout << "Bewertungsfenster: " << bewertungsrange;
		if (bewertungsrange <= 16000)
			cout << "\t";
		int sumCuts = 0;
		int sumEval = 0;
		int iter = 0;
		for (int i = 0; i < 10; i++) {
			AlphaBetaNoLog(31, 4, -bewertungsrange, bewertungsrange);
			sumCuts += countCuts;
			sumEval += countEval;
			iter++;
			countCuts = countEval = 0;
		}
		cout << "\t AvgCuts: " << sumCuts / iter << "\t Evals: " << sumEval / iter << endl;
	}
}

int main()
{
	benchRange();
	/*
	//for (int b = 2; b < 1000000; b = b*2) {
		bewertungsrange = 1000;
		cout << "Bewertungsfenster: " << bewertungsrange;
		int sumCuts = 0;
		int sumEval = 0;
		int iter = 0;
		for (int i = 0; i < 1000; i++) {
			AlphaBeta(31, 4, -bewertungsrange, bewertungsrange);
			sumCuts += countCuts;
			sumEval += countEval;
			iter++;
			countCuts = countEval = 0;
		}
		cout << "\t AvgCuts: " << sumCuts / iter << "\t Evals: " << sumEval / iter << endl;
	//}
		MiniMax(31, 4);
	*/
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