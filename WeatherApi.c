// Working on weather api

#include <stdio.h>
#include <stdlib.h>

void main()
{
	char city[20], url[200];

	printf("Enter city name: ");
	scanf("%s", city);

	sprintf(url, "%s%s%s", "curl \"https://api.openweathermap.org/data/2.5/weather?q=", city, "&appid=abe3a0f4d0b6cebfbe7393b4b4e3aa28&units=metric\" > Weather.txt");
	printf("url");
	system(url);

	FILE *fp_weather = fopen("weather.txt", "r");

	fseek(fp_weather, 0, SEEK_END);

	int length = ftell(fp_weather);

	char weatherData[length];

	fseek(fp_weather, 0, SEEK_SET);

	fread(weatherData, sizeof(weatherData), 1, fp_weather);

	fclose(fp_weather);

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
