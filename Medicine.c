#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "medicine.h"   //For using medicine structure
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
        if(!isalpha(name[a])){
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
        printf("Changes saved successfully\n");
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
            scanf("%*c");
            do{
                printf("Enter medicine name.It cannot be more than 30 characters \n");
                scanf("%31s",name);
            }while(!ValidateName(name,'M'));    //Validates user input of medicine name
            scanf("%*c");
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
        switch (option){
        case 3: //Uses concept of fall-through in switch cases for option 3
        case 1:
            do{
                printf("Enter the new medicine name.It cannot be more than 30 characters \n");
                scanf("%30s",name);
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
        for(int a=index;a<*ptr;a++){    //Deletes the record by shifting each record back by one index
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
int main(){
    struct Medicine MedicineData[size];
    int userinput,NoOfMedicine=0;
    printf("Welcome to Medicine Managment System!");
    int open=LoadData(MedicineData,&NoOfMedicine); 
    if(open){
        do{
            printf("To:\nAdd a new medicine record,enter 1\nSearch for a medicine,enter 2\nModify an existing record,enter 3\nDelete a existing record,enter 4\nDisplay inventory,enter 5\nExit the system,enter 6 \n");
            scanf("%d",&userinput);
            switch (userinput){
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
                    SaveData(MedicineData,NoOfMedicine);
                    printf("Exitted the medicine sub-menu\n");
                    break;
                default:
                    printf("Invalid option\n");
            }
        }while(userinput!=6);
    }
}