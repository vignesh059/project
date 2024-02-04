// Framework program

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FIELDVALUELENGTH 20
#define FIELDNAMELENGTH 50
#define MESSAGELENGTH 250
#define FIELDSFILE "fields.cfg"
#define MENUFILE "menu.cfg"
#define DATAFILE "framework0.dat"
#define MESSAGESFILE "messages.dat"

FILE *fp_data, *fp_fields, *fp_menu, *fp_messages;
char fieldValue[FIELDVALUELENGTH], fieldName[FIELDNAMELENGTH], message[FIELDNAMELENGTH];
char **fieldNames;
char **messages;
int FieldsCount;
char updatedValue[FIELDVALUELENGTH];
char RecordId[FIELDVALUELENGTH];

void showMenu();
char **getFieldsArray();
int getFieldsCount();
char **getRecordDetails();
void addRecord(char **);
void showAllRecords();
char *getRecordId();
int getRecordPosition(char *);
char* getUpdatedFieldvalue();
int getRecordsCount();
void PrintSearchRecord();
void updateRecord(char *);
void deleteRecord(char *);
void searchRecord(char *);
char **getMessagesArray();
int getMessagesCount();

void main()
{
	fp_fields = fopen(FIELDSFILE, "r");
	fp_messages = fopen(MESSAGESFILE,"r");
	FieldsCount = getFieldsCount();
	fieldNames = getFieldsArray();
	messages = getMessagesArray();
	do
	{
		showMenu();
	}
    while (1);
}

int getFieldsCount()
{
	int fieldsCounter = 0;
	while (fgets(fieldName, sizeof(fieldName), fp_fields))
	{
		fieldsCounter++;
	}
	return fieldsCounter;
}

char** getFieldsArray()
{
	int fieldsCounter;
	rewind(fp_fields);
	fieldNames = (char **)malloc(FieldsCount * sizeof(char *));
	for (fieldsCounter = 0; fieldsCounter < FieldsCount; fieldsCounter++)
	{
		fieldNames[fieldsCounter] = malloc(FIELDNAMELENGTH);
		fgets(fieldName, sizeof(fieldName), fp_fields);
		int fieldNameLength = strlen(fieldName);
		fieldName[fieldNameLength - 1] = '\0';
		strcpy(fieldNames[fieldsCounter], fieldName);
	}
	return fieldNames;
}

void showMenu()
{
	int menuSize, choice;
	fp_menu = fopen(MENUFILE, "rb");
	fseek(fp_menu, 0, SEEK_END);
	menuSize = ftell(fp_menu);
	fseek(fp_menu, 0, SEEK_SET);
	char menu[menuSize];
	fread(&menu, sizeof(menu), 1, fp_menu);	
	fwrite(&menu, sizeof(menu), 1, stdout);
	fclose(fp_menu);
	printf("\n");

	printf("Enter your choice: ");
	scanf("%d", &choice);

	switch (choice)
	{
		case 1: addRecord(getRecordDetails());
			break;

		case 2: showAllRecords();
			break;

		case 3: updateRecord(getRecordId());
			break;

		case 4: deleteRecord(getRecordId());
			break;

		case 5: searchRecord(getRecordId());
			break;

		case 6: printf("Exiting.");
			fclose(fp_fields);
			fclose(fp_messages);
			exit(0);

		default: printf("Enter a valid choice.\n");
	}
}

char** getRecordDetails()
{
	int counter;
	char **fieldValues = (char **)malloc(FieldsCount * sizeof(char *));
	for (counter = 0; counter < FieldsCount; counter++)
	{
		fieldValues[counter] = malloc(FIELDVALUELENGTH);
		if (counter == 0)
		{
			strcpy(fieldValue, "a");
			strcpy(fieldValues[counter], fieldValue);
			continue;
		}

		printf("Enter %s: ", fieldNames[counter]);
		scanf("%s", fieldValues[counter]);
	}
	return fieldValues;
}

void addRecord(char **fieldValues)
{
	int counter;
	fp_data = fopen(DATAFILE, "a");
	for (counter = 0; counter < FieldsCount; counter++)
	{
		strcpy(fieldValue, fieldValues[counter]);
		fwrite(fieldValue, sizeof(fieldValue), 1, fp_data);
	}
	fclose(fp_data);
	printf("%s\n", messages[0]);
}

int getRecordsCount()
{
	fp_data = fopen(DATAFILE,"r");
	fseek(fp_data, 0, SEEK_END);
	int filesize = ftell(fp_data);
	int recordsCount = filesize / (FieldsCount * FIELDVALUELENGTH);
	return recordsCount;
}

void showAllRecords()
{
	int counter, recordsCounter;
	int recordsCount = getRecordsCount();
	if (recordsCount < 1)
	{
		printf("%s\n", messages[1]);
	}
	else
	{
		printf("%s \n", messages[2]);
		printf("---------------------------------------------\n");
		fp_data = fopen(DATAFILE,"r");
		for (recordsCounter = 0; recordsCounter < recordsCount; recordsCounter++)
		{
			for (counter = 0; counter < FieldsCount; counter++)
			{
				fread(fieldValue, sizeof(fieldValue), 1, fp_data);
				if (counter == 0)
				{
					if (strcmp (fieldValue, "d") == 0)
					{
						fseek(fp_data, (FieldsCount - 1) * FIELDVALUELENGTH, SEEK_CUR);
						break;
					}
					continue;
				}

				printf("%s: %s\n", fieldNames[counter], fieldValue);
			}

			printf("------------------------------------------\n");
		}
	}
}

int getRecordPosition(char *RecordId)
{
	int recordPosition = -1;
	int recordsCounter, counter;
	int recordsCount = getRecordsCount();
	fp_data = fopen(DATAFILE, "r");
	for(recordsCounter = 0; recordsCounter < recordsCount; recordsCounter++)
	{
		fread(fieldValue, sizeof(fieldValue), 1, fp_data);
		if(strcmp(fieldValue, "a") == 0)
		{
			fread(fieldValue, sizeof(fieldValue), 1, fp_data);
			if (strcmp(fieldValue, RecordId) == 0)
			{
				fseek(fp_data, - (FIELDVALUELENGTH * 2), SEEK_CUR);
				recordPosition = ftell(fp_data);
				break;
			}
			else
			{
				fseek(fp_data, FIELDVALUELENGTH * (FieldsCount - 2), SEEK_CUR);
			}
		}
	}
	fclose(fp_data);
	return recordPosition;
}

char *getRecordId()
{
	printf("Enter %s: ", fieldNames[1]);
	scanf("%s", RecordId);
	return RecordId;
}

char* getUpdatedFieldvalue()
{
	printf("Enter new %s to update: ", fieldNames[FieldsCount - 1]);
	scanf("%s", updatedValue);
	return updatedValue;
}



void updateRecord(char *RecordId)
{
	int isRecordFound = 0;
	rewind(fp_messages);
	int recordPosition = getRecordPosition(RecordId);
	if(recordPosition != -1)
	{
		isRecordFound = 1;
		fp_data = fopen(DATAFILE, "r+");
		fseek(fp_data, recordPosition, SEEK_SET);
		fseek(fp_data, (FieldsCount - 1) * FIELDVALUELENGTH, SEEK_CUR);
		strcpy(fieldValue, getUpdatedFieldvalue());
		fwrite(fieldValue, sizeof(fieldValue), 1, fp_data);
		fclose(fp_data);
		printf("%s%s\n", messages[3], RecordId);
		
	}
	if(! isRecordFound)
	{
		printf("%s %s.\n",messages[5], RecordId);
	}
}

void deleteRecord(char *RecordId)
{
	int isRecordFound = 0;
	int recordPosition = getRecordPosition(RecordId);
	if (recordPosition != -1)
	{
		isRecordFound = 1;
		fp_data = fopen(DATAFILE,"r+");
		fseek(fp_data,recordPosition,SEEK_SET);
		fread(fieldValue, sizeof(fieldValue), 1, fp_data);
		fseek(fp_data, -FIELDVALUELENGTH, SEEK_CUR);
		strcpy(fieldValue,"d");
		fwrite(fieldValue, sizeof(fieldValue), 1, fp_data);
		fclose(fp_data);
		printf("%s%s\n", messages[4], RecordId);
		
	}

	if(! isRecordFound)
	{
		printf("%s %s.\n",messages[5], RecordId);
	}
}



void searchRecord(char *RecordId)
{
	rewind(fp_messages);
	int recordPosition = getRecordPosition(RecordId);
	if(recordPosition != -1)
	{
		PrintSearchRecord(recordPosition);
	}
	else
	{
		printf("%s %s.\n",messages[5], RecordId);
	}
}

void PrintSearchRecord(int recordPosition)
{
	int fieldsCounter;
	fp_data = fopen(DATAFILE,"r");
	fseek(fp_data, recordPosition, SEEK_SET);
	printf("Record details: \n");
	for (fieldsCounter = 0; fieldsCounter < FieldsCount; fieldsCounter++)
	{
		fread(&fieldValue, sizeof(fieldValue), 1, fp_data);
		if (fieldsCounter == 0)
		{
			continue;
		}
		printf("%s: %s\n", fieldNames[fieldsCounter], fieldValue);
	}
	fclose(fp_data);
	printf("----------------------------------------\n");
}

int getMessagesCount()
{
	int messageCounter = 0;
	int counter;
	while (fgets(message, sizeof(message), fp_messages))
	{
		messageCounter++;
	}
	rewind(fp_messages);
	return messageCounter;
}

char** getMessagesArray()
{
	int counter;
	char buffer[MESSAGELENGTH];
	int messagescount = getMessagesCount();
	messages = (char**) malloc (sizeof(char*) * messagescount);
	for (counter = 0; counter < messagescount; counter++)
	{
		messages[counter] = malloc (FIELDNAMELENGTH);
		fgets(buffer, sizeof(buffer), fp_messages);
		int messageLength = strlen(buffer);
		buffer[messageLength - 1] = '\0';
		strcpy(messages[counter], buffer);
	}
	return messages;
}
