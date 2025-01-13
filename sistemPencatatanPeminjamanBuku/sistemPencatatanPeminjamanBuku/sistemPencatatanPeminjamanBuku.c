

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// function declaration
int calculateDateDifference(char* tanggalPinjam, char* tanggalPengembalian);


// ========================================== bagian dynamic array
typedef struct
{
    // lama peminjaman buku dalam hari, misal 1 hari, 2 hari, etc
    // akan dijadikan sebagai nilai untuk pengurutan
    int lamaPeminjaman;

    char nim[25];
    char namaPeminjam[50];
    char namaBuku[50];
    char tanggalPinjam[15];
    char tanggalPengembalian[15];

} record;

typedef struct {
    record **array;
    size_t size;
    size_t capacity;
} DynamicArray;

DynamicArray* createDynamicArray(size_t initialCapacity) 
{
    DynamicArray* dynamicArray = malloc(sizeof(DynamicArray));
    dynamicArray->array = malloc(initialCapacity * sizeof(record*));
    dynamicArray->size = 0;
    dynamicArray->capacity = initialCapacity;
    return dynamicArray;
}

void destroyDynamicArray(DynamicArray* dynamicArray) 
{
    for (size_t i = 0; i < dynamicArray->size; i++)
    {
        free(dynamicArray->array[i]);
    }
    
    free(dynamicArray->array);
    free(dynamicArray);
}

void resizeDynamicArray(DynamicArray* dynamicArray, size_t newCapacity) 
{
    record** newArray = realloc(dynamicArray->array, newCapacity * sizeof(record*));
    if (newArray != NULL) {
        dynamicArray->array = newArray;
        dynamicArray->capacity = newCapacity;
    }
}

void pushBack(DynamicArray* dynamicArray, record *value) 
{
    if (dynamicArray->size >= dynamicArray->capacity) {
        size_t newCapacity = dynamicArray->capacity * 2; // Double the capacity
        resizeDynamicArray(dynamicArray, newCapacity);
    }
    dynamicArray->array[dynamicArray->size] = value;
    dynamicArray->size++;
}

record *createRecord(char nim[],
                     char namaPeminjam[],
                     char namaBuku[],
                     char tanggalPinjam[],
                     char tanggalPengembalian[]) 
{
    record *newRecord = (record *) malloc(sizeof(record));
    strcpy(newRecord->nim, nim);
    strcpy(newRecord->namaPeminjam, namaPeminjam);
    strcpy(newRecord->namaBuku, namaBuku);
    strcpy(newRecord->tanggalPinjam, tanggalPinjam);
    strcpy(newRecord->tanggalPengembalian, tanggalPengembalian);

    newRecord->lamaPeminjaman = calculateDateDifference(tanggalPinjam, tanggalPengembalian);

    if (newRecord->lamaPeminjaman < 0) {
        free(newRecord);
        printf("tanggal tidak valid, gagal membuat record baru\n");
        return NULL;
    }
    

    return newRecord;
}

void deleteRecord(DynamicArray *database)
{
    char nim[25];
    printf("masukan id kartu peminjam: ");
    fgets(nim, 50, stdin);
        
    int ketemu = 0;
    for (size_t i = 0; i < database->size; i++)
    {
        if ( !strcmp(nim, database->array[i]->nim) )
        {   
            record *target = database->array[i];
        
            free(target);
            
            // Shift elements after deletion point
            for (size_t j = i + 1; j < database->size; j++) 
                database->array[j - 1] = database->array[j];
               
            database->size--;  
            ketemu = 1;
            printf("record user ketemu dan sudah dihapus");
            break;
        }    
    }

    // jika tidak ketemu
    if (!ketemu)
        printf("user tidak ditemukan");
}

void deleteAllRecord(DynamicArray *database)
{
    for (size_t i = 0; i < database->size; i++)
    {
        record *delete = database->array[i]; 
        database->array[i] = NULL;
        free(delete);
    }
    database->size = 0;
    printf("Seluruh record telah dihapus");
}
// ========================================== akhir bagian dynamic array




// ========================================== bagian sorting
// sort ascending order
void bubbleSort(DynamicArray* dynamicArray) 
{
    size_t n = dynamicArray->size;
    record** array = dynamicArray->array;

    for (size_t i = 0; i < n - 1; i++) 
    {
        for (size_t j = 0; j < n - i - 1; j++) 
        {
            if (array[j]->lamaPeminjaman > array[j + 1]->lamaPeminjaman) 
            {
                // Swap the elements
                record * temp = array[j];
                array[j] = array[j + 1];
                array[j + 1] = temp;
            }
        }
    }
}
// ========================================== akhir bagian sorting

// ========================================== bagian parsing tanggal
int isLeapYear(int year) 
{
    if (year % 400 == 0)
        return 1;
    if (year % 100 == 0)
        return 0;
    if (year % 4 == 0)
        return 1;
    return 0;
}

int daysInMonth(int month, int year) 
{
    int days[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    if (month == 2 && isLeapYear(year))
        return 29;

    return days[month - 1];
}

int calculateDateDifference(char* tanggalPinjam, char* tanggalPengembalian) 
{
    int day1, month1, year1, day2, month2, year2;

    // cek apakah format tanggal benar, jika salah return -1
    if( sscanf(tanggalPinjam, "%d-%d-%d", &day1, &month1, &year1) != 3) 
        return -1;
    if( sscanf(tanggalPengembalian, "%d-%d-%d", &day2, &month2, &year2) != 3) 
        return -1;
    
    
    int days = 0;


    // Calculate the difference in years
    for (int year = year1; year < year2; year++) {
        days += isLeapYear(year) ? 366 : 365;
    }

    // Calculate the difference in days within the same year
    for (int month = 1; month < month1; month++) {
        days -= daysInMonth(month, year1);
    }
    days -= day1;

    for (int month = 1; month < month2; month++) {
        days += daysInMonth(month, year2);
    }
    days += day2;

    return days;
}
// ========================================== akhir bagian parsing tanggal


// ========================================== bagian ui command line
void clearInputBuffer()
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF) { }
}

int handleInput(int *n)
{
    // jika input sukses
    if ( scanf("%d", n) ) {
        clearInputBuffer();
        return 1;
    }
    else 
        printf("input Tidak Valid, ulangi lagi\n\n");
    
    clearInputBuffer();
    return 0;
}

int masukanIdentitasPeminjam(DynamicArray *database)
{
    // ambil data nim
    char nim[25];
    printf("masukan nim peminjam (misal 2210511036): ");
    fgets(nim, 25, stdin);
    
    // ambil data namaPeminjam
    char namaPeminjam[50];
    printf("masukan nama peminjam: ");
    fgets(namaPeminjam, 50, stdin);
    
    // ambil data namaBuku
    char namaBuku[50];
    printf("masukan nama buku yang dipinjam: ");
    fgets(namaBuku, 50, stdin);
    
    // ambil data tanggalPinjam
    char tanggalPinjam[15];
    printf("masukan tanggal pinjam (tanggal-bulan-tahun, misal 24-11-2022): ");
    fgets(tanggalPinjam, 15, stdin);
    
    // ambil data tanggalPengembalian
    char tanggalPengembalian[15];
    printf("masukan tanggal pengembalian (tanggal-bulan-tahun, misal 25-11-2022): ");
    fgets(tanggalPengembalian, 15, stdin);
    

    // masukan data kedalam dynamic array
    record *newR = createRecord(nim, namaPeminjam,
                                namaBuku, tanggalPinjam,
                                tanggalPengembalian);

    if (!newR) {
        // operasi gagal
        return 0;
    }
    
    pushBack(database, newR);
    return 1;
}

void printRecord(record *r)
{
    if (r != NULL)
    {
        printf("=========================================\n");
        printf("nim: %s", r->nim);
        printf("nama peminjam: %s", r->namaPeminjam);
        printf("nama buku: %s", r->namaBuku);
        printf("tanggal pinjam: %s", r->tanggalPinjam);
        printf("tanggal pengembalian: %s", r->tanggalPengembalian);
        printf("=========================================\n");
    }
}

void printSeluruhPeminjam(DynamicArray *dArray, size_t size)
{
    printf("\n\n");

    // jika kosong
    if (!size)
    {
        printf("Database kosong");
        return;
    }
    else
        printf("Database Peminjam\n");
    
    
    

    for (size_t i = 0; i < size; i++)
    {
        printRecord(dArray->array[i]);
    }
}



record *cariSequential(DynamicArray *database ,char nim[])
{
    printf("\n\n");
    for (size_t i = 0; i < database->size; i++)
    {
        if (!strcmp(database->array[i]->nim, nim))
        {
            printf("record ditemukan\n");
            return database->array[i];
        }
    }
    printf("tidak ada record dengan id kartu tersebut\n");
    return NULL;
}

record *cariBinarySearch(DynamicArray *database, char nim[], int lamaPinjam)
{
    printf("\n\n");
    int upper = database->size-1, lower = 0, middle;

    while (lower <= upper) {
        middle = (upper + lower) / 2;
        
        if (lamaPinjam == database->array[middle]->lamaPeminjaman && 
            !strcmp(nim, database->array[middle]->nim)) {
            printf("record ditemukan\n");
            return database->array[middle];
        } else if (lamaPinjam < database->array[middle]->lamaPeminjaman) {
            upper = middle - 1; // target is below middle
        } else {
            lower = middle + 1; // target is above middle
        }
    }

    if (lower > upper) {
        printf("record tidak ditemukan\n");
        return NULL;
    }
}


void cariPeminjam(DynamicArray *database)
{
    char nim[25];
    printf("masukan id kartu peminjam: ");
    fgets(nim, 50, stdin);
    
    int apakahTahu = 0;
    printf("apakah anda tahu lama peminjaman bukunya?\n");
    printf("[1] Ya\n");
    printf("[0] Tidak\n");

    if ( !handleInput(&apakahTahu) )
        return;
    
    
    if (apakahTahu)
    {
        int lamaPinjam;
        printf("masukan lama peminjaman: ");
        if ( !handleInput(&lamaPinjam) )
            return;

        printRecord( cariBinarySearch(database, nim, lamaPinjam) );
    }else
    {
        printRecord( cariSequential(database, nim) );
    }
}

// ========================================== akhir bagian ui command line

int main()
{
    DynamicArray *database = createDynamicArray(10);
    
    printf("===================================================\n");
    printf("Selamat datang di sistem pencatatan peminjaman buku\n");
    printf("===================================================\n");

    while (1)
    {
        int pilihan = 0;

        printf("opsi: \n");
        printf("[1] print seluruh peminjam buku\n");
        printf("[2] masukan identitas peminjam buku\n");
        printf("[3] cari peminjam\n");
        printf("[4] hapus record peminjam\n");
        printf("[5] hapus seluruh record\n");
        printf("[0] keluar dari program\n");
        printf("--> ");
        handleInput(&pilihan);
        printf("\n\n");
        
        switch (pilihan)
        {
        case 0:
            printf("keluar dari program\n");
            printf("\n\n");
            return 0;

        case 1:
            printSeluruhPeminjam(database, database->size);
            printf("\n\n");
            break;

        case 2:
            if (masukanIdentitasPeminjam(database))
                bubbleSort(database);
            
            printf("\n\n");
            break;

        case 3:
            cariPeminjam(database);
            printf("\n\n");
            
            break;

        case 4:
            deleteRecord(database);
            printf("\n\n");
            
            break;
        case 5:
            deleteAllRecord(database);
            printf("\n\n");
            
            break;

        default:
            printf("input tidak valid\n");
            printf("\n\n");
            break;
        }
    }
    return 0;
}

