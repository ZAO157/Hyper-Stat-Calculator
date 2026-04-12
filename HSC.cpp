#include <cstdlib>
#include <iostream>

using namespace std;

int main() {
	int points[16] = {1 , 2 , 4 , 8 , 10 , 15 , 20 , 25 , 30 , 35 , 50 , 65 , 80 , 95 , 110 , 114514};
	int totalPoints;
	cout << "**This if for Xenon only**\nYour hyper stat points: ";
	cin >> totalPoints;

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

	if(totalPoints <= 0) {
		cout << "\n\nQuit this game ASAP.";
		system("pause");
		return 0;
	}

	int** res = new int*[totalPoints+1];
	double* dp = new double[totalPoints+1];
	//IED
	int currentLevel = 0;
	int requirement = 1;
	for(int i = 0 ; i <= totalPoints ; i++) {
		if(i == requirement) {
			currentLevel++;
			requirement += points[currentLevel];
		}
		double currentIed = 1 - (1 - iniIed * 0.01) * (1 - currentLevel * 0.03);
		dp[i] = 1 - min(bossDef * 0.01 - currentIed * bossDef * 0.01 , 1.0);
		res[i] = new int[6];
		memset(res[i] , 0 , sizeof(res[i]));
		res[i][2] = currentLevel;
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
				res[j][4] = res[j-weight][4] + 1;
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
				res[j][3] = res[j-weight][3] + 1;
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
				res[j][1] = res[j-weight][1] + 1;
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
				res[j][5] = res[j-weight][5] + 1;
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
				res[j][0] = res[j-weight][0] + 1;
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
				res[j][0] = res[j-weight][0] + 1;
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
				res[j][0] = res[j-weight][0] + 1;
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
	cout << "with " << dp[totalPoints] * 100 / (1 - min(bossDef * 0.01 - iniIed * bossDef * 0.0001 , 0.99999)) - 100 << "% of gain.\n\n";

	system("pause");
	return 0;
}