//Madison Manankil
//COP 3502
//LAB 9
//April 12, 2024
#include <stdio.h>
#include <stdlib.h>
//!NOTE FOR TA!: When running my code please use command prompt and do LAB9.exe<input.txt and the output will primnt
// RecordType
struct RecordType
{
    int id;
    char name;
    int order;
    struct RecordType *next;  // I added this because for chaining
};

// Hash table structure
struct HashTable
{
    struct RecordType **table;  // this is an arrray of pointers used for RecordType
    int size;                   // this is used for being size of the hash table
};

// Compute the hash function
int hash(int x, int hashSz)
{
    return x % hashSz;
}

// Create a new hash table
struct HashTable *createHashTable(int size)
{
    struct HashTable *ht = (struct HashTable*)malloc(sizeof(struct HashTable));
    ht->size = size;
    ht->table = (struct RecordType**)calloc(size, sizeof(struct RecordType*));
    return ht;
}

// Insert a record into the hash table
void insert(struct HashTable *ht, struct RecordType *record)
{
    int index = hash(record->id, ht->size);
    record->next = ht->table[index];  // this function is used to insert record at the beginning part of the linked list
    ht->table[index] = record;
}

// parses input file to an integer array
int parseData(char *inputFileName, struct RecordType **ppData)
{
    FILE *inFile = fopen(inputFileName, "r");
    int dataSz = 0;
    int i, n;
    char c;
    struct RecordType *pRecord;
    *ppData = NULL;

    if (inFile)
    {
        fscanf(inFile, "%d\n", &dataSz);
        *ppData = (struct RecordType *)malloc(sizeof(struct RecordType) * dataSz);
        // Implement parse data block
        if (*ppData == NULL)
        {
            printf("Cannot allocate memory\n");
            exit(-1);
        }
        for (i = 0; i < dataSz; ++i)
        {
            pRecord = *ppData + i;
            fscanf(inFile, "%d ", &n);
            pRecord->id = n;
            fscanf(inFile, "%c ", &c);
            pRecord->name = c;
            fscanf(inFile, "%d ", &n);
            pRecord->order = n;
            pRecord->next = NULL;  //thsi is important for Initialize next pointer to NULL
        }

        fclose(inFile);
    }

    return dataSz;
}

// prints the records
void printRecords(struct RecordType pData[], int dataSz)
{
    int i;
    printf("\nRecords:\n");
    for (i = 0; i < dataSz; ++i)
    {
        printf("\t%d %c %d\n", pData[i].id, pData[i].name, pData[i].order);
    }
    printf("\n\n");
}

// display records in the hash structure
// skip the indices which are free
// the output will be in the format:
// index x -> id, name, order -> id, name, order ....
void displayRecordsInHash(struct HashTable *ht)
{
    for (int i = 0; i < ht->size; i++)
    {
        struct RecordType *current = ht->table[i];
        if (current != NULL)
        {
            printf("Index %d -> ", i);
            while (current != NULL)
            {
                printf("%d, %c, %d -> ", current->id, current->name, current->order);
                current = current->next;
            }
            printf("NULL\n");
        }
    }
}

int main(void)
{
    struct RecordType *pRecords;
    int recordSz = 0;
    char inputFileName[] = "input.txt";

    recordSz = parseData(inputFileName, &pRecords);
    printRecords(pRecords, recordSz);

    // Create a hash table
    int hashSz = 23; //TA said to make 23 hash size
    struct HashTable *ht = createHashTable(hashSz);

    // Insert records into the hash table
    for (int i = 0; i < recordSz; i++)
    {
        insert(ht, &pRecords[i]);
    }

    // this will display the records in the hash table
    displayRecordsInHash(ht);

    // this then clean up memory !
    free(pRecords);
    free(ht->table);
    free(ht);

    return 0;
}