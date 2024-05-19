#include "TVseries.hpp"
#include <iostream>
#include <algorithm>
#include <functional>
#include <stack>


TVSeriesAPP::TVSeriesAPP()
{
    SeriesMap = unordered_map<string, TitleBasics>();
    EpisodesMap = unordered_map<string, vector<TitleEpisode>>();
    PeopleMap = unordered_map<string, vector<TitlePrincipals>>();
}


TVSeriesAPP::~TVSeriesAPP()
{
    SeriesMap.clear();
    EpisodesMap.clear();
    PeopleMap.clear();  
}





void TVSeriesAPP::addTitleBasics(const TitleBasics& title)
{
    SeriesMap[title.tconst] = title;  
}


void TVSeriesAPP::addTitleEpisodes(const TitleEpisode& episode)
{
    EpisodesMap[episode.parentTconst].push_back(episode);
}


void TVSeriesAPP::addTitlePrincipal(const TitlePrincipals& principal)
{
    PeopleMap[principal.tconst].push_back(principal);
}





//PERGUNTA 1:

vector<string> TVSeriesAPP::getUniquePrincipals(const string& seriesTconst ) const
{
    return {};
}










//PERGUNTA 2:

string TVSeriesAPP::getMostSeriesGenre() const
{
    return "";
}










//PERGUNTA 3: 

vector<string> TVSeriesAPP::principalsWithMultipleCategories(const string& seriesTconst ) const
{
    return {};
}










//PERGUNTA 4

vector<string> TVSeriesAPP::principalsInAllEpisodes(const string& seriesTconst) const
{
    return {};
}










//PERGUNTA 5:

int TVSeriesAPP::principalInMultipleGenres(vector<string> vGenres)
{
    return -1;
}










//PERGUNTA 6: 
string TVSeriesAPP::getPrincipalFromCharacter(const string& character) const
{
    return "";
}
