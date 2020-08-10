#include <fstream>
#include <iostream>
#include <iostream>
#include <cstdlib>
#include <vector>
#include <sstream>
#include <bits/stdc++.h>
#include <math.h>
#include <time.h>
#include <algorithm>
using namespace std;

bool areDistinct(vector<int> arr)
{
    int n = arr.size();
    unordered_set<int> s;
    for (int i = 0; i < n; i++) {
        s.insert(arr[i]);
    }
    return (s.size() == arr.size());
}

bool checkconst1(int newv, vector<int> check, int n, vector<vector<int>> tcc){
	for(int i = 0 ; i<n ; i++){
		if(tcc[newv][check[i]] == 1 || tcc[check[i]][newv] == 1){
			return false;
		}
	}
	return true;
}
class Bloque{
	public:
		vector<int> charlas;
		int bsize;
		int total;

};
class Gen{
	public:
		vector<vector<Bloque>> horario;
		int costo;
		int hops;
		int conflictos;
};
class Charlas{
  public:
    int cruce;
    vector<int> charlas;
    int considerado;
	vector<int> otrascharlas;
	int lcharlas;
	int n;
};



class RPAC {
	public:
		int type;
		int talk;
		int day;
		vector<int> timeslot;
};
Gen CrossGen(Gen g1, Gen g2,int sessions,int ntalks){
	Gen child;
	int pos = 0;
	vector<int> considerados;
	child.horario.resize(g1.horario.size());
	for(int i = 0 ; (unsigned)i<g1.horario.size();i++){
		child.horario[i].resize(g1.horario[i].size());
		for(int j = 0 ; (unsigned) j<g1.horario[i].size();j++){
			int rep = 0;
			int chance = (rand()%100) /100;
			if(pos%2){
				for(int k = 0 ; (unsigned)k<g1.horario[i][j].charlas.size();k++){
					if(find(considerados.begin(),considerados.end(),g1.horario[i][j].charlas[k])!=considerados.end()){
						rep = 1;
						break;
					}
				}
				if(rep == 0){
					child.horario[i][j] = g1.horario[i][j];
					if(chance <= 0.2){
						next_permutation(child.horario[i][j].charlas.begin(),child.horario[i][j].charlas.end());
					}
					considerados.insert(considerados.end(),g1.horario[i][j].charlas.begin(),g1.horario[i][j].charlas.end());
				}
			}else{
				for(int k = 0 ; (unsigned)k<g2.horario[i][j].charlas.size();k++){
					if(find(considerados.begin(),considerados.end(),g2.horario[i][j].charlas[k])!=considerados.end()){
						rep = 1;
						break;
					}
				}
				if(rep == 0){
					child.horario[i][j] = g2.horario[i][j];
					next_permutation(child.horario[i][j].charlas.begin(),child.horario[i][j].charlas.end());
					considerados.insert(considerados.end(),g2.horario[i][j].charlas.begin(),g2.horario[i][j].charlas.end());
				}
			}

			pos++;
		}
	}
	while(considerados.size()!= (unsigned)ntalks){
		for(int i = 0 ; (unsigned)i<child.horario.size();i++){
			for(int j = 0 ; (unsigned) j<child.horario[i].size();j++){
				if(child.horario[i][j].charlas.size() == 0){
					for(int k = 0 ; k<g1.horario[i][j].bsize;k++){
						for(int z = 0 ; z<sessions;z++){
							bool found = false;
							int talk;
							while(!found){

								talk = rand()%ntalks;
								if(find(considerados.begin(),considerados.end(),talk) == considerados.end() ){
									found = true;
								}

							}
							child.horario[i][j].charlas.push_back(talk);
							considerados.push_back(talk);
						}
					}
				}
			}
		}
	}
	child.costo = 0 ;
	child.hops = 0;
	child.conflictos = 0 ;
	return child;

}
Gen generateGen(int days,int ntalks,vector<vector<int>> bloques,int sessions){
	Gen gen;
	int talk;
	gen.horario.resize(days);
	vector<int> tomadas;
	tomadas.resize(ntalks);
	int pos = 0;
	fill(tomadas.begin(),tomadas.end(),0);
	for(int i = 0; i<days;i++){
		gen.horario[i].resize(bloques[i].size());
		for(int j = 0 ;(unsigned)j<bloques[i].size();j++){
			if(bloques[i][j] != 0){
				for(int k = 0 ; k<bloques[i][j];k++){
					for(int z = 0 ; z<sessions;z++){
						bool found = false;
						while(!found){
							talk = rand()%ntalks;
							if(tomadas[talk] == 0){
								found = true;
							}

						}

						gen.horario[i][j].charlas.push_back(talk);
					}
					pos++;
				}
			}
			gen.horario[i][j].bsize = bloques[i][j];
			gen.horario[i][j].total = 0;
		}
	}
	gen.costo = 0;
	gen.conflictos = 0 ;
	gen.hops = 0 ;
	return gen;



}
bool compareInterval(Gen g1, Gen g2){
	return (g1.costo < g2.costo);
}
vector<string> split(const std::string& s, char delimiter)
{
   vector<string> tokens;
   string token;
   istringstream tokenStream(s);

   while (getline(tokenStream, token, delimiter))
   {
      tokens.push_back(token);
   }
   return tokens;
}




int main(int argc, char** argv){
	srand((int)argv[2]);
	int sessions,people,talks,i,j,days,PAC;
	vector<string> values;
	string line,valor;
	ifstream file;
	vector<vector<int>> preferences;
	vector<vector<int>> swaps;
	vector<vector<int>> horario;
	vector<vector<int>> bloques;
	vector<vector<Bloque>> dbloques;
	vector<Charlas> charlas;
	vector<RPAC> rpac;
	vector<vector<int>> rtcc;
	file.open(argv[1]);
	string instancia = argv[1];

	instancia = instancia.substr(0,instancia.size()-3);
	string nfile="Resultados-"+instancia+".csv";
	ofstream out(nfile);
	streambuf *coutbuf = cout.rdbuf();
	cout.rdbuf(out.rdbuf());
	getline(file,line);
	values = split(line,' ');
	sessions =stoi(values[2]);
	talks = stoi(values[1]);
	horario.resize(sessions+1);
	charlas.resize(talks);
	days = stoi(values[4]);
	people = stoi(values[0]);
	preferences.resize(people);
	dbloques.resize(days);
	bloques.resize(days);
	swaps.resize(sessions+1);
	j = 0;
	while(j!= people){
		getline(file,line);
		values = split(line,' ');
		for(int i = 0; (unsigned)i<values.size() ; i++){
			preferences[j].push_back(stoi(values[i]));
		}
		j++;
	}
	j = 0;
	while(j!=days){
		getline(file,line);
		values = split(line,' ');
		for(int i = 0; (unsigned)i<values.size() ; i++){
			bloques[j].push_back(stoi(values[i]));
		}
		j++;
	}
	getline(file,line);
	PAC = stoi(line);
	rpac.resize(PAC);
	j = 0;
	while(j!= PAC){
		getline(file,line);
		values = split(line,' ');
		rpac[j].type = stoi(values[0]);
		values.erase(values.begin());
		rpac[j].talk = stoi(values[0]);
		values.erase(values.begin());
		rpac[j].day = stoi(values[0]);
		values.erase(values.begin());
		for(i = 0 ; (unsigned)i<values.size();i++){
			rpac[j].timeslot.push_back(stoi(values[i]));
		}
		j++;
	}
	rtcc.resize(talks);
	for(i = 0;i<talks;i++ ){
		rtcc[i].resize(talks);
		fill(rtcc[i].begin(),rtcc[i].end(),0);
	}

	getline(file,line);
	while(getline(file,line)){
		values = split(line, ' ');
		int pos = stoi(values[0]);
		values.erase(values.begin());
		for(i = 0; (unsigned)i<values.size(); i++){
			rtcc[pos][stoi(values[i])] = 1;
		}
	}



	for(i = 0; i<talks;i++){
		charlas[i].cruce = 9999999;
		charlas[i].charlas.resize(sessions);
		charlas[i].considerado = 0;
		charlas[i].n = 0;

	}

	for(int i=0;i<talks;i++){
	  for(int j= 0;j<talks;j++){
		  for(int k = 0;k<people;k++){
			  if(preferences[k][i]== 1 && preferences[k][j]==1 && i!=j){
				  charlas[i].otrascharlas.push_back(j);
				  charlas[i].lcharlas++;
				  break;
			  }
		  }
	  }
	}


	// for(i=0;i<talks;i++){
	//   std::cerr << "Charla:"<<i+1 << '\n';
	//   for(int j=0;j<charlas[i].lcharlas;j++){
	// 	  std::cout << charlas[i].otrascharlas[j] +1<< '\t';
	//   }
	//   std::cout << "\n" << '\n';
	// }

	int n;
	for(i=0;i<talks;i++){
		n=0;
		int actualval = 0;
		if(charlas[i].considerado == 0){
			for(int j=0;j<charlas[i].lcharlas;j++){
				if(n==sessions){
					break;
				}
				if(charlas[i].otrascharlas[j]!= actualval){
					if(charlas[actualval].considerado==0){
						if(checkconst1(actualval,charlas[i].otrascharlas,n,rtcc)){
							int posible = 1;
							if(n>1){
								for(int r = 0; r<n;r++){
									for(int f =0; f < charlas[charlas[i].charlas[r]].lcharlas; f++){
										if(charlas[charlas[i].charlas[r]].otrascharlas[f] == actualval){
											posible = 0;
											break;
										}
									}
								}
							}
							if(posible == 1){
								int found = 0;

								for(int t = 0; t<n;t++){
									if(charlas[i].charlas[t] == i){
										found = 1;
									}
								}
								if(found == 0 && n==sessions-1){
									charlas[i].charlas[n]=i;
									charlas[i].cruce = 0;
									charlas[i].considerado = 1;
								}else{
									charlas[i].charlas[n]=actualval;
									charlas[i].cruce = 0;
									charlas[actualval].considerado = 1;
								}

								n++;

								for(int t = 0; t<n;t++){
									charlas[charlas[i].charlas[t]].n= n;
								}

							}
						}
					}
					if(actualval<charlas[i].otrascharlas[j]){
						j--;
					}
				}

				actualval++;
			}

		}
		if(sessions!= 2){
			if(charlas[i].n < sessions-1){
				charlas[i].cruce = 9999999;
				charlas[i].n = 0;
				charlas[i].considerado=0;
				for(int s= 0; s<n; s++){
					charlas[charlas[i].charlas[s]].considerado = 0;
					charlas[i].charlas[s] = 0;
				}
			}
		}else{
			if(charlas[i].n <= sessions-1){
				charlas[i].cruce = 9999999;
				charlas[i].n = 0;
				charlas[i].considerado=0;
				for(int s= 0; s<n; s++){
					charlas[charlas[i].charlas[s]].considerado = 0;
					charlas[i].charlas[s] = 0;
				}
			}
		}
	}
	std::cout << "resultado 1" << '\n';
	for(i=0;i<talks;i++){
		if(charlas[i].cruce == 0){
	    	cout << charlas[i].cruce <<"\t"<<charlas[i].charlas[0]+1<<"\t"<<charlas[i].charlas[1]+1<<"\t"<<charlas[i].charlas[2]+1<<'\n';
		}
	}
	vector<int> charlasrestantes;
	int lrestantes=0;
	for(i=0;i<talks+1;i++){
		if(charlas[i].considerado == 0){
			charlasrestantes.push_back(i);
			lrestantes++;
			// cout <<"charla: "<<i+1<<'\n';
		}
	}
	for(i = 0 ; i<talks; i++){
		if(charlas[i].n<sessions && charlas[i].cruce == 0){
			charlas[i].cruce = 5000;
			for(int k = 0 ; k<lrestantes; k++){
				int newsum = 0;
				for(int h = 0 ; h < people; h++){
					int tempsum = 0;
					for(int g = 0 ; g< charlas[i].n-1;g++){
						tempsum = tempsum + preferences[h][charlas[i].charlas[g]];
					}
					tempsum = tempsum + preferences[h][charlasrestantes[k]];
					if(tempsum != 0){
						newsum= newsum + tempsum-1;
					}
				}
				if(charlas[i].cruce>newsum){
					if(charlas[charlasrestantes[k]].considerado==0 && checkconst1(charlasrestantes[k],charlas[i].otrascharlas,sessions-1,rtcc)){
						charlas[i].cruce = newsum;
						if(charlas[i].charlas[sessions-1] == 0){
							charlas[charlasrestantes[k]].considerado = 1;
							charlas[i].charlas[sessions-1] = charlasrestantes[k];
						}else{
							charlas[charlasrestantes[k]].considerado = 1;
							charlas[charlas[i].charlas[sessions-1]].considerado = 0;
							charlas[i].charlas[sessions-1] = charlasrestantes[k];
						}
					}
				}
			}
		}
	}
	// std::cout << "resultado 2" << '\n';
	// for(i=0;i<talks;i++){
	// 	if(charlas[i].cruce != 9999999){
	// 		cout << charlas[i].cruce <<"\t"<<charlas[i].charlas[0]+1<<"\t"<<charlas[i].charlas[1]+1<<"\t"<<charlas[i].charlas[2]+1<<'\n';
	// 	}
	// }
	charlasrestantes.clear();
	lrestantes = 0;
	// std::cout << "segunda tanda" << '\n';
	for(int h = 0; h<talks; h++){
		if(charlas[h].considerado == 0){
			charlasrestantes.push_back(h);
			lrestantes++;
			// cout <<"charla: "<<h+1<<'\n';
		}
	}
	for(int h = 0 ; h<lrestantes;h++){
		if(charlas[charlasrestantes[h]].considerado == 0){
			charlas[charlasrestantes[h]].considerado = 1;
			charlas[charlasrestantes[h]].charlas[0] = charlasrestantes[h];
			charlas[charlasrestantes[h]].n=1;
		}
		int changed = 0;
		for(int j = 0 ; j <lrestantes;j++){
			if(h!=j&& charlas[charlasrestantes[j]].considerado!=1){
				int newsum = 0;
				for(int p = 0 ; p<people;p++){
					int tempsum = 0;
					for(int g = 0 ; g< charlas[charlasrestantes[h]].n;g++){
						tempsum = tempsum + preferences[p][charlas[charlasrestantes[h]].charlas[g]];
					}

					tempsum = tempsum + preferences[p][charlasrestantes[j]];
					if(tempsum != 0){
						newsum= newsum + tempsum-1;
					}
				}
				if (!checkconst1(charlasrestantes[h],charlas[charlasrestantes[h]].charlas,charlas[charlasrestantes[h]].n,rtcc)){
					newsum = newsum*2;
				}
				// std::cout << "nueva suma : "<<newsum <<" Cruce : " <<charlas[charlasrestantes[h]].cruce<< '\n';
				if(charlas[charlasrestantes[h]].cruce>newsum&&charlas[charlasrestantes[h]].n != sessions){
					changed = 1;
					charlas[charlasrestantes[h]].charlas[charlas[charlasrestantes[h]].n] =charlasrestantes[j];
					charlas[charlasrestantes[h]].cruce = newsum;
				}
			}
		}
		if(changed == 1){
			charlas[charlas[charlasrestantes[h]].charlas[charlas[charlasrestantes[h]].n]].considerado = 1;
			charlas[charlasrestantes[h]].n++;
			for(int p = 1 ; p<charlas[charlasrestantes[h]].n;p++){
				charlas[charlas[charlasrestantes[h]].charlas[p]].n = charlas[charlasrestantes[h]].n;
			}
		}
		// std::cout << "charla restante: "<<charlas[charlasrestantes[h]].n <<"En h:"<<h<< '\n';
		if(charlas[charlasrestantes[h]].n != sessions){
			charlas[charlasrestantes[h]].cruce = 9999999;
			h--;
		}
		// std::cout << "H:"<<h << '\n';

	}
	int conflicto = 0,thorario = 0;
	// string nfile="Resultados_"+(string)argv[1];
	// ofstream out(nfile);
	// streambuf *coutbuf = cout.rdbuf();
	// cout.rdbuf(out.rdbuf());
	cout <<"Conflictos"<<"\t";
	for(i = 0;i<sessions; i++){
		cout <<"Ch"<<i+1<< "\t";
	}
	std::cout <<'\n';
	for(i=0;i<talks;i++){
		if(charlas[i].cruce != 9999999){
			cout << charlas[i].cruce <<"\t\t\t";
			for(int j=0; j<sessions; j++){
				horario[j].push_back((int)charlas[i].charlas[j]);
				cout <<charlas[i].charlas[j]+1<<"\t";
			}
			std::cout  << '\n';
			conflicto = conflicto+ charlas[i].cruce;
			thorario++;
		}
	}
	std::cout <<"\n"<< "Conflicto total: "<< conflicto<< '\n';
	int swapl= 0;
	for(i= 0; i<thorario;i++){
		if(charlas[horario[0][i]].cruce!= 0){
			for(int j=0; j<sessions; j++){
				swaps[j].push_back(horario[j][i]);
			}
			swaps[sessions].push_back(charlas[horario[0][i]].cruce);
			swapl++;
		}
	}
	// std::cout << "Antes Hill Climbing Fase 1" << '\n';
	// for (i= 0; i<thorario;i++){
	// 	std::cout << horario[0][i]<<"\t"<<horario[1][i]<<"\t" <<horario[2][i]<< '\n';
	// }
	// cout.rdbuf(coutbuf);

	int temp;
	for(i = 0 ; i<swapl;i++){
		for(int z = 0 ; z<sessions ; z++){
			for(int j = i; j<swapl;j++){
				if(i!=j){
					for(int k = 0 ; k<sessions;k++){
						temp =  swaps[z][i];
						swaps[z][i] = swaps[k][j];
						swaps[k][j] = temp;

						int cruceswap = 0;
						int cruceswap2 = 0;
						for(int h = 0;h<people;h++){
							for(int  l = 0 ; l<sessions;l++){
								cruceswap += preferences[h][swaps[l][i]];
								cruceswap2 += preferences[h][swaps[l][j]];
							}
						}
						// std::cout <<"cruces: "<< cruceswap << '\n';
						int swapped=0,swapped2=0,ant1,ant2;
						ant1 = swaps[sessions][i];
						ant2 = swaps[sessions][j];
						if(cruceswap<swaps[sessions][i]){
							swaps[sessions][i] = cruceswap;
							swapped = 1;
						}
						if(cruceswap2<swaps[sessions][j] && swapped == 1){
							swapped = 1;
							swaps[sessions][j] = cruceswap;
						}
						if((swapped == 1 && swapped2 == 0 )||(swapped == 0 && swapped2 == 1) || (swapped == 0 && swapped2==0)){
							swaps[k][j] = swaps[z][i];
							swaps[z][i] = temp;
							swaps[sessions][j] = ant2;
							swaps[sessions][i] = ant1;

						}
					}
				}
			}
		}
	}
	int pos = 0;
	for(i= 0; i<thorario;i++){
		if(charlas[horario[0][i]].cruce!= 0){
			for(int j=0; j<sessions; j++){
				horario[j][i]= swaps[j][pos];
			}
			horario[sessions].push_back(swaps[sessions][pos]);
			pos++;
		}
	}
	// cout.rdbuf(coutbuf);
	conflicto = 0 ;
	std::cout << "Despues Hill Climbing Fase 1" << '\n';
	cout <<"Conflictos"<<"\t";
	for(i = 0;i<sessions; i++){
		cout <<"Ch"<<i+1<< "\t";
	}
	std::cout <<'\n';
	for (i= 0; i<thorario;i++){
		std::cout << horario[sessions][i] << "\t\t\t";
		conflicto += horario[sessions][i];
		for(int j = 0; j<sessions;j++){
			std::cout << horario[j][i]+1<<"\t";
		}
		std::cout  << '\n';
	}
	std::cout << "Conflicto total: "<<conflicto << '\n';
	pos = 0;
	for(i = 0; i<days;i++){
		dbloques[i].resize(bloques[i].size());
		for(int j = 0 ;(unsigned)j<bloques[i].size();j++){
			if(bloques[i][j] != 0){
				for(int k = 0 ; k<bloques[i][j];k++){
					for(int z = 0 ; z<sessions;z++){
						dbloques[i][j].charlas.push_back(horario[z][pos]);
					}
					pos++;
				}
			}
			dbloques[i][j].bsize = bloques[i][j];
			dbloques[i][j].total = 0;
		}
	}
	std::cout << "Horario Formado" << '\n';
	for (i= 0; i<days;i++){
		std::cout << "Dia: "<<i << '\n';
		for(int j = 0 ; (unsigned)j<dbloques[i].size();j++){
			std::cout << "Bloque: "<<j << '\n';
			for(int k = 0; (unsigned)k<dbloques[i][j].charlas.size();k++){
				if(k%sessions == 0 && k!= 0){
					std::cout << "|\n";
				}
				std::cout << dbloques[i][j].charlas[k]<<'\t';

			}
			std::cout  << "|\n------------------------------\n";
		}
	}
	int total1 = 0 ;
	int total = 0 ;
	vector<int> estado;
	vector<int> acumulado;
	int lugar;
	int profundidad = 0;
	for(int d = 0 ; d<days; d++){
		for(int b = 0 ; (unsigned)b<dbloques[d].size();b++){
			if(dbloques[d][b].bsize == 1){
				dbloques[d][b].total = 0 ;
			}
			else if(dbloques[d][b].bsize!=0){
				for (i = 0 ; i<sessions ; i++){
					total = 0 ;
					int actual = i;
					estado.resize(dbloques[d][b].bsize);
					acumulado.resize(dbloques[d][b].bsize);
					std::fill(estado.begin(),estado.end(),0);
					std::fill(acumulado.begin(),acumulado.end(),0);
					estado[0] = i;
					lugar = sessions;
					profundidad = 0 ;
					while(lugar!= i){
						for(int j = lugar; (unsigned)j<dbloques[d][b].charlas.size(); j++ ){
							if(j%sessions != actual%sessions &&  (int)floor(j/sessions) != (int)floor(actual/sessions)){
								estado[(int)floor(j/sessions)] = j;
								int cont = 0;
								for(int p = 0 ; p<people;p++){
									if (preferences[p][dbloques[d][b].charlas[actual]] == 1 && preferences[p][dbloques[d][b].charlas[j]] == 1){
										cont++;
									}
								}
								if((int)floor(j/sessions) != 0){
									acumulado[(int)floor(j/sessions)] = acumulado[(int)floor(j/sessions)-1] + cont;
								}else{
									acumulado[(int)floor(j/sessions)] = cont;
								}
								// std::cout << "path : "<<dbloques[d][b].charlas[actual]<<"-"<<dbloques[d][b].charlas[j] << '\n';
								total= total + cont;
								actual = j;
							}
						}
						if(estado.back() == (sessions*dbloques[d][b].bsize)-1 || ((estado.back() == (sessions*dbloques[d][b].bsize) -2) && (estado[dbloques[d][b].bsize -2] == (sessions*dbloques[d][b].bsize)- sessions-1))){
							profundidad++;
							lugar = estado[dbloques[d][b].bsize -profundidad];
							if(lugar != i){
								lugar++;
								actual = estado[dbloques[d][b].bsize -profundidad -1];
							}
						}else{
							if(acumulado[acumulado.size()-1] > dbloques[d][b].total){
								dbloques[d][b].total = acumulado[acumulado.size()-1];
							}
							lugar = estado.back()+1;
							actual = estado[(int)floor(estado.back()/sessions) -1];
						}
					}
				}
			}
			// std::cout << "d: " <<d<<"b:"<<b<< '\n';
			total1+= dbloques[d][b].total;
			// std::cout << "total del bloque: "<<dbloques[d][b].total << '\n';
		}
	}
	std::cout << "Total de saltos antes de Hill Climbing: "<< total1 << '\n';
	int anterior=0 ;
	int siguiente= 0;
	for(int d = 0 ; d<days; d++){
		for(int b = 0 ; (unsigned)b<dbloques[d].size();b++){
					for(int k = 0;(unsigned)k<dbloques[d][b].charlas.size();k++){
						anterior = dbloques[d][b].charlas[k];
						for(int r = k ; (unsigned)r<dbloques[d][b].charlas.size();r++){
							// std::cout << "R:"<<r << '\n';
							siguiente = dbloques[d][b].charlas[r];
							dbloques[d][b].charlas[r] = anterior;
							dbloques[d][b].charlas[k] = siguiente;

					if(dbloques[d][b].bsize!=0 && dbloques[d][b].bsize != 1){
						for (i = 0 ; i<sessions ; i++){
							total = 0 ;
							int actual = i;
							estado.resize(dbloques[d][b].bsize);
							acumulado.resize(dbloques[d][b].bsize);
							std::fill(estado.begin(),estado.end(),0);
							std::fill(acumulado.begin(),acumulado.end(),0);
							estado[0] = i;
							lugar = sessions;
							profundidad = 0 ;
							while(lugar!= i){
								for(int j = lugar; (unsigned)j<dbloques[d][b].charlas.size(); j++ ){
									if(j%sessions != actual%sessions &&  (int)floor(j/sessions) != (int)floor(actual/sessions)){
										estado[(int)floor(j/sessions)] = j;
										int cont = 0;
										for(int p = 0 ; p<people;p++){
											if (preferences[p][dbloques[d][b].charlas[actual]] == 1 && preferences[p][dbloques[d][b].charlas[j]] == 1){
												cont++;
											}
										}
										if((int)floor(j/sessions) != 0){
											acumulado[(int)floor(j/sessions)] = acumulado[(int)floor(j/sessions)-1] + cont;
										}else{
											acumulado[(int)floor(j/sessions)] = cont;
										}
										actual = j;
									}
								}
								if(estado.back() == (sessions*dbloques[d][b].bsize)-1 || ((estado.back() == (sessions*dbloques[d][b].bsize) -2) && (estado[dbloques[d][b].bsize -2] == (sessions*dbloques[d][b].bsize)- sessions-1))){
									profundidad++;
									lugar = estado[dbloques[d][b].bsize -profundidad];
									if(lugar != i){
										lugar++;
										actual = estado[dbloques[d][b].bsize -profundidad -1];
									}
								}else{
									if(acumulado[acumulado.size()-1] > total){
										total = acumulado[acumulado.size()-1];
									}
									lugar = estado.back()+1;
									actual = estado[(int)floor(estado.back()/sessions) -1];
								}
							}
						}
					}
					if(total <dbloques[d][b].total){
						dbloques[d][b].total = total;
						break;
					}else{
						dbloques[d][b].charlas[r] = siguiente;
						dbloques[d][b].charlas[k] = anterior;
					}
				}
			}
		}
	}
	std::cout << "Horario Final" << '\n';
	for (i= 0; i<days;i++){
		std::cout << "Dia: "<<i << '\n';
		for(int j = 0 ; (unsigned)j<dbloques[i].size();j++){
			std::cout << "Bloque: "<<j << '\n';
			for(int k = 0; (unsigned)k<dbloques[i][j].charlas.size();k++){
				if(k%sessions == 0 && k!= 0){
					std::cout << "|\n";
				}
				std::cout << dbloques[i][j].charlas[k]<<'\t';

			}
			std::cout  << "|\n------------------------------\n";
		}
	}
	int totalhops = 0;
	for(int d = 0 ; d<days; d++){
		for(int b = 0 ; (unsigned)b<dbloques[d].size();b++){
			totalhops += dbloques[d][b].total;
		}
	}
	std::cout << "Total de saltos Hill Climbing:" <<totalhops<< '\n';



	//Genetico
	int Poblacion = 100;
	vector<Gen> pob;
	// std::cerr << "arg"<<stoi(argv[3]) << '\n';
	int n_iter = stoi(argv[3]);
	Gen bestgene;
	int n_actual = 0;
	int mejor = 30000;
	pob.resize(Poblacion);
	float selected = 0.8;
	// int mejorhop = 99999;
	// int mejorconflicto = 9999999;
	while(n_iter != n_actual){
		for(int x = 0; x<Poblacion; x++){
			pob[x]= generateGen(days,talks,bloques,sessions);
			int total = 0 ;
			vector<int> estado;
			vector<int> acumulado;
			int lugar;
			int profundidad = 0;
			for(int d = 0 ; d<days; d++){
				for(int b = 0 ; (unsigned)b<pob[x].horario[d].size();b++){
					if(pob[x].horario[d][b].bsize == 1){
						pob[x].horario[d][b].total = 0 ;
					}
					else if(pob[x].horario[d][b].bsize!=0){
						for (i = 0 ; i<sessions ; i++){
							total = 0 ;
							int actual = i;
							estado.resize(pob[x].horario[d][b].bsize);
							acumulado.resize(pob[x].horario[d][b].bsize);
							std::fill(estado.begin(),estado.end(),0);
							std::fill(acumulado.begin(),acumulado.end(),0);
							estado[0] = i;
							lugar = sessions;
							profundidad = 0 ;
							while(lugar!= i){
								for(int j = lugar; (unsigned)j<pob[x].horario[d][b].charlas.size(); j++ ){
									if(j%sessions != actual%sessions &&  (int)floor(j/sessions) != (int)floor(actual/sessions)){
										estado[(int)floor(j/sessions)] = j;
										int cont = 0;
										for(int p = 0 ; p<people;p++){
											if (preferences[p][pob[x].horario[d][b].charlas[actual]] == 1 && preferences[p][pob[x].horario[d][b].charlas[j]] == 1){
												cont++;
											}
										}
										if((int)floor(j/sessions) != 0){
											acumulado[(int)floor(j/sessions)] = acumulado[(int)floor(j/sessions)-1] + cont;
										}else{
											acumulado[(int)floor(j/sessions)] = cont;
										}
										// std::cout << "path : "<<dbloques[d][b].charlas[actual]<<"-"<<dbloques[d][b].charlas[j] << '\n';
										total= total + cont;
										actual = j;
									}
								}
								if(estado.back() == (sessions*pob[x].horario[d][b].bsize)-1 || ((estado.back() == (sessions*pob[x].horario[d][b].bsize) -2) && (estado[pob[x].horario[d][b].bsize -2] == (sessions*pob[x].horario[d][b].bsize)- sessions-1))){
									profundidad++;
									lugar = estado[pob[x].horario[d][b].bsize -profundidad];
									if(lugar != i){
										lugar++;
										actual = estado[pob[x].horario[d][b].bsize -profundidad -1];
									}
								}else{
									if(acumulado[acumulado.size()-1] > pob[x].horario[d][b].total){
										pob[x].horario[d][b].total = acumulado[acumulado.size()-1];
									}
									lugar = estado.back()+1;
									actual = estado[(int)floor(estado.back()/sessions) -1];
								}
							}
						}
					}
					// std::cout << "d: " <<d<<"b:"<<b<< '\n';
					pob[x].hops += pob[x].horario[d][b].total;
					pob[x].costo+= pob[x].horario[d][b].total;
					// std::cout << "total del bloque: "<<pob[x].costo << '\n';
				}
			}
			for(int i= 0 ; (unsigned)i<pob[x].horario.size();i++){
				for(int j = 0 ; (unsigned)j<pob[x].horario[i].size();j++){
					for(int k = 0 ; k<pob[x].horario[i][j].bsize;k++){
						for(int pre = 0 ; (unsigned)pre<preferences.size();pre++){
							int temp = 0;
							for(int l = 0 ; l<sessions;l++){
								temp+= preferences[pre][pob[x].horario[i][j].charlas[l+sessions*k]];
							}
							if(temp>1){
								temp-=1;
							}else{
								temp = 0 ;
							}
							pob[x].conflictos += temp;
							pob[x].costo +=temp;
						}
					}
				}
			}
			// std::cout << "total = "<<pob[x].costo << '\n';
		}

		vector<Gen> nextgen;
		if(n_actual%stoi(argv[4]) == 0){
			selected /=2;
		}
		if(selected<0.1){
			selected= 0.1;
		}
		int size = Poblacion * selected;
		sort(pob.begin(),pob.end(),compareInterval);
		for(int i = 0 ; i<size;i++){
			nextgen.push_back(pob[i]);
		}
		if(mejor>nextgen[0].costo){
			bestgene = nextgen[0];
			mejor = nextgen[0].costo;
			// mejorhop = nextgen[0].hops;
			// mejorconflicto = nextgen[0].conflictos;
		}

		// std::cout << n_actual <<";"<<nextgen[0].costo<<";"<<mejor<<";"<<'\n';

		while(nextgen.size() != (unsigned)Poblacion){
			int randp = rand() % size;
			int randp2 = rand() %size;
			nextgen.push_back(CrossGen(nextgen[randp],nextgen[randp2],sessions,talks));
		}
		pob = nextgen;
		n_actual++;
	}
	std::cout << "Mejor Horario Genetico" << '\n';
	for (i= 0; i<days;i++){
		std::cout << "Dia: "<<i << '\n';
		for(int j = 0 ; (unsigned)j<bestgene.horario[i].size();j++){
			std::cout << "Bloque: "<<j << '\n';
			for(int k = 0; (unsigned)k<bestgene.horario[i][j].charlas.size();k++){
				if(k%sessions == 0 && k!= 0){
					std::cout << "|\n";
				}
				std::cout << bestgene.horario[i][j].charlas[k]<<'\t';

			}
			std::cout  << "|\n------------------------------\n";
		}
	}
	std::cout << "Total Hops: "<<bestgene.hops << '\n';
	std::cout << "Total Conflictos: "<<bestgene.conflictos << '\n';
	std::cout << "Suma: "<<bestgene.costo << '\n';
	cout.rdbuf(coutbuf);





  return 0;
}
