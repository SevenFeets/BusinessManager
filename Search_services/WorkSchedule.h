#ifndef __WORK__SCHEDULE__
#define __WORK__SCHEDULE__

typedef enum { eSun, eMon, eTue, eWed, eThu, eFri, eNoOfDays } eDayType;

extern const char* dayStr[eNoOfDays];

#define HOUR_MINUTES_MAX_SIZE 3

typedef	struct {
    int startHour;
    int startMinute;
    int endHour;
    int endMinute;
} ScheduleTime;

typedef struct {
    ScheduleTime daysArr[eNoOfDays];
} Schedule;

int     initSchedule(Schedule* pSchedule);
void    printSchedule(const Schedule* pSchedule);

int     isStartTimeBeforeEndTime(const ScheduleTime* startTime, const ScheduleTime* endTime);
int     readHour(const char* prompt);
int     isValidHour(int hour);

int     readMinute(const char* prompt);
int     isValidMinute(int minute);

//int     saveBusinessWorkHoursToFile(Schedule* pBusinessSchedule, FILE* fp);
int saveBusinessWorkHoursToCompressedBinFile(const Schedule* pBusinessSchedule, FILE* fp);
int readBusinessWorkHoursFromCompressedBinFile(Schedule* pBusinessSchedule, FILE* fp);


int savingBusinessWorkHoursToTxtFile(const Schedule* pBusinessSchedule, FILE* fp);
int readingBusinessWorkHoursToTxtFile(Schedule* pBusinessSchedule, FILE* fp);


#endif // !__WORK__SCHEDULE__

