#pragma once

#include "Date.h"
#include "General.h"
#define MAX 100
typedef struct 
{
	float	rating; // 1 - 5
	char	review[MAX_STR_LEN];
	Date	posReviewDate;

}customerRatingsReviews;

typedef struct {
	customerRatingsReviews arr[MAX];
	int numRatingsReviews;
		
}Ratings_Reviews;

void	initRatingsReviews(Ratings_Reviews* RR);
int		addNewReview(Ratings_Reviews* RR, const char* bName);
void	printRatingReview(const Ratings_Reviews* RR);
float	getAverageBusinessRating(const Ratings_Reviews* RR);






/*
------------- RATINGS & Reviews -----------
business name        rating       post date

Ikea				4				13/2/24
------------------------------------------
Review:





************** END REVIEW & RATING ********


----------------------- FLOW CHART ----------------------------------
1. print business list
2. enter business name
3. find if such a business exist
4. if 3.  ==  true,  print customers
5. enter name
6. find that customer in business arr of customers
7. if 6. == true:
8. add rating and review

pS: add relevant functionality in doBusiness function.















*/