#include "TVseries.hpp"
#include <iostream>
#include <algorithm>
#include <functional>
#include <stack>

TVSeriesAPP::TVSeriesAPP()
{
    auto TitleBasicsMap = unordered_map<string, TitleBasics>();
    auto TitleEpisodesMap = unordered_map<string, TitleEpisode>();
    auto TitlePrincipalsMap = unordered_map<string, TitlePrincipals>();
}


TVSeriesAPP::~TVSeriesAPP()
{
    TitleBasicsMap.clear();
    TitleEpisodesMap.clear();
    TitlePrincipalsMap.clear();
}





void TVSeriesAPP::addTitleBasics(const TitleBasics& title)
{
    
}


void TVSeriesAPP::addTitleEpisodes(const TitleEpisode& episode)
{
  
}


void TVSeriesAPP::addTitlePrincipal(const TitlePrincipals& principal)
{

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

string TVSeriesAPP::getPrincipalFromCharacter(const string& character) const
{
    return "";
}










//PERGUNTA 4

vector<string> TVSeriesAPP::principalsWithMultipleCategories(const string& seriesTconst ) const
{
    return {};
}










//PERGUNTA 5:

int TVSeriesAPP::principalInMultipleGenres(vector<string> vGenres)
{
    return -1;
}










//PERGUNTA 6: 
vector<string> TVSeriesAPP::principalsInAllEpisodes(const string& seriesTconst) const
{
    return {};
}
