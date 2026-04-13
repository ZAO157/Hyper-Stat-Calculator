#include <cstdlib>
#include <iostream>

using namespace std;

int main() {
	int points[16] = {1 , 2 , 4 , 8 , 10 , 15 , 20 , 25 , 30 , 35 , 50 , 65 , 80 , 95 , 110 , 114514};
	int totalPoints;
	cout << "**This is for Xenon only**\nYour hyper stat points: ";
	cin >> totalPoints;

	if(totalPoints <= 0) {
		cout << "\n\nQuit this game ASAP.";
		system("pause");
		return 0;
	}

	double iniSta;
	double iniCri;
	double iniIed;
	double iniDmg;
	double iniBdg;
	double iniAtk;
	cout << "Your main stats(sum of 3): ";
	cin >> iniSta;
	cout << "Your critical damage (%): ";
	cin >> iniCri;
	cout << "Your ignore defense (%): ";
	cin >> iniIed;
	cout << "Your total damage (%): ";
	cin >> iniDmg;
	cout << "Your boss damage (%): ";
	cin >> iniBdg;
	cout << "Your attack: ";
	cin >> iniAtk;

	double bossDef;
	cout << "Boss' defense (%): ";
	cin >> bossDef;


	int** res = new int*[totalPoints+1];
	double* dp = new double[totalPoints+1];
	//IED
	int currentLevel = 0;
	int requirement = 1;
	bossDef *= 0.01;
	iniIed = (1 - iniIed * 0.01);
	double currentRatio = max(1 - iniIed * bossDef , 0.0);
	for(int i = 0 ; i <= totalPoints ; i++) {
		if(i == requirement) {
			currentLevel++;
			requirement += points[currentLevel];
			double currentIed = iniIed * (1 - currentLevel * 0.03);
			currentRatio = max(1 - currentIed * bossDef , 0.0);
		}
		dp[i] = currentRatio;
		res[i] = new int[6];
		res[i][0] = 0;
		res[i][1] = 0;
		res[i][2] = currentLevel;
		res[i][3] = 0;
		res[i][4] = 0;
		res[i][5] = 0;
	}
	if(currentRatio == 0) {
		cout << "\n\nYou cannot deal any damage whatsoever. :(";
		system("pause");
		return 0;
	}
	
	//Dmg&BD
	//here we assume BD will get above level 5
	//so we can use BD gain = 4% at all levels to calculate
	//subtract the additional 5% to 100% base damage
	iniDmg += iniBdg + 95;
	double* dmg = new double[totalPoints+1];
	for(int i = totalPoints ; i >= 0 ; i--) {
		dmg[i] = iniDmg;
	}
	for(int i = 0 ; i < 15 ; i++) {
		bool nop = true;
		int& weight = points[i];
		double value = (iniDmg + 4) / iniDmg;
		iniDmg += 4;
		for(int j = totalPoints ; j >= weight ; j--) {
			if(dp[j-weight] * value > dp[j]) {
				dp[j] = dp[j-weight] * value;
				nop = false;
				res[j][2] = res[j-weight][2];
				res[j][4] = i + 1;
				dmg[j] = iniDmg;
			}
		}
		if(nop) break;
	}
	for(int i = 0 ; i < 15 ; i++) {
		bool nop = true;
		int& weight = points[i];
		for(int j = totalPoints ; j >= weight ; j--) {
			double value = (dmg[j-weight] + 3) / dmg[j-weight];
			if(dp[j-weight] * value > dp[j]) {
				dp[j] = dp[j-weight] * value;
				nop = false;
				res[j][2] = res[j-weight][2];
				res[j][3] = i + 1;
				res[j][4] = res[j-weight][4];
				dmg[j] = dmg[j-weight] + 3;
			}
		}
		if(nop) break;
	}

	//CD
	iniCri += 135;
	for(int i = 0 ; i < 15 ; i++) {
		bool nop = true;
		int& weight = points[i];
		double value = ((double)(iniCri + 1)) / ((double)iniCri);
		iniCri += 1;
		for(int j = totalPoints ; j >= weight ; j--) {
			if(dp[j-weight] * value > dp[j]) {
				dp[j] = dp[j-weight] * value;
				nop = false;
				res[j][1] = i + 1;
				res[j][2] = res[j-weight][2];
				res[j][3] = res[j-weight][3];
				res[j][4] = res[j-weight][4];
			}
		}
		if(nop) break;
	}

	//ATK
	for(int i = 0 ; i < 15 ; i++) {
		bool nop = true;
		int& weight = points[i];
		double value = ((double)(iniAtk + 3)) / ((double)iniAtk);
		iniAtk += 3;
		for(int j = totalPoints ; j >= weight ; j--) {
			if(dp[j-weight] * value > dp[j]) {
				dp[j] = dp[j-weight] * value;
				nop = false;
				res[j][1] = res[j-weight][1];
				res[j][2] = res[j-weight][2];
				res[j][3] = res[j-weight][3];
				res[j][4] = res[j-weight][4];
				res[j][5] = i + 1;
			}
		}
		if(nop) break;
	}

	//Sta
	for(int i = 0 ; i < 15 ; i++) {
		bool nop = true;
		int& weight = points[i];
		double value = ((double)(iniSta + 30)) / ((double)iniSta);
		iniSta += 30;
		for(int j = totalPoints ; j >= weight ; j--) {
			if(dp[j-weight] * value > dp[j]) {
				dp[j] = dp[j-weight] * value;
				nop = false;
				res[j][0] = i * 3 + 1;
				res[j][1] = res[j-weight][1];
				res[j][2] = res[j-weight][2];
				res[j][3] = res[j-weight][3];
				res[j][4] = res[j-weight][4];
				res[j][5] = res[j-weight][5];
			}
		}
		if(nop) break;
		nop = true;
		value = ((double)(iniSta + 30)) / ((double)iniSta);
		iniSta += 30;
		for(int j = totalPoints ; j >= weight ; j--) {
			if(dp[j-weight] * value > dp[j]) {
				dp[j] = dp[j-weight] * value;
				nop = false;
				res[j][0] = i * 3 + 2;
				res[j][1] = res[j-weight][1];
				res[j][2] = res[j-weight][2];
				res[j][3] = res[j-weight][3];
				res[j][4] = res[j-weight][4];
				res[j][5] = res[j-weight][5];
			}
		}
		if(nop) break;
		nop = true;
		value = ((double)(iniSta + 30)) / ((double)iniSta);
		iniSta += 30;
		for(int j = totalPoints ; j >= weight ; j--) {
			if(dp[j-weight] * value > dp[j]) {
				dp[j] = dp[j-weight] * value;
				nop = false;
				res[j][0] = i * 3 + 3;
				res[j][1] = res[j-weight][1];
				res[j][2] = res[j-weight][2];
				res[j][3] = res[j-weight][3];
				res[j][4] = res[j-weight][4];
				res[j][5] = res[j-weight][5];
			}
			if(i == 15) break;
		}
		if(nop) break;
	}

	cout << "\n\nYou should use:\n" << 
		res[totalPoints][0] << " level of main stats (total)\n" <<
		res[totalPoints][1] << " level of critical damage\n" <<
		res[totalPoints][2] << " level of ignore defense\n" <<
		res[totalPoints][3] << " level of total damage\n" <<
		res[totalPoints][4] << " level of boss damage\n" <<
		res[totalPoints][5] << " level of attack\n";
	if(iniIed * bossDef < 1){
		cout << "with " << dp[totalPoints] * 100 / (1 - iniIed * bossDef) - 100 << "% of gain.\n\n";
	}

	system("pause");
	return 0;
}