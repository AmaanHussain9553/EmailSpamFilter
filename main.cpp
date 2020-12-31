#include <iostream>
#include <string>
#include <algorithm>
#include <fstream>
#include "ourvector.h"


using namespace std;


//This function takes a file already created and transferred
//into a ourvector and displays it
void displayVector(ourvector <string> filenameDisplay){
    for(int i = 0; i < filenameDisplay.size(); i++){
        cout << filenameDisplay[i] << endl;
    }
    cout << endl;
}


//This function checks is a specific emails is always spam
//because of its username being an "*", in which case
//any username with this domain while be spam
void checkIfPermanentSpam(ourvector<string> emailList, ourvector<string> &permanentSpamEmails){
    bool isSpam[emailList.size()];
    for(int i = 0; i < emailList.size(); i++){
        if(emailList[i][0] == '*'){
            isSpam[i] = true;
        } else {
            isSpam[i] = false;
        }
        if(isSpam[i]){
            int position = emailList[i].find('@') + 1;
            permanentSpamEmails.push_back(emailList[i].substr(position, emailList[i].size()));
        }
    }
}


//This functions sorts through any file and arranges it in
//alphabetical order using bubble sort
void sortAlphabetical(ourvector <string> &file){
    for (int j = 0; j < file.size() - 1; j++) {
        for (int i = 0; i < file.size() - 1; i++) {
            if (file[i] > file[i + 1]) {
                //for alphabetical order: a < b
                string swap;
                //creating a temp variable to swap later
                swap = file[i + 1];
                file[i + 1] = file[i];
                file[i] = swap;
            }
        }
    }
}


//This function converts the vectors which has all username
//and domains and converts them into a single emailID with
//the format user@domain.com
void convertEmail(ourvector <string> usernames, ourvector <string> domains, ourvector<string> &emailList, ourvector<string> &permanentSpamEmails){
    //make permanentspam emails a vector in main
    string formattedEmail = "";
    for(int i = 0; i < usernames.size(); i++){
        formattedEmail = usernames[i] + "@" + domains[i];
        //email format
        emailList.push_back(formattedEmail);
        //storing into vector for later access
    }
    checkIfPermanentSpam(emailList, permanentSpamEmails);
    sortAlphabetical(permanentSpamEmails);
    sortAlphabetical(emailList);
}


//This function takes an intial file as its parameter and
//transfers the inputs of the file into a vector and also
//sorts through the inputs of the vector to seperate the
//usernames and domains and store them into seperate
//vectors
void loadIntoFile(string filename, ourvector<string> &spamList, ourvector<string> &emailList, ourvector<string> &permanentSpamEmails){
    ourvector<string> usernames;
    ourvector<string> domains;
    int i = 0;
    ifstream infile(filename);
    //opening a file
    if(!infile.good()) {
        cout << "**Error, unable to open '" << filename << "'" << endl << endl;
    } else {
        string a;
        cout << "Loading '" << filename << "'" << endl;
        while (!infile.eof()) {
            //while it is not the end of the file
            while(getline(infile, a)) {
                //to read the next line into the vector
                string domain = "";
                string username = "";
                if (a != " ") {
                    spamList.push_back(a);
                    for (int i = 0; i < a.find(':'); i++){
                        domain += a[i];
                    }
                    for (int i = a.find(':') + 1; i < a.size(); i++){
                        username += a[i];
                    }
                    usernames.push_back(username);
                    //storing usernames into a vector
                    domains.push_back(domain);
                    //storing domains into a vector
                    i++;
                }
            }
        }
        convertEmail(usernames, domains, emailList, permanentSpamEmails);
        cout << "# of spam entries: " << i << endl << endl;
    }
    infile.close();
}


//This function is a standard binary search which runs at
//log(n) rate much faster than a linear search and is used
//in check and filter functions
bool BinarySearch(ourvector <string> &emailList, string spamEmail){
    int mid = 0;
    int low = 0;
    int high = emailList.size() - 1;
    while (low <= high) {
        mid = (low + (high - low)/ 2);
        string A = emailList[mid];
        if(A == spamEmail ){
            return true;
        }
        else if (spamEmail < A) {
            high = mid - 1;
        }
        else
        {
            low = mid + 1;
        }
    }
    return false;
}


//This function can get just the username when the string is
//already in the form user@domain.com
//This function was used in filter function
string getUserName(string &email){
    int pos = email.find("@");
    string str;
    string a = "                                                             ";
    int count = 0;
    int apl = 0;
    int i = 0;
    while(isdigit(email[apl]) && !isspace(email[i])){
        apl++;
    }
    if(apl > 0){
        apl++;
    }
    for(i = apl; i < pos; i++) {
        a[count] = email[i];
        count++;
    }
    count++;
    a[count] = '@';
    str = a;
    str.erase(remove(str.begin(), str.end(), ' '), str.end());
    a = str;
    return a;
}


//This function can get just the username when the string is
//already in the form user@domain.com
//This function was used in filter function
string getDomain(string &email){
    int pos = email.find("@");
    string str;
    string a= "                                                             ";;
    int counter = 0;
    for(int i = pos + 1; i < email.size(); i++) {
        if(isspace(email[i])) {
            break;
        }
        else{
            a[counter] = email[i];
            counter++;
        }
    }
    str = a;
    str.erase(remove(str.begin(), str.end(), ' '), str.end());
    a = str;
    return a;
}


//This function filters through the inputFile given by user
//and compares it with filr already loaded by the user
//to remove all the spam emails and display the emailList
//without any spam emails into an output file
void filter(string inputFile, string outputFile, ourvector<string> emailList, ourvector <string> permanentSpamEmails){
    ifstream infile(inputFile);
    if(infile.is_open()){
        ofstream outputfile;
        outputfile.open(outputFile);
        string line;
        string userNameOfEmail;
        string DomainOfEmail;
        string emailID;
        int emailsAccessed = 0;
        int nonSpam = 0;
        while(getline(infile, line)){
            userNameOfEmail = getUserName(line);
            DomainOfEmail = getDomain(line);
            emailID = userNameOfEmail + DomainOfEmail;
            if(BinarySearch(emailList, emailID)){
                //Do nothing if the email is spam
            } else if (BinarySearch(permanentSpamEmails, DomainOfEmail)){
                //Do nothing if the email is a permanent spam
            } else {
                outputfile << line << endl;
                nonSpam++;
            }
            emailsAccessed++;
        }
        cout << "# emails processed: " << emailsAccessed << endl;
        cout << "# non-spam emails: " << nonSpam << endl << endl;
        outputfile.close();
        infile.close();
    } else {
        cout << "**Error, unable to open '" << inputFile << "'" << endl << endl;
    }
}


//Function that checks individual email for spam against a loaded spam File
void checkForSpam(string checkEmail, ourvector<string> permanentSpamEmails, ourvector<string> emailList){
    if(BinarySearch(emailList, checkEmail) || BinarySearch(permanentSpamEmails, checkEmail.substr(checkEmail.find("@") + 1))){
        cout << checkEmail << " is spam" << endl << endl;
    } else {
        cout << checkEmail << " is not spam" << endl << endl;
    }
}


int main(){
    ourvector<string> spamList;
    ourvector<string> emailList;
    ourvector<string> permanentSpamEmails;
    string userInput;

    cout << "** Welcome to spam filtering app **" << endl << endl;
    do{
        cout << "Enter command or # to exit> ";
        cin >> userInput;
        //action for input of loading a file
        if(userInput == "load"){
            if(emailList.size() != 0 || permanentSpamEmails.size() != 0){
                emailList.clear();
                permanentSpamEmails.clear();
            }
            string loadFile;
            cin >> loadFile;
            loadIntoFile(loadFile, spamList, emailList, permanentSpamEmails);
        }
        //action for input of display a loaded file
        else if(userInput == "display"){
            displayVector(spamList);
        }
        //action for input of checking an email for spam
        else if(userInput == "check") {
            string checkEmail;
            cin >> checkEmail;
            checkForSpam(checkEmail, permanentSpamEmails, emailList);

        }
        //action for input of filtering between a loaded spamFile and an email file
        //to filter
        else if (userInput == "filter"){
            string filterFile1;
            cin >> filterFile1;
            string filterFile2;
            cin >> filterFile2;
            filter(filterFile1, filterFile2,  emailList, permanentSpamEmails);
        }
        //to check for invlaid userInput
        if(userInput != "filter" && userInput != "load" && userInput != "check" && userInput != "display"
           && userInput != "#" && userInput != "\n"){
            cout << "**invalid command" << endl << endl;
        }

    }while(userInput != "#");

    //exit when user inputs '#'
    if(userInput == "#"){
        exit(0);
    }
    return 0;

}