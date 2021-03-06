#include <stdio.h>
#include <string.h>
#include <math.h>

// has to be compiled with -lm because of the math library

int is_name_duplicate(char name[255], const char *filename)
{
	FILE *cards_file = fopen(filename, "r+");
	if(!cards_file)
	{
		printf("Error opening the file to check for duplicates.\n");
		return -1;
	}

	char cur_card[255];
	char* token;
	char cur_value[255];

	while(fgets(cur_card, 255, cards_file))
	{	
		token = strtok(cur_card, ",");
		while(token)
		{
			sscanf(token, "%s", cur_value);
			if(strcmp(name, cur_value) == 0)
			{
				return 1;
			}
			break;
			token = strtok(NULL, ",");
		}
	}
	fclose(cards_file);
	return 0;	
}

int num_of_entries_in_file(const char *filename)
{
	FILE *cards_file = fopen(filename, "r");
	if(!cards_file)
	{
		printf("Error opening the file to check for duplicates.\n");
		return -1;
	}

	int entries = 0;

	char cur_entry[255];
	while(fgets(cur_entry, 255, cards_file))
	{	
		++entries;
	}

	fclose(cards_file);

	return entries;
}

int main (int argc, char *argv[])
{
	srand(time(NULL));

	FILE *cards_file = NULL;

	char* desc[] = { "The_Incredible_", "The_Mighty_", "The_Moaning_", 
		"The_Holy_", "The_Invincible_" };
	char* noun[] = { "Joe", "Banana", "Sombrero", "Armchair", "TV" };

	int length_desc = sizeof(desc) / sizeof(char*);
	int length_noun = sizeof(noun) / sizeof(char*);

	int num_generations_required;
	int num_generated_cards = 0;

	printf("How many cards do you want to generate?\n");
	scanf("%d", &num_generations_required);

	int entries_in_file = num_of_entries_in_file("cards.csv");
	
	while(num_generated_cards < num_generations_required && 
		num_generated_cards < length_desc * length_noun - entries_in_file)
	{
		char name[255];
		
		while(1)
		{
			strcpy(name, desc[rand() % length_desc]);
			strcat(name, noun[rand() % length_noun]);

			if(!is_name_duplicate(name, "cards.csv"))
			{
				cards_file = fopen("cards.csv", "a+");
				fprintf(cards_file, "%s,", name);

				int force;
				int life;
				
				while(1)
				{
					force = rand() % 10 + 1;
					life = rand() % 12 + 1;
					if((float)force/life < 1/1.8)
			 		{
						break;
					}
				}

				fprintf(cards_file, "%d,", life);
				fprintf(cards_file, "%d,", force);
		
				// I changed this up a little bit 
				// The project spec mentioned "the sum of the attack and defense"
				// but my cards have no defense field
				// this looked good to me
				int manacost = rand() % (int)ceil((float)(force+life)/4) + 1;

				fprintf(cards_file, "%d\n", manacost);
				++num_generated_cards;
				fclose(cards_file);
				break;
			}
		}
	}

	printf("Successfully generated %d cards.\n", num_generated_cards);

	if(num_generated_cards < num_generations_required)
	{
		printf("There are no more unique card name combinations left.\n");
	}

	return 0;
}
