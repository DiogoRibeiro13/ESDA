#include <vector>
#include <string>
#include <queue>
#include <list>
#include <iostream>
#include <sstream>
#include <fstream>
#include <algorithm>
#include "TVseries.hpp"

using namespace std;


// implementation of class TVSeries

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
    cout << "Displaying TV series info:" << endl;
    cout << "-----Title: " << title << endl;
    cout << "-----Number of Seasons: " << numberOfSeasons << endl;
    cout << "-----Episodes per Season:" << endl;
    for (size_t i = 0; i < episodesPerSeason.size(); ++i) {
        cout << "-----Season " << (i + 1) << ": " << episodesPerSeason[i] << " episodes" << endl;
    }
    cout << "-----Genre: " << genre << endl;
    cout << "-----Rating: " << rating << endl;
    cout << "-----Finished: " << (finished ? "Yes" : "No") << endl;
}


float TVSeries::updateRating(const vector<User*>& vectorUser)
{
    if (vectorUser.empty()) {
        cout << "Error: No users provided." << endl;
        return -1; // Return -1 in case of error
    }

    float totalRating = 0;
    int numRatings = 0;

    // Iterate through each user
    for ( User* user : vectorUser) {
        // Check if the user watched this series
        vector<TVSeries*> &watchedSeries = user->getWatchedSeries();
        auto it = find(watchedSeries.begin(), watchedSeries.end(), this);
        if (it != watchedSeries.end()) {
            // User watched this series, get the rating
            int index = distance(watchedSeries.begin(), it);
            totalRating += user->getRatings()[index];
            numRatings++;
        }
    }

    // Calculate the average rating
    if (numRatings > 0) {
        rating = totalRating / numRatings;
        return rating;
    } else {
        cout << "Error: No users have rated this series." << endl;
        return 0; // Return 0 
    }
}

// implementation of class User

User::User(string uname, string completeName, string userCountry, vector<string> genres) :
    username(uname), name(completeName), country(userCountry), favoriteGenres(genres) {};

string User::getUsername() const { return username; }

string User::getName() const { return name; }

vector<string> User::getFavoriteGenres() const { return favoriteGenres; }

vector<TVSeries*>& User::getWatchedSeries() { return watchedSeries; }

vector<int>& User::getRatings()  { return ratings; }

vector<int>& User::getEpisodesWatched()  { return episodesWatched; }

queue<TVSeries*>& User::getWishSeries()   { return wishSeries; }

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
    cout << "Genre already exists in favorite genres." << endl;
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
        cout << "Series is already in watched series." << endl;
        return 1; // Series already exists, no need to insert
    }

    // Series is not in watchedSeries, so insert it
    watchedSeries.push_back(series);
    episodesWatched.push_back(0); // Initialize episodesWatched for the new series
    ratings.push_back(0);
  //  cout << "New watched series inserted." << endl;
    return 0; // Successfully inserted
}
int User::addWishSeries(TVSeries* series)
{
    // Check if the series pointer is valid
    if (series == nullptr) {
        cout << "Error: Invalid series pointer." << endl;
        return -1; // Return -1 to indicate an error
    }

    // Check if the series is already in watchedSeries
    queue<TVSeries*> aux;
    bool teste=false;
    while(!wishSeries.empty())
    {
        if (wishSeries.front()==series)
        {
            teste=true;
        }
        aux.push(wishSeries.front());
        wishSeries.pop();
    }
    while(!aux.empty())
    {
        wishSeries.push(aux.front());
        aux.pop();
    }
    if (teste) return -1;
    // Series is not in wishSeries, so insert it
    wishSeries.push(series);
 
   // cout << "New wish series inserted." << endl;
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
        cout << "Error: Series not found in watched series." << endl;
        return -2;
    }
}

void User::displayUserInfo(ostream& os) const
{
    os << "Displaying user info:" << endl;
    os << "-----Username: " << username << endl;
    os << "-----Name: " << name << endl;
    os << "-----Country: " << country << endl;
    os << "-----Favorite Genres:" << endl;
    for (auto ir = favoriteGenres.begin(); ir != favoriteGenres.end(); ++ir){
        os << "------ " << *ir << endl;
    }
    os << "-----Watched Series:" << endl;
    for (size_t i = 0; i < watchedSeries.size(); ++i) {
        os << "------ " << watchedSeries[i]->getTitle() << ", Episodes Watched: " << episodesWatched[i] << endl;
    }
    cout << endl;
}

int User::addRating(TVSeries* series, float rating)
{
   // Check if the series pointer is valid
    if (series == nullptr) {
       // cout << "Error: Invalid series pointer." << endl;
        return -1; // Return -1 to indicate an error
    }

    // Find the index of the series in the watchedSeries vector
    auto it = find(watchedSeries.begin(), watchedSeries.end(), series);
    if (it != watchedSeries.end()) {
        // Calculate the index of the series
        size_t index = distance(watchedSeries.begin(), it);

        // Check if the rating vector has enough space
        if (index >= this->ratings.size()) {
            // Resize the rating vector to accommodate the new rating
            this->ratings.resize(index + 1, 0);
        }

        // Add the rating to the corresponding index
        this->ratings[index] = rating;
        return 0; // Successfully added rating
    } else {
       // cout << "Error: Series not found in watched series." << endl;
        return -2; // Return -1 to indicate series not found
    } 
}

// implementation of class TVSeriesManagement

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



int TVSeriesManagement::TVSeriesInsert(TVSeries* series)
{
    // Check if the series pointer is valid
    if (series == nullptr) {
        cout << "Error: Invalid series pointer." << endl;
        return -1; // Return -1 to indicate an error
    }
  if (vectorTVSeries.size()==0)
  {
    vectorTVSeries.push_back(series);
    return 0;
  }
    auto it = find(vectorTVSeries.begin(), vectorTVSeries.end(), series);


    if (it == vectorTVSeries.end()) {
        vectorTVSeries.push_back(series);
      
        return 0;
    } else { return 1;}

    return -1; // Series not found
}


// implementation of class UserManagement

UserManagement::UserManagement(){};

vector<User*> UserManagement::getVectorUsers() const { return vectorUsers; }

void UserManagement::addUser(User* newUser)
{
    vectorUsers.push_back(newUser);
}

// implementation of class TVSeriesManagementList

TVSeriesManagementList::TVSeriesManagementList(){};

list<TVSeries*> TVSeriesManagementList::getListTVSeries() const { return listTVSeries; }

int TVSeriesManagementList::TVSeriesInsert(TVSeries* series)
{
    // Check if the series pointer is valid
    if (series == nullptr) {
        cout << "Error: Invalid series pointer." << endl;
        return -1; // Return -1 to indicate an error
    }
  if (listTVSeries.size()==0)
  {
    listTVSeries.push_back(series);
    return 0;
  }
    auto it = find(listTVSeries.begin(), listTVSeries.end(), series);


    if (it == listTVSeries.end()) {
        listTVSeries.push_back(series);
      
        return 0;
    } else { return 1;}

    return -1; // Series not found
}

// implementation of class UserManagementList

UserManagementList::UserManagementList(){};

list<User*> UserManagementList::getListUsers() const { return listUsers; }

void UserManagementList::addUser(User* newUser)
{
    listUsers.push_back(newUser);
}




/**************************/
/*     A implementar      */
/**************************/



list<TVSeries*> TVSeriesManagementList::seriesByCategory(string cat) const
{
    //Cria uma lista vazia "CatList" à qual serão adicionadas as séries da categoria "cat"
    list<TVSeries*> CatList;


    /* VERIFICAÇÃO DE SE "cat" É VALIDO */

    int i; //Variável para iteração no ciclo "for"
    bool ValidCat = false; //Variável que tem valor "false" caso "cat" não seja uma categoria valida, e "true" caso seja valida

    //Ciclo "for" que precorre todos os géneros disponíveis na plataforma
    for(i=0; i < (N_GENRES-1); i++)
    {
        //Verifica se o género "cat" corresponde a algum dos géneros existentes
        if(cat == vGenres[i])
        {
            //Caso corresponda então é um género válido e "ValidCat" passa a ter valor "true"
            //Como não precisamos procurar mais e por isso fazemos break
            ValidCat = true;
            break;
        }
    }

    //Caso "ValidCat" mantenha o valor "false", então a série é inválida e é retornada uma lista vazia
    if(ValidCat == false)
    {
        return CatList; //Notar que ainda não foi adicionado qualquer elemento à lista 
    }


    /* ADIÇÃO DE SÉRIES DA CATEGORIA "cat" À LISTA */

    //Ciclo "for" que precorre a lista com todas as séries disponíveis na plataforma
    for(auto j = listTVSeries.begin(); j != listTVSeries.end(); j++)
    {
        //Verifica se a categoria da série na posição atual é igual a "cat"
        if((*j)->getGenre() == cat)
        {
            //Caso seja, adiciona a série à lista "CatList"
            CatList.push_back(*j);
        }
    }

    //Retorna a lista "CatList" que contém todas as séries da categoria "cat"
    return CatList;
}










list<User*> UserManagementList::seeAll(TVSeries* series)
{
    //Cria um vetor "SeenAll" inicialmente vazio que vai conter os Users que viram todos os episódios de uma determinada série
    list<User*> SeenAll;


    /* VERIFICAÇÃO DE SE "series" É VALIDO */

    //Verifica se o apontador para a série em questão não é um nullptr
    if(series == nullptr)
    {
        //Caso seja, retorna o vetor "SeenAll" ainda vazio
        return SeenAll;
    }


    /* cÁLCULO DO NÚMERO TOTAL DE EPISÓDIOS DA SÉRIE */

    size_t Season; //Variável utilizada no ciclo "for" para representar a posição da temporada na iteração atual
    int TotalEps = 0; //Variável que representa o número total de episódios da série

    //Ciclo "for" que percorre o vetor de Episódios por Temporada
    for(Season = 0; Season < series->getEpisodesPerSeason().size(); Season++)
    {
        //A cada iteração soma o número de episódios da temporada na posição atual
        //No final do ciclo "for", "TotalEps" representará o número total de episódios da série
        TotalEps += series->getEpisodesPerSeason()[Season];
    }


    /* VERIFICAÇÃO DOS USERS QUE VIRAM TODOS OS EPISÓDIOS DA SÉRIE */

    size_t SerPos; //Variável utilizada no ciclo "for" para representar a posição da série na iteração atual

    //Ciclo "for" que percorre a lista com todos os users
    //A variável "UserPtr" aponta para outro apontador, que por sua vez aponta para o User na posição atual
    for(auto UserPtr = listUsers.begin(); UserPtr != listUsers.end(); UserPtr++)
    {
        //Cria um vetor do tipo "TVSeries*", "CopyWatSer", que é uma cópia do vetor de séries vistas pelo user na posição atual da lista de Users, "listUsers"
        vector<TVSeries*> CopyWatSer = (*UserPtr)->getWatchedSeries();
        
        //Ciclo "for" que percorre o vetor "CopyWatSer"
        for(SerPos=0; SerPos < CopyWatSer.size(); SerPos++)
        {
            //Verifica que o User viu a série apontada por "series" e de que viu os episódios todos
            if((CopyWatSer[SerPos] == series) && ((*UserPtr)->getEpisodesWatched()[SerPos] == TotalEps))
            {
                //Se isso for verdade, então o User à lista de Users que viram todos os episódios da série apontada por "series", "SeenAll"
                SeenAll.push_back(*UserPtr);
            }
        }
    }

    //Retorna a lista "SeenAll", agora preenchida no caso de existirem users que cumpram todas as condições
    return SeenAll;
}










int User::numberOfEpisodesToSee(string title, list<TVSeries*> listTVSeries )
{
    /* VERIFICAÇÃO DE SE "title" É VÁLIDO */
    
    //Verifica se o título, "title", não é uma string vazia
    if(title == "")
    {
        //Caso seja retorna -1
        return -1;
    }


    /* VERIFICAÇÃO DE SE A SÉRIE EXISTE NA PLATAFORMA */

    bool ValidSer = false; //Variável que tem valor "true" ou "false" dependo se a série indicada por "title" existe ou não na plataforma, respetivamente 

    //Ciclo "for" que precorre a lista "listTVSeries" com todas as séries da plataforma
    //A variável "SerPtr" aponta para outro apontador, que por sua vez aponta para a série na posição atual
    for(auto SerPtr = listTVSeries.begin(); SerPtr != listTVSeries.end(); SerPtr++)
    {
        //Verifica se a série na posição atual é igual á que procuramos, "title"
        if((*SerPtr)->getTitle() == title)
        {
            //Se for igual, "ValidSer" passa a ter valor "true"
            //Como a série já foi encontrada não precisamos procurar mais e por isso usamos break para sair do ciclo "for"
            ValidSer = true;
            break;
        }
    }

    //Se "ValidSer" se mantiver com valor "false" retorna -1
    if(ValidSer == false)
    {
        return -1;
    }


    /* VERIFICAÇÃO DE SE A SÉRIE EXISTE NA FILA "wishSeries" DO USER */

    queue<TVSeries*> CopyWish; //Cria uma fila, "CopyWish", que será um cópia da fila "wishSeries"
    CopyWish = getWishSeries(); //Desta forma "wishSeries" nunca é alterada
    bool ValidWish = false; //Variável que tem valor "true" ou "false" dependo se a série indicada por "title" existe ou não na fila de Wished Series do User, respetivamente

    //Ciclo "while" que se repete até que a fila "CopyWish" estar vazia
    while(!CopyWish.empty())
    {
        //Verifica se o próximo série na fila é igual à que procuramos, "title"
        if(CopyWish.front()->getTitle() == title)
        {
            //Se for igual, "ValidWish" passa a ter valor "true"
            //Como a série já foi encontrada não precisamos procurar mais e por isso usamos break para sair do ciclo "while"
            ValidWish = true;
            break;
        }

        CopyWish.pop(); //A cada iteração remove o elemento à frente na lista
    }

    //Se "ValidWish" se mantiver com valor "false" retorna -1
    if(ValidWish == false)
    {
        return -1;
    }

    
    /* CÁLCULO DOS EPISÓDIOS QUE FALTAM ATÉ A SÉRIE "title" */

    CopyWish = getWishSeries(); //Dá reset às mudanças feitas a "CopyWish"
    int i; //Variável para iteração do loop "for"
    int TotalEps = 0; //Váriavel que representa o número total de episódios a assitir até à série "title"

    //Ciclo "while" que se repete até que a fila "CopyWish" estar vazia
    while(!CopyWish.empty())
    {
        //Verifica se a próxima série na fila é a que procuramos, "title"
        if(CopyWish.front()->getTitle() == title)
        {
            break; //Caso seja não precisamos procurar mais e por isso usamos break para sair do ciclo "while"
        }

        else
        {
            //Ciclo "for" que se repete para cada temporada da série atual
            for(i=0; i < CopyWish.front()->getNumberOfSeasons(); i++)
            {
                //A cada iteração adiciona o número de episódios da temporada ao número de episódios que faltam ver
                TotalEps += CopyWish.front()->getEpisodesPerSeason()[i];
            }

            CopyWish.pop(); //A cada iteração remove o elemento à frente na lista
        }
    }

    //Retorna o número de episódios que faltam ver até chegar à série "title"
    return TotalEps;
}










int TVSeriesManagementList::TVSeriesDelete(string title, UserManagementList& userManagementlist)
{
    bool SerFound = false;
    
    for(auto SerPos = listTVSeries.begin(); SerPos != listTVSeries.end(); SerPos++)
    {
        if((*SerPos)->getTitle() == title)
        {
            SerFound = true;

            listTVSeries.erase(SerPos);
            break;
        }
        
    }
    
    if(SerFound == false)
    {
        return -1;
    }
    

    list<User*> CopyUserList = userManagementlist.getListUsers();
    queue<TVSeries*> AuxWish;

    for(auto UserPos = CopyUserList.begin(); UserPos != CopyUserList.end(); UserPos++)
    {
        for(auto WatSerPos = (*UserPos)->getWatchedSeries().begin(); WatSerPos != (*UserPos)->getWatchedSeries().end(); WatSerPos++)
        {
            if((*WatSerPos)->getTitle() == title)
            {
                (*UserPos)->getWatchedSeries().erase(WatSerPos);
                

                auto RatingPos = (*UserPos)->getRatings().begin() + (WatSerPos - (*UserPos)->getWatchedSeries().begin());
                (*UserPos)->getRatings().erase(RatingPos);
                

                auto EpWatPos = (*UserPos)->getEpisodesWatched().begin() + (WatSerPos - (*UserPos)->getWatchedSeries().begin());
                (*UserPos)->getEpisodesWatched().erase(EpWatPos);
                

                (*UserPos)->getWishSeries().pop();

                break;
            }

            else
            {
                AuxWish.push((*UserPos)->getWishSeries().front());

                (*UserPos)->getWishSeries().pop();
            }

            (*UserPos)->getWishSeries() = AuxWish;
        }
    }
    
    return 0;
}










list<TVSeries*> TVSeriesManagementList::suggestsSeries(string username,string userWhoSuggests, list<User*> userlist ) const
{
    list<TVSeries*> SuggestedSeries;

    bool UserExist = false;
    auto UserPos = userlist.begin();

    for(UserPos = userlist.begin(); UserPos != userlist.end(); UserPos++)
    {
        if(username == "" && UserPos == userlist.begin())
        {
            break;
        }

        if((*UserPos)->getName() == username)
        {
            UserExist = true;
            break;
        }
    }

    if(UserExist == false)
    {
        return SuggestedSeries;
    }


    bool UserSugExist = false;
    auto UserSugPos = userlist.begin();

    for(UserSugPos = userlist.begin(); UserSugPos != userlist.end(); UserSugPos++)
    {
        if(userWhoSuggests == "" && UserSugPos == userlist.begin())
        {
            break;
        }
        
        if((*UserSugPos)->getName() == userWhoSuggests)
        {
            UserSugExist = true;
            break;
        }
    } 


    size_t i;
    bool ValidGenre = false;
    bool NotWatched = true;

    if(UserSugExist == true)
    {
        for(auto SugSerPos = (*UserSugPos)->getWatchedSeries().begin(); SugSerPos != (*UserSugPos)->getWatchedSeries().end(); SugSerPos++)
        {
            for(i = 0; i < (*UserPos)->getFavoriteGenres().size(); i++)
            {
                if((*UserPos)->getFavoriteGenres()[i] == (*SugSerPos)->getGenre())
                {
                    ValidGenre = true;
                    break;
                }
            }

            if(ValidGenre == true)
            {
                for(auto SerPos = (*UserPos)->getWatchedSeries().begin(); SerPos != (*UserPos)->getWatchedSeries().end(); SerPos++)
                {
                    if((*SerPos)->getTitle() == (*SugSerPos)->getTitle())
                    {
                        NotWatched = false;
                        break;
                    }
                }
            }

            if(ValidGenre == true && NotWatched == true)
            {
                SuggestedSeries.push_front(*SugSerPos);
            }
        }
    }


    else
    {
        for(auto SerListPos = getListTVSeries().begin(); SerListPos != getListTVSeries().end(); SerListPos++)
        {
            for(i = 0; i < (*UserPos)->getFavoriteGenres().size(); i++)
            {
                if((*UserPos)->getFavoriteGenres()[i] == (*SerListPos)->getGenre())
                {
                    ValidGenre = true;
                    break;
                }
            }

            if(ValidGenre == true)
            {
                for(auto SerPos = (*UserPos)->getWatchedSeries().begin(); SerPos != (*UserPos)->getWatchedSeries().end(); SerPos++)
                {
                    if((*SerListPos)->getTitle() == (*SerPos)->getTitle())
                    {
                        NotWatched = false;
                        break;
                    }
                }
            }

            if(ValidGenre == true && NotWatched == true)
            {
                SuggestedSeries.push_back(*SerListPos);
            }
        }
    }


    return SuggestedSeries;
}