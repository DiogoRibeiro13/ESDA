#include <vector>
#include <string>
#include <iostream>
#include <sstream>
#include <fstream>
#include <algorithm>
#include "TVseries.hpp"

using namespace std;

TVSeries::TVSeries(string seriesTitle, int seasons, vector<int> episodesPerSeason, string seriesGenre, float seriesRating, bool isFinished) :
    title(seriesTitle), numberOfSeasons(seasons), episodesPerSeason(episodesPerSeason), genre(seriesGenre), rating(seriesRating), finished(isFinished) {};

string TVSeries::getTitle() const { return title; }

int TVSeries::getNumberOfSeasons() const { return numberOfSeasons; }

vector<int> TVSeries::getEpisodesPerSeason() const { return episodesPerSeason; }

string TVSeries::getGenre() const { return genre; }

float TVSeries::getRating() const { return rating; }

bool TVSeries::isFinished() const { return finished; }

void TVSeries::displaySeriesInfo() const
{
   }

User::User(string uname, string completeName, string userCountry, vector<string> genres) :
    username(uname), name(completeName), country(userCountry), favoriteGenres(genres) {};

string User::getUsername() const { return username; }

string User::getName() const { return name; }

vector<string> User::getFavoriteGenres() const { return favoriteGenres; }

vector<TVSeries*> User::getWatchedSeries() const { return watchedSeries; }

vector<int> User::getRatings() const { return ratings; }

vector<int> User::getEpisodesWatched() const { return episodesWatched; }

int User::addFavoriteGenre(int genreIdx)
{
    // Check if the index of the genre exists in 'vGenres'
    if (!(genreIdx >= 0 && genreIdx < N_GENRES)) return -1;

    // Check if the genre already exists in the vector
    if (find(favoriteGenres.begin(), favoriteGenres.end(), vGenres[genreIdx]) == favoriteGenres.end()) {
        // Genre doesn't exist, so add it to the vector
        favoriteGenres.push_back(vGenres[genreIdx]);
    }

    // Genre already exists, do nothing
    //cout << "Genre already exists in favorite genres." << endl;
    return 1;
}

int User::addWatchedSeries(TVSeries* series)
{
    // Check if the series pointer is valid
    if (series == nullptr) {
        cout << "Error: Invalid series pointer." << endl;
        return -1; // Return -1 to indicate an error
    }

    // Check if the series is already in watchedSeries
    auto it = find(watchedSeries.begin(), watchedSeries.end(), series);
    if (it != watchedSeries.end()) {
       // cout << "Series is already in watched series." << endl;
        return 1; // Series already exists, no need to insert
    }

    // Series is not in watchedSeries, so insert it
    watchedSeries.push_back(series);
    episodesWatched.push_back(0); // Initialize episodesWatched for the new series
    ratings.push_back(0);
    //cout << "New watched series inserted." << endl;
    return 0; // Successfully inserted
}

int User::addEpisodesWatched(TVSeries* series, int n)
{
    // Check if the series pointer is valid
    if (series == nullptr) {
        cout << "Error: Invalid series pointer." << endl;
        return -1; // Exit the function if the series pointer is invalid
    }

    // Find the index of the series in the watchedSeries vector
    auto it = find(watchedSeries.begin(), watchedSeries.end(), series);
    if (it != watchedSeries.end()) {
        // Calculate the index of the series
        int index = distance(watchedSeries.begin(), it);

        // Check if the series has more episodes than the episodes watched
        vector <int> episodesperseason = series->getEpisodesPerSeason(); 
        int totalepisodes = 0;
        for (auto it = episodesperseason.begin(); it!=episodesperseason.end(); it++){
            totalepisodes += *it;
        }
        //cout << "Total episodes: " << totalepisodes;

        // ! Check if total episodes is smaller
        if (n > 0) {
            episodesWatched[index] = min(totalepisodes, n); // Update to the minimum of n or total episodes
        } else {
            // Increment the number of episodes watched by 1 if it's less than the total episodes
            if (episodesWatched[index] < totalepisodes) {
                episodesWatched[index]++;
            }
        }
        return 0;
    } else {
       // cout << "Error: Series not found in watched series." << endl;
        return -2;
    }
}

TVSeriesManagement::TVSeriesManagement(){};

vector<TVSeries*> TVSeriesManagement::getVectorTVSeries() const { return vectorTVSeries; }

int TVSeriesManagement::TVSeriesDelete(string title)
{
    for (auto it = vectorTVSeries.begin(); it != vectorTVSeries.end(); ++it) {
        if ((*it)->getTitle() == title) {
            delete *it; // Free the memory allocated for the TVSerie object
            vectorTVSeries.erase(it); // Remove the pointer from the vector
            return 0; // Series deleted successfully
        }
    }
    return -1; // Series not found
}

UserManagement::UserManagement(){};

vector<User*> UserManagement::getVectorUsers() const { return vectorUsers; }

void UserManagement::addUser(User* newUser)
{
    vectorUsers.push_back(newUser);
}











/**************************/
/*     A implementar      */
/**************************/






void User::displayUserInfo(ostream& os) const
{
    //Imprime o username, nome, e país
    
    cout << "Displaying user info:" << endl;
    cout << "-----Username: " << username << endl;
    cout << "-----Name: " << name << endl;
    cout << "-----Country: " << country << endl;

    
    cout << "-----Favorite Genres:" << endl;
    
    size_t i;   //Variável usada nos ciclos "for"

    //Ciclo "for" que se repete para cada género favorito do user
    
    for(i = 0; i < favoriteGenres.size(); i++)
    {
        //Imprime um género favorito no index atual

        cout << "------ " << favoriteGenres[i] << " " << endl;
    }
    

    cout << "-----Watched Series:" << endl;

    //Ciclo "for" que se repete para cada série visualizada pelo user

    for(i = 0; i < watchedSeries.size(); i++)
    {
        //Imprime o título e números de episódios visualizados para a série no index atual

        cout << "------ " << watchedSeries[i]->getTitle() << ", ";
        cout << "Episodes Watched: " << episodesWatched[i] << endl;
    }
}











int User::addRating(TVSeries* series, float rating)
{
    bool SeriesFound = false;   //Variável que é "true" ou "false" dependendo se encontramos ou não a série que queriamos, respetivamente
    size_t i;   //Variável utilizada no ciclo "for"
    

    //Verifica que o valor de rating é maior que 0 e que o apontador para a série não é nulo
    //Se uma das 2 condições acontecer, temos um erro e retorna -1 (Código de Erro)

    if(rating < 0 || series == nullptr)
    {
        return -1;
    }


    //Ciclo "for" que se repete para cada série vista pelo user

    for(i = 0; i < watchedSeries.size(); i++)
    {
        //Verifica se a série no index atual do vetor for igual á que procuramos

        if(watchedSeries[i] == series)
        {
            //Se sim "SeriesFound" passa a "true", e adicionamos o rating to user ao vetor "ratings"

            SeriesFound = true;
            ratings[i] = rating;
        }
    }
    

    //Se "SeriesFound" se mantiver "false" então a série não foi visualizada pelo utlizador
    //Retorna -2 (Código para quando a série não foi vista)

    if(SeriesFound == false)
    {
        return -2;
    }


    //Se tudo estiver bem, chega a esta parte do código e retorna 0 (Código de Sucesso)

    return 0;
}











float TVSeries::updateRating(const vector<User*>& vectorUser)
{
    float TotalRating = 0;   //Soma dos ratings de cada vetor
    int users = 0;   //Número de users que viram a série
    size_t i, j;   //Variáveis usadas em ciclos "for"


    //Ciclo "for" que se repete para cada user
    //A verificação não é feita no próximo ciclo para evitar iterações desnecessárias

    for(i = 0; i < vectorUser.size(); i++)
    {
        //Verifica que não existem apontadores para user que sejam nulos
        //Se existirem retorna -1

        if(vectorUser[i] == nullptr)
        {
            return -1;
        }
    }


    //Ciclo "for" que se repete para cada user

    for(i = 0; i < vectorUser.size(); i++)
    {
        //Ciclo "for" que se repete para cada série vista pelo user

        for(j = 0; j < vectorUser[i]->getWatchedSeries().size(); j++)
        {
            //Verifica se o título no index atual é igual ao que procuramos

            if(vectorUser[i]->getWatchedSeries()[j]->title == title)
            {
                //Se for igual, acrescentamos 1 ao número de user que viram a série 
                //E adicionamos o rating do user ao total de ratings da série

                users++;
                TotalRating += vectorUser[i]->getRatings()[j];
            }
        }
    }


    //Verifica se existem users que viram a série

    if(users != 0)
    {
        //Caso exista pelo menos 1 user que tenha visto, calcula o rating e retorna o seu valor

        rating = TotalRating / users;

        return rating;
    }

    //Caso não existam nenhum user que tenha visto, retorna 0 (Código de Erro)

    return 0;
}











int TVSeriesManagement::TVSeriesInsert(TVSeries* series)
{
    //Verifica se o apontador para a série é nulo
    //Se sim retorna -1 (Código de Erro)
    
    if(series == nullptr) 
    {
        return -1;
    }


    size_t i;   //Variável utilizada do ciclo "for"

    //Ciclo "for" que precorre as séries no vetor "VectorTVSeries"
    
    for(i = 0; i < getVectorTVSeries().size(); i++)
    {
        //Verifica se a série no index atual é igual á que procuramos
        //Se for igual então a série já existe

        if(getVectorTVSeries()[i]->getTitle() == series->getTitle())
        {
            //Atualiza a série que já existe e retorna 1 (Código para série que já existe)
            
            getVectorTVSeries()[i] = series;
            return 1;
        }
    }


    //Se a série ainda não existir, adiciona-a no final do vetor "vectorTVSeries" 
    //E retorna 0 (Código de Sucesso)
    
    vectorTVSeries.push_back(series);
    return 0;
}











int UserManagement::updateWatched(string filename, TVSeriesManagement& manager)
{
    ifstream file(filename);   //Abre o ficheiro guardado na string "filename"
    
    //Verifica se o nome do ficheiro não está vazia e se o mesmo abre corretamente
    //Em caso de erro, retorna -1 (Código de Erro)
    
    if(filename == "" || !file.is_open())
    {
        return -1;
    }


    string line; //String que vai guardar uma linha do ficheiro


    //Ciclo "while" com condição getline lê uma linha do ficheiro para a string "line" a cada iteração
    //A função getline retorna um valor "true" se conseguir ler, o que faz com que o "while" seja executado

    while(getline(file, line))
    {
        stringstream ss(line);   //Cria uma stringstream para a string "line"
        ss.clear();   //Dá clear à stringstream

        string SeriesTitle, Username, EpisodesWatched;   //Variáveis string que vão guardar parte da informação de cada linha do ficheiro

        bool SeriesFound = false;   //Variável que é "true" ou "false" dependendo se encontramos ou não a Série que queriamos, respetivamente
        bool UserFound = false;   //Variável que é "true" ou "false" dependendo se encontramos ou não o User que queriamos, respetivamente

        size_t i, j;   //Variáveis usadas nos ciclos "for"

        //Copia o vetor "VectorTVSeries" para um vetor do mesmo tipo "CopySeries"
        //Isto é necessário uma vez que "VectorTVSeries" é privado
        vector<TVSeries*> CopySeries = manager.getVectorTVSeries();

        //Copia o vetor "VectorUsers" para um vetor do mesmo tipo "CopyUsers"
        //Isto é necessário uma vez que "VectorUsers" é privado
        vector<User*> CopyUsers = getVectorUsers();


        //Estrutura "if" com a confição getline, que lê uma parta da stringstream "ss" para a respetiva variável
        //A função getline retorna um valor "true" se conseguir ler, o que faz com que o "if" seja executado

        if(getline(ss, SeriesTitle, ',') && getline(ss, Username, ',') && (ss >> EpisodesWatched))
        {
            //Ciclo "for" que precorre o vetor com todas as séries da plataforma

            for(i = 0; i < CopySeries.size(); i++)
            {
                //Verifica se a série no index atual é igual á série que procuramos

                if(CopySeries[i]->getTitle() == SeriesTitle)
                {
                    //Se sim, então a série existe e foi encontrada, logo "SeriesFound" passa a ter valor "true"
                    //Não são necessárias mais iterações logo damos break

                    SeriesFound = true;
                    break;
                }
            }

            //Caso o valor de "SeriesFound" se mantenha a "false" retorna -2 (Código para série que não existe)

            if(SeriesFound == false)
            {

                return -2;
            }   


            //Ciclo "for" que precorre o vetor com todos os users

            for(j = 0; j < CopyUsers.size(); j++)
            {
                //Verifica se o user no index atual é igual ao user que procuramos

                if(CopyUsers[j]->getUsername() == Username)
                {
                    //Se sim, então o user já existe e foi encontrado, logo "UserFound" passa a ter valor "true"
                    //Não são necessárias mais iterações logo damos break

                    UserFound = true;
                    break;
                }
            }

            //Caso o valor de "UserFound" se mantenha a "false" o user não existe e precisa ser criado

            if(UserFound == false)
            {
                vector<string> fav;   //Vetor com os géneros de séries favoritos do user
                
                User newuser(Username, "Unknown", "Unknown", fav);   //Criação de um novo user com os parâmetros "name" e "país" como "Unknown"
                
                User* AptUser = &newuser;   //Criação de um apontador para o novo user

                addUser(AptUser);   //Adiciona o novo user
            }   


            //Adiciona a série que está na posição "i" do vetor "VectorTVSeries", ao vetor das séries assistidas pelo user que está na posição "j" do vetor "VectorUsers"
            //Retorna 0 em caso de sucesso, 1 caso a série já tenha sido vista, e -1 em caso de erro

            int value = vectorUsers[j]->addWatchedSeries(CopySeries[i]);

            //Caso "value" tenha valor de -1 então ocorreu um erro

            if(value == -1)
            {
                //Sendo assim fechamos o ficheiro e retorna -1 (Código de Erro)

                file.close();
                return -1;
            }


            //Como "EpisodesWatched" é uma string, usamos a função stoi para convertar para int

            int EpWat = stoi(EpisodesWatched);

            //Adiciona o número de episódios visualizados, pelo user que está na posição "j" do vetor "VectorUsers", à série que está na posição "i" do vetor "VectorTVSeries"
            //Retorna 0 em caso de sucesso, -1 caso os parâmetros sejam inválidos, e -2 caso a série não exista

            int value2 = vectorUsers[j]->addEpisodesWatched(CopySeries[i],EpWat);
            
            //Caso o valor de "value2" seja -1 ou -2, ambos códigos de erro, fecha o ficheiro e retorna -1 (Código de Erro)

            if(value2 == -1 || value2 == -2)
            {
                file.close();
                return -1;
            }
        }
    }

    //Se tudo funcionar corretamente no código, ele fecha o ficheiro e retorna 0 (Código de Sucesso)

    file.close();
    return 0;
}