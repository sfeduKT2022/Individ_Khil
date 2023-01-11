/******************************************************************************
Хиль Дмитрий Сергеевич
КТбо1-2
Индивидуальная работа No 1
Вариант № 25
Задание: Отчёт - По двум указанным услугам подсчитать и сравнить их текущую месячную оплату; Параметры - Наименование двух услуг.
*******************************************************************************/

#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<string.h>

struct Client {
	char surname[20];
	char name[20];
	char otc[20];
	char phone[20];
	int d1;
	int m1;
	int y1;
	int d2;
	int m2;
	int y2;
	int dolg;
	int credit;
};
Client info[20];

struct Uslugi
{
	char name[100];
	int code;
	char tarif[7];
	char time[20];
};
Uslugi dat[15];

struct Information
{
	char number[20];
	int ucode;
	int sec;
	int min;
	int hours;
	int day;
	int month;
	int year;
	char sectime[10];
};
Information itog[12];


struct Param {
	char usluga1[20];//MMS
	double d;//170.3 1867
};
Param usluga[10];

int readclients() {
	FILE* file;
	fopen_s(&file, "users.txt", "r");
	int i = 0;
	if (file != NULL)
	{
		while (!feof(file)) {
			fscanf_s(file, "%s %s %[a-zA-Z]%*c%*c%[0-9]%*c %d%*c%d%*c%d%*c %d%*c%d%*c%d%*c%d%*c%d\n",
				info[i].surname, 20, info[i].name, 20, info[i].otc, 20, info[i].phone, 20, &info[i].d1,
				&info[i].m1, &info[i].y1, &info[i].d2, &info[i].m2, &info[i].y2, &info[i].dolg, &info[i].credit);
			i++;
		}
		fclose(file);
	}
	return i;
}
int readuslugi()
{
	int t = 0;
	FILE* file1;
	fopen_s(&file1, "usluga.txt", "r");
	if (file1 != NULL)
	{
		while (!feof(file1)) {
			fscanf_s(file1, "%[^0123456789,]%*c %d%*c %[0-9.]%*c %s\n", dat[t].name, 100, &dat[t].code,
				dat[t].tarif, 7, dat[t].time, 20);
			t++;
		}
		fclose(file1);
	}
	return t;
}
int readsusp()
{
	FILE* file2;
	int q = 0;
	fopen_s(&file2, "uinfo.txt", "r");
	if (file2 != NULL)
	{
		while (!feof(file2)) {
			fscanf_s(file2, "%[0-9]%*c %d%*c %d.%d.%d %d:%d:%d%*c %s\n", itog[q].number, 20, &itog[q].ucode,
				&itog[q].day, &itog[q].month, &itog[q].year, &itog[q].hours,
				&itog[q].min, &itog[q].sec, itog[q].sectime, 10);
			q++;
		}
		fclose(file2);
	}
	return q;
}
int readparam() {
	FILE* file3;
	fopen_s(&file3, "param.ini", "r");
	int d = 0;
	if (file3 != NULL)
	{
		while (!feof(file3)) {

			fscanf_s(file3, "%s", usluga[d].usluga1, 20);
			d++;

		}
		fclose(file3);
	}
	return d;
}

bool checktime(int day, int month, int year) {
	char buffer[100] = { 0 };
	int nday, nmonth, nyear;
	struct tm* ptr;
	const time_t timer = time(NULL);
	ptr = localtime(&timer);
	strftime(buffer, 100, "%d %m %Y", ptr);
	sscanf_s(buffer, "%d %d %d", &nday, &nmonth, &nyear);
	if ((year == nyear) && (month == nmonth) && (nday >= day))
	{
		return true;
	}
	else {
		return false;
	}
}

double getTime(int code, int ls, int lpc, char* tar, char* cif) {
	int cday, cmonth, cyear, fr = 1;
	double sum = 0;
	char dmin[10] = { "min" }, dday[10] = { "day" };
	char dmonth[10] = { "month" };
	bool flag;
	for (int j = 0; j < ls; j++) {
		if ((code == itog[j].ucode) && (strcmp(itog[j].sectime, "#") != 0)) {
			cday = itog[j].day;
			cmonth = itog[j].month;
			cyear = itog[j].year;
			flag = checktime(cday, cmonth, cyear);
			if (flag) {
				if (strcmp(cif, dmin) == 0) {
					fr = fr * 60;
				}
				if (strcmp(cif, dday) == 0) {
					fr = fr * 86400;
				}
				if (strcmp(cif, dmonth) == 0) {
					if (cmonth % 2 == 0) {
						fr = fr * 2592000;
					}
					else {
						fr = fr * 2678400;
					}
				}
				sum = sum + ((strtod(tar, NULL) * strtod(itog[j].sectime, NULL)) / fr);
				fr = 1;
			}
		}
	}
	return sum;
}

void checkname(int lc, int lu, int ls, int lp) {
	FILE* out;
	out = fopen("output.txt", "w");
	int code = 0;
	double ft = 0;
	bool flag = false;
	for (int y = 0; y < lp; y++)
	{
		for (int r = 0; r < lu; r++)
		{
			if (strcmp(usluga[y].usluga1, dat[r].name) == 0) {
				if (strcmp(dat[r].time, "#") != 0) {
					code = dat[r].code;
					ft = getTime(code, ls, lp, dat[r].tarif, dat[r].time);
					usluga[y].d = ft;
				}
			}
		}
	}
	if (usluga[0].d >= usluga[1].d) {
		fprintf(out, "%s - %.4g\n%s - %.4g", usluga[0].usluga1, usluga[0].d, usluga[1].usluga1, usluga[1].d);
	}
	else {
		fprintf(out, "%s - %.4g\n%s - %.4g", usluga[1].usluga1, usluga[1].d, usluga[0].usluga1, usluga[0].d);
	}
	fclose(out);
}


int main()
{
	int lenCl, lenUs, lenSu, lenPa;
	readclients();
	readuslugi();
	readsusp();
	readparam();
	lenCl = readclients();
	lenUs = readuslugi();
	lenSu = readsusp();
	lenPa = readparam();
	checkname(lenCl, lenUs, lenSu, lenPa);
	return 0;
}

