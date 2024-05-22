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
    //Inicializa o mapa que guarda as categorias de trabalho que cada pessoa desempenhou
    unordered_map<string, unordered_set<string>> CategoriasPessoas;

    //Inicializa o mapa que nos permite acessar o "primaryName" de cada pessoa através da váriavel "nconst"
    unordered_map<string, string> NconstParaNome;
    
    //Inicializa o vetor com o conjunto de pessoas que desempanharam múltiplas categorias de trabalho
    vector<string> Pessoas;


    //Verifica se "seriesTconst" é encontrado em "SeriesMap"
    if(SeriesMap.find(seriesTconst) == SeriesMap.end())
    {
        //Caso não seja, significa que não existem episódios ou pessoas, logo retorna um vetor vazio
        return Pessoas;
    }


    //Verifica se existem episódios da série no mapa de episódios, "EpisodesMap"
    if(EpisodesMap.find(seriesTconst) != EpisodesMap.end())
    {
        //Guarda os episódios da série em "EpisodiosSerie"
        auto& EpisodiosSerie = EpisodesMap.at(seriesTconst);

        //Ciclo "for" que precorre os episódios da série
        for(auto& Episodio : EpisodiosSerie)
        {
            //Verifica que pessoas participaram neste episódio
            if(PeopleMap.find(Episodio.tconst) != PeopleMap.end())
            {
                //Guarda os pessoas que participaram no episódio em "PessoasEpisodio"
                auto& PessoasEpisodio = PeopleMap.at(Episodio.tconst);

                //Ciclo "for" que precorre as pessoas do episódio
                for(auto& Pessoa : PessoasEpisodio)
                {
                    //Adiciona a categoria da pessoa ao seu conjunto de categorias
                    CategoriasPessoas[Pessoa.nconst].insert(Pessoa.category);

                    //Adiciona ao mapa "NconstParaNome" o "primaryName" da pessoa que corresponde a nconst
                    NconstParaNome[Pessoa.nconst] = Pessoa.primaryName;
                }
            }
        }
    }


    //Ciclo "for" que precorre "CategoriasPessoas"
    for(auto& i : CategoriasPessoas) 
    {
        //Verifica se a pessoa tem mais do que uma categoria associada
        if(i.second.size() > 1) 
        {
            //Caso tenha, adiciona o nome da pessoa ao vetor "Pessoas" para ser retornado
            Pessoas.push_back(NconstParaNome[i.first]);
        }
    }


    //Ordena alfabeticamente o vetor "Pessoas" antes de ser retornado
    sort(Pessoas.begin(), Pessoas.end());

    return Pessoas;
}










//PERGUNTA 4

vector<string> TVSeriesAPP::principalsInAllEpisodes(const string& seriesTconst) const
{
    //Inicializa um mapa para guardar o número de episódios em que cada pessoa aparece
    unordered_map<string, int> ContagemParticipaçoes;

    //Inicializa o mapa que nos permite acessar o "primaryName" de cada pessoa através da váriavel "nconst"
    unordered_map<string, string> NconstParaNome;

    //Inicializa o vetor com o conjunto de pessoas que participaram em todos os episódios
    vector<string> Pessoas;


    //Verifica se se a série existe no mapa "SeriesMap"
    if(SeriesMap.find(seriesTconst) == SeriesMap.end()) 
    {
        //Caso não exista, retorna o vetor "Pessoas" ainda vazio
        return Pessoas;
    }


    //Inicializa a variável "totalEpisodes" com o número total de episódios da série
    int totalEpisodes = EpisodesMap.at(seriesTconst).size();


    //Ciclo "for" que precorre "CategoriasPessoas"
    for(const auto& Episodio : EpisodesMap.at(seriesTconst))
    {
        //Verifica quais foram as pessoas que participaram no episódio
        if (PeopleMap.find(Episodio.tconst) != PeopleMap.end())
        {
            //Guarda os pessoas que participaram no episódio em "PessoasEpisodio"
            const auto& PessoasEpisodio = PeopleMap.at(Episodio.tconst);

            //Conjunto de pessoas que já foram contadas, usado para evitar repetições na contagem
            unordered_set<string> PessoasContadas;

            //Ciclo "for" que precorre as pessoas que participaram no episódio
            for(const auto& principal : PessoasEpisodio)
            {
                //Verifica se a pessoa já foi contada
                if(PessoasContadas.find(principal.nconst) == PessoasContadas.end())
                {
                    ContagemParticipaçoes[principal.nconst]++; //Incrementa em 1 a partipação da pessoa
                    PessoasContadas.insert(principal.nconst); //Adiciona a pessoa ao conjunto de contados com nconst
                    NconstParaNome[principal.nconst] = principal.primaryName; //Adiciona ao mapa "NconstParaNome" o "primaryName" da pessoa que corresponde a nconst
                }
            }
        }
    }


    //Ciclo "for" que precorre "ContagemParticipaçoes"
    for(const auto& entry : ContagemParticipaçoes) 
    {
        //Verifica se a pessoa aprece em todos os episódios
        if(entry.second == totalEpisodes)
        {
            //Caso tenha participado, adiciona a pessoa ao vetor "Pessoas"
            Pessoas.push_back(NconstParaNome[entry.first]);
        }
    }
    

    //Ordena alfabeticamente o vetor "Pessoas" antes de ser retornado
    sort(Pessoas.begin(), Pessoas.end());

    return Pessoas;
}










//PERGUNTA 5:

int TVSeriesAPP::principalInMultipleGenres(vector<string> vGenres)
{
    unordered_map<string, unordered_set<string>> GenerosPessoas; //mapa para guardar, para cada pessoa, um conjunto dos gêneros das séries em que participou

    // para cada série no mapa de titulos verifica-se se a série tem os gêneros pedidos e se tem episódios 
    for(auto& i : SeriesMap) 
    {
        TitleBasics& Titulo = i.second; // título atual

        // para cada gênero da série atual se o genero estiver no vetor de gêneros pedidos e se a série tem episódios
        for(string& Genero : Titulo.genres) 
        {
            if(find(vGenres.begin(), vGenres.end(), Genero) != vGenres.end() && EpisodesMap.find(Titulo.tconst) != EpisodesMap.end())
            {
                auto& episodes = EpisodesMap.at(Titulo.tconst);
                for(auto& episode : episodes) //para cada episódio da série atual
                {
                    if(PeopleMap.find(episode.tconst) != PeopleMap.end()) //se houver principais para o episódio atual
                    {
                        auto& PessoasEpisodio = PeopleMap.at(episode.tconst);

                        for(auto& Pessoa : PessoasEpisodio) // para cada principal no episódio atual
                        {
                            GenerosPessoas[Pessoa.nconst].insert(Genero); // adiciona o gênero ao conjunto de gêneros da pessoa atual
                        }
                    }
                }
            }
        }
    }

    
    unordered_set<string> principalsInGenres; //guarda as pessoas que participaram em todas as séries dos gêneros pedidos

    for (auto& i : GenerosPessoas) //PAra cada pessoa no mapa
    {
        if (i.second.size() == vGenres.size()) //se a pessoa participou em todas as séries dos gêneros pedidos
        {
            principalsInGenres.insert(i.first); // adiciona a pessoa
        }
    }

    return principalsInGenres.size(); //dá return do númeor de pessoas
}










//PERGUNTA 6: 
string TVSeriesAPP::getPrincipalFromCharacter(const string& character) const
{
    return "";
}