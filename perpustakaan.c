#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

// =========== Struct ===========================
struct buku
{
    int idBuku;
    char judul[100];
    char pengarang[100];
    char tahun[10];
    int status; // 0 = dipinjam, 1 = tersedia
};

struct heapBuku {
    struct buku *data;
    int size;
    int capacity;
};

struct bstNode {
    struct buku data;
    struct bstNode *left;
    struct bstNode *right;
    int height;
};

struct akun
{
    int id;
    char username[20];
    char password[20];
};

struct node
{
    struct buku data;
    struct node *next;
};

struct queue
{
    struct node *front;
    struct node *rear;
    int size;
};

// =========== Function Prototypes ==============
void tampilPerBuku(struct buku *buku);
FILE *openFile(const char *filename, const char *mode);

// =========== BST Function Prototypes ============
struct bstNode* createNode(struct buku data);
struct bstNode* searchBST(struct bstNode* root, int idBuku);
struct bstNode* insertBST(struct bstNode* root, struct buku data);
struct bstNode* minValueNode(struct bstNode* node);
struct bstNode* deleteBST(struct bstNode* root, int idBuku);
struct bstNode* bacaBukuBST();
void freeBST(struct bstNode* root);
int countNodes(struct bstNode* root);
void storeInorder(struct bstNode* root, struct bstNode** arr, int* index);
void lihatBukuUrutan(struct bstNode** nodes, int count, int sortType);
void lihatSemuaBukuBST(struct bstNode* root);
int compareJudulAZ(const void* a, const void* b);
int compareJudulZA(const void* a, const void* b);
void cariBukuBST(struct bstNode* root);
struct bstNode* tambahBukuBST(struct bstNode* root);
struct bstNode* hapusBukuBST(struct bstNode* root);
void tampilkanMenuBST();
void bstHome(int idUser, char username[]);
void menuHeap(int idUser, char username[]);

// =========== Heap Function Prototypes ==========
struct heapBuku* createHeap(int capacity);
void freeHeap(struct heapBuku* heap);
void heapify(struct heapBuku* heap, int i);
void heapifyMinID(struct heapBuku* heap, int i);
void heapifyMaxJudul(struct heapBuku* heap, int i);
void heapifyMinJudul(struct heapBuku* heap, int i);
void insertHeap(struct heapBuku* heap, struct buku book);
struct buku extractMax(struct heapBuku* heap);
void displayHeap(struct heapBuku* heap);
void searchInHeap(struct heapBuku* heap, int idBuku);
struct heapBuku* bacaBukuHeap();
void buildHeap(struct heapBuku* heap, int heapType, int sortBy);
void tampilanMenuHeap();
void tampilkanHeap(int idUser, char username[], struct heapBuku* heap, int heapType, int sortBy);
void cariBukuHeap(int idUser, char username[], struct heapBuku* heap);
void tambahBukuHeap(int idUser, char username[], struct heapBuku* heap, int heapType, int sortBy);
void hapusBukuPrioritas(int idUser, char username[], struct heapBuku* heap, int heapType, int sortBy);

// ================= Linear ==============================

FILE *openFile(const char *filename, const char *mode)
{
    FILE *file = fopen(filename, mode);
    if (file == NULL)
    {
        printf("Gagal membuka file %s\n", filename);
        return NULL;
    }
    return file;
}

void initQueue(struct queue *queue)
{
    queue->front = NULL;
    queue->rear = NULL;
    queue->size = 0;
}

int isEmpty(struct queue *queue)
{
    return queue->size == 0;
}

void enqueue(struct queue *queue, struct buku buku)
{
    struct node *newNode = (struct node *)malloc(sizeof(struct node));
    if (newNode == NULL)
    {
        printf("Gagal mengalokasi memori untuk buku baru.\n");
        return;
    }
    newNode->data = buku;
    newNode->next = NULL;

    if (isEmpty(queue))
    {
        queue->front = newNode;
    }
    else
    {
        queue->rear->next = newNode;
    }
    queue->rear = newNode;
    queue->size++;
}

struct buku dequeue(struct queue *queue)
{
    struct buku emptyBuku = {0};
    if (isEmpty(queue))
    {
        printf("Queue kosong.\n");
        return emptyBuku;
    }

    struct node *temp = queue->front;
    struct buku data = temp->data;

    queue->front = queue->front->next;
    queue->size--;

    if (queue->size == 0)
    {
        queue->rear = NULL;
    }

    free(temp);
    return data;
}

void freeQueue(struct queue *queue)
{
    if (queue == NULL)
        return;

    while (!isEmpty(queue))
    {
        dequeue(queue);
    }
}

void toLowerString(char *str)
{
    for (int i = 0; str[i]; i++)
    {
        str[i] = tolower(str[i]);
    }
}

int bacaBuku(struct queue *queue)
{
    FILE *file = openFile("databuku.txt", "r");
    if (file == NULL)
        return 0;

    initQueue(queue);
    struct buku tempBuku;
    int jumlah = 0;

    while (fscanf(file, "%d#%99[^#]#%99[^#]#%9[^#]#%d\n",
                  &tempBuku.idBuku, tempBuku.judul, tempBuku.pengarang,
                  tempBuku.tahun, &tempBuku.status) == 5)
    {
        enqueue(queue, tempBuku);
        jumlah++;
    }

    fclose(file);
    return jumlah;
}

void tampilPerBuku(struct buku *buku)
{
    printf("\n=== Buku %d ===\n", buku->idBuku);
    printf("Judul    : %s\n", buku->judul);
    printf("Pengarang: %s\n", buku->pengarang);
    printf("Tahun    : %s\n", buku->tahun);
    printf("Status   : %s\n", buku->status == 1 ? "Tersedia" : "Dipinjam");
    printf("====================\n");
}

void ambilBuku(struct buku buku, int idUser, char username[])
{
    if (buku.status == 0)
    {
        printf("Buku \"%s\" sedang dipinjam, tidak dapat diambil.\n", buku.judul);

        char pilihan;
        printf("(q) Kembali ke Home\n");
        printf("Pilihan: ");
        scanf(" %c", &pilihan);

        if (pilihan == 'q')
        {
            return;
        }
        else
        {
            printf("Input tidak valid!\n");
        }
        return;
    }

    // catat semua buku yang dipinjam, kapan dan siapa yang pinjam
    FILE *takenFile = openFile("takenBook.txt", "a");
    if (takenFile == NULL)
        return;

    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    // Format: idBuku#idUser#tanggal
    fprintf(takenFile, "%d#%d#%04d-%02d-%02d\n",
            buku.idBuku, idUser, tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday);

    fclose(takenFile);

    // file history overall
    FILE *historyFile = openFile("history.txt", "a");
    if (historyFile != NULL)
    {
        fprintf(historyFile, "%s#%s#%s#%04d-%02d-%02d#Meminjam\n",
                username, buku.judul, buku.pengarang,
                tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday);
        fclose(historyFile);
    }

    // file history tiap user
    char historyUser[50];
    sprintf(historyUser, "history%s.txt", username);
    FILE *userHistoryFile = openFile(historyUser, "a");
    if (userHistoryFile == NULL)
        return;

    // Format: idBuku#judul#pengarang#status#tanggalPinjam
    fprintf(userHistoryFile, "%d#%s#%s#%s#%d#%04d-%02d-%02d#Meminjam\n",
            buku.idBuku, buku.judul, buku.pengarang, buku.tahun, 0,
            tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday); // status 0 = dipinjam

    fclose(userHistoryFile);

    printf("Buku \"%s\" berhasil diambil!\n", buku.judul);

    // update status buku di databuku jadi 0
    FILE *file = openFile("databuku.txt", "r");
    FILE *tempFile = openFile("temp.txt", "w");

    if (file == NULL || tempFile == NULL)
    {
        if (file != NULL)
            fclose(file);
        if (tempFile != NULL)
            fclose(tempFile);
        return;
    }

    struct buku tempBuku;
    // salin databuku.txt ke temp.txt
    while (fscanf(file, "%d#%99[^#]#%99[^#]#%9[^#]#%d\n",
                  &tempBuku.idBuku, tempBuku.judul, tempBuku.pengarang, tempBuku.tahun, &tempBuku.status) == 5)
    {
        if (tempBuku.idBuku == buku.idBuku)
        {
            tempBuku.status = 0; // ubah status buku yang dipinjam
        }
        fprintf(tempFile, "%d#%s#%s#%s#%d\n",
                tempBuku.idBuku, tempBuku.judul, tempBuku.pengarang, tempBuku.tahun, tempBuku.status);
    }

    fclose(file);
    fclose(tempFile);
    remove("databuku.txt");             // hapus databuku.txt lama yang belum diupdate
    rename("temp.txt", "databuku.txt"); // databuku.txt yang sudah diupdate statusnya
}

void lihatBuku(int idUser, struct queue *queue, char username[])
{
    if (isEmpty(queue))
    {
        printf("Tidak ada data buku.\n");
        return;
    }

    struct node *current = queue->front;
    char pilihan;

    while (1)
    {
        tampilPerBuku(&current->data);

        printf("Pilih aksi: (n) Buku Selanjutnya | (p) Buku Sebelumnya | (o) Ambil Buku | (q) Kembali Ke Home\n");
        printf("Pilihan: ");
        scanf(" %c", &pilihan);

        switch (pilihan)
        {
        case 'n':
            if (current->next != NULL)
            {
                current = current->next;
            }
            else
            {
                current = queue->front;
            }
            break;
        case 'p':
            if (current == queue->front)
            {
                // cari node terakhir
                struct node *temp = queue->front;
                while (temp->next != NULL)
                {
                    temp = temp->next;
                }
                current = temp;
            }
            else
            {
                // cari node sebelumnya
                struct node *temp = queue->front;
                while (temp->next != current)
                {
                    temp = temp->next;
                }
                current = temp;
            }
            break;
        case 'o':
            if (current->data.status == 1)
            {
                ambilBuku(current->data, idUser, username);
                current->data.status = 0;
            }
            else
            {
                printf("Buku \"%s\" sedang dipinjam, silahkan ke menu reservasi buku.\n", current->data.judul);
            }
            break;
        case 'q':
            return;
        default:
            printf("Pilihan tidak valid!\n");
        }
    }
}

void lihatBukuDiambil(int idUser, struct queue *queue, char username[])
{
    FILE *file = openFile("takenBook.txt", "r");
    if (file == NULL)
    {
        printf("Belum ada buku yang dipinjam.\n");
        return;
    }

    int idBuku, userID;
    char tanggal[11];
    int found = 0;

    printf("\n=== Buku yang Anda Pinjam ===\n");

    while (fscanf(file, "%d#%d#%10s\n", &idBuku, &userID, tanggal) == 3)
    {
        if (userID == idUser)
        {
            found = 1;

            struct node *current = queue->front;
            while (current != NULL)
            {
                if (current->data.idBuku == idBuku)
                {
                    printf("Judul           : %s\n", current->data.judul);
                    printf("Pengarang       : %s\n", current->data.pengarang);
                    printf("Tahun           : %s\n", current->data.tahun);
                    printf("Tanggal Pinjam  : %s\n", tanggal);
                    printf("=============================\n");
                    break;
                }
                current = current->next;
            }
        }
    }

    if (!found)
    {
        printf("Anda tidak sedang meminjam buku apapun.\n");
    }

    fclose(file);
}

void kembalikanBuku(int idUser, struct queue *queue, char username[])
{
    FILE *file = openFile("takenBook.txt", "r");
    if (file == NULL)
    {
        printf("Belum ada buku yang dipinjam.\n");
        return;
    }

    int idBuku, userID;
    char tanggal[11];
    int found = 0;
    int count = 0;

    int bukuDipinjam[100];

    printf("\n=== Buku yang Anda Pinjam ===\n");

    while (fscanf(file, "%d#%d#%10s\n", &idBuku, &userID, tanggal) == 3)
    {
        if (userID == idUser)
        {
            found = 1;

            struct node *current = queue->front;
            while (current != NULL)
            {
                if (current->data.idBuku == idBuku)
                {
                    count++;
                    bukuDipinjam[count - 1] = idBuku;

                    printf("%d. Judul    : %s\n", count, current->data.judul);
                    printf("   Pengarang: %s\n", current->data.pengarang);
                    printf("   Tahun    : %s\n", current->data.tahun);
                    printf("   Tanggal Pinjam: %s\n", tanggal);
                    printf("   ====================\n");
                    break;
                }
                current = current->next;
            }
        }
    }

    if (!found)
    {
        printf("Anda tidak sedang meminjam buku apapun.\n");
        fclose(file);
        return;
    }

    int pilihan;
    printf("\nPilih nomor buku yang ingin dikembalikan (1-%d) atau 0 untuk batal: ", count);
    scanf("%d", &pilihan);

    if (pilihan < 1 || pilihan > count)
    {
        printf("Pengembalian dibatalkan karena pilihan tidak valid.\n");
        fclose(file);
        return;
    }

    int target = bukuDipinjam[pilihan - 1];

    fclose(file);
    file = openFile("takenBook.txt", "r");
    FILE *tempFile = openFile("temp.txt", "w");
    FILE *historyFile = openFile("history.txt", "a");

    if (file == NULL || tempFile == NULL || historyFile == NULL)
        return;

    while (fscanf(file, "%d#%d#%10s\n", &idBuku, &userID, tanggal) == 3)
    {
        if (userID == idUser && idBuku == target)
        {
            // update database
            FILE *dataFile = openFile("databuku.txt", "r");
            FILE *tempDataFile = openFile("tempData.txt", "w");

            if (dataFile != NULL && tempDataFile != NULL)
            {
                struct buku tempBuku;
                while (fscanf(dataFile, "%d#%99[^#]#%99[^#]#%9[^#]#%d\n",
                              &tempBuku.idBuku, tempBuku.judul, tempBuku.pengarang,
                              tempBuku.tahun, &tempBuku.status) == 5)
                {
                    if (tempBuku.idBuku == target)
                    {
                        tempBuku.status = 1; // update status jadi tersedia
                    }
                    fprintf(tempDataFile, "%d#%s#%s#%s#%d\n",
                            tempBuku.idBuku, tempBuku.judul, tempBuku.pengarang,
                            tempBuku.tahun, tempBuku.status);
                }
                fclose(dataFile);
                fclose(tempDataFile);
                remove("databuku.txt");
                rename("tempData.txt", "databuku.txt");

                // update queue 
                freeQueue(queue);
                initQueue(queue);
                bacaBuku(queue);

                // cari buku yang dikembalikan di queue
                struct node *current = queue->front;
                while (current != NULL)
                {
                    if (current->data.idBuku == target)
                    {
                        printf("\nBuku \"%s\" telah dikembalikan.\n", current->data.judul);

                        // Catat pengembalian di history
                        char historyFileName[50];
                        sprintf(historyFileName, "history%s.txt", username);
                        FILE *userHistoryFile = openFile(historyFileName, "a");
                        if (userHistoryFile != NULL)
                        {
                            time_t t = time(NULL);
                            struct tm tm = *localtime(&t);
                            fprintf(userHistoryFile, "%d#%s#%s#%s#%d#%04d-%02d-%02d#Mengembalikan\n",
                                    idBuku, current->data.judul, current->data.pengarang, current->data.tahun,
                                    1, // status 1 = dikembalikan
                                    tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday);
                            fclose(userHistoryFile);
                        }
                        break;
                    }
                    current = current->next;
                }
            }
        }
        else
        {
            fprintf(tempFile, "%d#%d#%s\n", idBuku, userID, tanggal);
        }
    }
    fclose(file);
    fclose(tempFile);
    fclose(historyFile);

    remove("takenBook.txt");
    rename("temp.txt", "takenBook.txt");
    printf("Pengembalian buku berhasil!\n");
}

void lihatHistory(int idUser, char username[])
{
    // buka file history user
    char historyFileName[50];
    sprintf(historyFileName, "history%s.txt", username);
    FILE *file = openFile(historyFileName, "r");

    if (file == NULL)
    {
        printf("Belum ada riwayat peminjaman atau pengembalian buku.\n");
        return;
    }

    int idBuku, status;
    char judul[100], pengarang[100], tahun[10], tanggal[11], aksi[15];
    printf("\n=== Riwayat Peminjaman dan Pengembalian Buku ===\n");

    int found = 0;

    // Format: idBuku#judul#pengarang#status#tanggalPinjam
    while (fscanf(file, "%d#%99[^#]#%99[^#]#%9[^#]#%d#%10[^#]#%[^\n]\n",
                  &idBuku, judul, pengarang, tahun, &status, tanggal, aksi) == 7)
    {
        printf("Judul         : %s\n", judul);
        printf("Pengarang     : %s\n", pengarang);
        printf("Tahun         : %s\n", tahun);
        printf("Tanggal       : %s\n", tanggal);
        printf("Aksi          : %s\n", aksi);
        printf("====================\n");
        found = 1;
    }

    if (!found)
    {
        printf("Belum ada riwayat peminjaman atau pengembalian buku.\n");
    }
    else
    {
        char pilihan;
        printf("(q) Kembali ke Home\n");
        printf("Pilihan: ");
        scanf(" %c", &pilihan);

        if (pilihan == 'q')
        {
            fclose(file);
            return;
        }
        else
        {
            printf("Input tidak valid!\n");
        }
    }

    fclose(file);
}

void reservasiBuku(int idUser, struct queue *queue, char username[])
{
    FILE *reservasiFile = openFile("reservasi.txt", "a");
    if (reservasiFile == NULL)
        return;

    char judulBuku[100];
    int found = 0;

    printf("Masukkan Judul Buku yang ingin dipesan: ");
    getchar();
    fgets(judulBuku, sizeof(judulBuku), stdin);
    judulBuku[strcspn(judulBuku, "\n")] = '\0';

    struct node *current = queue->front;
    while (current != NULL)
    {
        if (strcasecmp(current->data.judul, judulBuku) == 0)
        {
            found = 1;

            FILE *checkFile = openFile("reservasi.txt", "r");
            if (checkFile == NULL)
            {
                fclose(reservasiFile);
                return;
            }

            char existingUsername[20];
            int existingIdBuku;
            int sudahReservasi = 0;

            while (fscanf(checkFile, "%[^#]#%d#%*[^\n]\n", existingUsername, &existingIdBuku) == 2)
            {
                if (strcmp(existingUsername, username) == 0 && existingIdBuku == current->data.idBuku)
                {
                    sudahReservasi = 1;
                    break;
                }
            }
            fclose(checkFile);

            if (current->data.status == 0 && !sudahReservasi)
            {
                fprintf(reservasiFile, "%s#%d#%s\n", username, current->data.idBuku, current->data.judul);
                printf("Reservasi buku \"%s\" berhasil! Anda akan mendapat prioritas saat buku tersedia.\n",
                       current->data.judul);
            }
            else if (sudahReservasi)
            {
                printf("Anda sudah memiliki reservasi untuk buku ini.\n");
            }
            else
            {
                printf("Buku \"%s\" tersedia, Anda bisa langsung meminjam.\n", current->data.judul);
            }
            break;
        }
        current = current->next;
    }

    if (!found)
    {
        printf("Judul Buku tidak ditemukan.\n");
    }

    fclose(reservasiFile);
}

void cariBuku(int idUser, struct queue *queue, char username[])
{
    char target[100], judulLow[100], targetLow[100];
    int found = 0;
    char pilihan;

    printf("Masukkan judul buku yang dicari: ");
    scanf(" %[^\n]", target);

    strcpy(targetLow, target);
    toLowerString(targetLow);

    struct node *current = queue->front;
    while (current != NULL)
    {
        strcpy(judulLow, current->data.judul);
        toLowerString(judulLow);

        if (strcmp(targetLow, judulLow) == 0)
        {
            printf("\n=== Buku Ditemukan ===\n");
            printf("Judul    : %s\n", current->data.judul);
            printf("Pengarang: %s\n", current->data.pengarang);
            printf("Tahun    : %s\n", current->data.tahun);
            printf("Status   : %s\n", current->data.status == 1 ? "Tersedia" : "Dipinjam");
            printf("====================\n");
            found = 1;

            printf("Pilih aksi: (o) Ambil Buku | (q) Kembali Ke Home\n");
            printf("Pilihan Anda: ");
            scanf(" %c", &pilihan);

            if (pilihan == 'o')
            {
                ambilBuku(current->data, idUser, username);
                current->data.status = 0; // Update status in memory
            }
            return;
        }
        current = current->next;
    }

    if (!found)
    {
        printf("Buku dengan judul \"%s\" tidak ditemukan.\n", target);
    }
}

void pinjamBuku(int idUser, struct queue *queue, char username[])
{
    char target[100], judulLow[100], targetLow[100];
    int found = 0;
    struct node *current = queue->front;
    struct buku *bukuPilihan = NULL;

    printf("Masukkan judul buku yang ingin dipinjam: ");
    scanf(" %[^\n]", target);

    strcpy(targetLow, target);
    toLowerString(targetLow);

    while (current != NULL)
    {
        strcpy(judulLow, current->data.judul);
        toLowerString(judulLow);

        if (strcmp(targetLow, judulLow) == 0)
        {
            printf("\n=== Buku Ditemukan ===\n");
            tampilPerBuku(&current->data);
            found = 1;
            bukuPilihan = &current->data;
            break;
        }
        current = current->next;
    }

    if (found && bukuPilihan->status == 1)
    {
        char pilihan;
        printf("Apakah anda ingin meminjam buku ini? (y/n): ");
        scanf(" %c", &pilihan);

        if (pilihan == 'y' || pilihan == 'Y')
        {
            // cek reservasi
            FILE *reservasiFile = openFile("reservasi.txt", "r");
            int isReserved = 0;
            int userReservasi = 0;

            if (reservasiFile != NULL)
            { // baca data di reservasi.txt
                char reservedUser[20];
                int reservedIdBuku;
                char line[256];
                // baca semua di reservasi.txt
                while (fscanf(reservasiFile, "%[^#]#%d#%*[^\n]\n", reservedUser, &reservedIdBuku) == 2)
                {
                    if (reservedIdBuku == bukuPilihan->idBuku) // cek kalo idBuku yang udah direservasi sama kayak yang lagi dicari
                    {
                        isReserved = 1;
                        if (strcmp(username, reservedUser) == 0) // cek yang reservasi sama dengan pengguna sekarang atau bukan
                        {
                            userReservasi = 1;
                        }
                        break;
                    }
                }
                fclose(reservasiFile);

                if (isReserved && !userReservasi)
                {
                    printf("Maaf, buku ini sudah direservasi oleh pengguna lain.\n");
                    return;
                }
                if (userReservasi)
                {
                    FILE *tempFile = openFile("temp.txt", "w");
                    reservasiFile = openFile("reservasi.txt", "r");
                    if (tempFile != NULL && reservasiFile != NULL)
                    {
                        while (fgets(line, sizeof(line), reservasiFile))
                        {
                            if (strstr(line, username) == NULL || strstr(line, bukuPilihan->judul) == NULL)
                            {
                                fputs(line, tempFile);
                            }
                        }
                        fclose(reservasiFile);
                        fclose(tempFile);
                        remove("reservasi.txt");
                        rename("temp.txt", "reservasi.txt");
                    }
                }
            }
            ambilBuku(*bukuPilihan, idUser, username);
            bukuPilihan->status = 0; // update status habis dipinjam
        }
    }
    else if (found)
    {
        printf("Buku \"%s\" sedang dipinjam.\n", bukuPilihan->judul);
        printf("Anda dapat melakukan reservasi buku.\n");
    }
    else
    {
        printf("Buku \"%s\" tidak ditemukan.\n", target);
    }
}

void wishlist()
{
    FILE *file = openFile("wishlist.txt", "a");

    if (file == NULL)
        return;

    char judul[100];
    char pengarang[100];
    char tahun[10];
    char harga[20];

    printf("\n=== Wishlist ===\n");

    printf("Judul buku yang diinginkan     : ");
    scanf(" %99[^\n]", judul);

    printf("Pengarang buku                 : ");
    scanf(" %99[^\n]", pengarang);

    printf("Tahun terbit                   : ");
    scanf(" %9[^\n]", tahun);

    printf("Harga yang diharapkan          : ");
    scanf(" %19[^\n]", harga);

    // membuat file wishlist.txt dengan format judul#pengarang#tahun#harga
    fprintf(file, "%s#%s#%s#%s\n", judul, pengarang, tahun, harga);

    fclose(file);

    printf("\nBuku \"%s\" berhasil ditambahkan ke wishlist!\n", judul);
}

void tambahBuku(struct queue *queue)
{
    struct buku bukuBaru;

    // Generate new ID based on last element or start with 1 if empty
    if (!isEmpty(queue))
    {
        struct node *current = queue->rear;
        bukuBaru.idBuku = current->data.idBuku + 1;
    }
    else
    {
        bukuBaru.idBuku = 1;
    }

    printf("\n=== Tambahkan Buku Baru ===\n");
    printf("Masukkan judul buku: ");
    scanf(" %99[^\n]", bukuBaru.judul);

    printf("Masukkan pengarang buku: ");
    scanf(" %99[^\n]", bukuBaru.pengarang);

    printf("Masukkan tahun terbit: ");
    scanf(" %9[^\n]", bukuBaru.tahun);

    // mencegah terjadinya eror karena user salah input status selain 1 atau 0
    int validInput = 0;
    do
    {
        printf("Apakah buku sedang tersedia? (1 jika iya, 0 jika tidak): ");
        if (scanf("%d", &bukuBaru.status) != 1 || (bukuBaru.status != 0 && bukuBaru.status != 1))
        {
            printf("Input tidak valid! Masukkan 1 untuk tersedia atau 0 untuk tidak tersedia.\n");
            while (getchar() != '\n')
                ;
        }
        else
        {
            validInput = 1;
        }
    } while (!validInput);

    FILE *file = openFile("databuku.txt", "a");
    if (file == NULL)
    {
        printf("Gagal membuka file database.\n");
        return;
    }

    fprintf(file, "%d#%s#%s#%s#%d\n", bukuBaru.idBuku, bukuBaru.judul, bukuBaru.pengarang, bukuBaru.tahun, bukuBaru.status);
    fclose(file);

    enqueue(queue, bukuBaru); // tambahkan buku ke queue
    printf("Buku berhasil ditambahkan!\n");
}

void hapusByJudul(struct queue *queue)
{
    char judul[100], judulLow[100], bukuJudulLow[100];
    int found = 0;

    printf("\n=== Hapus Buku ===\n");
    printf("Masukkan judul buku: ");
    scanf(" %99[^\n]", judul);

    toLowerString(judul);
    // cek reservasi.txt, kalau ada reservasi, ga bisa dihapus bukunya
    FILE *reservasiFile = openFile("reservasi.txt", "r");
    int isReserved = 0;

    if (reservasiFile != NULL)
    {
        char reservedUser[20];
        int reservedIdBuku;
        char reservedJudul[100];

        while (fscanf(reservasiFile, "%[^#]#%d#%[^\n]\n", reservedUser, &reservedIdBuku, reservedJudul) == 3)
        {
            char reservedJudulLow[100];
            strcpy(reservedJudulLow, reservedJudul);
            toLowerString(reservedJudulLow);

            if (strcmp(judul, reservedJudulLow) == 0)
            {
                isReserved = 1;
                printf("Buku tidak bisa dihapus karena sudah direservasi.\n");
                fclose(reservasiFile);
                return;
            }
        }
        fclose(reservasiFile);
    }

    FILE *file = openFile("databuku.txt", "r");
    FILE *tempFile = openFile("temp.txt", "w");

    if (file == NULL || tempFile == NULL)
    {
        printf("Gagal membuka file.\n");
        return;
    }

    struct buku tempBuku;
    while (fscanf(file, "%d#%99[^#]#%99[^#]#%9[^#]#%d\n",
                  &tempBuku.idBuku, tempBuku.judul, tempBuku.pengarang, tempBuku.tahun, &tempBuku.status) == 5)
    {
        strcpy(bukuJudulLow, tempBuku.judul);
        toLowerString(bukuJudulLow);

        if (strcmp(judul, bukuJudulLow) == 0)
        {
            found = 1;
            if (tempBuku.status == 0)
            {
                printf("Buku \"%s\" sedang dipinjam, tidak dapat dihapus.\n", tempBuku.judul);
                fprintf(tempFile, "%d#%s#%s#%s#%d\n", tempBuku.idBuku, tempBuku.judul, tempBuku.pengarang, tempBuku.tahun, tempBuku.status);
            }
            else
            {
                printf("Buku \"%s\" berhasil dihapus.\n", tempBuku.judul);
            }
        }
        else
        {
            fprintf(tempFile, "%d#%s#%s#%s#%d\n", tempBuku.idBuku, tempBuku.judul, tempBuku.pengarang, tempBuku.tahun, tempBuku.status);
        }
    }

    fclose(file);
    fclose(tempFile);

    if (found)
    {
        remove("databuku.txt");
        rename("temp.txt", "databuku.txt");

        // Update queue setelah menghapus buku
        freeQueue(queue);
        initQueue(queue);
        bacaBuku(queue);
    }
    else
    {
        remove("temp.txt");
        printf("Buku dengan judul \"%s\" tidak ditemukan.\n", judul);
    }
}

void editBuku(struct queue *queue, int *jumlahBuku)
{
    int pilihan;
    *jumlahBuku = bacaBuku(queue);
    while (1)
    {
        printf("\n=== EDIT BUKU ===\n");
        printf("1. Tambahkan buku\n");
        printf("2. Hapus buku\n");
        printf("3. Kembali ke Menu Utama\n");
        printf("Pilihan: ");
        scanf("%d", &pilihan);

        switch (pilihan)
        {
        case 1:
            tambahBuku(queue);
            *jumlahBuku = bacaBuku(queue);
            break;
        case 2:
            hapusByJudul(queue);
            *jumlahBuku = bacaBuku(queue);
            break;
        case 3:
            return;
        default:
            printf("Pilihan tidak valid! Masukkan angka 1-3.\n");
            break;
        }
    }
}

// ======== Login Function ========
struct akun login()
{
    FILE *file = openFile("datauser.txt", "r");
    struct akun user = {0};
    
    if (file == NULL)
        return user;

    char username[20], password[20];
    int cekData = 0;

    do
    {
        printf("\n=== LOGIN ===\n");
        printf("Masukkan Username : ");
        if (scanf("%19s", username) != 1) {
            while (getchar() != '\n'); // Clear input buffer
            printf("Input tidak valid!\n");
            continue;
        }
        while (getchar() != '\n'); // Clear any remaining input

        printf("Masukkan Password : ");
        if (scanf("%19s", password) != 1) {
            while (getchar() != '\n'); // Clear input buffer
            printf("Input tidak valid!\n");
            continue;
        }
        while (getchar() != '\n'); // Clear any remaining input

        rewind(file); // baca ulang file dari awal

        while (fscanf(file, "%d#%[^#]#%[^\n]", &user.id, user.username, user.password) != EOF)
        {
            if (strcmp(username, user.username) == 0 && strcmp(password, user.password) == 0)
            {
                cekData = 1;
                break;
            }
        }

        if (!cekData)
        {
            printf("Username atau Password salah, silakan coba lagi.\n");
        }
    } while (!cekData);

    fclose(file);

    printf("\nLogin berhasil!\nSelamat datang, %s\n", user.username);
    return user;
}

//=========== BST ==============================

// Fungsi untuk mendapatkan tinggi node
int height(struct bstNode *N) {
    if (N == NULL)
        return 0;
    return N->height;
}

// Fungsi untuk mendapatkan nilai maksimum dari dua integer
int max(int a, int b) {
    return (a > b) ? a : b;
}

// Fungsi untuk membuat node baru
struct bstNode* createNode(struct buku data) {
    struct bstNode* node = (struct bstNode*)malloc(sizeof(struct bstNode));
    node->data = data;
    node->left = NULL;
    node->right = NULL;
    node->height = 1;  // Node baru memiliki tinggi 1
    return node;
}

// Fungsi untuk memutar ke kanan
struct bstNode *rightRotate(struct bstNode *y) {
    struct bstNode *x = y->left;
    struct bstNode *T2 = x->right;

    x->right = y;
    y->left = T2;

    // Update tinggi
    y->height = max(height(y->left), height(y->right)) + 1;
    x->height = max(height(x->left), height(x->right)) + 1;

    return x;
}

// Fungsi untuk memutar ke kiri
struct bstNode *leftRotate(struct bstNode *x) {
    struct bstNode *y = x->right;
    struct bstNode *T2 = y->left;

    y->left = x;
    x->right = T2;

    // Update tinggi
    x->height = max(height(x->left), height(x->right)) + 1;
    y->height = max(height(y->left), height(y->right)) + 1;

    return y;
}

// Mendapatkan balance factor dari node N
int getBalance(struct bstNode *N) {
    if (N == NULL)
        return 0;
    return height(N->left) - height(N->right);
}

// Fungsi untuk mencari node dalam BST
struct bstNode* searchBST(struct bstNode* root, int idBuku) {
    if (root == NULL || root->data.idBuku == idBuku)
        return root;
    
    if (idBuku < root->data.idBuku)
        return searchBST(root->left, idBuku);
    
    return searchBST(root->right, idBuku);
}

// Fungsi untuk memasukkan node baru ke BST
struct bstNode* insertBST(struct bstNode* root, struct buku data) {
    // 1. BST insert normal
    if (root == NULL)
        return createNode(data);

    if (data.idBuku < root->data.idBuku)
        root->left = insertBST(root->left, data);
    else if (data.idBuku > root->data.idBuku)
        root->right = insertBST(root->right, data);
    else // Tidak boleh node duplikat
        return root;

    // 2. Update tinggi node
    root->height = 1 + max(height(root->left), height(root->right));

    // 3. Cek seimbang dengan balance factor
    int balance = getBalance(root);

    // Kasus Left Left
    if (balance > 1 && data.idBuku < root->left->data.idBuku)
        return rightRotate(root);

    // Kasus Right Right
    if (balance < -1 && data.idBuku > root->right->data.idBuku)
        return leftRotate(root);

    // Kasus Left Right
    if (balance > 1 && data.idBuku > root->left->data.idBuku) {
        root->left = leftRotate(root->left);
        return rightRotate(root);
    }

    // Kasus Right Left
    if (balance < -1 && data.idBuku < root->right->data.idBuku) {
        root->right = rightRotate(root->right);
        return leftRotate(root);
    }

    return root;
}

// Fungsi untuk menemukan node dengan nilai minimum
struct bstNode* minValueNode(struct bstNode* node) {
    struct bstNode* current = node;
    while (current && current->left != NULL)
        current = current->left;
    return current;
}

// Fungsi untuk menghapus node dari BST
struct bstNode* deleteBST(struct bstNode* root, int idBuku) {
    if (root == NULL)
        return root;

    if (idBuku < root->data.idBuku)
        root->left = deleteBST(root->left, idBuku);
    else if (idBuku > root->data.idBuku)
        root->right = deleteBST(root->right, idBuku);
    else {
        if (root->left == NULL) {
            struct bstNode* temp = root->right;
            free(root);
            return temp;
        }
        else if (root->right == NULL) {
            struct bstNode* temp = root->left;
            free(root);
            return temp;
        }

        struct bstNode* temp = minValueNode(root->right);
        root->data = temp->data;
        root->right = deleteBST(root->right, temp->data.idBuku);
    }
    return root;
}

// Fungsi untuk menghitung jumlah node dalam BST
int countNodes(struct bstNode* root) {
    if (root == NULL) return 0;
    return 1 + countNodes(root->left) + countNodes(root->right);
}

// Fungsi untuk menyimpan node BST ke array secara inorder
void storeInorder(struct bstNode* root, struct bstNode** arr, int* index) {
    if (root == NULL) return;
    
    storeInorder(root->left, arr, index);
    arr[*index] = root;
    (*index)++;
    storeInorder(root->right, arr, index);
}

// Fungsi untuk membandingkan judul buku (A-Z)
int compareJudulAZ(const void* a, const void* b) {
    struct bstNode* nodeA = *(struct bstNode**)a;
    struct bstNode* nodeB = *(struct bstNode**)b;
    return strcmp(nodeA->data.judul, nodeB->data.judul);
}

// Fungsi untuk membandingkan judul buku (Z-A)
int compareJudulZA(const void* a, const void* b) {
    struct bstNode* nodeA = *(struct bstNode**)a;
    struct bstNode* nodeB = *(struct bstNode**)b;
    return strcmp(nodeB->data.judul, nodeA->data.judul);
}

void lihatBukuUrutan(struct bstNode** nodes, int count, int sortType) {
    if (sortType == 1) { // untuk A-Z
        qsort(nodes, count, sizeof(struct bstNode*), compareJudulAZ);
    } else if (sortType == 2) { // untuk Z-A
        qsort(nodes, count, sizeof(struct bstNode*), compareJudulZA);
    }
    
    int currentIndex = 0;
    char pilihan;

    while (1) {
        tampilPerBuku(&(nodes[currentIndex]->data));

        printf("Pilih aksi: (n) Buku Selanjutnya | (p) Buku Sebelumnya | (q) Kembali\n");
        printf("Pilihan: ");
        scanf(" %c", &pilihan);

        switch (pilihan) {
            case 'n':
                if (currentIndex < count - 1) {
                    currentIndex++;
                } else {
                    currentIndex = 0;
                }
                break;
            case 'p':
                if (currentIndex > 0) {
                    currentIndex--;
                } else {
                    currentIndex = count - 1;
                }
                break;
            case 'q':
                return;
            default:
                printf("Pilihan tidak valid!\n");
        }
    }
}

void lihatSemuaBukuBST(struct bstNode* root) {
    if (root == NULL) {
        printf("Tidak ada data buku.\n");
        return;
    }

    int count = countNodes(root);
    struct bstNode** nodes = (struct bstNode**)malloc(count * sizeof(struct bstNode*));
    int index = 0;
    storeInorder(root, nodes, &index);

    int pilihan;
    while (1) {
        printf("\n=== PILIHAN URUTAN TAMPILAN ===\n");
        printf("1. Berurutan berdasarkan ID\n");
        printf("2. Berurutan berdasarkan Alfabet\n");
        printf("0. Kembali\n");
        printf("Pilihan: ");
        scanf("%d", &pilihan);

        switch (pilihan) {
            case 1:
                lihatBukuUrutan(nodes, count, 0);
                break;
            case 2: {
                int subPilihan;
                printf("\n=== PILIHAN URUTAN ALFABET ===\n");
                printf("1. A-Z\n");
                printf("2. Z-A\n");
                printf("0. Kembali\n");
                printf("Pilihan: ");
                scanf("%d", &subPilihan);

                switch (subPilihan) {
                    case 1:
                        lihatBukuUrutan(nodes, count, 1);
                        break;
                    case 2:
                        lihatBukuUrutan(nodes, count, 2);
                        break;
                    case 0:
                        break;
                    default:
                        printf("Pilihan tidak valid!\n");
                }
                break;
            }
            case 0:
                free(nodes);
                return;
            default:
                printf("Pilihan tidak valid!\n");
        }
    }
}

void cariBukuBST(struct bstNode* root) {
    int idCari;
    printf("Masukkan ID Buku yang dicari: ");
    scanf("%d", &idCari);
    while (getchar() != '\n');
    
    struct bstNode* hasil = searchBST(root, idCari);
    if (hasil != NULL) {
        printf("\n=== BUKU DITEMUKAN ===\n");
        tampilPerBuku(&hasil->data);
    } else {
        printf("Buku dengan ID %d tidak ditemukan.\n", idCari);
    }
}

struct bstNode* tambahBukuBST(struct bstNode* root) {
    struct buku bukuBaru;
    printf("\n=== TAMBAH BUKU ===\n");
    printf("Masukkan ID Buku: ");
    scanf("%d", &bukuBaru.idBuku);
    while (getchar() != '\n');
    
    printf("Masukkan Judul: ");
    fgets(bukuBaru.judul, sizeof(bukuBaru.judul), stdin);
    bukuBaru.judul[strcspn(bukuBaru.judul, "\n")] = 0;
    
    printf("Masukkan Pengarang: ");
    fgets(bukuBaru.pengarang, sizeof(bukuBaru.pengarang), stdin);
    bukuBaru.pengarang[strcspn(bukuBaru.pengarang, "\n")] = 0;
    
    printf("Masukkan Tahun: ");
    fgets(bukuBaru.tahun, sizeof(bukuBaru.tahun), stdin);
    bukuBaru.tahun[strcspn(bukuBaru.tahun, "\n")] = 0;
    
    bukuBaru.status = 1; // Buku baru selalu tersedia
    
    root = insertBST(root, bukuBaru);
    printf("Buku berhasil ditambahkan!\n");
    return root;
}

struct bstNode* hapusBukuBST(struct bstNode* root) {
    int idHapus;
    printf("Masukkan ID Buku yang akan dihapus: ");
    scanf("%d", &idHapus);
    while (getchar() != '\n');
    
    struct bstNode* cek = searchBST(root, idHapus);
    if (cek == NULL) {
        printf("Buku dengan ID %d tidak ditemukan.\n", idHapus);
    } else if (cek->data.status == 0) {
        printf("Buku sedang dipinjam, tidak dapat dihapus.\n");
    } else {
        root = deleteBST(root, idHapus);
        printf("Buku berhasil dihapus!\n");
    }
    return root;
}

void tampilkanMenuBST() {
    printf("\n=== MENU BST ===\n");
    printf("1. Lihat Semua Buku (Inorder)\n");
    printf("2. Cari Buku\n");
    printf("3. Tambah Buku\n");
    printf("4. Hapus Buku\n");
    printf("0. Home\n");
    printf("Pilihan: ");
}

void bstHome(int idUser, char username[]) {
    struct bstNode* root = NULL;
    int pilihan;
    
    // Baca data buku ke BST
    root = bacaBukuBST();

    while (1) {
        tampilkanMenuBST();
        
        if (scanf("%d", &pilihan) != 1) {
            while (getchar() != '\n');
            printf("Input tidak valid!\n");
            continue;
        }
        while (getchar() != '\n');

        switch (pilihan) {
            case 1:
                lihatSemuaBukuBST(root);
                break;
            case 2:
                cariBukuBST(root);
                break;
            case 3:
                root = tambahBukuBST(root);
                break;
            case 4:
                root = hapusBukuBST(root);
                break;
            case 0:
                freeBST(root);
                return;
            default:
                printf("Pilihan tidak valid!\n");
        }
    }
}

// Fungsi untuk membebaskan memori BST
void freeBST(struct bstNode* root) {
    if (root != NULL) {
        freeBST(root->left);
        freeBST(root->right);
        free(root);
    }
}

// Fungsi untuk membaca data buku ke BST
struct bstNode* bacaBukuBST() {
    FILE *file = openFile("databuku.txt", "r");
    if (file == NULL)
        return NULL;

    struct bstNode* root = NULL;
    struct buku tempBuku;

    while (fscanf(file, "%d#%99[^#]#%99[^#]#%9[^#]#%d\n",
                &tempBuku.idBuku, tempBuku.judul, tempBuku.pengarang,
                tempBuku.tahun, &tempBuku.status) == 5) {
        root = insertBST(root, tempBuku);
    }

    fclose(file);
    return root;
}

// ======== Heap Function ========

// Fungsi untuk menampilkan heap dengan konfigurasi
void tampilkanHeap(int idUser, char username[], struct heapBuku* heap, int heapType, int sortBy) {
    if (heap == NULL) {
        printf("Heap belum dibuat. Silakan buat heap terlebih dahulu.\n");
        printf("\nTekan Enter untuk melanjutkan...");
        getchar();
        return;
    }
    
    printf("\n=== TAMPILAN HEAP ===\n");
    printf("Jenis: %s\n", heapType == 1 ? "MAX HEAP" : "MIN HEAP");
    printf("Kriteria: %s\n", sortBy == 1 ? "ID Buku" : "Judul Buku");
    printf("===================\n");
    displayHeap(heap);
    printf("\nTekan Enter untuk melanjutkan...");
    getchar();
}

// Fungsi untuk mencari buku dalam heap
void cariBukuHeap(int idUser, char username[], struct heapBuku* heap) {
    if (heap == NULL) {
        printf("Heap belum dibuat. Silakan buat heap terlebih dahulu.\n");
        printf("\nTekan Enter untuk melanjutkan...");
        getchar();
        return;
    }
    
    int idBuku;
    printf("Masukkan ID buku yang dicari: ");
    if (scanf("%d", &idBuku) == 1) {
        while (getchar() != '\n');
        searchInHeap(heap, idBuku);
    } else {
        while (getchar() != '\n');
        printf("ID tidak valid!\n");
    }
}

// Fungsi untuk menambah buku ke heap
void tambahBukuHeap(int idUser, char username[], struct heapBuku* heap, int heapType, int sortBy) {
    if (heap == NULL) {
        printf("Heap belum dibuat. Silakan buat heap terlebih dahulu.\n");
        printf("\nTekan Enter untuk melanjutkan...");
        getchar();
        return;
    }
    
    struct buku bukuBaru;
    printf("\n=== TAMBAH BUKU KE HEAP ===\n");
    printf("ID Buku: ");
    if (scanf("%d", &bukuBaru.idBuku) != 1) {
        while (getchar() != '\n');
        printf("ID tidak valid!\n");
        return;
    }
    while (getchar() != '\n');
    
    printf("Judul: ");
    scanf(" %99[^\n]", bukuBaru.judul);
    
    printf("Pengarang: ");
    scanf(" %99[^\n]", bukuBaru.pengarang);
    
    printf("Tahun: ");
    scanf(" %9[^\n]", bukuBaru.tahun);
    
    bukuBaru.status = 1; // Default tersedia
    
    insertHeap(heap, bukuBaru);
    // Rebuild heap sesuai konfigurasi yang dipilih user
    buildHeap(heap, heapType, sortBy);
    printf("Buku berhasil ditambahkan ke heap!\n");
    
    printf("\nTekan Enter untuk melanjutkan...");
    getchar();
}

// Fungsi untuk menghapus buku dengan prioritas tertinggi
void hapusBukuPrioritas(int idUser, char username[], struct heapBuku* heap, int heapType, int sortBy) {
    if (heap == NULL) {
        printf("Heap belum dibuat. Silakan buat heap terlebih dahulu.\n");
        printf("\nTekan Enter untuk melanjutkan...");
        getchar();
        return;
    }
    
    if (heap->size <= 0) {
        printf("Heap kosong, tidak ada buku yang dapat dihapus.\n");
        printf("\nTekan Enter untuk melanjutkan...");
        getchar();
        return;
    }
    
    struct buku bukuDihapus = extractMax(heap);
    
    printf("\n=== BUKU DENGAN PRIORITAS TERTINGGI DIHAPUS ===\n");
    printf("Heap Type: %s\n", heapType == 1 ? "MAX HEAP" : "MIN HEAP");
    printf("Kriteria: %s\n", sortBy == 1 ? "ID Buku" : "Judul Buku");
    printf("============================================\n");
    tampilPerBuku(&bukuDihapus);
    printf("Buku berhasil dihapus dari heap!\n");
    
    printf("\nTekan Enter untuk melanjutkan...");
    getchar();
}

void menuHeap(int idUser, char username[]) {
    struct heapBuku* heap = NULL;
    int pilihan;
    int heapType, sortBy;
    
    // Ask user to choose heap type first
    printf("\n=== PILIH JENIS HEAP ===\n");
    printf("1. MAX HEAP\n");
    printf("2. MIN HEAP\n");
    printf("Pilihan: ");
    
    if (scanf("%d", &heapType) != 1 || (heapType != 1 && heapType != 2)) {
        while (getchar() != '\n');
        printf("Pilihan tidak valid! Kembali ke menu utama.\n");
        return;
    }
    while (getchar() != '\n');
    
    // Ask user to choose sorting criteria
    printf("\n=== PILIH KRITERIA PENGURUTAN ===\n");
    printf("1. Berdasarkan ID Buku\n");
    printf("2. Berdasarkan Judul Buku\n");
    printf("Pilihan: ");
    
    if (scanf("%d", &sortBy) != 1 || (sortBy != 1 && sortBy != 2)) {
        while (getchar() != '\n');
        printf("Pilihan tidak valid! Kembali ke menu utama.\n");
        return;
    }
    while (getchar() != '\n');
    
    // Load books into heap
    heap = bacaBukuHeap();
    if (heap == NULL) {
        printf("Gagal memuat data buku ke heap.\n");
        return;
    }
      // Build heap according to user's choice
    buildHeap(heap, heapType, sortBy);
    
    // Display heap type information
    printf("\n=== HEAP CONFIGURATION ===\n");
    printf("Jenis Heap: %s\n", heapType == 1 ? "MAX HEAP" : "MIN HEAP");
    printf("Kriteria: %s\n", sortBy == 1 ? "ID Buku" : "Judul Buku (Alfabetis)");
    printf("==========================\n");

    while (1) {
        tampilanMenuHeap();
        
        if (scanf("%d", &pilihan) != 1) {
            while (getchar() != '\n');
            printf("Input tidak valid!\n");
            continue;
        }
        while (getchar() != '\n');          switch (pilihan) {
            case 1:
                tampilkanHeap(idUser, username, heap, heapType, sortBy);
                break;
            case 2:
                cariBukuHeap(idUser, username, heap);
                break;
            case 3:
                tambahBukuHeap(idUser, username, heap, heapType, sortBy);
                break;
            case 4:
                hapusBukuPrioritas(idUser, username, heap, heapType, sortBy);
                break;
            case 0:
                freeHeap(heap);
                return;
            default:
                printf("Pilihan tidak valid!\n");}
    }
}

// Buat heap dengan kapasitas tertentu
struct heapBuku* createHeap(int capacity) {
    struct heapBuku* heap = (struct heapBuku*)malloc(sizeof(struct heapBuku));
    if (heap == NULL) return NULL;
    
    heap->data = (struct buku*)malloc(capacity * sizeof(struct buku));
    if (heap->data == NULL) {
        free(heap);
        return NULL;
    }
    heap->size = 0;
    heap->capacity = capacity;
    return heap;
}

// Free heap memory
void freeHeap(struct heapBuku* heap) {
    if (heap) {
        if (heap->data) free(heap->data);
        free(heap);
    }
}

// Max heap dari file databuku.txt
void heapify(struct heapBuku* heap, int i) {
    int largest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;
    
    if (left < heap->size && heap->data[left].idBuku > heap->data[largest].idBuku)
        largest = left;
    
    if (right < heap->size && heap->data[right].idBuku > heap->data[largest].idBuku)
        largest = right;
    
    if (largest != i) {
        struct buku temp = heap->data[i];
        heap->data[i] = heap->data[largest];
        heap->data[largest] = temp;
        heapify(heap, largest);
    }
}

// Min heap dari file databuku.txt
void heapifyMinID(struct heapBuku* heap, int i) {
    int smallest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;
    
    if (left < heap->size && heap->data[left].idBuku < heap->data[smallest].idBuku)
        smallest = left;
    
    if (right < heap->size && heap->data[right].idBuku < heap->data[smallest].idBuku)
        smallest = right;
    
    if (smallest != i) {
        struct buku temp = heap->data[i];
        heap->data[i] = heap->data[smallest];
        heap->data[smallest] = temp;
        heapifyMinID(heap, smallest);
    }
}

// Max heapif dari judul buku (alphabetical)
void heapifyMaxJudul(struct heapBuku* heap, int i) {
    int largest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;
    
    if (left < heap->size && strcmp(heap->data[left].judul, heap->data[largest].judul) > 0)
        largest = left;
    
    if (right < heap->size && strcmp(heap->data[right].judul, heap->data[largest].judul) > 0)
        largest = right;
    
    if (largest != i) {
        struct buku temp = heap->data[i];
        heap->data[i] = heap->data[largest];
        heap->data[largest] = temp;
        heapifyMaxJudul(heap, largest);
    }
}

// Min heapif dari judul buku (alphabetical)
void heapifyMinJudul(struct heapBuku* heap, int i) {
    int smallest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;
    
    if (left < heap->size && strcmp(heap->data[left].judul, heap->data[smallest].judul) < 0)
        smallest = left;
    
    if (right < heap->size && strcmp(heap->data[right].judul, heap->data[smallest].judul) < 0)
        smallest = right;
    
    if (smallest != i) {
        struct buku temp = heap->data[i];
        heap->data[i] = heap->data[smallest];
        heap->data[smallest] = temp;
        heapifyMinJudul(heap, smallest);
    }
}

// masukkan buku ke dalam heap
void insertHeap(struct heapBuku* heap, struct buku book) {
    if (heap->size >= heap->capacity) {
        printf("Heap penuh! Tidak dapat menambah buku.\n");
        return;
    }
    
    heap->data[heap->size] = book;
    heap->size++;
}

// ekstrak buku dengan prioritas tertinggi dari heap
struct buku extractMax(struct heapBuku* heap) {
    struct buku emptyBook = {0};
    if (heap->size <= 0) {
        return emptyBook;
    }
    
    struct buku max = heap->data[0];
    heap->data[0] = heap->data[heap->size - 1];
    heap->size--;
    
    if (heap->size > 0) {
        heapify(heap, 0); // Default to max heap by ID
    }
    
    return max;
}

// display buku dalam heap
void displayHeap(struct heapBuku* heap) {
    if (heap->size == 0) {
        printf("Heap kosong.\n");
        return;
    }
    
    printf("Heap memiliki %d buku.\n", heap->size);
    printf("Navigasi: (n) Buku Selanjutnya | (p) Buku Sebelumnya | (q) Kembali\n\n");
    
    int currentIndex = 0;
    char pilihan;

    while (1) {
        printf("=== Buku %d dari %d ===\n", currentIndex + 1, heap->size);
        tampilPerBuku(&heap->data[currentIndex]);

        printf("Pilih aksi: (n) Buku Selanjutnya | (p) Buku Sebelumnya | (q) Kembali\n");
        printf("Pilihan: ");
        scanf(" %c", &pilihan);

        switch (pilihan) {
            case 'n':
                if (currentIndex < heap->size - 1) {
                    currentIndex++;
                } else {
                    currentIndex = 0; // Kembali ke awal
                }
                break;
            case 'p':
                if (currentIndex > 0) {
                    currentIndex--;
                } else {
                    currentIndex = heap->size - 1; // Ke akhir
                }
                break;
            case 'q':
                return;
            default:
                printf("Pilihan tidak valid!\n");
        }
    }
}

// cari buku dalam heap berdasarkan ID
void searchInHeap(struct heapBuku* heap, int idBuku) {
    int found = 0;
    for (int i = 0; i < heap->size; i++) {
        if (heap->data[i].idBuku == idBuku) {
            printf("\n=== BUKU DITEMUKAN ===\n");
            tampilPerBuku(&heap->data[i]);
            found = 1;
            break;
        }
    }
    
    if (!found) {
        printf("Buku dengan ID %d tidak ditemukan dalam heap.\n", idBuku);
    }
    
    printf("\nTekan Enter untuk melanjutkan...");
    getchar();
}

// baca buku dari file databuku.txt ke dalam heap
struct heapBuku* bacaBukuHeap() {
    FILE *file = openFile("databuku.txt", "r");
    if (file == NULL) {
        printf("Gagal membuka file databuku.txt\n");
        return NULL;
    }
    
    // hitung jumlah buku dalam file
    int count = 0;
    struct buku tempBuku;
    while (fscanf(file, "%d#%99[^#]#%99[^#]#%9[^#]#%d\n",
                  &tempBuku.idBuku, tempBuku.judul, tempBuku.pengarang,
                  tempBuku.tahun, &tempBuku.status) == 5) {
        count++;
    }
    
    rewind(file);
    
    struct heapBuku* heap = createHeap(count + 10); // Extra space for new books
    if (heap == NULL) {
        fclose(file);
        return NULL;
    }
    
    // baca buku dari file dan masukkan ke dalam heap
    while (fscanf(file, "%d#%99[^#]#%99[^#]#%9[^#]#%d\n",
                  &tempBuku.idBuku, tempBuku.judul, tempBuku.pengarang,
                  tempBuku.tahun, &tempBuku.status) == 5) {
        insertHeap(heap, tempBuku);
    }
    
    fclose(file);
    return heap;
}

// buat heap berdasarkan pilihan user
void buildHeap(struct heapBuku* heap, int heapType, int sortBy) {
    if (heap->size <= 1) return;
    
    // buat heap dari bawah ke atas
    for (int i = (heap->size / 2) - 1; i >= 0; i--) {
        if (heapType == 1) {                    // MAX HEAP
            if (sortBy == 1) {                  // ID
                heapify(heap, i);
            } else {                            // Judul
                heapifyMaxJudul(heap, i);
            }
        } else {                                // MIN HEAP
            if (sortBy == 1) {                  // ID
                heapifyMinID(heap, i);
            } else {                            // Title
                heapifyMinJudul(heap, i);
            }
        }
    }
}

// Tampilkan menu heap
void tampilanMenuHeap() {
    printf("\n=== MENU HEAP ===\n");
    printf("1. Tampilkan Heap\n");
    printf("2. Cari Buku\n");
    printf("3. Tambah Buku\n");
    printf("4. Extract Buku\n");
    printf("0. Kembali ke Home\n");
    printf("Pilihan: ");
}

// ======== Home Function  =============

void linearHome(int idUser, char username[]);

void firstHome(int idUser, char username[])
{
    int pilihan;
    do {
        printf("\n=== SELAMAT DATANG DI PERPUSTAKAAN ===\n");
        printf("Pilih Algoritma yang di inginkan:\n");
        printf("1. Linear\n");
        printf("2. BST\n");
        printf("3. Heap\n");
        printf("4. Log out\n");
        printf("======================================\n");
        printf("Pilihan: ");
        
        if (scanf("%d", &pilihan) != 1) {
            while (getchar() != '\n'); // Clear input buffer
            printf("Input tidak valid! Masukkan angka 1-4.\n");
            continue;
        }
        while (getchar() != '\n'); // Clear any remaining input
        
        switch (pilihan)
        {
        case 1:
            linearHome(idUser, username);
            break;
        case 2:
            bstHome(idUser, username);
            break;
        case 3:
            menuHeap(idUser, username);
            break;
        case 4:
            printf("Logging out...\n");
            return;
        default:
            printf("Pilihan tidak valid! Masukkan angka 1-4.\n");
        }
    } while (pilihan != 4);
}

void linearHome(int idUser, char username[])
{
    int pilihan;
    struct queue queue;
    int jumlahBuku;

    initQueue(&queue);
    jumlahBuku = bacaBuku(&queue);

    while (1)
    {
        printf("\n=== MENU UTAMA ===\n");
        printf("1. Lihat Semua Buku\n");
        printf("2. Lihat Buku yang Dipinjam\n");
        printf("3. Cari Buku\n");
        printf("4. Pinjam Buku\n");
        printf("5. Kembalikan Buku\n");
        printf("6. Lihat Riwayat Peminjaman\n");
        printf("7. Reservasi Buku\n");
        printf("8. Wishlist\n");
        printf("9. Edit List Buku\n");
        printf("0. Home\n");
        printf("Pilihan: ");
        scanf("%d", &pilihan);

        switch (pilihan)
        {
        case 1:
            lihatBuku(idUser, &queue, username);
            break;
        case 2:
            lihatBukuDiambil(idUser, &queue, username);
            break;
        case 3:
            cariBuku(idUser, &queue, username);
            break;
        case 4:
            pinjamBuku(idUser, &queue, username);
            break;
        case 5:
            kembalikanBuku(idUser, &queue, username);
            // Reload books after return
            freeQueue(&queue);
            initQueue(&queue);
            jumlahBuku = bacaBuku(&queue);
            break;
        case 6:
            lihatHistory(idUser, username);
            break;
        case 7:
            reservasiBuku(idUser, &queue, username);
            break;
        case 8:
            wishlist();
            break;
        case 9:
            editBuku(&queue, &jumlahBuku);
            break;
        case 0:
            freeQueue(&queue);
            return;
        default:
            printf("Pilihan tidak valid!\n");
        }
    }
}

//======== Main Function ========

int main()
{
    struct akun user = login();
    if(user.id == 0) { 
        printf("Gagal membuka file data user.\n");
    }
    firstHome(user.id, user.username);
    
    return 0;
}