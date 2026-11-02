/* HostelDesk - assignments, to-do list, budget and reminders (simple version) */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX  100
#define LINE "===================================="

struct Assignment { char subject[30]; char title[50]; int day, month, year; int done; };
struct Task       { char text[80]; int done; };
struct Expense    { int category; float amount; };

struct Assignment asg[MAX];   int asgCount = 0;
struct Task       task[MAX];  int taskCount = 0;
struct Expense    spend[MAX]; int spendCount = 0;
float budget = 0;

char *categories[4] = {"Food", "Travel", "Study", "Other"};

/* ---------- input helpers ---------- */
void getText(char *msg, char *buf, int size)
{
    printf("%s", msg);
    fgets(buf, size, stdin);
    buf[strcspn(buf, "\n")] = '\0';
    for (int i = 0; buf[i]; i++)            /* '|' is our file separator */
        if (buf[i] == '|') buf[i] = '-';
}

int getNumber(char *msg)
{
    char line[20];
    printf("%s", msg);
    fgets(line, sizeof line, stdin);
    return atoi(line);
}

/* ---------- dates ---------- */
/* How many days from today until the due date (negative = already late) */
int daysLeft(struct Assignment a)
{
    time_t t = time(NULL);
    struct tm today = *localtime(&t);
    struct tm due = {0};

    today.tm_hour = 12; today.tm_min = 0; today.tm_sec = 0;
    due.tm_mday = a.day;
    due.tm_mon  = a.month - 1;
    due.tm_year = a.year - 1900;
    due.tm_hour = 12;
    due.tm_isdst = -1;

    double secs = difftime(mktime(&due), mktime(&today));
    return (int)(secs / 86400.0 + (secs >= 0 ? 0.5 : -0.5));
}

/* ---------- saving and loading ---------- */
void saveAll(void)
{
    FILE *fp = fopen("assignments.txt", "w");
    for (int i = 0; i < asgCount; i++)
        fprintf(fp, "%s|%s|%d %d %d %d\n", asg[i].subject, asg[i].title,
                asg[i].day, asg[i].month, asg[i].year, asg[i].done);
    fclose(fp);

    fp = fopen("tasks.txt", "w");
    for (int i = 0; i < taskCount; i++)
        fprintf(fp, "%s|%d\n", task[i].text, task[i].done);
    fclose(fp);

    fp = fopen("expenses.txt", "w");
    fprintf(fp, "%f\n", budget);
    for (int i = 0; i < spendCount; i++)
        fprintf(fp, "%d %f\n", spend[i].category, spend[i].amount);
    fclose(fp);
}

void loadAll(void)
{
    FILE *fp = fopen("assignments.txt", "r");
    if (fp) {
        struct Assignment a;
        while (asgCount < MAX &&
               fscanf(fp, " %29[^|]|%49[^|]|%d %d %d %d", a.subject, a.title,
                      &a.day, &a.month, &a.year, &a.done) == 6)
            asg[asgCount++] = a;
        fclose(fp);
    }

    fp = fopen("tasks.txt", "r");
    if (fp) {
        struct Task t;
        while (taskCount < MAX && fscanf(fp, " %79[^|]|%d", t.text, &t.done) == 2)
            task[taskCount++] = t;
        fclose(fp);
    }

    fp = fopen("expenses.txt", "r");
    if (fp) {
        if (fscanf(fp, "%f", &budget) == 1)
            while (spendCount < MAX &&
                   fscanf(fp, "%d %f", &spend[spendCount].category,
                          &spend[spendCount].amount) == 2)
                spendCount++;
        fclose(fp);
    }
}

/* ---------- counting (used by the dashboard) ---------- */
int countOverdue(void)
{
    int n = 0;
    for (int i = 0; i < asgCount; i++)
        if (!asg[i].done && daysLeft(asg[i]) < 0) n++;
    return n;
}

int countDueSoon(int days)
{
    int n = 0;
    for (int i = 0; i < asgCount; i++)
        if (!asg[i].done && daysLeft(asg[i]) >= 0 && daysLeft(asg[i]) <= days) n++;
    return n;
}

int countPending(void)
{
    int n = 0;
    for (int i = 0; i < taskCount; i++)
        if (!task[i].done) n++;
    return n;
}

float totalSpent(void)
{
    float total = 0;
    for (int i = 0; i < spendCount; i++)
        total += spend[i].amount;
    return total;
}

/* ---------- dashboard ---------- */
void showDashboard(void)
{
    time_t t = time(NULL);
    struct tm *now = localtime(&t);

    printf("\n" LINE "\n");
    printf("   HostelDesk      %02d-%02d-%d\n", now->tm_mday, now->tm_mon + 1,
           now->tm_year + 1900);
    printf(LINE "\n");
    printf("Overdue assignments  : %d\n", countOverdue());
    printf("Due in next 3 days   : %d\n", countDueSoon(3));
    printf("Pending tasks        : %d\n", countPending());
    if (budget > 0)
        printf("Budget used          : %.0f%% (Rs %.0f of Rs %.0f)\n",
               totalSpent() / budget * 100, totalSpent(), budget);
    else
        printf("Budget used          : (no budget set)\n");
    printf(LINE "\n");
}

/* ---------- assignments ---------- */
void addAssignment(void)
{
    if (asgCount == MAX) { printf("List is full.\n"); return; }
    struct Assignment a;
    getText("Subject          : ", a.subject, 30);
    getText("Assignment name  : ", a.title, 50);
    do {
        a.day   = getNumber("Due day (1-31)    : ");
        a.month = getNumber("Due month (1-12)  : ");
        a.year  = getNumber("Due year (2026)   : ");
    } while (a.day < 1 || a.day > 31 || a.month < 1 || a.month > 12 || a.year < 2000);
    a.done = 0;
    asg[asgCount++] = a;
    saveAll();
    printf("\n[OK] Assignment added.\n");
}

void viewAssignments(void)
{
    printf("\n--- Your Assignments ---\n");
    if (asgCount == 0) printf("No assignments yet.\n");
    for (int i = 0; i < asgCount; i++) {
        int d = daysLeft(asg[i]);
        char status[50];
        if (asg[i].done)    strcpy(status, "Done");
        else if (d < 0)     sprintf(status, "OVERDUE by %d day(s)", -d);
        else if (d == 0)    strcpy(status, "DUE TODAY");
        else                sprintf(status, "Due in %d day(s)", d);

        printf("%d. %s - %s\n", i + 1, asg[i].subject, asg[i].title);
        printf("   %02d-%02d-%d  [%s]\n", asg[i].day, asg[i].month, asg[i].year,
               status);
    }
}

void completeAssignment(void)
{
    viewAssignments();
    int n = getNumber("\nNumber to mark done (0 = cancel): ");
    if (n >= 1 && n <= asgCount) {
        asg[n - 1].done = 1;
        saveAll();
        printf("[OK] Marked as done.\n");
    }
}

/* ---------- to-do list ---------- */
void addTask(void)
{
    if (taskCount == MAX) { printf("List is full.\n"); return; }
    getText("Task: ", task[taskCount].text, 80);
    task[taskCount].done = 0;
    taskCount++;
    saveAll();
    printf("\n[OK] Task added.\n");
}

void viewTasks(void)
{
    printf("\n--- Your Tasks ---\n");
    if (taskCount == 0) printf("No tasks yet.\n");
    for (int i = 0; i < taskCount; i++)
        printf("%d. [%c] %s\n", i + 1, task[i].done ? 'x' : ' ', task[i].text);
}

void completeTask(void)
{
    viewTasks();
    int n = getNumber("\nNumber to mark done (0 = cancel): ");
    if (n >= 1 && n <= taskCount) {
        task[n - 1].done = 1;
        saveAll();
        printf("[OK] Marked as done.\n");
    }
}

/* ---------- budget ---------- */
void addExpense(void)
{
    if (spendCount == MAX) { printf("Expense list is full.\n"); return; }
    printf("\nCategories:\n");
    for (int i = 0; i < 4; i++)
        printf("  %d. %s\n", i + 1, categories[i]);
    int c = getNumber("Choose category : ");
    if (c < 1 || c > 4) { printf("Invalid category.\n"); return; }
    spend[spendCount].category = c - 1;
    spend[spendCount].amount = (float)getNumber("Amount (Rs)     : ");
    spendCount++;
    saveAll();
    printf("\n[OK] Expense saved.\n");
    if (budget > 0 && totalSpent() >= budget * 0.8)
        printf("[!] Warning: %.0f%% of your budget is used.\n",
               totalSpent() / budget * 100);
}

void budgetSummary(void)
{
    printf("\n--- Budget Summary ---\n");
    printf("Monthly budget : Rs %.0f\n", budget);
    printf("Total spent    : Rs %.0f\n", totalSpent());
    if (budget > 0)
        printf("Remaining      : Rs %.0f\n", budget - totalSpent());

    printf("\nSpending by category:\n");
    for (int c = 0; c < 4; c++) {
        float sum = 0;
        for (int i = 0; i < spendCount; i++)
            if (spend[i].category == c) sum += spend[i].amount;

        int bars = (totalSpent() > 0) ? (int)(sum / totalSpent() * 10) : 0;
        printf("  %-7s Rs %6.0f  ", categories[c], sum);
        for (int k = 0; k < bars; k++) putchar('#');
        putchar('\n');
    }
}

/* ---------- reminders ---------- */
void showReminders(void)
{
    int found;

    printf("\n--- Reminders ---\n");

    printf("OVERDUE:\n");
    found = 0;
    for (int i = 0; i < asgCount; i++)
        if (!asg[i].done && daysLeft(asg[i]) < 0) {
            printf("  [!] %s - %s (%d day(s) late)\n", asg[i].subject,
                   asg[i].title, -daysLeft(asg[i]));
            found = 1;
        }
    if (!found) printf("  none\n");

    printf("\nDUE IN THE NEXT 7 DAYS:\n");
    found = 0;
    for (int i = 0; i < asgCount; i++)
        if (!asg[i].done && daysLeft(asg[i]) >= 0 && daysLeft(asg[i]) <= 7) {
            printf("  [*] %s - %s (in %d day(s))\n", asg[i].subject,
                   asg[i].title, daysLeft(asg[i]));
            found = 1;
        }
    if (!found) printf("  none\n");

    printf("\nPENDING TASKS:\n");
    found = 0;
    for (int i = 0; i < taskCount; i++)
        if (!task[i].done) { printf("  [ ] %s\n", task[i].text); found = 1; }
    if (!found) printf("  none\n");
}

/* ---------- menus ---------- */
void assignmentMenu(void)
{
    int ch;
    do {
        printf("\n--- Assignments ---\n");
        printf("1. Add assignment\n2. View all\n3. Mark as done\n4. Back\n");
        ch = getNumber("Enter choice: ");
        if (ch == 1) addAssignment();
        else if (ch == 2) viewAssignments();
        else if (ch == 3) completeAssignment();
    } while (ch != 4);
}

void taskMenu(void)
{
    int ch;
    do {
        printf("\n--- To-Do List ---\n");
        printf("1. Add task\n2. View all\n3. Mark as done\n4. Back\n");
        ch = getNumber("Enter choice: ");
        if (ch == 1) addTask();
        else if (ch == 2) viewTasks();
        else if (ch == 3) completeTask();
    } while (ch != 4);
}

void budgetMenu(void)
{
    int ch;
    do {
        printf("\n--- Budget Tracker ---\n");
        printf("1. Set monthly budget\n2. Add expense\n3. View summary\n4. Back\n");
        ch = getNumber("Enter choice: ");
        if (ch == 1) {
            budget = (float)getNumber("Monthly budget (Rs): ");
            saveAll();
            printf("[OK] Budget saved.\n");
        }
        else if (ch == 2) addExpense();
        else if (ch == 3) budgetSummary();
    } while (ch != 4);
}

int main(void)
{
    int ch;
    loadAll();

    do {
        showDashboard();
        printf("\n===== MAIN MENU =====\n");
        printf("1. Assignments\n2. To-Do List\n3. Budget Tracker\n");
        printf("4. Reminders\n5. Exit\n");
        ch = getNumber("Enter choice: ");

        if (ch == 1) assignmentMenu();
        else if (ch == 2) taskMenu();
        else if (ch == 3) budgetMenu();
        else if (ch == 4) showReminders();
    } while (ch != 5);

    printf("\nGoodbye! Your data is saved.\n");
    return 0;
}
