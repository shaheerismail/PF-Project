#ifndef SUPPLIER_H
#define SUPPLIER_H
struct Supplier
{
    int ID;
    char Name[31];
    char Number[16];
};
int ReadData(struct Supplier SupplierData[],int *ptr);
void WriteData(struct Supplier SupplierData[],int NoOfSupplier);
void AddSupplier(struct Supplier SupplierData[],int *ptr);
int SearchSupplier(struct Supplier SupplierData[],int NoOfSupplier);
void ModifySupplier(struct Supplier SupplierData[],int NoOfSupplier);
void DeleteSupplier(struct Supplier SupplierData[],int *ptr);
void DisplaySupplierRecords(struct Supplier SupplierData[],int NoOfSupplier);
#endif