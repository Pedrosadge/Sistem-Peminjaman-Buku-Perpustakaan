// program sistem databasebuku
// hanya "prototype"
// ekspetasi:
// connect ke database 
// more features

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

// case 1 tampilkan isi seluruh buku pada database?
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

void masukanBuku()
{
    // ======================= WRITING TO A TXT FILE =======================
    // Open the output file for writing
    FILE *outputFile = fopen("databaseBuku.txt", "a");
    char line[MAX_LENGTH];
    if (outputFile == NULL) 
    {
        printf("Failed to open/create the output file.\n");
        return ;
    }

    int berapa = 0;
    printf("berapa buku yang ingin dimasukan?\n");
    printf("--> ");
    if (! handleInput(&berapa) ) return;
    
    printf("\nformat buku: \ntitle buku \ntitle penjelas \nauthor\n\n");
    for (size_t i = 0; i < berapa; i++)
    {    
        // Write to the output file
        printf("Masukan info buku (Ctrl + Z lalu enter untuk selesai):\n");

        while (fgets(line, MAX_LENGTH, stdin) != NULL) 
            fprintf(outputFile, "%s", line);
            
        fprintf(outputFile, "info lebih lanjut tentang buku tersebut\nstatus: bisa dipinjam\n");
        fprintf(outputFile, "=\n");
    }
    fclose(outputFile);       
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

void hapusBuku()
{

    FILE *inputFile = fopen("databaseBuku.txt", "r");
    FILE *outputFile = fopen("temp.txt", "w");

    if (inputFile == NULL) 
    {
        printf("Failed to open the file.\n");
        return;
    }
    if (outputFile == NULL) 
    {
        printf("Failed to open/create the output file.\n");
        return ;
    }

    char line[MAX_LENGTH];
    char block[MAX_LENGTH * 2];

    char titleBuku[MAX_LENGTH];
    printf("title buku yang ingin dihapus: ");
    fgets(titleBuku, MAX_LENGTH, stdin);

    int ketemu = 0;
    while (fgets(line, MAX_LENGTH, inputFile) != NULL)
    {        
        strcat(block, line);

        if (strstr(block, "status") != NULL)
        {
            // perform search, if ketemu, g dimasukin kedalam output txt
            if (strstr(block, titleBuku) != NULL)
            {
                printf("data buku berhasil dihapus\n");
                strcpy(block, "");
                ketemu = 1;
                continue;
            }
                
            
            fprintf(outputFile, "%s", block);
            strcpy(block, "");
        }
    }

    if (!ketemu)        
        printf("gagal menghapus\nbuku tidak ditemukan\n");
    
    
    fclose(inputFile);
    remove("databaseBuku.txt");
    
    fprintf(outputFile, "=\n");
    fclose(outputFile);

    rename("temp.txt", "databaseBuku.txt");
}

int main()
{
    while (1)
    {    
        printf("\n\n");
        printf("===================================\n");
        printf("Prototype Database Buku untuk Staff\n");
        printf("===================================\n");

        int pilihan;
        printf("Anda masuk sebagai Staff\n");
        printf("pilihan: \n");
        printf("[1] lihat seluruh buku dari database\n");
        printf("[2] masukan buku baru ke dalam database\n");
        printf("[3] cari buku di database\n");
        printf("[4] hapus buku dari database\n");
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
            masukanBuku();
            break;
        case 3:
            cariBuku();
            break;
        case 4:
            hapusBuku();
            break;
        case 0:
            printf("anda keluar dari program\n");
            return 0;
        default:
            printf("pilihan tidak valid\n");
            break;
        }
    }
}


