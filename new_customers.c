#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int userchoice;

typedef struct {
    int ID;
    char name[50];
    int contact;
} Customer;

void addCustomer(int ID, char name[], int contact);
void CustomerMenu();
void SearchRecord(int ID);
void loadCustomersFromFile();
void saveCustomerToFile(Customer customer);
void printAllRecords();
int customerExists(int ID);

int main()
{
    int Id, Id_2, Contact;
    char Name[50];
    
    loadCustomersFromFile();
    
    while (1)
    {
        CustomerMenu();
        
        if (userchoice == 4)
        {
            printf("\nExiting System...");
            break;
        }
        
        switch (userchoice)
        {
        case (1):
            printf("\nEnter ID: ");
            scanf("%d", &Id);
            getchar();
            printf("Enter Name: ");
            fgets(Name, 50, stdin);
            Name[strcspn(Name, "\n")] = 0;
            printf("Enter Contact: ");
            scanf("%d", &Contact);
            addCustomer(Id, Name, Contact);
            break;
            
        case (2):
            printf("\nEnter ID of record to search: ");
            scanf("%d", &Id_2);
            SearchRecord(Id_2);
            break;
            
        case (3):
            printAllRecords();
            break;
            
        default:
            printf("\nInvalid number entered!");
            break;
        }
        
        printf("\n");
    }
    
    return 0;
}

void loadCustomersFromFile()
{
    FILE *file = fopen("Customers.txt", "r");
    
    if (file == NULL)
    {
        file = fopen("Customers.txt", "w");
        if (file != NULL)
        {
            fclose(file);
            printf("\nCustomer database file created.\n");
        }
        return;
    }
    
    fclose(file);
    printf("\nCustomer records loaded from file.\n");
}

void addCustomer(int ID, char name[], int contact)
{
    if (ID <= 0)
    {
        printf("\nError: ID must be a positive number!");
        return;
    }
    
    if (contact <= 0)
    {
        printf("\nError: Contact number must be a positive number!");
        return;
    }
    
    if (strlen(name) == 0 || name[0] == '\0')
    {
        printf("\nError: Name cannot be empty!");
        return;
    }
    
    if (customerExists(ID))
    {
        printf("\nThis customer already exists, enter a different ID!");
        return;
    }
    
    Customer newCustomer;
    newCustomer.ID = ID;
    strcpy(newCustomer.name, name);
    newCustomer.contact = contact;
    
    saveCustomerToFile(newCustomer);
    printf("\nCustomer added successfully!");
}

void saveCustomerToFile(Customer customer)
{
    FILE *file = fopen("Customers.txt", "a");
    
    if (file == NULL)
    {
        printf("\nError: Could not open file for writing!");
        return;
    }
    
    fprintf(file, "%d|%s|%d\n", customer.ID, customer.name, customer.contact);
    
    fclose(file);
}

int customerExists(int ID)
{
    FILE *file = fopen("Customers.txt", "r");
    
    if (file == NULL)
    {
        return 0;
    }
    
    Customer temp;
    while (fscanf(file, "%d|%[^|]|%d\n", &temp.ID, temp.name, &temp.contact) == 3)
    {
        if (temp.ID == ID)
        {
            fclose(file);
            return 1;
        }
    }
    
    fclose(file);
    return 0;
}

void SearchRecord(int ID)
{
    FILE *file = fopen("Customers.txt", "r");
    
    if (file == NULL)
    {
        printf("\nNo records found!");
        return;
    }
    
    Customer temp;
    int found = 0;
    
    while (fscanf(file, "%d|%[^|]|%d\n", &temp.ID, temp.name, &temp.contact) == 3)
    {
        if (temp.ID == ID)
        {
            printf("\nRecord found, fetching info...");
            printf("\nCustomer ID: %d", temp.ID);
            printf("\nCustomer Name: %s", temp.name);
            printf("\nCustomer Contact: %d", temp.contact);
            found = 1;
            break;
        }
    }
    
    if (!found)
    {
        printf("\nRecord Not Found, enter a valid ID!");
    }
    
    fclose(file);
}

void printAllRecords()
{
    FILE *file = fopen("Customers.txt", "r");
    
    printf("\n\t--ALL RECORDS--\t\n");
    
    if (file == NULL)
    {
        printf("\nNo records found!\n");
        return;
    }
    
    Customer temp;
    int count = 0;
    
    while (fscanf(file, "%d|%[^|]|%d\n", &temp.ID, temp.name, &temp.contact) == 3)
    {
        printf("\nID: %d\tName: %s\tContact: %d", temp.ID, temp.name, temp.contact);
        count++;
    }
    
    if (count == 0)
    {
        printf("\nNo records found!\n");
    }
    
    fclose(file);
}

void CustomerMenu()
{
    printf("\n\n\t--CUSTOMER MENU--\t");
    printf("\n\tEnter between 1 to 4 to perform an operation");
    printf("\n\t1 - Add a Customer record");
    printf("\n\t2 - Search for a customer record");
    printf("\n\t3 - Print All Records");
    printf("\n\t4 - Exit");
    printf("\n\tEnter: ");
    scanf("%d", &userchoice);
}
