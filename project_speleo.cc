#include <iostream>
#include <vector>
#include <random>
#include <ctime>
#include <iomanip>
#include <cmath>
#include <cstdlib>
#include <algorithm>
using namespace std; 

vector<vector<bool>> lire_grille();
void affiche_grille(const vector<vector<bool>>& grille);
void construire_passage(int j, const vector<vector<bool>>& libre, 
						vector<vector<bool>>& passage,int i);
void int_grille_true(vector<vector<bool>>& grille);
void int_grille_false(vector<vector<bool>>& grille);
bool traverse(const vector<vector<bool>>& grille);
double calcule_P(int n, unsigned int nbt, double p);
void dichotomie(vector<vector<double>>& p_pair, double min, double max,
				double n, double nbt,double,double);
bool compare_first_val(vector<double> a, vector<double> b);
//----------------------------------------------------------------------
int main()
{
	char a;
	cin.get(a);
	if (a=='a') {
		vector<vector<bool>> libre(lire_grille());
		vector<vector<bool>> passage(libre.size(), vector<bool>(libre.size()));
		int_grille_true(passage);
		int nb(libre.size());
		for (int j(0); j<nb;++j) {
			if (libre[0][j]==false) {
				construire_passage(j,libre,passage,0);
			}
		}
		affiche_grille(passage);
	}
	if (a=='b') {
			int n;
			double p;
			cin >> n >> p;
			if (p<0. or p>1.) {
				cout << "La probabilité p n'est pas comprise dans [0 ; 1]" << endl;
				return 0;
			}
			unsigned int nbt;
			cin >> nbt;
			cout << fixed;
			cout << setprecision(6) << calcule_P(n,nbt,p) << endl;
	}
	if (a=='c') {
		double n,nbt;
		cin >> n >> nbt;
		vector<vector<double>> p_pair({{0.,0.}, {1.,1.}});
		dichotomie(p_pair,0.,1.,n,nbt,0.,1.);
		sort(p_pair.begin(), p_pair.end(), compare_first_val);
		for (auto ligne : p_pair) {
			for (auto cell : ligne) {
				cout << fixed;
				cout << setprecision(6) << cell << " ";
			}
		cout << endl;
		}
	}
	return 0;
}
//----------------------------------------------------------------------
void dichotomie(vector<vector<double>>& p_pair, double min, double max, 
				double n, double nbt, double min_p, double max_p)
{
	double mid = (max+min)/2;
	double max_error(pow(10,-2)), min_delta_p(pow(10,-6)), 
	mid_p((max_p+min_p)/2), P(calcule_P(n,nbt,mid)), error(P-mid_p);
	vector<double> pP(2);
	pP[0]=mid;
	pP[1]=P;
	p_pair.push_back(pP);
	if ((max-min)>min_delta_p and abs(error)>max_error) {
		if (error<0) {
			dichotomie(p_pair, mid, max, n, nbt, P, max_p);
		} else {
			dichotomie(p_pair, min, mid, n, nbt, min_p, P);
		}
	}
}
//----------------------------------------------------------------------
bool compare_first_val(vector<double> a, vector<double> b)
{
	return a[0] < b[0];
}
//----------------------------------------------------------------------
bool traverse(const vector<vector<bool>>& grille)
{
	unsigned int n(grille.size()-1);
	for (size_t j(0);j<grille.size();++j) {
		if (grille[n][j]==false) {
			return true;
		}
	}
	return false;
}
//----------------------------------------------------------------------
double calcule_P(int n, unsigned int nbt, double p)
{			
	vector<vector<bool>> passage(n,vector<bool>(n));
	int_grille_true(passage);
	int counter(0);
	bernoulli_distribution b(p);
	time_t result = time(nullptr);
	default_random_engine e(result);
	vector<vector<bool>> libre(n,vector<bool>(n));
	for (unsigned int m(0);m<nbt;++m) {
		int_grille_false(libre);
		int_grille_true(passage);
		for (int i(0); i<n; ++i) {
			for (int j(0); j<n; ++j) {
				if (b(e)==true) {
					libre[i][j]= false;
				} else {
					libre[i][j]=true;
				}
			}
		}
		for (int j(0); j<n;++j) {
			if (libre[0][j]==false) {
				construire_passage(j,libre,passage,0);
			}
		}	
		if (traverse(passage)==true) {
			++counter;
		}
	}
	double NBT(nbt), p_(counter/NBT);
	return p_;
}
//----------------------------------------------------------------------
void construire_passage(int j, const vector<vector<bool>>& libre, 
						vector<vector<bool>>& passage,int i)
{
	int n(libre.size());
	passage[i][j]=false;
	if (i+1<n) {
		i+=1;
		if (libre[i][j]==false and passage[i][j]!=false) {
			construire_passage(j,libre,passage,i);                      
			i-=1;
		} else { 
			i-=1;
		}
	}
	if (i-1>=0) {
		i-=1;
		if (libre[i][j]==false and passage[i][j]!=false) {
				construire_passage(j,libre,passage,i);                   
				i+=1;
		} else {
			i+=1;
		}
	}
	if (j+1<n) {
		j+=1;
		if (libre[i][j]==false and passage[i][j]!=false) {
				construire_passage(j,libre,passage,i);                   
				j-=1;
		} else {
			j-=1;
		}
	}
	if (j-1>=0) {
		j-=1;
		if (libre[i][j]==false and passage[i][j]!=false) {
				construire_passage(j,libre,passage,i);                   
				j+=1;
		} else {
			j+=1;
		}	
	}
}
//----------------------------------------------------------------------
vector<vector<bool>> lire_grille()
{
	unsigned int n;
	cin >> n;
	vector<vector<bool>> grille(n, vector<bool>(n));
	for (unsigned int i(0);i<n;++i) {
		for (unsigned int j(0);j<n;++j) {
			bool b;
			cin >> b;
			grille[i][j]=b;
		}
	}
	return grille;
}
//----------------------------------------------------------------------
void affiche_grille(const vector<vector<bool>>& grille)
{
	int counter (0);
	cout << "P1" << endl;
	cout << "# Speleo project" << endl;
	cout << grille.size() << " " << grille.size() << endl;
	
	for (auto ligne : grille) {
		for (auto cell : ligne) {
			cout << cell << " ";
			counter+=2;
			if (counter % 70 == 0) {
				cout << endl;
			}
		}
		counter=0;
		cout << endl;
	}
}
//----------------------------------------------------------------------
void int_grille_true(vector<vector<bool>>& grille)
{
	for (unsigned int i(0); i<grille.size(); ++i) {
		for (unsigned int j(0); j<grille.size(); ++j) {
			grille[i][j]=true;
		}
	}
}
void int_grille_false(vector<vector<bool>>& grille)
{
	for (unsigned int i(0); i<grille.size(); ++i) {
		for (unsigned int j(0); j<grille.size(); ++j) {
			grille[i][j]=false;
		}
	}
}
