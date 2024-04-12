#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// RecordType
struct RecordType
{
	int		id;
	char	name;
	int		order; 
};

// Fill out this structure
struct HashType
{
	struct RecordType *record;
	struct HashType *next;
	int key;
};

// Compute the hash function
int hash(int x)
{
	return x % 23;
}

// parses input file to an integer array
int parseData(char* inputFileName, struct RecordType** ppData)
{
	FILE* inFile = fopen(inputFileName, "r");
	int dataSz = 0;
	int i, n;
	char c;
	struct RecordType *pRecord;
	*ppData = NULL;

	if (inFile)
	{
		fscanf(inFile, "%d\n", &dataSz);
		*ppData = (struct RecordType*) malloc(sizeof(struct RecordType) * dataSz);
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
	printf("\nHash Records:\n");
	for (i=0;i<hashSz;++i)
	{
		// if index is occupied with any records, print all
		if(pHashArray[i].record != NULL) {
			int key = pHashArray[i].key;

			if(key < 0) {
				
			} else {
				printf("\tindex %d -> ", i);
				printf("%d, %c, %d", pHashArray[key].record->id, pHashArray[key].record->name, pHashArray[key].record->order);
				if(pHashArray[key].next != NULL) {
					printf(" -> ");
					printf("%d, %d , %c, %d", pHashArray[key].next->key, 
												pHashArray[key].next->record->id, 
												pHashArray[key].next->record->name, 
												pHashArray[key].next->record->order);
				}
				printf("\n");
			}
		}
	}
}

int main(void)
{
	struct RecordType *pRecords;
	int recordSz = 0;

	recordSz = parseData("input_lab_9.txt", &pRecords);
	printRecords(pRecords, recordSz);
	// Your hash implementation
	
	//struct HashType pHashArray[recordSz];
	struct HashType *pHashArray = (struct HashType*) malloc(sizeof(struct HashType) * recordSz);

	for(int i = 0; i < recordSz; i++) {
		
		int key = hash(pRecords[i].id);
		if(pHashArray[key].key != key) {
			
			pHashArray[key].record = (struct RecordType*)malloc(sizeof(struct RecordType));
			pHashArray[key].next = NULL;
			pHashArray[key].key = key;
			pHashArray[key].record->id = pRecords[i].id;
			pHashArray[key].record->name = pRecords[i].name;
			pHashArray[key].record->order = pRecords[i].order;
		} else if (pHashArray[key].key == key ) {

			pHashArray[key].next = (struct HashType*)malloc(sizeof(struct HashType));
			pHashArray[key].next->record = (struct RecordType*)malloc(sizeof(struct RecordType));
			pHashArray[key].next->key = key;
			pHashArray[key].next->record->id = pRecords[i].id;
			pHashArray[key].next->record->name = pRecords[i].name;
			pHashArray[key].next->record->order = pRecords[i].order;
		}
		
	}
	displayRecordsInHash(pHashArray, recordSz);
	free(pHashArray);
}