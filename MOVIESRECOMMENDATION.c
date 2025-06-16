#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_MOVIES 100
#define MAX_GENRES 3

const char *allGenres[] = {
    "Action", "Comedy", "Drama", "Horror", "SciFi", "Romance", "Thriller", "Adventure", "Fantasy", "Documentary"};
const char *allLanguages[] = {
    "English", "Urdu", "Hindi", "Spanish", "French", "German", "Korean"};

int totalGenres = sizeof(allGenres) / sizeof(allGenres[0]);
int totalLanguages = sizeof(allLanguages) / sizeof(allLanguages[0]);

struct Movie
{
    char title[100];
    char genres[MAX_GENRES][30];
    char language[30];
    int ageLimit;
    float rating;
};

// Load movies from file
int loadMovies(struct Movie movies[])
{
    FILE *fp = fopen("movies.txt", "r");
    if (!fp)
        return 0;

    int count = 0;
    while (!feof(fp) && count < MAX_MOVIES)
    {
        char genreLine[100];
        fscanf(fp, " %[^|]|%[^|]|%[^|]|%d|%f\n",
               movies[count].title,
               genreLine,
               movies[count].language,
               &movies[count].ageLimit,
               &movies[count].rating);

        char *token = strtok(genreLine, ",");
        int g = 0;
        while (token && g < MAX_GENRES)
        {
            strcpy(movies[count].genres[g++], token);
            token = strtok(NULL, ",");
        }
        count++;
    }
    fclose(fp);
    return count;
}

// Recommend movies
void recommend(struct Movie movies[], int total, int age, char genres[][30], char *language)
{
    printf("\n=====================================================\n");
    printf("                  Recommended Movies\n");
    printf("=====================================================\n");

    int found = 0;
    for (int i = 0; i < total; i++)
    {
        int genreMatch = 0;
        for (int g = 0; g < MAX_GENRES; g++)
        {
            for (int u = 0; u < MAX_GENRES; u++)
            {
                if (strcasecmp(movies[i].genres[g], genres[u]) == 0)
                {
                    genreMatch = 1;
                    break;
                }
            }
            if (genreMatch)
                break;
        }

        if (age >= movies[i].ageLimit &&
            genreMatch &&
            strcasecmp(language, movies[i].language) == 0)
        {

            printf("\n-----------------------------------------------------\n");
            printf(" Title      : %s\n", movies[i].title);
            printf(" Genres     : %s, %s, %s\n", movies[i].genres[0], movies[i].genres[1], movies[i].genres[2]);
            printf(" Language   : %s\n", movies[i].language);
            printf(" Age Limit  : %d+\n", movies[i].ageLimit);
            printf(" Rating     : %.1f / 10\n", movies[i].rating);
            found++;
        }
    }
    if (!found)
    {
        printf("\nNo movies matched your preferences.\n");
    }
    printf("=====================================================\n");
}

// Select from menu
void selectFromMenu(const char *options[], int total, int countToSelect, char selected[][30], char *typeName)
{
    for (int i = 0; i < countToSelect; i++)
    {
        int choice;
        printf("\nSelect %s %d:\n", typeName, i + 1);
        for (int j = 0; j < total; j++)
        {
            printf("  %d. %s\n", j + 1, options[j]);
        }
        printf("Your choice: ");
        scanf("%d", &choice);
        while (choice < 1 || choice > total)
        {
            printf("Invalid! Try again: ");
            scanf("%d", &choice);
        }
        strcpy(selected[i], options[choice - 1]);
    }
}

int main()
{
    struct Movie movies[MAX_MOVIES];
    int total = loadMovies(movies);

    if (total == 0)
    {
        printf("\n[ERROR] No movies loaded. Check 'movies.txt'.\n");
        return 1;
    }

    int age;
    char language[30];
    char userGenres[MAX_GENRES][30];

    printf("=====================================================\n");
    printf("         Welcome to the Movie Recommendation System\n");
    printf("=====================================================\n");

    printf("Enter your age: ");
    scanf("%d", &age);

    // Menu for genres
    selectFromMenu(allGenres, totalGenres, MAX_GENRES, userGenres, "Genre");

    // Menu for language (only one)
    char langSelect[1][30];
    selectFromMenu(allLanguages, totalLanguages, 1, langSelect, "Language");
    strcpy(language, langSelect[0]);

    recommend(movies, total, age, userGenres, language);

    printf("\nThank you for using the system!\n");
    return 0;
}
