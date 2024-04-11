/*
	Carson Davie
	Lab 9
	Torosdagli

	** line 118 i changed input.txt to input_lab_9.txt
*/
#include <stdio.h>
#include <stdlib.h>

// RecordType
struct RecordType
{
	int id;
	char name;
	int order;
};

struct Node
{
	struct RecordType data;
	struct Node *next;
};

struct HashType
{
	struct Node *table[31];
};

// Compute the hash function
int hash(int x)
{
	return x % 23;
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
void displayRecordsInHash(struct HashType *pHashArray, int hashSz)
{
	int i;

	for (i = 0; i < hashSz; ++i)
	{
		if (pHashArray->table[i] != NULL)
		{
			printf("index %d -> ", i);
			struct Node *current = pHashArray->table[i];
			while (current != NULL)
			{
				printf("%d, %c, %d", current->data.id, current->data.name, current->data.order);
				if (current->next != NULL)
				{
					printf(" -> ");
				}
				current = current->next;
			}
			printf("\n");
		}
	}
}

int main(void)
{
	struct RecordType *pRecords;
	int recordSz = 0;

	recordSz = parseData("input_lab_9.txt", &pRecords);
	printRecords(pRecords, recordSz);

	// initialize hash table
	struct HashType *hashTable = (struct HashType *)malloc(sizeof(struct HashType));
	for (int i = 0; i < 23; i++)
	{
		hashTable->table[i] = NULL;
	}

	// fill hash table
	for (int i = 0; i < recordSz; i++)
	{
		int hashIndex = hash(pRecords[i].id);

		struct Node *insert = (struct Node *)malloc(sizeof(struct Node));
		insert->data = pRecords[i];
		insert->next = NULL;

		if (hashTable->table[hashIndex] == NULL)
		{
			hashTable->table[hashIndex] = insert;
		}
		else
		{
			struct Node *current = hashTable->table[hashIndex];

			while (current->next != NULL)
			{
				current = current->next;
			}

			current->next = insert;
		}
	}

	displayRecordsInHash(hashTable, 23);
}