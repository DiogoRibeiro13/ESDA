#include <vector>
#include <string>
#include <queue>
#include <list>
#include <iostream>
#include <sstream>
#include <fstream>
#include <algorithm>
#include <cctype>
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
bool TVSeries::operator <(const TVSeries& tv) const
    {
        // this will return true when second TVSeries
        // has greater rating. Suppose we have tv1.rating=5
        // and tv2.rating=5.5 then the object which
        // have max rating will be at the top(or
        // max priority)
         return this->rating < tv.rating;
    }
// implementation of class User

User::User(string uname, string completeName, string userCountry, vector<string> genres) :
    username(uname), name(completeName), country(userCountry), favoriteGenres(genres), length(0) {};

string User::getUsername() const { return username; }

string User::getName() const { return name; }

string User::getCountry() const { return country; }

vector<string> User::getFavoriteGenres() const { return favoriteGenres; }

vector<TVSeries*>& User::getWatchedSeries() { return watchedSeries; }

vector<int>& User::getRatings()  { return ratings; }

vector<int>& User::getEpisodesWatched()  { return episodesWatched; }

queue<TVSeries*>& User::getWishSeries()   { return wishSeries; }

int User::getLength() const { return length; }

int User::setLength(int n)
{ if (n<0) return -1;
  length=n; 
  return 0; 
}

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
   //     cout << "Series is already in watched series." << endl;
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


int User::numberOfEpisodesToSee(string title, list<TVSeries*> listTVSeries )
{
    if((title.empty())|| listTVSeries.empty())
    {
        return -1;
    }
    int total=0;
    queue<TVSeries*> aux;
    vector<int> nepisodes;
    int n=wishSeries.size();
    bool exist=false;
    for (int i=0;i<n;i++)
    {
        
        if(wishSeries.front()->getTitle()==title)
        {
        exist=true;
        break;
        }
        aux.push(wishSeries.front());
        nepisodes=wishSeries.front()->getEpisodesPerSeason();
        for (int j=0;j<(int)nepisodes.size();j++)
        {
          total+=nepisodes[j];
        }
        wishSeries.pop();
    }
    while(!wishSeries.empty())
    {
        aux.push(wishSeries.front());
        wishSeries.pop();
    }
    while(!aux.empty())
    {
        wishSeries.push(aux.front());
        aux.pop();
    }
    if (!exist) total=0;
    return total;
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

int UserManagement::updateWatched(string filename, TVSeriesManagementList& manager)
{
    ifstream file(filename);
    if (!file.is_open()) {
        cout << "Error: Unable to open file." << endl;
        return -1;
    }

    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        string seriesName, username,eps;
        int episodesSeen;
        getline(ss, seriesName, ',');
        getline(ss, username, ',');
        getline(ss, eps, ',');
        episodesSeen = stoi(eps);
       // cout << seriesName << username << episodesSeen << endl;

        // Check if user exists, if not, create and add to vectorUser
        User* userPtr = nullptr;
        for (User* user : vectorUsers) {
            if (user->getUsername() == username) {
                userPtr = user;
                break;
            }
        }
        if (userPtr == nullptr) {
            // User does not exist, create and add to vectorUser
            vectorUsers.push_back(new User(username, "Unknown", "Unknown", {}));
            userPtr = vectorUsers.back();
        }
        // Check if the series exists
        bool seriesExists = false;
        for (const auto& seriesPtr : manager.getListTVSeries()) {
            
            if (seriesPtr->getTitle() == seriesName) {
                seriesExists = true;
                //add series to user. 
                userPtr->addWatchedSeries(seriesPtr);
                userPtr->addEpisodesWatched(seriesPtr, episodesSeen);
            }
        }

        if (!seriesExists) {
            cout << "TV series does not exist so it wasn't added to the user." << endl;
            continue;
        }
    }
    file.close();
    return 0;
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

list<TVSeries*> TVSeriesManagementList::seriesByCategory(string cat) const
{
  list<TVSeries*> ltv;
  if (cat.empty())
  return ltv;
  for( auto it=listTVSeries.begin();it!=listTVSeries.end();it++)
  {
 
    if ((*it)->getGenre()==cat)
    ltv.push_back((*it));
  }
  return ltv;

}


int TVSeriesManagementList::TVSeriesDelete(string title, UserManagementList& userManagementlist)
{
    if (title.empty())
    return -1;
    int nUser=0,nWatched=0;
    list<User*> listUsers=userManagementlist.getListUsers();
   // vector<TVSeries*>& watechedSeries;
    for(auto it=listTVSeries.begin();it!=listTVSeries.end();it++)
    {
        if ((*it)->getTitle()==title)
        {
            nUser=listUsers.size();
            for (auto itU=listUsers.begin();itU!=listUsers.end();itU++)
            {
                vector<TVSeries*>& watechedSeries=(*itU)->getWatchedSeries();
                nWatched=watechedSeries.size();
                for (int i=0; i<nWatched;i++)
                {
                    cout << watechedSeries[i]->getTitle()<<endl;
                    if(watechedSeries[i]->getTitle()==title)
                    {
                        
                        watechedSeries.erase(watechedSeries.begin()+i);
                        
                        vector<int>& vepisodes=(*itU)->getEpisodesWatched();
                        vepisodes.erase(vepisodes.begin()+i);
                        vector<int>& vrating=(*itU)->getRatings();
                        vrating.erase(vrating.begin()+i);
                    }
                }
            }
            delete(*it);
            listTVSeries.erase(it);
            return 0;
        }
    }
    return -1;
}

// implementation of class UserManagementList

UserManagementList::UserManagementList(){};

list<User*> UserManagementList::getListUsers() const { return listUsers; }

void UserManagementList::addUser(User* newUser)
{
    listUsers.push_back(newUser);
}

list<User*> UserManagementList::seeAll(TVSeries* series)
{
    list<User*> lUser;
    if (series==nullptr)
    {
        return lUser;
    }
    vector<TVSeries*> vWateched;
    vector<int> vepisodes;
    int total=0;
    vector<int> nepisodes;
    nepisodes=series->getEpisodesPerSeason();
    for (int j=0;j<(int)nepisodes.size();j++)
    {
        total+=nepisodes[j];
    }
    for(auto it=listUsers.begin();it!=listUsers.end();it++)
    {
        vWateched=(*it)->getWatchedSeries();
        for (int i=0;i<(int)vWateched.size();i++)
        {
            if ((vWateched[i]==series) && ((*it)->getEpisodesWatched()[i]==total))
            {
                lUser.push_back(*it);

            }
        }
    }
    return lUser;
}

list<TVSeries*> TVSeriesManagementList::suggestsSeries(string username,string userWhoSuggests, list<User*> userlist ) const
{
    list<TVSeries*> lTVSeries;
    if (username.empty()|| userlist.empty() )
    {
        return lTVSeries;
    }
    bool exist=false;
    auto itU=userlist.begin();
    for (itU=userlist.begin();itU!=userlist.end();itU++)
    {
        if((*itU)->getUsername()==username)
        {exist=true;
        break;}
    }
 
     
    if (!exist)
    {
        return lTVSeries;
    }
   vector<string> vcatU=(*itU)->getFavoriteGenres();
    vector<TVSeries*> vTVSeries=(*itU)->getWatchedSeries();
    vector<TVSeries*> vWhoTVSeries;
    exist=false;
    auto itWho=userlist.begin();
for (itWho=userlist.begin();itWho!=userlist.end();itWho++)
    {
        if((*itWho)->getUsername()==userWhoSuggests)
        {exist=true;
        break;}
    }

    if (!exist)
    {
        for(auto it=listTVSeries.begin();it!=listTVSeries.end();it++)
        {
            if ((find(vTVSeries.begin(),vTVSeries.end(),*it)==vTVSeries.end()) && (find(vcatU.begin(),vcatU.end(),(*it)->getGenre())!=vcatU.end()))
            {
                lTVSeries.push_back(*it);
            }
        }
    } else
    {
        vWhoTVSeries=(*itWho)->getWatchedSeries();
          for(auto it=vWhoTVSeries.begin();it!=vWhoTVSeries.end();it++)
        {
            if ((find(vTVSeries.begin(),vTVSeries.end(),*it)==vTVSeries.end()) && (find(vcatU.begin(),vcatU.end(),(*it)->getGenre())!=vcatU.end()))
            {
                lTVSeries.push_back(*it);
            }
        }
    }
   
    return lTVSeries;
}


// implementation of class NodeUser
NodeUser::NodeUser(User* user){
    this->user=user;
    left=nullptr;
    right=nullptr;
} 

// implementation of class UserManagementTree
UserManagementTree::UserManagementTree() {
    root=nullptr;
}

NodeUser* UserManagementTree::insertNode(NodeUser* root, User* newUser) {
        // If the tree is empty, create a new node and return it
        if (root == nullptr) {
            return new NodeUser(newUser);
        }

        // Compares username to decide where to insert the node
        if (newUser->getUsername() < root->user->getUsername()) {
            root->left = insertNode(root->left, newUser);
        } else if (newUser->getUsername() > root->user->getUsername()) {
            root->right = insertNode(root->right, newUser);
        }

        return root;
    }
    
NodeUser* UserManagementTree::findMinNode(NodeUser* node) {
    NodeUser* current = node;
    while (current && current->left != nullptr) {
        current = current->left;
    }
    return current;
    }

NodeUser* UserManagementTree::deleteNode(NodeUser* root, string user) {
    // Base case: empty tree
    if (root == nullptr) {
        return root;
    }

    // If the username to be deleted is smaller than the username of the current node, search the left subtree
    if (user < root->user->getUsername()) {
        root->left = deleteNode(root->left, user);
    }
    // If the username to be deleted is greater than the username of the current node, search the right subtree
    else if (user > root->user->getUsername()) {
        root->right = deleteNode(root->right, user);
    }
    // If the user is the same as the year of the current node, this is the node to be deleted
       else {
        // Case 1: node without children or with only one child
        if (root->left == nullptr) {
                NodeUser* temp = root->right;
                delete root;
                return temp;
        } else if (root->right == nullptr) {
                NodeUser* temp = root->left;
                delete root;
                return temp;
            }

        // Case 2: node with two children
        // Finds the minimum node of the right subtree (the successor)
        NodeUser* temp = findMinNode(root->right);
        // Copy successor to this node
        root->user = temp->user;
        // Exclui o sucessor
        root->right = deleteNode(root->right, temp->user->getUsername());
        }
    return root;
}

void UserManagementTree::inorderTraversal(NodeUser* root) {
    if (root != nullptr) {
        inorderTraversal(root->left);
        cout << root->user->getUsername() << " " << root->user->getName() << endl;
        inorderTraversal(root->right);
    }
}

NodeUser* UserManagementTree::getRoot() const
{
    return root;
}

void UserManagementTree::addUser(User* newUser) {
    root = insertNode(root, newUser);
}

void UserManagementTree::remove(string user) {
        root = deleteNode(root, user);
    }

void UserManagementTree::inorder() {
        inorderTraversal(root);
    }

priority_queue<TVSeries> UserManagement::queueTVSeriesCategory(priority_queue<TVSeries>& pq, string cat) {

  /*  priority_queue<TVSeries> pqnew;
    if (pq.empty() || cat.empty()) {
        return pqnew;
    }
    priority_queue<TVSeries> aux=pq;
    while(!aux.empty()) {
        if(aux.top().getGenre()==cat)
        {
            pqnew.push(aux.top());
        }

        aux.pop();
    }
    return pqnew;
    */priority_queue<TVSeries> pqnew;
       if (pq.empty() || cat.empty()) {
        return pqnew;
    }
    //priority_queue<TVSeries> aux=pq;
    while(!pq.empty()) {
        if(pq.top().getGenre()==cat)
        {
            pqnew.push(pq.top());
        }

        pq.pop();
    }
    return pqnew;
}


priority_queue<TVSeries> UserManagement::queueTVSeries(list<TVSeries*> listTV,int min) {
    priority_queue<TVSeries> q;
    if (listTV.empty()){
        return q;
    }
    int nUser=0;
    for (auto it=listTV.begin();it!=listTV.end();it++)
    {
        nUser=0;
   
        for ( int i=0;i<(int)vectorUsers.size();i++)
        {
           vector<TVSeries*> wtv=vectorUsers[i]->getWatchedSeries();
           vector<int> etv=vectorUsers[i]->getEpisodesWatched(); 
         
           for (auto j=0;j<wtv.size();j++)
           {
                if ((wtv[j]->getTitle()==(*it)->getTitle()) && (etv[j]>=2)) {
                    nUser++;
                 
                }
           }
    
        }
         if (nUser>=min) {
            q.push(**it);
        }
    }

    
    return q;
}

vector<User*> UserManagementTree::usersInitialLetter(NodeUser* root,char ch) {
    vector<User*> v;
    if (root==nullptr) {
        return v;
    }
    vector<User*> vleft;
    if (toupper(root->user->getUsername()[0])>=toupper(ch)) {
        vleft=usersInitialLetter(root->left,toupper(ch));
    }
    vector<User*> vright;
    if (toupper(root->user->getUsername()[0])<=toupper(ch)) {
        vright=usersInitialLetter(root->right,toupper(ch));
    }

    if  (toupper(root->user->getUsername()[0])==toupper(ch)) {
        v.push_back(root->user);
    }
 
    v.insert(v.end(),vleft.begin(),vleft.end());
    v.insert(v.end(),vright.begin(),vright.end());

    return v;
 }

list<User*> UserManagementTree::usersNotFan(NodeUser* root) {

    list<User*> lt;
    if (root==nullptr) {
        return lt;
    }

    list<User*> ltleft;
    ltleft= usersNotFan(root->left);
    list<User*> ltright;
    ltright= usersNotFan(root->right);

    vector<TVSeries*> wtv=root->user->getWatchedSeries();
    vector<int> etv=root->user->getEpisodesWatched();
    int total=0;
    int nc=0;
    for (auto i=0;i<wtv.size();i++)
    {
        vector<int> nepisodes=wtv[i]->getEpisodesPerSeason();
        total=0;
        for (int j=0;j<(int)nepisodes.size();j++)
        {
            total+=nepisodes[j];
        }
        if (total>etv[i]){
            nc++;
        }
    }
    if (nc>2) {
        lt.push_back(root->user);
    }

    lt.insert(lt.end(),ltleft.begin(),ltleft.end());
    lt.insert(lt.end(),ltright.begin(),ltright.end());

    return lt;        
}

vector<int> UserManagementTree::usersCategoryStatistics(NodeUser* root,string cat,int perc) {
    vector<int> v(3);
   
    if (root==nullptr || cat.empty() || perc<0 || perc>100){
    return v;
    }
 
    vector<TVSeries*> wtv=root->user->getWatchedSeries();
    vector<int> etv=root->user->getEpisodesWatched();
    int total=0;
 
    for (auto i=0;i<wtv.size();i++)
    {
        if ((wtv[i]->getGenre()==cat) && (etv[i]>=1)) {
            
            v[0]=1;
            vector<int> nepisodes=wtv[i]->getEpisodesPerSeason();
            total=0;
            for (int j=0;j<(int)nepisodes.size();j++)
            {
                total+=nepisodes[j];
            }
            if(etv[i]*100/total>=perc){
                v[1]=1;
                vector<string> fg=root->user->getFavoriteGenres();
                auto it=find(fg.begin(),fg.end(),cat);
                if (it!=fg.end()){
                    v[2]=1;
                }
            }
        }
    }    
    vector<int> vleft;
    vleft= usersCategoryStatistics(root->left,cat,perc);
    vector<int> vright;
    vright= usersCategoryStatistics(root->right,cat,perc);
    for (int i=0;i<3;i++){
    v[i]+=vleft[i]+vright[i];
    }
    return v;
}

HashTable::HashTable(int ts)
{
    if (ts>0)
    {
    tableSize = ts;
    totalCountryStats=0;
    table.resize(ts,nullptr);
  
    } else delete this;
}

/* DESTRUTOR */

HashTable::~HashTable()
{

   for (int i=0;i<tableSize;i++)
   {
       
        delete (table[i]);
            
   } 
 
}

int HashTable::getTableSize() const
{
    
    return tableSize;
}
int HashTable::getTotalCountryStats() const
{
    
    return totalCountryStats;
}
vector<CountryStats*> HashTable::getTable() const
{
    return table;
}

int HashTable::hashFunction(string key)
{
    if (!key.empty())
    {
    int h = 7; 
    for(int i = 0; key[i]; i++)
    {
        h += (int)key[i];
    }

    return h % tableSize;
    } else return -1;
}


int HashTable::probingFunction(string key,int i)
{
    if (!key.empty())
    {
    int h = hashFunction(key); 
   
        h += 23*i*i;
 

    return h % tableSize;
    } else return -1;
}

int HashTable::searchCountryStats(string country) 
{
            
    if (country.empty())
    {
        return -1;
    }
   
    int index = hashFunction(country);
    if (table[index]==nullptr) return -1;
    if (table[index]->country==country)
    {
        
        return index;
    } else 
    {
        int i=1;
        
        while((table[index]->country!=country))
        {
            
            index=probingFunction(country,i);
            if (table[index]==nullptr) return -1;
            if (table[index]->country==country)
            {
                return index;
            } 
            i++;
            
        }
    }
    return -1;        
}

int HashTable::deleteAccountStats(string country)
{       
    if (country.empty())
    {
        return -1;
    }
    int index = hashFunction(country);
    if (table[index]->country==country)
    {
        delete(table[index]);
        totalCountryStats--;
        CountryStats *as=new CountryStats("apagado",0,0,0.0,{0,0,0,0,0});
        table[index]=as;
        return 0;
    } else 
    {
        int i=1;
        while((table[index]!=nullptr))
        {
            index=probingFunction(country,i);
            if (table[index]->country==country)
            {
                delete(table[index]);
                totalCountryStats--;
                CountryStats *as=new CountryStats("apagado",0,0,0.0,{0,0,0,0,0});
                table[index]=as;
                return 0;
            } 
            i++;
        }
    }
    return -1; 

}     


void HashTable::show(void)      
{
    for(int i=0;i<tableSize;i++)
    {
        if (table[i]!=nullptr)
        {cout << i << "->" << table[i]->country << "-" <<table[i]->nUsers<< "-" <<table[i]->nTVSeries<<"-" <<table[i]->averageTVseries ;
        for (int j=0;j<N_GENRES; j++)
            {
            cout<<"-"<<vGenres[j]<<"("<<table[i]->nGenre[j]<<")"<<endl;
            } 
        }else
        cout << i << endl;
    }
}


UserManagementGraph::UserManagementGraph()
{
    totalUsers = 0;
    userNodes.clear();
    network.clear();
}

size_t UserManagementGraph::getTotal() const
{
    return totalUsers;
}

vector<User*> UserManagementGraph::getUserNodes() const
{
    return userNodes;
}

vector<list<User*>> UserManagementGraph::getNetwork() const
{
    return network;
}

int UserManagementGraph::addUserNode(User* userPtr)
{
    if (!userPtr) return -1;

    if (userNodePosition(userPtr) >= 0) return 1; // User already exists in the graph
    
    userNodes.push_back(userPtr);
    network.resize(userNodes.size());
    totalUsers = userNodes.size();
    return 0;
}

int UserManagementGraph::userNodePosition(User* userPtr)
{
    if (!userPtr) return -2;

    auto it = find(userNodes.begin(), userNodes.end(), userPtr);
    if (it == userNodes.end()) return -1; // Node does not exist

    return it - userNodes.begin();
}

int UserManagementGraph::addFollower(User* userA, User* userB)
{
    if (!userA || !userB) return -1;

    int posA = userNodePosition(userA), posB = userNodePosition(userB);

    if (posA == -2 || posB == -2) return -1; // Error

    // Check if the nodes exist
    if (posA == -1) {
        addUserNode(userA); // Add userA
        posA = userNodePosition(userA); // Update posA with userA position
    }
    if (posB == -1) {
        addUserNode(userB); // Add userB
        posB = userNodePosition(userB); // Update posB with userB position
    }

    // Check if the edge already exists
    if (find(network.at(posA).begin(), network.at(posA).end(), userB) != network.at(posA).end()) return 1;

    network.at(posA).push_back(userB);
    return 0;
}

int UserManagementGraph::removeFollower(User* userA, User* userB)
{
    if (!userA || !userB) return -1;

    int posA = userNodePosition(userA), posB = userNodePosition(userB);

    if (posA == -1 || posB == -1 || posA == -2 || posB == -2) return -1; // Error

    // Check if the edge does not exist
    if (find(network.at(posA).begin(), network.at(posA).end(), userB) == network.at(posA).end()) return 1;

    network.at(posA).remove(userB);
    return 0;
}

void UserManagementGraph::printGraph()
{
    for (int i = 0; i < (int) totalUsers; i++) {
        cout << "(" << i << ") " << userNodes.at(i)->getUsername() << " -> ";

        size_t j = 0;
        for (auto u_follow : network.at(i)){
            cout << u_follow->getUsername();
            j++;
            if (j != network.at(i).size()) cout << " | ";
        }
        cout << endl;
    }
    cout << endl;
} 

struct CompareP {
    bool operator () (User *user1, User *user2)
    {

        return user1->getLength() > user2->getLength();
    }
};










/**************************/
/*     A implementar      */
/**************************/








//1

vector<User*> UserManagementGraph::mostFollowing()
{
    vector<User*> ReturnVec;
    int NumFollow = 0, Record = 0;
    

    if(userNodes.empty() || totalUsers <= 0)
    {
        return ReturnVec;
    }
    

    for(auto UserPos = userNodes.begin(); UserPos != userNodes.end(); UserPos++)
    {
        NumFollow = network[userNodePosition(*UserPos)].size();
        

        if(NumFollow > Record)
        {
            while(!ReturnVec.empty())
            {
                ReturnVec.pop_back();
            }
            
            ReturnVec.push_back(*UserPos);
            Record = NumFollow;
        }
        

        else if(NumFollow == Record)
        {
            ReturnVec.push_back(*UserPos);
        }    
    }
    
    return ReturnVec;
    
    //answer here
    //Because there is no other option :)
}








//2

TVSeries* UserManagementGraph::followingMostWatchedSeries(User* userPtr)
{
    //Declaração de Variáveis e Vetores
    TVSeries* vMostViewed = nullptr;
    vector<TVSeries*> AdjWatchedSer;
    vector<int> EpsCnt, UsersCnt;
    bool FoundUser = false;


    //Verifica se userPtr é nullptr
    if(userPtr == nullptr)
    {
        return NULL;
    }


    //Verifica se o Nó pertence ao Grafo
    for(size_t i = 0; i < userNodes.size(); i++)
    {
        if(userNodes[i] == userPtr)
        {
            FoundUser = true;
            break;
        }
    }

    if(!FoundUser)
    {
        return NULL;
    }


    //Cria um vetor com todas as séries vistas pelos users adjacentes
    for(auto UserPos = network[userNodePosition(userPtr)].begin(); UserPos != network[userNodePosition(userPtr)].end(); UserPos++)
    {
        for(auto SerPos = (*UserPos)->getWatchedSeries().begin(); SerPos != (*UserPos)->getWatchedSeries().end(); SerPos++)
        {
            if(find(AdjWatchedSer.begin(), AdjWatchedSer.end(), *SerPos) == AdjWatchedSer.end())
            {
                AdjWatchedSer.push_back(*SerPos);
                EpsCnt.push_back(0);
                UsersCnt.push_back(0);
            }
        }
    }


    //Calcula o número de episódios vistos e o número de utilizadores que viram cada série
    for(size_t AdjSerPos = 0; AdjSerPos < AdjWatchedSer.size(); AdjSerPos++)
    {
        for(auto UserPos = network[userNodePosition(userPtr)].begin(); UserPos != network[userNodePosition(userPtr)].end(); UserPos++)
        {
            auto it = find((*UserPos)->getWatchedSeries().begin(), (*UserPos)->getWatchedSeries().end(), AdjWatchedSer[AdjSerPos]);

            if(it != (*UserPos)->getWatchedSeries().end())
            {
                size_t i = distance((*UserPos)->getWatchedSeries().begin(), it);
                EpsCnt[AdjSerPos] += (*UserPos)->getEpisodesWatched()[i];
                UsersCnt[AdjSerPos]++;
            }
        }
    }


    //Seleciona a série mais assistida de acordo com os critérios especificados
    int EpsRec = 0;
    int UserRec = 0;

    for(size_t i = 0; i < AdjWatchedSer.size(); i++)
    {
        if(EpsCnt[i] > EpsRec || (EpsCnt[i] == EpsRec && UsersCnt[i] > UserRec) || (EpsCnt[i] == EpsRec && UsersCnt[i] == UserRec && AdjWatchedSer[i]->getTitle() < vMostViewed->getTitle()))
        {
            EpsRec = EpsCnt[i];
            UserRec = UsersCnt[i];
            vMostViewed = AdjWatchedSer[i];
        }
    }

    return vMostViewed;

    //answer here
    //Because there is no other option :)
}








//3

int UserManagementGraph::shortestPaths(User* userSrc, User* userDst)
{
    //Declaração de Variáveis e Listas
    bool FoundUserSrc = false;
    bool FoundUserDst = false;
    int Max = 10000;

    //Verifica se o User de Partida e Destino são nullptr
    if(userSrc == nullptr || userDst == nullptr)
    {
        return -1;
    }


    //Verifica se os Nós pertencem ao Grafo
    for(size_t i = 0; i < userNodes.size(); i++)
    {
        if(userNodes[i] == userSrc)
        {
            FoundUserSrc = true;
        }

        if(userNodes[i] == userDst)
        {
            FoundUserDst = true;
        }
    }

    if(FoundUserSrc == false || FoundUserDst == false)
    {
        return -1;
    }


    //Inicializa o atributo length dos nós
    for(auto NodePos = userNodes.begin(); NodePos != userNodes.end(); NodePos++)
    {
        if(*NodePos == userSrc)
        {
            (*NodePos)->setLength(0);
        } 
        
        else
        {
            (*NodePos)->setLength(Max);
        }
    }
    

    //Inicialização de uma min-heap priority queue "UsersPQueue"
    //Esta queue armazena User* num Vetor e determina a sua prioridade através da estrutura "CompareP"
    priority_queue<User*, vector<User*>, CompareP> UsersPQueue;
    
    UsersPQueue.push(userSrc);

    list<User*> AdjList;
    int Distance = 0;

    //Ciclo "while" que precorre a fila "UsersPQueue"
    while(!UsersPQueue.empty())
    {
        // Move o User atual para "Aux" e remove-o de UsersPQueue
        User* Aux = UsersPQueue.top();
        UsersPQueue.pop();

        // Verifica se o User atual é o o nosso userDst
        if(Aux == userDst)
        {
            break;
        }

        // Cria uma lista com todos os Users adjacentes ao User Atual 
        for(auto UserPos = network[userNodePosition(Aux)].begin(); UserPos != network[userNodePosition(Aux)].end(); UserPos++)
        {
            AdjList.push_back(*UserPos);
        }

        // Adiciona os Users Adjacentes a "UsersPQueue" 
        for(auto AdjUserPos = AdjList.begin(); AdjUserPos != AdjList.end(); AdjUserPos++)
        {
            Distance = Aux->getLength() + 1;

            if(Distance < (*AdjUserPos)->getLength())
            {
                (*AdjUserPos)->setLength(Distance);
                UsersPQueue.push(*AdjUserPos);
            }
        }
    }
    

    //Caso não exista um caminho entre os dois nós
    if(userDst->getLength() == Max)
    {
        return -2;
    }


    return userDst->getLength();

    //answer here
    //Because there is no other option :)
}








//4

int HashTable::insertCountryStats(CountryStats &countryS)
{       
    //Inicialização do índice e key respetivamente
    int i;
    string key = countryS.country;
    

    //Verifica se a key não é uma string vazia
    if(key.empty())
    {
        return -1;
    }


    //Calcula o índice do elemento usando a função "hashFunction"
    i = hashFunction(key);


    //Verifica se o elemento já existe, caso exista retorna -1
    if(table[i] == &countryS)
    {
        return -1;
    }


    //Inicializada o Grau de Colisão a 0
    int DegCol = 1;

    //Verifica se já existe um elemento no índice onde este deveria estar este elemento, ou seja, se existe uma colisão
    //A função "probingFunction" vai tentar resolver a colisão a cada iteração, aumentando também DegCol em 1
    //Se o DegCol passar de 10, consideramos que a tabela de disperção não está otimizada e tentamos resolver a colisão colocando num espaço de um elemnto apagado
    while(table[i] != nullptr && DegCol < 10)
    {
        i = probingFunction(key, DegCol++);
    }


    //Verifica se já existe um elemento no índice onde este deveria estar este elemento, ou seja, se existe uma colisão
    //Em caso de colisão, tenta colocar o elemento no índice de um elemento que tenha sido previamente apagado
    if(table[i] != nullptr)
    {
        i = hashFunction("apagado");
    }


    //Quando não existir colisão, adicionamos o elemento "&countryS" ao índice que lhe foi atribuido
    table[i] = &countryS;
    

    //Atualizamos o número de elementosefetivos na tabela de dispersão
    totalCountryStats++;
    
    return i;

    //answer here
    //Because there is no other option :)
}








//5

int HashTable::importFromVector(UserManagement &userManager)      
{
    vector<vector<TVSeries*>> SeriesVec;

    SeriesVec.resize(tableSize);

    for(auto UserPos = userManager.getVectorUsers().begin(); UserPos != userManager.getVectorUsers().end(); UserPos++)
    {

        if(*UserPos == nullptr)
        {
            return -1;
        }

        int i = searchCountryStats(UserPos->getCountry());
        vector<int> SerPerGen(N_GENRES, 0);

        if(i == -1)
        {
            for(size_t i = 0; i < UserPos->getWatchedSeries().size(); i++)
            {
                for(int j = 0; j < N_GENRES; j++)
                {
                    if(UserPos->getWatchedSeries()[i]->getGenre() == vGenres[j])
                    {
                        SerPerGen[j]++;
                    }
                }
            }

            CountryStats *NewCountryStats = new CountryStats(UserPos->getCountry(), 1, UserPos->getWatchedSeries().size(), UserPos->getWatchedSeries().size(), SerPerGen);
            
            i = insertCountryStats(*NewCountryStats);

            for(size_t i = 0; i < UserPos->getWatchedSeries().size(); i++)
            {
                SeriesVec[i].push_back(UserPos->getWatchedSeries()[i]);
            }
        }

        else
        {
            table[i]->nUsers++;
            
            for(size_t i = 0; i < UserPos->getWatchedSeries().size(); i++)
            {
                if(find(SeriesVec[i].begin(), SeriesVec[i].end(), UserPos->getWatchedSeries()[i]) == SeriesVec[i].end())
                {
                    SeriesVec[i].push_back(UserPos->getWatchedSeries()[i]);
                }
            }

            table[i]->nTVSeries = SeriesVec[i].size();

            float aux = (table[i]->averageTVseries) * (table[i]->nUsers - 1);

            aux = aux + UserPos->getWatchedSeries().size();

            aux = aux / (float)table[i]->nUsers;

            table[i]->averageTVseries = aux;

            for(size_t i = 0; i < SeriesVec[i].size(); i++)
            {
                for(size_t j = 0; j < N_GENRES; j++)
                {
                    if(SeriesVec[i][i]->getGenre() == vGenres[j])
                    {
                        SerPerGen[j]++;
                    }
                }
            }

            table[i]->nGenre = SerPerGen;
        }
    }

    return 0;

    //answer here
    //Because there is no other option :)
}



    //TEST CODE THAT COMPILES BUT DOESN'T WORK//

    vector<vector<TVSeries*>> SeriesVec;

    SeriesVec.resize(tableSize);

    for(auto UserPos = userManager.getVectorUsers().begin(); UserPos != userManager.getVectorUsers().end(); UserPos++)
    {
        if(*UserPos == nullptr)
        { 
            return -1;
        }

        int i = searchCountryStats((*UserPos)->getCountry()); 

        vector<int> SerPerGen(N_GENRES, 0);

        if (i == -1)
        {
            for(size_t i = 0; i < (*UserPos)->getWatchedSeries().size(); i++)
            {
                for(int j = 0; j < N_GENRES; j++) {
                    if((*UserPos)->getWatchedSeries()[i]->getGenre() == vGenres[j])
                    {
                        SerPerGen[j]++;
                    }
                }
            }

            CountryStats *NewCountryStats = new CountryStats((*UserPos)->getCountry(), 1, (*UserPos)->getWatchedSeries().size(), (*UserPos)->getWatchedSeries().size(), SerPerGen);
            i = insertCountryStats(*NewCountryStats);

            for(size_t i = 0; i < (*UserPos)->getWatchedSeries().size(); i++)
            {
                SeriesVec[i].push_back((*UserPos)->getWatchedSeries()[i]);
            }

        } 
        
        else
        {
            table[i]->nUsers++;

            for(size_t i = 0; i < (*UserPos)->getWatchedSeries().size(); i++)
            {
                if(find(SeriesVec[i].begin(), SeriesVec[i].end(), (*UserPos)->getWatchedSeries()[i]) == SeriesVec[i].end())
                {
                    SeriesVec[i].push_back((*UserPos)->getWatchedSeries()[i]);
                }
            }

            table[i]->nTVSeries = SeriesVec[i].size();

            float aux = (table[i]->averageTVseries) * (table[i]->nUsers - 1);

            aux = aux + (*UserPos)->getWatchedSeries().size();

            aux = aux / (float)table[i]->nUsers;

            table[i]->averageTVseries = aux;

            for(size_t i = 0; i < SeriesVec[i].size(); i++)
            {
                for(size_t j = 0; j < N_GENRES; j++)
                {
                    if(SeriesVec[i][i]->getGenre() == vGenres[j])
                    { 
                        SerPerGen[j]++;
                    }
                }
            }
            
            table[i]->nGenre = SerPerGen;
        }
    }

    return 0; 
