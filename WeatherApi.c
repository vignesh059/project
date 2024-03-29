// Working on weather Api

#include <stdio.h>
#include <stdlib.h>

void main()
{
	char city[20], url[200];

	printf("Enter city name: ");
	scanf("%s", city);

	sprintf(url, "%s%s%s", "curl \"https://api.openweathermap.org/data/2.5/weather?q=", city, "&appid=abe3a0f4d0b6cebfbe7393b4b4e3aa28&units=metric\" > weather.txt");
	printf("%s",url);
	system(url);

	FILE *fp_weatherData = fopen("weather.txt", "r");

	fseek(fp_weatherData, 0, SEEK_END);

	int length = ftell(fp_weatherData);

	char weatherData[length];

	fseek(fp_weatherData, 0, SEEK_SET);

	fread(weatherData, sizeof(weatherData), 1, fp_weatherData);

	fclose(fp_weatherData);

	char *token = strtok(weatherData, ":");

	while (token != NULL)
	{
		if (strcmp("{\"temp\"", token) == 0)
		{
			token = strtok(NULL, ",");
			printf("\nTemperature: %s\n", token);
			break;
		}
		else
		{
			token = strtok(NULL, ":");
		}
	}
}
