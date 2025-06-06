#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

struct buku
{
    int idBuku;
    char judul[100];
    char pengarang[100];
    char tahun[10];
    int status; // 0 = dipinjam, 1 = tersedia
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

// Struktur node BST
struct bstNode {
    int data;
    struct bstNode *left, *right;
};

struct bstNode* newBSTNode(int data) {
    struct bstNode* node = (struct bstNode*)malloc(sizeof(struct bstNode));
    node->data = data;
    node->left = node->right = NULL;
    return node;
}

// a. Pencarian node pada BST
struct bstNode* searchBST(struct bstNode* root, int key) {
    if (root == NULL || root->data == key)
        return root;
    if (key < root->data)
        return searchBST(root->left, key);
    else
        return searchBST(root->right, key);
}

// b. Memasukkan node baru ke dalam BST
struct bstNode* insertBST(struct bstNode* root, int data) {
    if (root == NULL)
        return newBSTNode(data);
    if (data < root->data)
        root->left = insertBST(root->left, data);
    else if (data > root->data)
        root->right = insertBST(root->right, data);
    return root;
}

// Fungsi bantu: mencari node terkecil (untuk penghapusan node dengan 2 anak)
struct bstNode* minValueNode(struct bstNode* node) {
    struct bstNode* current = node;
    while (current && current->left != NULL)
        current = current->left;
    return current;
}

// c, d, e. Menghapus node dari BST
struct bstNode* deleteBST(struct bstNode* root, int key) {
    if (root == NULL) return root;

    if (key < root->data)
        root->left = deleteBST(root->left, key);
    else if (key > root->data)
        root->right = deleteBST(root->right, key);
    else {
        // Node ditemukan
        // c. Leaf node (tidak punya anak)
        if (root->left == NULL && root->right == NULL) {
            free(root);
            return NULL;
        }
        // d. Node dengan 1 child
        else if (root->left == NULL) {
            struct bstNode* temp = root->right;
            free(root);
            return temp;
        }
        else if (root->right == NULL) {
            struct bstNode* temp = root->left;
            free(root);
            return temp;
        }
        // e. Node dengan 2 children
        else {
            struct bstNode* temp = minValueNode(root->right);
            root->data = temp->data;
            root->right = deleteBST(root->right, temp->data);
        }
    }
    return root;
}

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
                // Find last node
                struct node *temp = queue->front;
                while (temp->next != NULL)
                {
                    temp = temp->next;
                }
                current = temp;
            }
            else
            {
                // Find previous node
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
    // Current node is managed by queue, no need to free here
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

                // Update queue with new data to ensure consistency
                freeQueue(queue);
                initQueue(queue);
                bacaBuku(queue);

                // Find the returned book in the updated queue
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

// Fungsi untuk membandingkan string secara case-insensitive
int cmpIgnoreCase(const char *a, const char *b) {
    while (*a && *b) {
        char ca = tolower(*a);
        char cb = tolower(*b);
        if (ca != cb) return ca - cb;
        a++; b++;
    }
    return *a - *b;
}

// BST untuk judul buku
struct bstJudul {
    char judul[100];
    struct node *bukuNode; // pointer ke node buku di queue
    struct bstJudul *left, *right;
};

struct bstJudul* newBSTJudul(const char *judul, struct node *bukuNode) {
    struct bstJudul* node = (struct bstJudul*)malloc(sizeof(struct bstJudul));
    strcpy(node->judul, judul);
    node->bukuNode = bukuNode;
    node->left = node->right = NULL;
    return node;
}

struct bstJudul* insertBSTJudul(struct bstJudul* root, const char *judul, struct node *bukuNode) {
    if (root == NULL)
        return newBSTJudul(judul, bukuNode);
    int cmp = cmpIgnoreCase(judul, root->judul);
    if (cmp < 0)
        root->left = insertBSTJudul(root->left, judul, bukuNode);
    else if (cmp > 0)
        root->right = insertBSTJudul(root->right, judul, bukuNode);
    return root;
}

struct bstJudul* searchBSTJudul(struct bstJudul* root, const char *judul) {
    if (root == NULL)
        return NULL;
    int cmp = cmpIgnoreCase(judul, root->judul);
    if (cmp == 0)
        return root;
    else if (cmp < 0)
        return searchBSTJudul(root->left, judul);
    else
        return searchBSTJudul(root->right, judul);
}

void freeBSTJudul(struct bstJudul* root) {
    if (root == NULL) return;
    freeBSTJudul(root->left);
    freeBSTJudul(root->right);
    free(root);
}

// Fungsi cari buku pakai BST
void cariBukuBST(int idUser, struct queue *queue, char username[]) {
    if (isEmpty(queue)) {
        printf("Tidak ada data buku.\n");
        return;
    }
    // Bangun BST dari queue
    struct bstJudul *root = NULL;
    struct node *current = queue->front;
    while (current != NULL) {
        root = insertBSTJudul(root, current->data.judul, current);
        current = current->next;
    }

    char target[100];
    printf("Masukkan judul buku yang dicari: ");
    scanf(" %[^\n]", target);

    struct bstJudul *result = searchBSTJudul(root, target);
    if (result != NULL) {
        printf("\n=== Buku Ditemukan ===\n");
        tampilPerBuku(&result->bukuNode->data);
        char pilihan;
        printf("Pilih aksi: (o) Ambil Buku | (q) Kembali Ke Home\n");
        printf("Pilihan Anda: ");
        scanf(" %c", &pilihan);
        if (pilihan == 'o') {
            ambilBuku(result->bukuNode->data, idUser, username);
            result->bukuNode->data.status = 0;
        }
    } else {
        printf("Buku dengan judul \"%s\" tidak ditemukan.\n", target);
    }
    freeBSTJudul(root);
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

        // Update queue by re-reading from file (to ensure consistency)
        // First, free all nodes in the current queue
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

void menuLinear(int idUser, char username[])
{
    int pilihan;
    struct queue queue;
    int jumlahBuku;

    initQueue(&queue);

    while (1)
    {
        freeQueue(&queue);
        initQueue(&queue);
        jumlahBuku = bacaBuku(&queue);

        printf("\n=== MENU UTAMA ===\n");
        printf("1. Lihat Semua Buku\n");
        printf("2. Lihat Buku yang Dipinjam\n");
        printf("3. Cari Buku ( Linear Search )\n");
        printf("4. Cari Buku ( pakai BST )\n");
        printf("5. Pinjam Buku\n");
        printf("6. Kembalikan Buku\n");
        printf("7. Lihat Riwayat Peminjaman\n");
        printf("8. Reservasi Buku\n");
        printf("9. Wishlist\n");
        printf("10. Edit List Buku\n");
        printf("00. Logout\n");
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
            cariBukuBST(idUser, &queue, username); // Panggil fungsi BST
            break;
        case 5:
            pinjamBuku(idUser, &queue, username);
            break;
        case 6:
            kembalikanBuku(idUser, &queue, username);
            break;
        case 7:
            lihatHistory(idUser, username);
            break;
        case 8:
            reservasiBuku(idUser, &queue, username);
            break;
        case 9:
            wishlist();
            break;
        case 10:
            editBuku(&queue, &jumlahBuku);
            break;
        case 0:
            printf("Logout berhasil!\n");
            freeQueue(&queue);
            return;
        default:
            printf("Pilihan tidak valid!\n");
        }
    }
}

void menuBST(int idUser, char username[])
{
    int pilihan;
    struct queue queue;
    int jumlahBuku;
    struct bstJudul *root = NULL; // Simpan root BST selama menu BST aktif

    initQueue(&queue);

    while (1)
    {
        freeQueue(&queue);
        initQueue(&queue);
        jumlahBuku = bacaBuku(&queue);

        printf("\n=== MENU BST ===\n");
        printf("1. Cari Buku (BST)\n");
        printf("2. Tambah Node Baru ke BST\n");
        printf("3. Hapus Leaf Node dari BST\n");
        printf("4. Kembali ke Menu Utama\n");
        printf("Pilihan: ");
        scanf("%d", &pilihan);

        switch (pilihan)
        {
        case 1:
            // Bangun BST dari queue setiap kali cari (atau bisa gunakan root yang sudah ada)
            freeBSTJudul(root);
            root = NULL;
            {
                struct node *current = queue.front;
                while (current != NULL) {
                    root = insertBSTJudul(root, current->data.judul, current);
                    current = current->next;
                }
            }
            cariBukuBST(idUser, &queue, username);
            break;
        case 2:
            tambahNodeBST(&root, &queue);
            break;
        case 3: {
            char judul[100];
            printf("Masukkan judul leaf node yang ingin dihapus dari BST: ");
            scanf(" %[^\n]", judul);
            int deleted = 0;
            root = hapusLeafBSTJudul(root, judul, &deleted);
            if (!deleted) {
                printf("Node tidak ditemukan sebagai leaf atau tidak ada di BST.\n");
            }
            break;
        }
        case 4:
            freeBSTJudul(root);
            freeQueue(&queue);
            return;
        default:
            printf("Pilihan tidak valid!\n");
        }
    }
}

void home(int idUser, char username[])
{
    int pilihan;
    while (1)
    {
        printf("\n=== MENU UTAMA ===\n");
        printf("1. Menu Linear\n");
        printf("2. Menu BST\n");
        printf("3. Logout\n");
        printf("Pilihan: ");
        scanf("%d", &pilihan);

        switch (pilihan)
        {
        case 1:
            menuLinear(idUser, username);
            break;
        case 2:
            menuBST(idUser, username);
            break;
        case 3:
            printf("Logout berhasil!\n");
            return;
        default:
            printf("Pilihan tidak valid!\n");
        }
    }
}

void login()
{
    FILE *file = openFile("datauser.txt", "r");
    if (file == NULL)
        return;

    struct akun user;
    char username[20], password[20];
    int cekData = 0;

    do
    {
        printf("\n=== LOGIN ===\n");
        printf("Masukkan Username: ");
        scanf("%s", username);

        printf("Masukkan Password: ");
        scanf("%s", password);

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
    home(user.id, user.username);
}

// Fungsi untuk menambah node baru ke BST Judul
void tambahNodeBST(struct bstJudul **root, struct queue *queue) {
    char judul[100];
    printf("Masukkan judul buku yang ingin dimasukkan ke BST: ");
    scanf(" %[^\n]", judul);

    struct node *current = queue->front;
    while (current != NULL) {
        if (cmpIgnoreCase(current->data.judul, judul) == 0) {
            *root = insertBSTJudul(*root, current->data.judul, current);
            printf("Node judul \"%s\" berhasil dimasukkan ke BST.\n", judul);
            return;
        }
        current = current->next;
    }
    printf("Judul buku tidak ditemukan di database.\n");
}

// Fungsi untuk menghapus leaf node dari BST Judul
struct bstJudul* hapusLeafBSTJudul(struct bstJudul* root, const char* judul, int *deleted) {
    if (root == NULL) return NULL;
    int cmp = cmpIgnoreCase(judul, root->judul);
    if (cmp < 0)
        root->left = hapusLeafBSTJudul(root->left, judul, deleted);
    else if (cmp > 0)
        root->right = hapusLeafBSTJudul(root->right, judul, deleted);
    else {
        // Cek leaf
        if (root->left == NULL && root->right == NULL) {
            printf("Leaf node \"%s\" dihapus dari BST.\n", root->judul);
            free(root);
            *deleted = 1;
            return NULL;
        } else {
            printf("Node \"%s\" bukan leaf node, tidak dihapus.\n", root->judul);
        }
    }
    return root;
}