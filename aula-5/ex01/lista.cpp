#include <iostream>
#include <fstream>
#include <string>
#include <list>

using namespace std;

void imprime_lista(list<string> lst)
{
  list<string>::iterator it;

  for(it = lst.begin(); it != lst.end(); it++)
  {
    cout << *it << endl;
  }
}

int main()
{

  // a) criar a lista

  list <string> lst_jogos; 

  // b) inserir jogos na lista

  ifstream file("arquivo.txt");

  if(!file) return -1;

  string line;

  while(getline(file,line)) lst_jogos.push_back(line);

  // c) imprimir tamanho da lista

  cout<<"Foram carregados "<<lst_jogos.size()<<" jogos."<<endl;

  // d) imprimir conteúdo da lista

  int i = 0;

  for(auto it = lst_jogos.begin(); it != lst_jogos.end(); it++)
  {
    cout << "Pos " << i << " -> " << *it << endl;
    i++;
  }
  
  // e) ordenar lista e imprimir

  lst_jogos.sort();

  int i = 0;

  cout<<"Lista Ordenada:"<<endl;

  for(auto it = lst_jogos.begin(); it != lst_jogos.end(); it++)
  {
    cout <<"Pos "<<i<<" -> "<< *it << endl;
    i++;
  }
  
  // f) remover "Mario Kart (Wii)" e imprimir

  int i = 0;

  for(auto it=lst_jogos.begin();it!=lst_jogos.end();++it) {
      if((*it).find("Mario Kart (Wii)") != string::npos)
      {
        lst_jogos.erase(it);
       break;
      }
  }

  cout<<"Lista sem o jogo Mario Kart (Wii):"<<endl;

  for(auto it = lst_jogos.begin(); it != lst_jogos.end(); it++)
  {
    cout <<"Pos "<<i<<" -> "<< *it << endl;
    i++;
  }
  
  // g) remover jogos PS2 e imprimir

  int i = 0;
  auto it = lst_jogos.begin();
  while(it!=lst_jogos.end()) {
      if((*it).find("(PS2)") != string::npos) it = lst_jogos.erase(it);
      else it++;
  }

  cout<<"Lista sem jogos da PS2:"<<endl;

  for(auto it = lst_jogos.begin(); it != lst_jogos.end(); it++)
  {
    cout <<"Pos "<<i<<" -> "<< *it << endl;
    i++;
  }

  cout << "\nLista sem jogos da PS2:" << endl;

  imprime_lista(lst_jogos);
  
  return 0;
}