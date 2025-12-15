#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "WorkSchedule.h"
#include "General.h"

#include "FileHelper.h"

const char* dayStr[eNoOfDays] = { "Sunday", "Monday", "Tuesday"
								, "Wednesday", "Thursday"
								,"Friday" };


int initSchedule(Schedule* pSchedule) {
	int i;
	int isValidInput = 1;

	do {
		isValidInput = 1;
		for (i = 0; i < eNoOfDays; i++) {
			printf("Schedule for day %s:\n", dayStr[i]);
			pSchedule->daysArr[i].startHour = readHour("Enter start hour");
			pSchedule->daysArr[i].startMinute = readMinute("Enter start minute");
			pSchedule->daysArr[i].endHour = readHour("Enter end hour");
			pSchedule->daysArr[i].endMinute = readMinute("Enter end minute");

			// check valid input
			if (!isStartTimeBeforeEndTime(&(pSchedule->daysArr[i]), &(pSchedule->daysArr[i]))) {
				printf("Invalid input! Start time must be before end time.\n");
				isValidInput = 0; // exit condition
				break;
			}
		}
	} while (!isValidInput);

	return 1;
}

int readHour(const char* prompt) {
	int hour;
	char input[HOUR_MINUTES_MAX_SIZE];
	int isValidInput;

	do {
		isValidInput = 1; // Assume input is valid initially
		printf("%s (00-23): ", prompt);
		myGets(input, sizeof(input), stdin);

		// Check if the input is a single digit
		if (strlen(input) == 1 && isdigit(input[0])) {
			// Prepend '0' before the single digit if there's enough space
			if (sizeof(input) >= 3) {
				printf("Adding leading '0' to represent two digits.\n");
				memmove(input + 1, input, strlen(input) + 1); // Shift existing characters to make room for '0'
				input[0] = '0'; // Prepend '0'
			}
			else {
				printf("Input buffer is too small to add leading '0'.\n");
			}
		}

		// Convert string to integer
		if (sscanf(input, "%d", &hour) != 1 || !isValidHour(hour)) {
			printf("Invalid input. Please enter a valid hour (00-23).\n");
			isValidInput = 0; // Set flag to indicate invalid input
		}
	} while (!isValidInput);

	printf("\n");
	return hour;
}


int readMinute(const char* prompt) {
	int minute;
	char input[HOUR_MINUTES_MAX_SIZE];
	int isValidInput;

	do {
		isValidInput = 1; // Assume input is valid initially
		printf("%s (00-59): ", prompt);
		myGets(input, sizeof(input), stdin);

		// Check if the input is a single digit
		if (strlen(input) == 1 && isdigit(input[0])) {
			// Prepend '0' before the single digit if there's enough space
			if (sizeof(input) >= 3) {
				printf("Adding leading '0' to represent two digits.\n");
				memmove(input + 1, input, strlen(input) + 1); // Shift existing characters to make room for '0'
				input[0] = '0'; // Prepend '0'
			}
			else {
				printf("Input buffer is too small to add leading '0'.\n");
			}
		}

		// Convert string to integer
		if (sscanf(input, "%d", &minute) != 1 || !isValidMinute(minute)) {
			printf("Invalid input. Please enter a valid minute (00-59).\n");
			isValidInput = 0; // Set flag to indicate invalid input
		}
	} while (!isValidInput);

	printf("\n");
	return minute;
}

int isValidHour(int hour) {
	return hour >= 0 && hour <= 23;
}

int isValidMinute(int minute) {
	return minute >= 0 && minute <= 59;
}

int isStartTimeBeforeEndTime(const ScheduleTime* startTime, const ScheduleTime* endTime)
{
	
	if (startTime->startHour > endTime->endHour)
		return 0;
	else if (startTime->startHour == endTime->endHour)
		return (startTime->startMinute < endTime->endMinute);
	return 1;
}

void printSchedule(const Schedule* pSchedule)
{
	printf("%-10s %-10s %-10s\n", "Day", "Start Time", "End Time");
	printf("%s\n\n", SPACE_BARS);
	
	for (int i = 0; i < eNoOfDays; i++) {
		printf("%-10s %02d:%02d %10s %02d:%02d\n", dayStr[i],
			pSchedule->daysArr[i].startHour, pSchedule->daysArr[i].startMinute,
			"", // Placeholder for alignment
			pSchedule->daysArr[i].endHour, pSchedule->daysArr[i].endMinute);
	}
	printf("%s\n\n", ASTERICKS);
}


int saveBusinessWorkHoursToCompressedBinFile(const Schedule* pBusinessSchedule, FILE* fp)
{
	// Loop through each day's schedule
	for (int i = 0; i < eNoOfDays; i++) {
		BYTE data[3] = { 0 }; // We need 3 bytes to store the compressed ScheduleTime

		// Extract ScheduleTime data for the current day
		const ScheduleTime* currentDay = &(pBusinessSchedule->daysArr[i]);

		// Compress and store ScheduleTime data
		data[0] = (currentDay->startHour << 3) | ((currentDay->startMinute >> 3) & 0x7);
		data[1] = ((currentDay->startMinute & 0x7) << 5) | (currentDay->endHour & 0x1F);
		data[2] = (currentDay->endMinute << 1) | ((i >> 2) & 0x1);

		// Write the compressed ScheduleTime data to the file
		if (fwrite(&data, sizeof(BYTE), 3, fp) != 3) {
			printf("Error writing compressed ScheduleTime data to file\n");
			return 0;
		}
	}

	return 1;
}

int readBusinessWorkHoursFromCompressedBinFile(Schedule* pBusinessSchedule, FILE* fp)
{
	// Loop through each day's schedule
	for (int i = 0; i < eNoOfDays; i++) {
		BYTE data[3];

		// Read the compressed ScheduleTime data from the file
		if (fread(&data, sizeof(BYTE), 3, fp) != 3) {
			printf("Error reading compressed ScheduleTime data from file\n");
			return 0;
		}

		// Extract ScheduleTime data from the compressed data
		ScheduleTime* currentDay = &(pBusinessSchedule->daysArr[i]);
		currentDay->startHour = (data[0] >> 3) & 0x1F;
		currentDay->startMinute = ((data[0] & 0x7) << 3) | ((data[1] >> 5) & 0x7);
		currentDay->endHour = data[1] & 0x1F;
		currentDay->endMinute = (data[1] >> 5) | ((data[2] & 0x1) << 7);
	}

	return 1;
}

int savingBusinessWorkHoursToTxtFile(const Schedule* pBusinessSchedule, FILE* fp)
{
	for (int i = 0; i < eNoOfDays; i++) {
		const ScheduleTime* currentDay = &(pBusinessSchedule->daysArr[i]);

		// Write the start and end times to the file
		fprintf(fp, "%02d:%02d-%02d:%02d\n",
			currentDay->startHour, currentDay->startMinute,
			currentDay->endHour, currentDay->endMinute);
	}

	return 1;
	
}

int readingBusinessWorkHoursToTxtFile(Schedule* pBusinessSchedule, FILE* fp)
{
	for (int i = 0; i < eNoOfDays; i++) {
		ScheduleTime* currentDay = &(pBusinessSchedule->daysArr[i]);

		// Read the start and end times from the file
		if (fscanf(fp, "%d:%d-%d:%d",
			&(currentDay->startHour), &(currentDay->startMinute),
			&(currentDay->endHour), &(currentDay->endMinute)) != 4) {
			printf("Error reading schedule from file.\n");
			return 0;
		}
	}

	return 1;
}



//int saveBusinessWorkHoursToFile(Schedule* pBusinessSchedule, FILE* fp)
//{
//	
//	if (!writeIntToFile(eNoOfDays, fp, "Error writing number of days.")) 
//		return 0;
//
//	// Iterate over each day in the schedule
//	for (int i = 0; i < eNoOfDays; i++) 
//	{
//		ScheduleTime* workHours = &(pBusinessSchedule->daysArr[i]);
//
//			// Write the day and its work hours to the file
//		if (!writeIntToFile(i, fp, "Error writing day."))
//			return 0; // Write the day index
//		if (!writeIntToFile(workHours->startHour, fp, "Error writing start hour.")) 
//			return 0;
//		if (!writeIntToFile(workHours->startMinute, fp, "Error writing start minute."))
//			return 0;
//		if (!writeIntToFile(workHours->endHour, fp, "Error writing end hour.")) 
//			return 0;
//		if (!writeIntToFile(workHours->endMinute, fp, "Error writing end minute.")) 
//			return 0;
//	}
//
//	return 1;
//
//}





