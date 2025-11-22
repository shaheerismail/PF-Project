#ifndef MEDICINE_H
#define MEDICINE_H
struct Medicine
{
    int ID;
    char Name[31];
    int Quantity;
};
int LoadData(struct Medicine MedicineData[],int *ptr);
void SaveData(struct Medicine MedicineData[],int NoOfMedicine);
void AddMedicine(struct Medicine MedicineData[],int *ptr);
int SearchMedicine(struct Medicine MedicineData[],int NoOfMedicine);
void ModifyMedicine(struct Medicine MedicineData[],int NoOfMedicine);
void DeleteMedicine(struct Medicine MedicineData[],int *ptr);
void DisplayInventory(struct Medicine MedicineData[],int NoOfMedicine);
#endif