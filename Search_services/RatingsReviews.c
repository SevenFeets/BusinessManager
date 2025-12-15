#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>

#include "RatingsReviews.h"


void initRatingsReviews(Ratings_Reviews* RR)
{
    RR->numRatingsReviews = 0;
}

int addNewReview(Ratings_Reviews* RR, const char* bName) {
    float tempNum;
    char tempReview[MAX_STR_LEN];

    if (RR == NULL) {
        printf("Error: Ratings_Reviews is NULL.\n");
        return 0;
    }

    printf("Add rating to business - %s:\n", bName);

    customerRatingsReviews newReview;
    do {
        printf("Enter rating between 1 and 5:\n");
        printf(" 1 is worst experience ----> 5 is best experience\n");
        scanf("%f", &tempNum);

        if (tempNum < 1 || tempNum > 5) {
            printf("Invalid rating. Please enter a rating between 1 and 5.\n");
        }
        // Clear input buffer
        while (getchar() != '\n'); // Consume newline character

    } while (tempNum < 1 || tempNum > 5);

    newReview.rating = tempNum;

    printf("Enter review text:\n");
    myGets(tempReview, MAX_STR_LEN, stdin);

    strcpy(newReview.review, tempReview);

    printf("Add review date\n");
    getCorrectDate(&newReview.posReviewDate);

    if (RR->numRatingsReviews < MAX) {
        RR->arr[RR->numRatingsReviews] = newReview;
        RR->numRatingsReviews++;
        printf("Review added successfully.\n");
        return 1;
    }
    else {
        printf("Error: Maximum number of reviews reached.\n");
        return 0;
    }
}

float getAverageBusinessRating(const Ratings_Reviews* RR)
{
    if (RR == NULL || RR->numRatingsReviews == 0) {
        // Handle the case where there are no ratings or the input is invalid
        printf("Error: No ratings available.\n");
        return 0.0; // Return an appropriate default value
    }

    float total = 0.0;
    for (int i = 0; i < RR->numRatingsReviews; i++)
    {
        total += RR->arr[i].rating; // Sum all ratings
    }

    return total / RR->numRatingsReviews;
}


void printRatingReview(const Ratings_Reviews* RR)
{
    if (RR->numRatingsReviews == 0)
        printf("There no reviews for business\n");

    for (int i = 0; i < RR->numRatingsReviews; i++)
    {
        printf("rating is:\t\t %.2f\n", RR->arr[i].rating);
        printf("%s\n", RR->arr[i].review);
        printDate(&RR->arr[i].posReviewDate);
    }
}
