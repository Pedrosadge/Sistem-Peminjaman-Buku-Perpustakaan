// program sistem databasebuku
// hanya "prototype"
// ekspetasi:
// connect ke database 
// more features
// kekurangan
// Error handling be like: "aight bro imma head out"


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LENGTH 256

void clearInputBuffer()
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF) { }
}

int handleInput(int *n)
{
    // jika input sukses
    if ( scanf("%d", n) )  {
        clearInputBuffer();
        return 1;
    }
    else 
        printf("input Tidak Valid, ulangi lagi\n\n");
    
    clearInputBuffer();
    return 0;
}

void tampilkan()
{
    // ======================= READING FROM A TXT FILE =======================
    // Open the input file for reading
    FILE *inputFile = fopen("databaseBuku.txt", "r");
    char line[MAX_LENGTH];
    if (inputFile == NULL) 
    {
        printf("Failed to open the file.\n");
        return;
    }
    // Read lines from the input file and display them
    printf("\nIsi Database Buku:\n");
    while (fgets(line, MAX_LENGTH, inputFile) != NULL) 
    {
        // ketemu delimiter block
        if (strstr(line, "=") != NULL)
        {
            printf("==========================================\n");
            continue;
        }
        
        printf("%s", line);
    }

    fclose(inputFile);
}

void cariBuku()
{
    FILE *inputFile = fopen("databaseBuku.txt", "r");
    
    if (inputFile == NULL) 
    {
        printf("Failed to open the file.\n");
        return;
    }

    char line[MAX_LENGTH];
    char block[MAX_LENGTH * 2];


    char titleBuku[MAX_LENGTH];
    printf("title buku yang ingin dicari: ");
    fgets(titleBuku, MAX_LENGTH, stdin);

 
    while (fgets(line, MAX_LENGTH, inputFile) != NULL)
    {
        // if encounter "=", title is found, 
        // skip add to block,  
        if (!strcmp(line, "=\n"))
            continue;
        
        
        strcat(block, line);

        if (strstr(block, "status") != NULL)
        {
            // perform search
            if (strstr(block, titleBuku) != NULL)
            {
                printf("buku ditemukan\n");
                printf("==========================================\n");
                printf("%s", block);
                printf("==========================================\n");
                return;
            }
            strcpy(block, "");
        }
    }
    fclose(inputFile); 
    printf("buku tidak ditemukan\n\n\n");
}


int main()
{
    printf("\n\n");
    printf("========================================\n");
    printf("Prototype Database Buku untuk Pengunjung\n");
    printf("========================================\n");  

    while (1)
    {    
        int pilihan;
        printf("pilihan: \n");
        printf("[1] lihat seluruh buku dari database\n");
        printf("[2] cari buku di database\n");
        printf("[0] keluar \n");
        printf("--> ");
        if (! handleInput(&pilihan) ) continue;
        printf("\n");

        switch (pilihan)
        {
        case 1:
            tampilkan();
            break;
        case 2:
            cariBuku();
            break;
        case 0:
            printf("Anda keluar dari program\n");
            return 0;
        
        default:
            printf("pilihan tidak valid\n");
            break;
        }
    }
}


