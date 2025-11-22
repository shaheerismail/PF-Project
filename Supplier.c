#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "supplier.h" 
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
    switch (Section){   //Use same function for all three sections
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
int ValidateNumber(char number[]){
    int flag=1;
    for(int a=0;number[a]!='\0';a++){
        if(!isdigit(number[a])){
            flag=0;
            break;
        }
    }
    if(!flag)
        printf("Phone number can only contain numbers\n");
    return flag;
}
int ReadData(struct Supplier SupplierData[],int *ptr){
    FILE *fptr=fopen("supplier.txt","r");
    if(fptr==NULL){ //Error handling
        printf("\nError in loading data from file 'supplier.txt' \n");
        return 0;   
    }
    else{   //Read data and store it in array of supplier structure for faster processing
        while(fscanf(fptr,"ID:%d,Name:%[^,],Phone Number:%d\n",&SupplierData[*ptr].ID,SupplierData[*ptr].Name,&SupplierData[*ptr].Number)!=EOF){
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
                scanf("%30s",name);
            }while(!ValidateName(name,'S'));    //Validates user input of supplier name
            scanf("%*c");
            do{
                printf("Enter phone number of supplier \n");
                scanf("%d",number);
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
        switch (option){
        case 3: //Uses concept of fall-through in switch cases for option 3
        case 1:
            do{
                printf("Enter the new supplier name.It cannot be more than 30 characters \n");
                scanf("%30s",name);
            }while(!ValidateName(name,'S'));
            strcpy(SupplierData[index].Name,name);
            printf("Supplier name updated successfully\n");
            if(option==1)
                break;
        case 2:
            do{
                printf("Enter the new phone number of the supplier \n");
                scanf("%s",number);
            }while(!ValidateNumber(number));
            strcpy(SupplierData[index].Number,number);
            printf("Supplier phone number updated successfully\n");
            break;
        }
    }
    printf("--------------------------------------\n");
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
        printf("%-5d%-15s%-15s\n",SupplierData[a].ID,SupplierData[a].Name,SupplierData[a].Number);
    }
    printf("----------------------------\n");
}
int main(){
    struct Supplier SupplierData[size];
    int NoOfSupplier=0;
}
