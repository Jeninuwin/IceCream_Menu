//Author: Jenny Nguyen
//Date: April 2, 2019
//Write a program that stores the following information about products at an ice cream store in a structure.
//This program will NOT use an array of structures.
//Instead will store the data in a file and use a single structure variable to read. write a record as needed

#include<iostream>
#include<iomanip>
#include<cstdlib>
#include<fstream>
#include<cstring>

using namespace std;

const int SIZE = 40;

//structure
struct iceCream
{
    char flavor[SIZE];
    float amount;   //in lbs
    float price;    //per pound
};

//enumerator
enum choice {   add = 1,
                all,
                look,
                edit,
                quit    };

//prototypes
void readFile(fstream &ifile);  //opens || creates file if it does not exist
int showMenu(fstream &ifile);   //shows menu to user
void addItem(fstream &ifile);   //adds all the items together
void printInfo(fstream &ifile); //prints information in a table
void spItem(fstream &ifile);    //searches for a record
void editInfo(fstream &ifile);  //edits a record

int main()
{
    iceCream type;
    int choice;
    fstream ifile;

    cout << fixed << setprecision(2);

    readFile(ifile);

    //show user menu to choose from
    showMenu(ifile);
    return 0;
}

void readFile(fstream &ifile)
{
    //open file for binary input/output
    //keep an existing file
    //if the file does not exist, create it

    ifile.open ("ice_cream.dat", ios::in | ios::out | ios::binary);

    if (ifile.fail())
    {
        cout << "File does not exist. Creating a new one." << endl;
        ifile.clear();
        ifile.open("ice_cream.dat", ios::out | ios::binary);
    }

    ifile.close();

    ifile.open ("ice_cream.dat", ios::in | ios::out | ios::binary);
    cout << "File is completed." << endl;
}

int showMenu(fstream &ifile)
{
    int choice;

    cout << fixed << setprecision(2) << endl;

    do
    {
        //displays choices for the user to pick from
        cout << "1. Add an item." << endl;
        cout << "2. Print the information for all items." << endl;
        cout << "3. Print the information for a particular item." << endl;
        cout << "4. Edit an item." << endl;
        cout << "5. Quit program. \n" << endl;
        cout << "Enter your choice: ";
        cin >> choice;
        cin.clear();
        cin.ignore();

        while (choice < add || choice > quit)
        {
            cout << "Choice is invalid. Enter Again: ";
            cin >> choice;
            cin.clear();
            cin.ignore();
        }

        if(choice != quit)
        {
            switch(choice)
            {
                case add:
                    addItem(ifile);
                    break;
                case all:
                    printInfo(ifile);
                    break;
                case look:
                    spItem(ifile);
                    break;
                case edit:
                     editInfo(ifile);
                     break;
            }
        }

    }while( choice != quit);

    ifile.close();
    return choice;
}

void addItem(fstream &ifile)
{
    char again;
    iceCream type;

    do
    {
        cout << "---------------------------------------------------------" << endl;

        cout << "Enter an ice cream flavor: ";
        cin.getline(type.flavor, SIZE);

        cout << "Enter amount(lbs): ";
        cin >> type.amount;

        cout << "Enter price: ";
        cin >> type.price;

        ifile.seekp(0L, ios::end);

        //writes into the file
        ifile.write(reinterpret_cast<char *> (&type), sizeof(type));

        //asks user if they want to add another icecream flavor
        cout << "Add another record y/n: ";
        cin >> again;
        cin.clear();
        cin.ignore();

        cout << "---------------------------------------------------------" << endl;

    } while (again == 'Y' || again == 'y');

    cout << endl;
}

void printInfo(fstream &ifile)
{
    iceCream type;
    float total;
    float itemVal =0;

    //starts in the beginning
    ifile.seekg(0L, ios::beg);

    cout << endl;
    cout << "---------------------------------------------------------" << endl;
    cout << setw(30) << "Ice Cream's information\n" << endl;
    cout << left << setw(20) << "Flavor" <<  setw(12) << "Quantity" <<  setw(12) << "Price" <<  "Totals\n";

    while(ifile.read(reinterpret_cast<char *> (&type), sizeof(type)))
    {
        cout << left << setw(20) << type.flavor
        << setw(12) << type.amount
        << setw(12) << type.price
        <<  (itemVal = (type.amount * type.price))  << endl;    //calculates the price and amount to find the price

        total += itemVal; //overall total from all the prices added up
    }


    cout << fixed << "Overall Total: " << setw(50) << total << endl;
    cout << "---------------------------------------------------------" << endl;

    cout << endl;

}

void spItem(fstream &ifile)
{
    iceCream type = {"", 0, 0};
    char search[SIZE];
    char *strptr = 0;
    int index = 0;

    ifile.clear();
    ifile.seekg(0L, ios :: beg);

    //Getting the ice-cream
    cout <<"What flavor are you looking for?: " << endl;
    cin.getline(search, SIZE);

    ifile.read(reinterpret_cast<char*> (&type), sizeof(type));

    while (!ifile.eof())
    {
        strptr = strstr(type.flavor, search);

        if(strptr != 0)
        {
            cout << "Your ice cream was located" << endl;
            break;
        }

        ifile.read(reinterpret_cast<char*> (&type), sizeof(type));
    }

    if (strptr != 0)
    {
        cout << fixed << setw(20) << "Flavor";
        cout << setw(5) << "Amount";
        cout << setw(5) << "Price";
        cout << setw(20) << type.flavor << setw(5) << type.amount
        << setw(5) << type.price << endl;
    }
}

void editInfo(fstream &ifile)
{
    iceCream type;

    char userFlavor [SIZE];
    int place = 0;

    cout << "\nWhat flavor would you like to edit?\n";
    cin.ignore();
    cin.getline(userFlavor, SIZE);


    ifile.seekg(0L, ios::beg);
    ifile.read(reinterpret_cast<char *> (&type), sizeof(type));

    while (!ifile.eof() && strstr (type.flavor, userFlavor) == nullptr)
    {
        ifile.read(reinterpret_cast<char *> (&type), sizeof(type));
        place++;

    }
    if(ifile.eof())
        cout << "Item not found.";
    else
    {
        cout << "\nEnter flavor: " << endl;
        cin.getline(type.flavor, SIZE);


        cout << "Enter amount(lbs): " << endl;
        cin >> type.amount;


        cout << "Enter price: " << endl;
        cin >> type.price;


        ifile.seekp((sizeof(type) * place) , ios::beg);

        ifile.write(reinterpret_cast<char *> (&type), sizeof(type));
    }
    ifile.clear();
}
