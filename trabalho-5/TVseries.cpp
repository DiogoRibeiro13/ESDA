#include "TVseries.hpp"
#include <iostream>
#include <algorithm>
#include <functional>
#include <stack>
#include <unordered_set>


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
    //Inicializa o vetor que contém os participantes da série, para depois ser retornado
    vector<string> Participantes; 

    
    //Verifica se "seriesTconst" existe no mapa de séries
    if(SeriesMap.find(seriesTconst) != SeriesMap.end())
    {
        //Verifica se "seriesTconst" existe no mapa de episódios
        if(EpisodesMap.find(seriesTconst) != EpisodesMap.end()) 
        {
            //Se "seriesTconst" existir em ambos, então podemos analisar os episódios


            //"SeriesEpisodes" retira do mapa de episódios, "EpisodesMap", os episódios relativos à série que queremos 
            auto& SeriesEpisodes = EpisodesMap.at(seriesTconst);
            
            //Ciclo "for" para percorrer os episódios da série
            for(auto& i : SeriesEpisodes)
            {
                //Verifica se as pessoas que procuramos existem no mapa de pessoas, "PeopleMap"
                if(PeopleMap.find(i.tconst) != PeopleMap.end())
                {
                    //Se existirem, retira do mapa essas pessoas para a variável "PessoasEp"
                    auto& PessoasEp = PeopleMap.at(i.tconst);
                    
                    //Ciclo "for" para percorrer todas as pessoas
                    for(auto& j : PessoasEp)
                    {
                        //Adiciona ao vetor "Participantes" as pessoas que participaram no episódio
                        Participantes.push_back(j.primaryName);
                    }
                }
            }
        }
    }


    //Ordena o vetor por ordem alfabética
    sort(Participantes.begin(), Participantes.end());
    
    //Remove participantes que estejam duplicados
    Participantes.erase(unique(Participantes.begin(), Participantes.end()), Participantes.end());

    return Participantes;
}










//PERGUNTA 2:

string TVSeriesAPP::getMostSeriesGenre() const
{
    //Inicializa um mapa desordenado para guardar a contagem dos géneros
    unordered_map<string, int> ContagemGeneros;

    //Inicializa uma "string" para guardar o género mais frequente e a variável "Max", para guardar o valor máximo
    string GeneroMaisFreq;
    int Max = 0;


    //Ciclo "for" que precorre o mapa de séries
    for(auto& i : SeriesMap)
    {
        //Cria uma cópia do "TitleBasics" da série atual
        const TitleBasics& SerieBasics = i.second;

        //Ciclo "for" para precorrer os géneros das série
        for(const string& GeneroAtual : SerieBasics.genres)
        {
            //Incrementa em 1 a contagem do género atual
            ContagemGeneros[GeneroAtual]++;
        }
    }


    //Loop "for" para precorrer a contagem de géneros, "ContagemGeneros"
    for(auto& i : ContagemGeneros)
    {
        //Guarda o género e o seu valor na contagem nas variáveis "GeneroAtual" e "ValorCont" respetivamente
        const string& GeneroAtual = i.first;
        int ValorCont = i.second;

        //Verifica se "GeneroAtual" tem o valor na contagem maior do que o género mais frequente
        //Ou, caso o valor seja igual, verifica se tem menos caracteres no nome
        if(ValorCont > Max || (ValorCont == Max && GeneroAtual.length() < GeneroMaisFreq.length()))
        {
            //Caso cumpra as condições acima, passa a ser o género mais frequente
            GeneroMaisFreq = GeneroAtual;
            Max = ValorCont;
        }
    }


    return GeneroMaisFreq;
}










//PERGUNTA 3: 

vector<string> TVSeriesAPP::principalsWithMultipleCategories(const string& seriesTconst ) const
{
    unordered_map<string, unordered_set<string>> principalCategories;
    unordered_map<string, string> nconstToName;
    vector<string> principals;


    // Verificar se a série existe
    if(SeriesMap.find(seriesTconst) == SeriesMap.end())
    {
        return principals; // Retorna vetor vazio
    }


    // Iterar sobre todos os episódios da série
    if(EpisodesMap.find(seriesTconst) != EpisodesMap.end())
    {
        const auto& episodes = EpisodesMap.at(seriesTconst);

        for(const auto& episode : episodes)
        {
            // Verificar se existem principais para este episódio
            if(PeopleMap.find(episode.tconst) != PeopleMap.end())
            {
                const auto& principalsForEpisode = PeopleMap.at(episode.tconst);

                for(const auto& principal : principalsForEpisode)
                {
                    // Adicionar a categoria do principal ao conjunto de categorias
                    principalCategories[principal.nconst].insert(principal.category);

                    // Mapear nconst para primaryName
                    nconstToName[principal.nconst] = principal.primaryName;
                }
            }
        }
    }


    // Iterar sobre todos os principais e suas categorias
    for(const auto& entry : principalCategories) 
    {
        // Se o principal tem mais de uma categoria, adicione-o à lista
        if(entry.second.size() > 1) 
        {
            principals.push_back(nconstToName[entry.first]);
        }
    }


    // Ordenar a lista de principais alfabeticamente
    sort(principals.begin(), principals.end());


    return principals;
}










//PERGUNTA 4

vector<string> TVSeriesAPP::principalsInAllEpisodes(const string& seriesTconst) const
{
    unordered_map<string, int> principalEpisodeCount;
    unordered_map<string, string> nconstToName;
    vector<string> principals;

    // Verificar se a série existe
    if (SeriesMap.find(seriesTconst) == SeriesMap.end()) {
        return principals; // Retorna vetor vazio
    }

    // Contar o número total de episódios da série
    int totalEpisodes = EpisodesMap.at(seriesTconst).size();

    // Iterar sobre todos os episódios da série
    for (const auto& episode : EpisodesMap.at(seriesTconst)) {
        // Verificar se existem principais para este episódio
        if (PeopleMap.find(episode.tconst) != PeopleMap.end()) {
            const auto& principalsForEpisode = PeopleMap.at(episode.tconst);
            unordered_set<string> countedPrincipals;
            for (const auto& principal : principalsForEpisode) {
                // Se o principal ainda não foi contado para este episódio, incrementar a contagem
                if (countedPrincipals.find(principal.nconst) == countedPrincipals.end()) {
                    principalEpisodeCount[principal.nconst]++;
                    countedPrincipals.insert(principal.nconst);
                    // Mapear nconst para primaryName
                    nconstToName[principal.nconst] = principal.primaryName;
                }
            }
        }
    }

    // Iterar sobre todos os principais e suas contagens de episódios
    for (const auto& entry : principalEpisodeCount) 
    {
        // Se o principal aparece em todos os episódios, adicione-o à lista
        if (entry.second == totalEpisodes) 
        {
            principals.push_back(nconstToName[entry.first]);
        }
    }

    // Ordenar a lista de principais alfabeticamente
    sort(principals.begin(), principals.end());

    return principals;
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
