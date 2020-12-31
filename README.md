This is a program the sorts out emails by removing all the spam emails and displaying only relevant emails.

Description: There are 2 main files to consider: spamlist_10k.txt and emails_1k.txt. In this project, we read in the spam file and filter it against the emails to display 
             only the emails that are relevant using binary search. We also utilize code to read in text files and store it as a string for comparison. We can edit the 
             spamlist file to emails that we classify as spam. Furthermore, sometimes any email from a certain domain is considered spam so in order to account for that:
             in the scenario we don't want any emails from _____@groupon.com, then we can type that in the spamlist as *@groupon.com making sure any email from Groupon is 
             filtered. The other textfiles can be used to test the program with various other spamlist and so forth.



Coding details:

Language: C++ 11

Used Valgrind to debug and ensure no memory leaks. 

Works on all IDEs where C++ code can run.

No graphical requirements.
