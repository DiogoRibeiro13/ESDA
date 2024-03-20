#include <iostream>

using namespace std;

class Aluno
{
public: // Porque e', para ja', importante? Experimente comentar a linha...
  string nome;
  string curso;
  int numero;
  float media;
  
  Aluno(string nom)
  {
    nome = nom;
    curso = "";
    numero = 0;
    media = 0;
  }
  
  Aluno(string nom, string cur, int num)
  {
    nome = nom;
    curso = cur;
    numero = num;
    media = 0;
  }

  string getNome() const
  {
    return this->nome;
  }

  string getCurso() const
  {
    return this->curso;  
  }

  int getNumero() const
  {
    return this->numero;  
  }

  float getMedia() const
  {
    return this->media; 
  }

  void setNome(string nom)
  {
    this->nome = nom;
  }

  void setCurso(string cur)
  {
    this->curso = cur;
  }

  void setNumero(int num)
  {
    this->numero = num;
  }

  void setMedia(float med)
  {
    this->media = med;
  }

  void imprimir (ostream & os) const
  {
    //cout << this->nome;
    //cout << " | " << this->curso;
    //cout << " | " << this->numero;
    //cout << " | " << this->media << endl;

    os << this->nome;
    os << " | " << this->curso;
    os << " | " << this->numero;
    os << " | " << this->media << endl;
  }

  string sigla() const
  {
    string iniciais = "";
    bool nova = true;
    for (char c : nome)
    {
      if (nova && c != ' ')
      {
        iniciais += toupper(c);
        nova = false;
      }
      else if (c == ' ')
      {
        nova = true;
      }
    } 

    return iniciais;
  }

};

int main()
{

  Aluno a1("Pedro Ferreira");
  Aluno a2("Ana Martins", "Direito", 2022045);
  Aluno a3("Manuel Silva", "Arquitetura", 2021033);

  a1.setCurso("Medicina");
  a1.setNumero(2020123);

  cout << "Lista de alunos: (nome | curso | numero | media)" << endl;
  cout << "\t" << a1.nome << "\t| " << a1.curso << "\t| " << a1.numero << "\t| " << a1.media << endl;
  cout << "\t" << a2.nome << "\t| " << a2.curso << "\t| " << a2.numero << "\t| " << a2.media << endl;
  cout << "\t" << a3.nome << "\t| " << a3.curso << "\t| " << a3.numero << "\t| " << a3.media << endl;

  Aluno al("Alberto Mateus", "Engenharia", 2019197);
  al.imprimir(cout); 
  cout << "Sigla de " << al.nome << ": " << al.sigla() << endl;

  return 0;
}