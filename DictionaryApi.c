// Working on the Dictionary Api

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define WORDLENGTH 30
#define URLLENGTH 250
#define DEFINITIONLENGTH 300
#define AUDIOURLLENGTH 500

char word[WORDLENGTH];
char *wordDefinition;
char *audioUrl;
FILE *fp_dictionary;

char* getWordDefinition(char *);
char* getAudioUrl(char *);
void playPronunciation(char *);

void main()
{
    char url[URLLENGTH];
    printf("Enter a word to get definition and pronunciation: ");
    scanf("%s", word);
    sprintf(url,"%s%s%s", "curl -s \"https://api.dictionaryapi.dev/api/v2/entries/en/", word, "\" > dictionary.txt");
    system(url);

    fp_dictionary = fopen("dictionary.txt", "r");

   fseek(fp_dictionary, 0, SEEK_END);
   int dictionarySize = ftell(fp_dictionary);
   fseek(fp_dictionary, 0, SEEK_SET);
   char dictionaryApi[dictionarySize];

   fgets(dictionaryApi, sizeof(dictionaryApi), fp_dictionary);

   wordDefinition = malloc(DEFINITIONLENGTH);

   wordDefinition = getWordDefinition(dictionaryApi);

   if (strcmp(wordDefinition, "Not Found") == 0)
   {
    printf("%s not found in the dictionary\n", word);
   }
   else
   {
    printf("Definition of %s: %s\n", word, wordDefinition);
   }

   fseek(fp_dictionary, 0, SEEK_SET);
   fgets(dictionaryApi, dictionarySize, fp_dictionary);
   audioUrl = malloc(AUDIOURLLENGTH);
   audioUrl = getAudioUrl(dictionaryApi);

   if (strcmp (wordDefinition, "Not Found") == 0)
   {
    printf("%s not found in the dictionary.\n", word);
   }

   else
   {
    playPronunciation(audioUrl);
   }

   fclose(fp_dictionary);
}

char *getWordDefinition(char *dictionaryApi)
{
    strcpy(wordDefinition, "Not Found");
    char *token = (char*) strtok (dictionaryApi, ":");
    while (token != NULL)
    {
        if (strcmp(token, "[{\"definition\"") == 0)
        {
            token = strtok(NULL, "\";.");
            strcpy(wordDefinition, token);
            break;
        }

        token = strtok(NULL, ":");
    }

    return wordDefinition;
}

char *getAudioUrl(char *dictionaryApi)
{
    int isAudioFound = 0;
    strcpy(audioUrl, "Not Found");
    int dictionarySize = ftell(fp_dictionary);

    char *token = strtok(dictionaryApi, "\"");
    while (token != NULL)
    {
        if (strcmp(token, "audio") == 0)
        {
            token = strtok(NULL, "\"");
            token = strtok(NULL, "\"");
            strcpy(audioUrl, token);
            if (strlen(audioUrl) < 10)
            {
                token = strtok(NULL, "\"");
                isAudioFound = 1;
                continue;
            }
            else
            {
                isAudioFound = 0;
                break;
            }
        }
        token = strtok(NULL, "\"");
    }
    if (isAudioFound == 1)
    {
        fseek(fp_dictionary, 0, SEEK_SET);
        fgets(dictionaryApi, dictionarySize, fp_dictionary);
        char *token = strtok(dictionaryApi, ":");
        while (token != NULL)
        {
            if (strcmp(token, "[{\"audio\"") == 0)
            {
                token = strtok(NULL, "\"");
                strcpy(audioUrl, token);
                break;
            }
            token = strtok(NULL,":");
        }
    }
    return audioUrl;
}

void playPronunciation(char *audioUrl)
{
    printf("\nPlaying %s pronunciation..\n", word);
    char playCommand[URLLENGTH];
    sprintf(playCommand, "curl -s %s > audio.mp3", audioUrl);
    // printf("%s\n", playCommand);
    system(playCommand);
    system("vlc audio.mp3");
}
