#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "medicine.h"   //For using medicine structure
#include "supplier.h" //For using supplier structure
#define size 100
int ValidateId(int id){ //Checks if Id is valid
    if(id<0){
        printf("Invalid Id\n");
        return 0;
    }
    else
        return 1;
}
int ValidateName(char name[],char Section){  //Checks if name is valid
    char message[10];
    switch (Section){   //Use same function for all three sections with different output messages
        case 'M':
            strcpy(message,"Medicine");
            break;
        case 'S':
            strcpy(message,"Supplier");
            break;
        case 'C':
            strcpy(message,"Customer");
            break;
    }
    int flag=1;
    for(int a=0;name[a]!='\0';a++){
        if(!isalpha(name[a]) && !isspace(name[a])){
            flag=0; 
            break;
        }
    }
    if(!flag)
        printf("%s name can only contain alphabhets\n",message);
    return flag;
}
int ValidateQuantity(int quantity){ //Checks if quantity is valid
    if(quantity<0){
            printf("Quantity cannot be less than zero\n");
            return 0;    
    }
    else
        return 1;
}
int ValidateNumber(char number[]){  //Validates phone number
    if(strlen(number)<10){
        printf("Phone number must contain atleast 10 digits\n");
        return 0;
    }
    for(int a=0;number[a]!='\0';a++){
        if(!isdigit(number[a])){
            printf("Phone number can only contain numbers\n");
            return 0;
        }
    }
    return 1;
}
int LoadData(struct Medicine MedicineData[],int *ptr){
    FILE *fptr=fopen("medicine.txt","r");
    if(fptr==NULL){ //Error handling
        printf("\nError in loading data from file 'medicine.txt' \n");
        return 0;   
    }
    else{   //Read data and store it in array of medicine structure for faster processing
        while(fscanf(fptr,"ID:%d,Name:%[^,],Quantity:%d\n",&MedicineData[*ptr].ID,MedicineData[*ptr].Name,&MedicineData[*ptr].Quantity)!=EOF){
            (*ptr)++;   
        }
    }
    fclose(fptr);
    return 1;
}
void SaveData(struct Medicine MedicineData[],int NoOfMedicine){
    FILE *fptr=fopen("medicine.txt","w");
    if(fptr==NULL)  //Error handling
        printf("Error in saving changes to file 'medicine.txt'\n");
    else{
        for(int a=0;a<NoOfMedicine;a++){    //Copy all data from arrays to the file before termination of program
            fprintf(fptr,"ID:%d,Name:%s,Quantity:%d\n",MedicineData[a].ID,MedicineData[a].Name,MedicineData[a].Quantity);
        }
    }
    fclose(fptr);
}
void AddMedicine(struct Medicine MedicineData[],int *ptr){
    if(*ptr>size-1) //Check if Inventory is full
        printf("Inventory is full\n");
    else{
        int Id;
        do{
            printf("Enter medicine Id \n");
            scanf("%d",&Id);
        }while(!ValidateId(Id));    //Validates user input of Id
        scanf("%*c");
        int Flag=0,x=0;
        while(x<*ptr){  //Prevent duplication of same medicine
            if(MedicineData[x].ID==Id){
                Flag=1;
                break;
            }
            x++;
        }
        if(Flag==1)
            printf("This medicine already exists in the inventory\n");
        else{
            int quantity;
            char name[31];
            do{
                printf("Enter medicine name.It cannot be more than 30 characters \n");
                fgets(name,31,stdin);
                name[strcspn(name,"\n")]='\0';
            }while(!ValidateName(name,'M'));    //Validates user input of medicine name
            do{
                printf("Enter quantity of the medicine \n");
                scanf("%d",&quantity);
            }while(!ValidateQuantity(quantity));    //Validates user input of quantity
            MedicineData[*ptr].ID=Id;
            MedicineData[*ptr].Quantity=quantity;
            strcpy(MedicineData[*ptr].Name,name);
            (*ptr)++;   //Increase the count of medicines
            printf("New medicine added successfully\n");
        }
    }
    printf("---------------------------------------------\n");
}
int SearchMedicine(struct Medicine MedicineData[],int NoOfMedicine){
    int Id,found=-1;
    do{
        printf("Enter medicine Id \n");
        scanf("%d",&Id);
    }while(!ValidateId(Id));    //Validates user input of Id
    scanf("%*c");
    for(int a=0;a<NoOfMedicine;a++){
        if(MedicineData[a].ID==Id){
            found=a;    //Stores the index at which the medicine is found
            break;
        }
    }
    return found;
}
void ModifyMedicine(struct Medicine MedicineData[],int NoOfMedicine){
    int option,quantity;
    char name[31];
    int index=SearchMedicine(MedicineData,NoOfMedicine);  //Uses search function to check if medicine exists or not
    if(index==-1)   
        printf("Medicine Id does not exist\n");
    else{    
        do{
            printf("If you want to change only medicine name,enter 1 or enter 2 for only changing quantity or enter 3 for both \n");
            scanf("%d",&option);
        }while(option<1||option>3);
        scanf("%*c");
        switch (option){
        case 3: //Uses concept of fall-through in switch cases for option 3
        case 1:
            do{
                printf("Enter the new medicine name.It cannot be more than 30 characters \n");
                fgets(name,31,stdin);
                name[strcspn(name,"\n")]='\0';
            }while(!ValidateName(name,'M'));
            strcpy(MedicineData[index].Name,name);
            printf("Medicine name updated successfully\n");
            if(option==1)
                break;
        case 2:
            do{
                printf("Enter the new quantity of the medicine \n");
                scanf("%d",&quantity);
            }while(!ValidateQuantity(quantity));
            MedicineData[index].Quantity=quantity;
            printf("Medicine quantity updated successfully\n");
            break;
        }
    }
    printf("--------------------------------------\n");
}
void DeleteMedicine(struct Medicine MedicineData[],int *ptr){
    int index=SearchMedicine(MedicineData,*ptr);
    if(index==-1)
        printf("Medicine Id does not exist\n");
    else{
        for(int a=index;a<*ptr-1;a++){    //Deletes the record by shifting each record back by one index
            MedicineData[a].ID=MedicineData[a+1].ID;
            MedicineData[a].Quantity=MedicineData[a+1].Quantity;
            strcpy(MedicineData[a].Name,MedicineData[a+1].Name);
        }
        (*ptr)--;   //Updates the value of number of medicines
        printf("Medicine record deleted successfully\n");
    }
    printf("------------------------------------\n");
}
void DisplayInventory(struct Medicine MedicineData[],int NoOfMedicine){
    printf("%-5s%-15s%-10s\n", "Id", "Name", "Quantity");
    for(int a=0;a<NoOfMedicine;a++){
        printf("%-5d%-15s%-10d\n",MedicineData[a].ID,MedicineData[a].Name,MedicineData[a].Quantity);
    }
    printf("----------------------------\n");
}
int ReadData(struct Supplier SupplierData[],int *ptr){
    FILE *fptr=fopen("supplier.txt","r");
    if(fptr==NULL){ //Error handling
        printf("\nError in loading data from file 'supplier.txt' \n");
        return 0;   
    }
    else{   //Read data and store it in array of supplier structure for faster processing
        while(fscanf(fptr,"ID:%d,Name:%[^,],Phone Number:%s\n",&SupplierData[*ptr].ID,SupplierData[*ptr].Name,&SupplierData[*ptr].Number)!=EOF){
            (*ptr)++;   
        }
    }
    fclose(fptr);
    return 1;
}
void WriteData(struct Supplier SupplierData[],int NoOfSupplier){
    FILE *fptr=fopen("supplier.txt","w");
    if(fptr==NULL)  //Error handling
        printf("Error in saving changes to file 'supplier.txt'\n");
    else{
        for(int a=0;a<NoOfSupplier;a++){    //Copy all data from arrays to the file before termination of program
            fprintf(fptr,"ID:%d,Name:%s,Phone Number:%s\n",SupplierData[a].ID,SupplierData[a].Name,SupplierData[a].Number);
        }
        printf("Changes saved successfully\n");
    }
    fclose(fptr);
}
void AddSupplier(struct Supplier SupplierData[],int *ptr){
    if(*ptr>size-1) //Check if there is space for another record
        printf("No space for another record\n");
    else{
        int Id;
        do{
            printf("Enter supplier Id \n");
            scanf("%d",&Id);
        }while(!ValidateId(Id));    //Validates user input of Id
        int Flag=0,x=0;
        while(x<*ptr){  //Prevent duplication of same supplier
            if(SupplierData[x].ID==Id){
                Flag=1;
                break;
            }
            x++;
        }
        if(Flag==1)
            printf("This supplier already exists in the records\n");
        else{
            char name[31],number[16];
            scanf("%*c");
            do{
                printf("Enter supplier name.It cannot be more than 30 characters \n");
                fgets(name,31,stdin);
                name[strcspn(name,"\n")]='\0';
            }while(!ValidateName(name,'S'));    //Validates user input of supplier name
            do{
                printf("Enter phone number of supplier \n");
                fgets(number,16,stdin);
                number[strcspn(number,"\n")]='\0';
            }while(!ValidateNumber(number));    //Validates user input of phone number
            SupplierData[*ptr].ID=Id;
            strcpy(SupplierData[*ptr].Name,name);
            strcpy(SupplierData[*ptr].Number,number);
            (*ptr)++;   //Increase the count of suppliers
            printf("New supplier record added successfully\n");
        }
    }
    printf("---------------------------------------------\n");
}
int SearchSupplier(struct Supplier SupplierData[],int NoOfSupplier){
    int Id,found=-1;
    do{
        printf("Enter supplier Id \n");
        scanf("%d",&Id);
    }while(!ValidateId(Id));    //Validates user input of Id
    scanf("%*c");
    for(int a=0;a<NoOfSupplier;a++){
        if(SupplierData[a].ID==Id){
            found=a;    //Stores the index at which the supplier record is found
            break;
        }
    }
    return found;

}
void ModifySupplier(struct Supplier SupplierData[],int NoOfSupplier){
    int option;
    char name[31],number[16];
    int index=SearchSupplier(SupplierData,NoOfSupplier);  //Uses search function to check if supplier record exists or not
    if(index==-1)   
        printf("Supplier Id does not exist\n");
    else{    
        do{
            printf("If you want to change only supplier name,enter 1 or enter 2 for only changing phone number or enter 3 for both \n");
            scanf("%d",&option);
        }while(option<1||option>3);
        scanf("%*c");
        switch (option){
        case 3: //Uses concept of fall-through in switch cases for option 3
        case 1:
            do{
                printf("Enter the new supplier name.It cannot be more than 30 characters \n");
                fgets(name,31,stdin);
                name[strcspn(name,"\n")]='\0';
            }while(!ValidateName(name,'S'));
            strcpy(SupplierData[index].Name,name);
            printf("Supplier name updated successfully\n");
            if(option==1)
                break;
        case 2:
            do{
                printf("Enter the new phone number of the supplier \n");
                scanf("%15s",number);
            }while(!ValidateNumber(number));
            strcpy(SupplierData[index].Number,number);
            printf("Supplier phone number updated successfully\n");
            break;
        }
    }
    printf("---------------------------------------------\n");
}
void DeleteSupplier(struct Supplier SupplierData[],int *ptr){
    int index=SearchSupplier(SupplierData,*ptr);
    if(index==-1)
        printf("Supplier Id does not exist\n");
    else{
        for(int a=index;a<*ptr-1;a++){    //Deletes the record by shifting each record back by one index
            SupplierData[a].ID=SupplierData[a+1].ID;
            strcpy(SupplierData[a].Name,SupplierData[a+1].Name);
            strcpy(SupplierData[a].Number,SupplierData[a+1].Number);
        }
        (*ptr)--;   //Updates the value of number of suppliers
        printf("Supplier record deleted successfully\n");
    }
    printf("------------------------------------\n");
}
void DisplaySupplierRecords(struct Supplier SupplierData[],int NoOfSupplier){
    printf("%-5s%-15s%-10s\n", "Id", "Name", "Phone Number");
    for(int a=0;a<NoOfSupplier;a++){
        printf("%-5d%-15s%-14s\n",SupplierData[a].ID,SupplierData[a].Name,SupplierData[a].Number);
    }
    printf("-----------------------------\n");
}
int main(){
    struct Medicine MedicineData[size];
    struct Supplier SupplierData[size];
    int userinput,subinput,NoOfMedicine=0,NoOfSupplier=0;
    int open1=LoadData(MedicineData,&NoOfMedicine);
    int open2=ReadData(SupplierData,&NoOfSupplier);
    printf("Welcome to Medicine Record Managment System!");
    do{
        printf("\nMain Menu.To:\nGo to Medicine Section,enter 1\nGo to Supplier Section,enter 2\nGo to Customer Section,enter 3\nExit the main menu,enter 4\n");
        scanf("%d",&userinput);
        switch (userinput){
            case 1:
                printf("Medicine Section\n");
                if(open1){
                    do{
                        printf("To:\nAdd a new medicine record,enter 1\nSearch for a medicine,enter 2\nModify an existing record,enter 3\nDelete a existing record,enter 4\nDisplay inventory,enter 5\nExit the medicine section,enter 6 \n");
                        scanf("%d",&subinput);
                        switch (subinput){
                            case 1:
                                AddMedicine(MedicineData,&NoOfMedicine);
                                break;
                            case 2:
                                int result=SearchMedicine(MedicineData,NoOfMedicine);
                                (result==-1)?printf("Medicine not found in inventory\n"):printf("Medicine %s with quantity %d found at index %d\n",MedicineData[result].Name,MedicineData[result].Quantity,result);
                                printf("-------------------------------------\n");
                                break;
                            case 3:
                                ModifyMedicine(MedicineData,NoOfMedicine);
                                break;
                            case 4:
                                DeleteMedicine(MedicineData,&NoOfMedicine);
                                break;
                            case 5:
                                DisplayInventory(MedicineData,NoOfMedicine);
                                break;
                            case 6:
                                printf("Exitted the medicine section\n");
                                break;
                            default:
                                printf("Invalid option\n");
                        }
                    }while(subinput!=6);
                }
                break;
            case 2:
                printf("Supplier Section\n");
                if(open2){
                    do{
                        printf("To:\nAdd a new supplier,enter 1\nSearch for a supplier,enter 2\nModify an existing supplier record,enter 3\nDelete a existing supplier record,enter 4\nDisplay all records,enter 5\nExit the supplier section,enter 6 \n");
                        scanf("%d",&subinput);
                        switch (subinput){
                            case 1:
                                AddSupplier(SupplierData,&NoOfSupplier);
                                break;
                            case 2:
                                int result=SearchSupplier(SupplierData,NoOfSupplier);
                                (result==-1)?printf("Supplier not found in records\n"):printf("Supplier %s with Id %d and phone number %s found at index %d\n",SupplierData[result].Name,SupplierData[result].ID,SupplierData[result].Number,result);
                                printf("-------------------------------------------------\n");
                                break;
                            case 3:
                                ModifySupplier(SupplierData,NoOfSupplier);
                                break;
                            case 4:
                                DeleteSupplier(SupplierData,&NoOfSupplier);
                                break;
                            case 5:
                                DisplaySupplierRecords(SupplierData,NoOfSupplier);
                                break;
                            case 6:
                                printf("Exitted the supplier section\n");
                                break;
                            default:
                                printf("Invalid option\n");
                        }
                    }while(subinput!=6);
                }
                break;
            case 3:
            printf("Customer Section\n");
                break;
            case 4:
                printf("Saving changes.....\n");
                SaveData(MedicineData,NoOfMedicine);
                WriteData(SupplierData,NoOfSupplier);
                printf("Exitted the main menu\n");
                break;
            default:
                printf("Invaild option\n");
        }
    }while(userinput!=4);   
}