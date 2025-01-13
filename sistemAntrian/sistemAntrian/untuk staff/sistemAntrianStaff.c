// program sistem antrian 
// hanya "prototype"
// ekspetasi:
// connect ke database 
// more features
// kekurangan
// Error handling be like: "aight bro imma head out"


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>


// structure for the data
typedef struct 
{
    char nim[25];
    char namaPengunjung[50];
    char tanggalMasuk[50];
    char tujuan[50];
    char status[15];    

} dataPengunjung;


// Structure for each node in the queue
struct Node 
{
    dataPengunjung *data;
    struct Node* next;
};

// Structure for the queue
struct Queue 
{
    struct Node* front;
    struct Node* rear;
};


// Function to create each dataPengunjung
dataPengunjung *createDataPengunjung(char nim[],
                                     char namaPengunjung[],
                                     char tanggalMasuk[],
                                     char tujuan[],
                                     char status[])
{
    dataPengunjung *newDP = (dataPengunjung *) malloc(sizeof(dataPengunjung));
    if (newDP == NULL)
    {
        printf("fail to allocate memory\n");
        return NULL;
    }
    
    strcpy(newDP->nim, nim);
    strcpy(newDP->namaPengunjung, namaPengunjung);
    strcpy(newDP->tanggalMasuk, tanggalMasuk);
    strcpy(newDP->tujuan, tujuan);
    strcpy(newDP->status, status);
    
    return newDP;
}

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

void bersihkanDariNewline(char nim[], char namaPengunjung[],
                          char tanggalMasuk[], char tujuan[],
                          char status[])
{
    nim[strcspn(nim, "\n")] = 0;
    namaPengunjung[strcspn(namaPengunjung, "\n")] = 0;
    tanggalMasuk[strcspn(tanggalMasuk, "\n")] = 0;
    tujuan[strcspn(tujuan, "\n")] = 0;
    status[strcspn(status, "\n")] = 0;
}

void bersihkanNewline(char str[]) 
{
    str[strcspn(str, "\n")] = 0;
}

// Function to create an empty queue
struct Queue* createQueue() 
{
    struct Queue* queue = (struct Queue*)malloc(sizeof(struct Queue));
    queue->front = NULL;
    queue->rear = NULL;
    return queue;
}

// Function to check if the queue is empty
int isEmpty(struct Queue* queue) 
{
    return (queue->front == NULL);
}

// Function to enqueue an element into the queue
void enqueue(struct Queue* queue, dataPengunjung *data) 
{
    // Create a new node
    struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
    newNode->data = data;
    newNode->next = NULL;

    // If the queue is empty, set the new node as both front and rear
    if (isEmpty(queue)) 
    {
        queue->front = newNode;
        queue->rear = newNode;
    } else 
    {
        // Otherwise, update the rear node and link the new node at the rear
        queue->rear->next = newNode;
        queue->rear = newNode;
    }
}

// Function to dequeue an element from the queue
void dequeue(struct Queue* queue) 
{
    if (isEmpty(queue)) 
    {
        printf("Antrian kosong\n\n");
        return ;
    }
    printf("antrian pertama berhasil dikeluarkan\n\n");

    // Move the front pointer to the next node and free the old front node
    struct Node* temp = queue->front;
    queue->front = queue->front->next;
    free(temp->data);
    free(temp);

    // If the front becomes NULL, update the rear to NULL as well
    if (queue->front == NULL) 
        queue->rear = NULL;
    
    return ;
}

struct Node *find(struct Queue *q, char nim[])
{
    struct Node *temp = q->front;

    while (temp != NULL)
    {
        // jika ketemu
        if (!strcmp(temp->data->nim, nim))
            return temp;

        temp = temp->next;
    }

    // tidak ketemu
    return NULL;   
}

void printEachNode(struct Node *current)
{
    if (current != NULL)
    {
        printf("============================================\n");
        printf("nim: %s\n", current->data->nim);
        printf("nama pengunjung: %s\n", current->data->namaPengunjung);
        printf("tanggal dan waktu masuk: %s\n", current->data->tanggalMasuk);
        printf("tujuan: %s\n", current->data->tujuan);
        printf("status: %s\n", current->data->status);
        if (current->next == NULL)
            printf("============================================\n");
    }
}

// Function to display the elements of the queue
void displayQueue(struct Queue* queue) 
{
    if (isEmpty(queue)) 
    {
        printf("Antrian kosong\n\n");
        return;
    }

    struct Node* current = queue->front;
    printf("Isi Antrian Saat Ini:\n");
    while (current != NULL) 
    {
        printEachNode(current);

        current = current->next;
    }
    printf("\n");
}

int masukanIdentitasPengunjung(struct Queue *queue)
{
    // ambil data nim
    char nim[25];
    printf("masukan nim pengunjung (misal 2210511036): ");
    fgets(nim, 25, stdin);
    
    // ambil data namaPeminjam
    char namaPengunjung[50];
    printf("masukan nama Pengunjung: ");
    fgets(namaPengunjung, 50, stdin);
    
    // ambil data tanggalMasuk
    char tanggalMasuk[50];
    // ambil waktu saat ini
    time_t currentTime = time(NULL);

    // merubah nya ke waktu lokal
    struct tm *localTime = localtime(&currentTime);

    // format ke dalam string
    strftime(tanggalMasuk, sizeof(tanggalMasuk), "%Y-%m-%d %H:%M:%S", localTime);
    
    // ambil data tujuan
    char tujuan[50];
    printf("masukan tujuan datang ke perpustakaan: ");
    fgets(tujuan, 50, stdin);
    
    // ambil data tanggalPengembalian
    char status[15] = "tercatat";
    
    bersihkanDariNewline(nim, namaPengunjung, tanggalMasuk, tujuan, status);
    
    dataPengunjung *newDP = createDataPengunjung(nim, namaPengunjung,
                                                 tanggalMasuk, tujuan,
                                                 status);
    // masukan data kedalam dynamic array
    enqueue(queue, newDP);

    // perintah berhasil dijalankan
    return 1;
}

void editAntrian(struct Queue *queue)
{
    if (isEmpty(queue))
    {
        printf("antrian kosong, tidak bisa edit\n\n");
        return ;
    }

    // ambil data nim untuk mencari
    char nimCari[25];
    printf("masukan nim untuk mencari  (misal 2210511036): ");
    fgets(nimCari, 25, stdin);
    bersihkanNewline(nimCari);

    struct Node *target = find(queue, nimCari);

    if (target == NULL)
    {
        printf("antrian tidak ditemukan, tidak bisa edit\n\n");
        return ;
    }
    
    printf("Data Lama: \n");
    printf("============================================\n");
    printf("nim: %s\n", target->data->nim);
    printf("nama pengunjung: %s\n", target->data->namaPengunjung);
    printf("tanggal dan waktu masuk: %s\n", target->data->tanggalMasuk);
    printf("tujuan: %s\n", target->data->tujuan);
    printf("status: %s\n", target->data->status);
    printf("============================================\n\n");


    printf("Data Baru: \n");
    printf("============================================\n");
    // masukan data baru  
    // ambil data nim baru
    char nim[25];
    printf("masukan nim yang baru (misal 2210511036): ");
    fgets(nim, 25, stdin);
    
    // ambil data namaPeminjam baru
    char namaPengunjung[50];
    printf("masukan nama Pengunjung yang baru: ");
    fgets(namaPengunjung, 50, stdin);
    
    // ambil data tanggalMasuk baru
    char tanggalMasuk[] = "";
    
    // ambil data tujuan baru
    char tujuan[50];
    printf("masukan tujuan datang ke perpustakaan yang baru: ");
    fgets(tujuan, 50, stdin);
    
    // ambil data tanggalPengembalian baru
    char status[15];
    printf("masukan status yang baru (tercatat/dibatalkan): ");
    fgets(status, 15, stdin);

    bersihkanDariNewline(nim, namaPengunjung, tanggalMasuk, tujuan, status);

    strcpy(target->data->nim, nim);
    strcpy(target->data->namaPengunjung, namaPengunjung);
    strcpy(target->data->tujuan, tujuan);
    strcpy(target->data->status, status);
    printf("============================================\n\n");

}

void cariTiketSeseorang(struct Queue *queue)
{
     if (isEmpty(queue))
    {
        printf("antrian kosong, tidak bisa menampilkan tiket\n\n");
        return ;
    }

    // ambil data nim untuk mencari
    char nimCari[25];
    printf("masukan nim untuk mencari  (misal 2210511036): ");
    fgets(nimCari, 25, stdin);
    bersihkanNewline(nimCari);

    struct Node *target = find(queue, nimCari);

    if (target == NULL)
    {
        printf("antrian tidak ditemukan, tidak bisa menampilkan\n\n");
        return ;
    }
    
    printf("tiket dengan nim %s\n", nimCari);
    printEachNode(target);
    printf("\n");

}
            

int main()
{
    struct Queue* queue = createQueue();

    printf("========================================\n");
    printf("Program Antrian Perpustakaan untuk Staff\n");
    printf("========================================\n");

    while (1)
    {
        int pilihan = 0;
        printf("pilihan: \n");
        printf("[1] Tampilkan Antrian \n[2] Keluarkan antrian pertama \n");
        printf("[3] Masukan Antrian   \n[4] Edit Antrian \n");
        printf("[5] cari tiket seseorang \n[0] keluar\n");
        printf("--> ");
        if (! handleInput(&pilihan) ) continue;
        printf("\n");


        switch (pilihan)
        {
        case 1 :
            displayQueue(queue);
            break;
        case 2 :
            dequeue(queue);
            break;
        case 3 :
            masukanIdentitasPengunjung(queue);
            break;
        case 4 :
            editAntrian(queue);
            break;
        case 5:
            cariTiketSeseorang(queue);
            break;
        
        case 0 :
            printf("Anda keluar menu pengunjung\n");
            return 0;
        
        default:
            printf("pilihan tidak valid\n\n");
            break;
        }
    }

    return 0;
}


