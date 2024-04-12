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
				//prints formatiting
				printf("\tindex %d -> ", i);
				//prints first item in array
				printf("%d, %c, %d", pHashArray[key].record->id, pHashArray[key].record->name, pHashArray[key].record->order);

				//sets a temp variable for use in the folowing loop
				struct HashType *temp = &pHashArray[key];
				//loop checks all next elements to see if there is more items to be printed
				while (temp->next != NULL) {
					printf(" -> %d , %c, %d ", temp->next->record->id, 
										  temp->next->record->name, 
										  temp->next->record->order);
					
					//moves to next item in list if next is not NULL!
					temp = temp->next;
				}
				//formatting
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
	
	//declares and allocates memory to pHashArray
	struct HashType *pHashArray = (struct HashType*) malloc(sizeof(struct HashType) * recordSz);

	//opens for loop for iterating thought array
	for(int i = 0; i < recordSz; i++) {
		
		//declares and sets key for
		int key = hash(pRecords[i].id);
		//if the key is not matching then we can simply set the matching index from pRecords[] to it 
		if(pHashArray[key].key != key) {
			
			pHashArray[key].record = (struct RecordType*)malloc(sizeof(struct RecordType));
			//since this is the first in its index, the next pointer must be NULL
			pHashArray[key].next = NULL;
			pHashArray[key].key = key;
			pHashArray[key].record->id = pRecords[i].id;
			pHashArray[key].record->name = pRecords[i].name;
			pHashArray[key].record->order = pRecords[i].order;
			
			//if the key is matching then we must move to next available next pointer
		} else if (pHashArray[key].key == key ) {

			//declare temp for use in while loop
			struct HashType *temp = &pHashArray[key];
			//open while loop and loop until next is NULL, this means we have an open spot in the linked list
			while (temp->next != NULL) {
				temp = temp->next;
			}
				//malloc all neccessary structs
				temp->next = (struct HashType*)malloc(sizeof(struct HashType));
				temp->next->record = (struct RecordType*)malloc(sizeof(struct RecordType));

				//set all valid fields to its respective value found in the matching index from pRecords[]
				temp->next->key = key;
				temp->next->record->id = pRecords[i].id;
				temp->next->record->name = pRecords[i].name;
				temp->next->record->order = pRecords[i].order;
				//set next to NULL because this must be the last item in the linked list!
				temp->next->next = NULL;
		}
	}
	displayRecordsInHash(pHashArray, recordSz);
	free(pHashArray);
}