#include <sys/ioctl.h>
#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <iomanip>
#include <ctype.h>
#include <locale.h>
#include <limits.h>
#include <fcntl.h>
#define max 50
using namespace std;

int num=0;
void gotoXY(int,int);
void Cdelay(int);
void border(int, int,int, int);
void borderNoDelay(int, int,int, int);
void loginFrame(int, int, int, int);
void intro();
void login();
void menu();
void insert();
void edit();
void editmenu();
void editname(int);
void editcode(int);
void editdes(int);
void editexp(int);
void editage(int);
void editsalary(int);
void list();
void deletes();
void search();
void setWindowSize(int,int);
void saverecords();
void getrecords();
bool isFilePresent();
void displayPayslip();
void log_message(const char* message);

// Update the terminal handling functions at the top
struct termios orig_termios;

void initTerminal() {
    // Save original terminal settings
    tcgetattr(STDIN_FILENO, &orig_termios);
    
    // Set up new terminal settings
    struct termios raw = orig_termios;
    raw.c_lflag &= ~(ICANON | ECHO);
    raw.c_cc[VMIN] = 1;
    raw.c_cc[VTIME] = 0;
    
    // Apply new settings
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
    
    // Enable alternative buffer and clear screen
    printf("\033[?1049h\033[H");
    fflush(stdout);
}

void resetTerminal() {
    // Restore original terminal settings
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios);
    
    // Return to main buffer and clear screen
    printf("\033[?1049l");
    fflush(stdout);
}

// Update linux_getch for better key handling
char linux_getch() {
    char c;
    read(STDIN_FILENO, &c, 1);
    
    // Handle special keys
    if (c == '\033') {
        char seq[3];
        
        // Read the rest of the escape sequence
        if (read(STDIN_FILENO, &seq[0], 1) != 1) return '\033';
        if (read(STDIN_FILENO, &seq[1], 1) != 1) return '\033';
        
        // Handle arrow keys and other special keys
        if (seq[0] == '[') {
            switch (seq[1]) {
                case 'A': return 'k'; // Up arrow
                case 'B': return 'j'; // Down arrow
                case 'C': return 'l'; // Right arrow
                case 'D': return 'h'; // Left arrow
            }
        }
        return '\033';
    }
    
    return c;
}

// Add this with the other function declarations at the top of the file
char linux_getche();

// Move the linux_getche definition before insert()
char linux_getche() {
    char c = linux_getch();
    putchar(c);
    fflush(stdout);
    return c;
}

class employee
{
public:
    char name[20];
    int code;
    char designation[20];
    int exp;
    int age;
    int salary;
    char AnyLoan;

    int HRA;
    int PF;
    int tax;
    int MealAllowance;
    int TransportAllowance;
    int MedicalAllowance;
    int LoanBalance;
    int LoanDebit;
    int grosspay;
    int workingHours;
    int DA;

};
employee emp[max],tempemp[max];


void getrecords()
{
    FILE *fp;
    fp = fopen("Records.txt","r");
    int c=0;
    if(fp!=NULL)
    {
        while(feof(fp)==0)
        {
            fscanf(fp,"%19[^\t]\t%d\t%19[^\t]\t%d\t%d\t%d\t%c\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\n",
                emp[c].name,
                &emp[c].code,
                emp[c].designation,
                &emp[c].exp,
                &emp[c].age,
                &emp[c].salary,
                &emp[c].AnyLoan,
                &emp[c].HRA,
                &emp[c].PF,
                &emp[c].tax,
                &emp[c].MealAllowance,
                &emp[c].TransportAllowance,
                &emp[c].MedicalAllowance,
                &emp[c].LoanBalance,
                &emp[c].LoanDebit,
                &emp[c].grosspay,
                &emp[c].workingHours,
                &emp[c].DA);
            c++;
        }
        num=c;
    }
    fclose(fp);
}

void saverecords()
{
    if(num == 0)
    {
        unlink("Records.txt");  // Use unlink instead of system("del")
    }
    else
    {
        FILE *fp;
        fp = fopen("Records.txt", "w");
        if(fp != NULL)
        {
            for(int i=0; i<num; i++)
            {
                fprintf(fp,"%s\t%d\t%s\t%d\t%d\t%d\t%c\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\n",
                    emp[i].name,emp[i].code,emp[i].designation,emp[i].exp,
                    emp[i].age,emp[i].salary,emp[i].AnyLoan,emp[i].HRA,
                    emp[i].PF,emp[i].tax,emp[i].MealAllowance,
                    emp[i].TransportAllowance,emp[i].MedicalAllowance,
                    emp[i].LoanBalance,emp[i].LoanDebit,emp[i].grosspay,
                    emp[i].workingHours,emp[i].DA);
            }
            fclose(fp);
        }
    }
}

void Cdelay(int msec)
{
    long goal = msec + (clock());
    while (goal > (clock()));
}

bool isFilePresent()
{
    FILE *fp;
    fp = fopen("Records.txt","r");
    if(fp==NULL)
        return false;
    else
        return true;
}
void loginFrame1(int xLenS = 18, int yLenS = 8, int xLenE = 55, int yLenE = 15)
{
    printf("\033[2J\033[H");  // Clear screen
    
    // Box corners
    gotoXY(xLenS,yLenS);printf("┌");
    gotoXY(xLenS,yLenE);printf("└");
    gotoXY(xLenE,yLenS);printf("┐");
    gotoXY(xLenE,yLenE);printf("┘");

    // Top and Bottom borders
    for(int i=xLenS+1; i<=xLenE-1; i++)
    {
        gotoXY(i,yLenS);
        printf("─");
        gotoXY(i,yLenE);
        printf("─");
    }

    // Left and Right borders
    for(int i=yLenS+1; i<=yLenE-1; i++)
    {
        gotoXY(xLenS, i);
        printf("│");
        gotoXY(xLenE, i);
        printf("│");
    }
    
    // Add title
    gotoXY((xLenS + xLenE)/2 - 10, yLenS + 1);
    printf("LOGIN SYSTEM");
    
    fflush(stdout);
}
void login()
{
    char UserName[30], Password[30];
    int i = 0;
    
    printf("\033[2J\033[H");
    loginFrame1();
    
    // Username input
    gotoXY(20,10);
    printf("Enter UserName : ");
    fflush(stdout);
    
    // Read username character by character
    i = 0;
    while(1) {
        char c = linux_getch();
        if(c == '\n' || c == '\r') {
            UserName[i] = '\0';
            break;
        }
        if(c == 127 || c == 8) { // Backspace
            if(i > 0) {
                i--;
                printf("\b \b");
                fflush(stdout);
            }
            continue;
        }
        if(i < 29 && isprint(c)) {
            UserName[i++] = c;
            printf("%c", c);
            fflush(stdout);
        }
    }
    
    // Password input
    gotoXY(20,12);
    printf("Enter Password : ");
    fflush(stdout);
    
    // Read password character by character
    i = 0;
    while(1) {
        char c = linux_getch();
        if(c == '\n' || c == '\r') {
            Password[i] = '\0';
            break;
        }
        if(c == 127 || c == 8) { // Backspace
            if(i > 0) {
                i--;
                printf("\b \b");
                fflush(stdout);
            }
            continue;
        }
        if(i < 29 && isprint(c)) {
            Password[i++] = c;
            printf("*");
            fflush(stdout);
        }
    }
    
    // Rest of the login function remains the same
    if(strcmp(UserName,"admin")==0 && strcmp(Password,"admin")==0)
    {
        printf("\033[2J\033[H");
        loginFrame1();
        gotoXY(27,10);
        cout<<"Login Success!!!";
        gotoXY(21,12);
        cout<<"Will be redirected in 3 Seconds...";
        gotoXY(56,12);
        Cdelay(1000);
        gotoXY(44,12);
        cout<<"\b \b2";
        gotoXY(56,12);
        Cdelay(1000);
        gotoXY(44,12);
        cout<<"\b \b1";
        gotoXY(56,12);
        Cdelay(1000);
    }
    else
    {
        printf("\033[2J\033[H");
        loginFrame1();
        gotoXY(27,10);
        printf("Access Denied!!!\a");
        gotoXY(21,12);
        cout<<"Will be redirected in 3 Seconds...";
        gotoXY(56,12);
        Cdelay(1000);
        gotoXY(44,12);
        cout<<"\b \b2";
        gotoXY(56,12);
        Cdelay(1000);
        gotoXY(44,12);
        cout<<"\b \b1";
        gotoXY(56,12);
        Cdelay(1000);
        printf("\033[2J\033[H");
        loginFrame1();
        login();
    }
}
void setWindowSize()
{
    // Clear screen
    printf("\033[2J\033[H");
    // Set terminal size (if supported)
    printf("\033[8;24;80t");
    fflush(stdout);
}
void gotoXY(int x, int y) 
{
    printf("\033[%d;%dH", y+1, x+1);
    fflush(stdout);  // Force output to ensure cursor position is updated
}

void borderNoDelay(int xLenS = 2, int yLenS = 2,int xLenE = 76, int yLenE = 24 )
{
    printf("\033[2J\033[H");
    
    // Box corners
    gotoXY(xLenS,yLenS);printf("┌");
    gotoXY(xLenS,yLenE);printf("└");
    gotoXY(xLenE,yLenS);printf("┐");
    gotoXY(xLenE,yLenE);printf("┘");

    // Top and Bottom Border lines
    for(int i=xLenS+1; i<=xLenE-1; i++)
    {
        gotoXY(i,yLenS);
        printf("─");
        gotoXY(i,yLenE);
        printf("─");
    }

    // Left and Right Border Lines
    for(int i=yLenS+1; i<=yLenE-1; i++)
    {
        gotoXY(xLenS, i);
        printf("│");
        gotoXY(xLenE, i);
        printf("│");
    }
    fflush(stdout);
}
void border(int xLenS = 2, int yLenS = 2,int xLenE = 76, int yLenE = 22 )
{
    printf("\033[2J\033[H");  // Clear screen
    
    // Box corners
    gotoXY(xLenS,yLenS);printf("┌");    // Top left (was 201)
    gotoXY(xLenS,yLenE);printf("└");    // Bottom left (was 200)
    gotoXY(xLenE,yLenS);printf("┐");    // Top right (was 187)
    gotoXY(xLenE,yLenE);printf("┘");    // Bottom right (was 188)

    // Top and Bottom Border lines
    for(int i=xLenS+1; i<=xLenE-1; i++)
    {
        Cdelay(15);
        gotoXY(i,yLenS);
        printf("─");          // Horizontal line (was 205)
        gotoXY(i,yLenE);
        printf("─");
    }

    // Left and Right Border Lines
    for(int i=yLenS+1; i<=yLenE-1; i++)
    {
        Cdelay(15);
        gotoXY(xLenS, i);
        printf("│");          // Vertical line (was 186)
        gotoXY(xLenE, i);
        printf("│");
    }
    printf("\n\n");
}
void loginFrame(int xLenS = 18, int yLenS = 8, int xLenE = 55, int yLenE = 15)
{
    border(xLenS,yLenS,xLenE,yLenE);
}
void insert()
{
    log_message("Starting insert() function");
    borderNoDelay();
    int i = num;
    int sal, PF, TAX, HRA, MealA, MedicalA, TransportA, LoanBal, LoanDeb, h, DA;
    char loan;
    char log_buf[100];  // Buffer for log messages
    
    // Clear any pending input
    tcflush(STDIN_FILENO, TCIFLUSH);
    log_message("Cleared input buffer");
    
    num += 1;
    
    gotoXY(28,4);
    printf("Insert New Record");
    fflush(stdout);
    
    // Name input with logging
    gotoXY(10,6);
    printf("Name : ");
    fflush(stdout);
    log_message("Prompting for name input");
    
    char name_buf[20];
    int pos = 0;
    while(1) {
        char c = linux_getch();
        sprintf(log_buf, "Received character for name: ASCII=%d, Char='%c'", (int)c, isprint(c) ? c : ' ');
        log_message(log_buf);
        
        if(c == '\n' || c == '\r') {
            if(pos > 0) {
                name_buf[pos] = '\0';
                strcpy(emp[i].name, name_buf);
                log_message("Name input complete");
                break;
            }
            log_message("Empty name input - waiting for valid input");
            continue;
        }
        
        if(c == 127 || c == 8) { // Backspace
            if(pos > 0) {
                pos--;
                printf("\b \b");
                fflush(stdout);
                log_message("Backspace processed");
            }
            continue;
        }
        
        if(pos < 19 && isprint(c)) {
            name_buf[pos++] = c;
            printf("%c", c);
            fflush(stdout);
            sprintf(log_buf, "Added character to name: %c", c);
            log_message(log_buf);
        }
    }
    printf("\n");
    fflush(stdout);
    
    // Code input with improved logging and handling
    gotoXY(10,8);
    printf("Code : ");
    fflush(stdout);
    log_message("Prompting for code input");
    
    char code_buf[6];  // Buffer for code input
    pos = 0;
    bool showMessage = false;
    
    while(1) {
        if (showMessage) {
            gotoXY(30,8);
            printf("Please enter numbers only!");
            fflush(stdout);
        }
        
        char c = linux_getch();
        sprintf(log_buf, "Received character for code: ASCII=%d, Char='%c', isdigit=%d", 
                (int)c, isprint(c) ? c : ' ', isdigit(c));
        log_message(log_buf);
        
        if(c == '\n' || c == '\r') {
            if(pos > 0) {
                code_buf[pos] = '\0';
                emp[i].code = atoi(code_buf);
                log_message("Code input complete");
                break;
            }
            log_message("Empty code input - waiting for valid input");
            continue;
        }
        
        if(c == 127 || c == 8) { // Backspace
            if(pos > 0) {
                pos--;
                printf("\b \b");
                // Clear any error message
                gotoXY(30,8);
                printf("                          ");
                gotoXY(16+pos,8);  // Return to input position
                fflush(stdout);
                sprintf(log_buf, "Backspace processed, current pos=%d", pos);
                log_message(log_buf);
                showMessage = false;
            }
            continue;
        }
        
        if(pos < 5 && isdigit(c)) {
            code_buf[pos++] = c;
            printf("%c", c);
            // Clear any error message
            gotoXY(30,8);
            printf("                          ");
            gotoXY(16+pos,8);  // Return to input position
            fflush(stdout);
            sprintf(log_buf, "Added digit to code: %c, current pos=%d", c, pos);
            log_message(log_buf);
            showMessage = false;
        } else {
            sprintf(log_buf, "Ignored non-digit character: ASCII=%d", (int)c);
            log_message(log_buf);
            showMessage = true;
            // Make a beep sound for invalid input
            printf("\a");
            fflush(stdout);
        }
    }
    
    // Clear any remaining error message
    gotoXY(30,8);
    printf("                          ");
    printf("\n");
    fflush(stdout);
    
    // Designation input
    gotoXY(10,10);
    printf("Designation : ");
    fflush(stdout);
    log_message("Prompting for designation");
    
    char des_buf[20];
    pos = 0;
    while(1) {
        char c = linux_getch();
        sprintf(log_buf, "Received character for designation: ASCII=%d, Char='%c'", (int)c, isprint(c) ? c : ' ');
        log_message(log_buf);
        
        if(c == '\n' || c == '\r') {
            if(pos > 0) {
                des_buf[pos] = '\0';
                strcpy(emp[i].designation, des_buf);
                log_message("Designation input complete");
                break;
            }
            continue;
        }
        
        if(c == 127 || c == 8) { // Backspace
            if(pos > 0) {
                pos--;
                printf("\b \b");
                fflush(stdout);
            }
            continue;
        }
        
        if(pos < 19 && isprint(c)) {
            des_buf[pos++] = c;
            printf("%c", c);
            fflush(stdout);
        }
    }
    printf("\n");
    fflush(stdout);

    // Experience input
    gotoXY(10,12);
    printf("Years of Experience : ");
    fflush(stdout);
    log_message("Prompting for experience");
    
    char exp_buf[3];
    pos = 0;
    while(1) {
        char c = linux_getch();
        if(c == '\n' || c == '\r') {
            if(pos > 0) {
                exp_buf[pos] = '\0';
                emp[i].exp = atoi(exp_buf);
                break;
            }
            continue;
        }
        
        if(c == 127 || c == 8) { // Backspace
            if(pos > 0) {
                pos--;
                printf("\b \b");
                fflush(stdout);
            }
            continue;
        }
        
        if(pos < 2 && isdigit(c)) {
            exp_buf[pos++] = c;
            printf("%c", c);
            fflush(stdout);
        }
    }
    printf("\n");
    fflush(stdout);

    // Age input
    gotoXY(10,14);
    printf("Age : ");
    fflush(stdout);
    log_message("Prompting for age");
    
    char age_buf[3];
    pos = 0;
    while(1) {
        char c = linux_getch();
        if(c == '\n' || c == '\r') {
            if(pos > 0) {
                age_buf[pos] = '\0';
                emp[i].age = atoi(age_buf);
                break;
            }
            continue;
        }
        
        if(c == 127 || c == 8) { // Backspace
            if(pos > 0) {
                pos--;
                printf("\b \b");
                fflush(stdout);
            }
            continue;
        }
        
        if(pos < 2 && isdigit(c)) {
            age_buf[pos++] = c;
            printf("%c", c);
            fflush(stdout);
        }
    }
    printf("\n");
    fflush(stdout);

    // Working Hours input
    gotoXY(10,16);
    printf("Enter Working Hours : ");
    fflush(stdout);
    log_message("Prompting for working hours");
    
    char hours_buf[4];
    pos = 0;
    while(1) {
        char c = linux_getch();
        if(c == '\n' || c == '\r') {
            if(pos > 0) {
                hours_buf[pos] = '\0';
                h = atoi(hours_buf);
                break;
            }
            continue;
        }
        
        if(c == 127 || c == 8) { // Backspace
            if(pos > 0) {
                pos--;
                printf("\b \b");
                fflush(stdout);
            }
            continue;
        }
        
        if(pos < 3 && isdigit(c)) {
            hours_buf[pos++] = c;
            printf("%c", c);
            fflush(stdout);
        }
    }
    printf("\n");
    fflush(stdout);

    sal = h * 300;
    emp[i].workingHours = h;

    // Loan input
    do {
        gotoXY(10,18);
        printf("Any Loan (Y/N)? : ");
        fflush(stdout);
        loan = linux_getch();
        printf("%c", loan);
        fflush(stdout);
    } while(loan != 'Y' && loan != 'y' && loan != 'N' && loan != 'n');
    printf("\n");
    fflush(stdout);

    if(loan == 'y' || loan == 'Y') {
        gotoXY(10,20);
        printf("Enter Loan Balance : ");
        fflush(stdout);
        
        char loan_buf[7];
        pos = 0;
        while(1) {
            char c = linux_getch();
            if(c == '\n' || c == '\r') {
                if(pos > 0) {
                    loan_buf[pos] = '\0';
                    LoanBal = atoi(loan_buf);
                    break;
                }
                continue;
            }
            
            if(c == 127 || c == 8) { // Backspace
                if(pos > 0) {
                    pos--;
                    printf("\b \b");
                    fflush(stdout);
                }
                continue;
            }
            
            if(pos < 6 && isdigit(c)) {
                loan_buf[pos++] = c;
                printf("%c", c);
                fflush(stdout);
            }
        }
    } else {
        LoanBal = 0;
    }

    // Calculate all allowances and deductions
    TAX = 0.04 * sal;
    DA = 1.20 * sal;
    PF = 0.12 * sal;
    HRA = sal * 0.27;
    MealA = 300;
    MedicalA = 300;
    TransportA = 300;
    LoanDeb = sal * 0.09;

    if(LoanDeb > LoanBal) {
        LoanBal = 0;
        LoanDeb = LoanBal;
    }

    // Store all calculated values
    emp[i].salary = sal;
    emp[i].DA = DA;
    emp[i].tax = TAX;
    emp[i].PF = PF;
    emp[i].HRA = HRA;
    emp[i].MealAllowance = MealA;
    emp[i].MedicalAllowance = MedicalA;
    emp[i].TransportAllowance = TransportA;
    emp[i].LoanBalance = LoanBal - LoanDeb;
    emp[i].AnyLoan = loan;
    emp[i].LoanDebit = LoanDeb;
    emp[i].grosspay = (sal + MealA + MedicalA + TransportA + HRA + DA) - (PF + TAX + LoanDeb);

    gotoXY(14,22);
    printf("Record Successfully Added!");
    fflush(stdout);
    log_message("Record added successfully");

    linux_getch();
}

void intro()
{
    border();  // Draw the main border first
    
    gotoXY(27,4); 
    printf("\033[1m");  // Bold text
    printf("PAYROLL MANAGEMENT SYSTEM");
    printf("\033[0m");  // Reset text formatting
    
    gotoXY(20,8); 
    printf("Welcome to the Payroll Management System");
    
    gotoXY(20,11);
    printf("This system helps you manage employee payroll");
    
    gotoXY(20,13);
    printf("Developed by: Your Name");
    
    gotoXY(20,15);
    printf("Version 1.0");
    
    gotoXY(24,20);
    printf("Press Any key to continue...");
    
    fflush(stdout);  // Ensure all text is displayed
    linux_getch();
}

void  list()
{
    //system("cls");
    borderNoDelay();
    gotoXY(20,4);
    printf("       ******** List of the Employees ********");
    gotoXY(6,6);
    cout<<"Name\tCode\tDesignation\tYears(EXP)\tAge\tSalary "<<endl;
    gotoXY(6,7);
    cout<<"------------------------------------------------------------------"<<endl;
    for(int i=0,j=8;i<=num-1;i++,j++)
    {
        gotoXY(6,j);
        cout<<emp[i].name;
        gotoXY(19,j);
        cout<<emp[i].code;
        gotoXY(26,j);
        cout<<emp[i].designation;
        gotoXY(47,j);
        cout<<emp[i].exp;
        gotoXY(58,j);
        cout<<emp[i].age;
        gotoXY(66,j);
        cout<<emp[i].grosspay;
    }
    linux_getch();
}

void loading()
{
    printf("\033[2J\033[H");
    border();
    gotoXY(55,20);
    printf("Loading...");
    gotoXY(50,22);
    for(int i = 0; i < 20; i++)
    {
        Cdelay(200);
        printf("█");  // Using block character for loading bar
        fflush(stdout);
    }
}
void menu()
{
    borderNoDelay();
    gotoXY(16,4);
    printf("*****  Payroll Management System 1.0 ***** ");
    gotoXY(12,6);
    cout<<"Press  a ----> Add New Record.";
    gotoXY(12,8);
    cout<<"Press  u ----> Update a Record.";
    gotoXY(12,10);
    cout<<"Press  d ----> Delete a Record.";
    gotoXY(12,12);
    cout<<"Press  s ----> Search a Record.";
    gotoXY(12,14);
    cout<<"Press  l ----> List The Employee Table.";
    gotoXY(12,16);
    cout<<"Press  p ----> Print Employee PaySlip.";
    gotoXY(12,18);
    cout<<"Press  q ----> Quit Program.";
    gotoXY(16,22);
    cout<<"Select Your Option ====> ";
    fflush(stdout);
}

void deletes()
{
    for(int i=0;i<num;i++)
    {
        tempemp[i]=emp[i];
    }
    //system("cls");
    borderNoDelay();
    int code;
    int check=-1;
    gotoXY(28,4);
    printf("Delete a Record");
    gotoXY(10,6);
    cout<<"Enter the JobCode To Delete That Record  :";
    cin>>code;
    int i,j;
    for(i=0;i<=num-1;i++)
    {
         if(emp[i].code==code)
        {
            check=i;
        }
    }
    if(check!=-1)
    {
        for(i=0,j=0;i<=num-1;i++,j++)
        {
            if(i==check)
            {
                i++;
            }
            emp[j]=tempemp[i];
        }
        num--;
    }
}

void search()
{
    //system("cls");
    borderNoDelay();
    int jobcode;
    bool found = false;
    gotoXY(10,4);
    cout<<"You can Search only through the Jobcode of an Employee";
    gotoXY(10,6);
    cout<<"Enter Code Of the Employee : ";
    cin>>jobcode;
    for(int i=0;i<=num-1;i++)
    {
        if(emp[i].code==jobcode)
        {
            gotoXY(6,8);
            cout<<"Name\tCode\tDesignation\tYears(EXP)\tAge\tSalary "<<endl;
            gotoXY(6,9);
            cout<<"------------------------------------------------------------------"<<endl;
            gotoXY(6,11);
            cout<<emp[i].name;
            gotoXY(19,11);
            cout<<emp[i].code;
            gotoXY(26,11);
            cout<<emp[i].designation;
            gotoXY(47,11);
            cout<<emp[i].exp;
            gotoXY(58,11);
            cout<<emp[i].age;
            gotoXY(66,11);
            cout<<emp[i].grosspay;
            found = true;
        }
        //else
        //
    }
    if(!found)
    {
        gotoXY(26,11);
        cout<<"No records Found...!!!\a";
    }
    linux_getch();
}



void editmenu()
{
    //system("cls");
    borderNoDelay();
    gotoXY(28,4);
    printf("Edit An Entry");
    gotoXY(10,6);
    cout<<"What Do You Want To edit";
    gotoXY(12,8);
    cout<<"n ---------> Name ";
    gotoXY(12,9);
    cout<<"c ---------> Code ";
    gotoXY(12,10);
    cout<<"d ---------> Designation";
    gotoXY(12,11);
    cout<<"e ---------> Experience ";
    gotoXY(12,12);
    cout<<"a ---------> Age";
    gotoXY(12,13);
    cout<<"s ---------> Salary";
    gotoXY(12,14);
    cout<<"q ---------> QUIT";
    gotoXY(10,16);
    cout<<"Enter Choice ---->>>  ";
}

void editname(int i)
{
    gotoXY(10,18);
    cout<<"Enter New Name----->  ";
    cin>>emp[i].name;
}

void editcode(int i)
{
    gotoXY(10,18);
    cout<<"Enter New Job Code----->  ";
    cin>>emp[i].code;
}
void editdes(int i)
{
    gotoXY(10,18);
    cout<<"enter new designation----->  ";
    cin>>emp[i].designation;
}

void editexp(int i)
{
    gotoXY(10,18);
    cout<<"Enter new Years of Experience";
    cin>>emp[i].exp;
}
void editage(int i)
{
    gotoXY(10,18);
    cout<<"Enter new Age ";
    cin>>emp[i].age;
}

void editsalary(int i)
{
    int sal,PF,TAX,HRA,MealA,MedicalA,TransportA,LoanBal=emp[i].LoanBalance,LoanDeb,DA;
    char loan;
    gotoXY(10,18);
    cout<<"Enter new Salary ";
    cin>>sal;
    DA = 1.20 * sal;
    TAX =  0.04 * sal;
    PF = 0.12 * sal;
    HRA = 4000;
    MealA = 300;
    MedicalA = 300;
    TransportA = 300;
    LoanDeb = sal * 0.09;
    if(LoanDeb > LoanBal)
    {
        LoanBal = 0;
        LoanDeb = LoanBal;
    }
    emp[i].salary = sal;
    emp[i].tax=TAX;
    emp[i].PF = PF;
    emp[i].HRA = HRA;
    emp[i].MealAllowance = MealA;
    emp[i].MedicalAllowance = MedicalA;
    emp[i].TransportAllowance = TransportA;
    emp[i].LoanBalance = LoanBal;
    emp[i].AnyLoan = loan;
    emp[i].LoanDebit = LoanDeb;
    emp[i].grosspay = (sal+MealA+MedicalA+TransportA+HRA+DA)-(PF+TAX+LoanDeb) ;
}

void edit()
{
    //system("cls");
    borderNoDelay();
    int jobcode;
    gotoXY(28,4);
    printf("Edit a Record");
    int i;
    char option;
    gotoXY(10,6);
    cout<<"Enter the jobcode To Edit : ";
    cin>>jobcode;
    editmenu();
    for(i=0;i<=num-1;i++)
    {
        if(emp[i].code==jobcode)
        {
            while((option=linux_getch())!='q')
            {
                switch(option)
                {
                    case 'n':
                        editname(i);
                        break;
                    case 'c':
                        editcode(i);
                        break;
                    case 'd':
                        editdes(i);
                        break;
                    case 'e':
                        editexp(i);
                        break;
                    case 'a':
                        editage(i);
                        break;
                    case 's':
                        editsalary(i);
                        break;
                }
                   editmenu();
            }
        }
    }
}


void displayPayslip()
{
    printf("\033[2J\033[H");
    borderNoDelay();
    gotoXY(10,4);
    int code,i;
    cout<<"Enter Employee Job Code :";
    cin>>code;
    for(i=0;i<=num-1;i++)
    {
        if(emp[i].code==code)
        {
            gotoXY(12,6);
            cout<<"Name : "<<emp[i].name;
            gotoXY(45,6);
            cout<<"Working Hours : "<<emp[i].workingHours<<" Hrs";
            gotoXY(6,8);
            cout<<"Earnings :-";
            gotoXY(8,10);
            cout<<"Basic Pay : "<<emp[i].salary<<endl;
            gotoXY(8,12);
            cout<<"HRA(27% of Basic): "<<emp[i].HRA<<endl;
            gotoXY(8,14);
            cout<<"DA (120% of Basic):"<<emp[i].DA;
            gotoXY(8,16);
            cout<<"Meal Allowance : "<<emp[i].MealAllowance<<endl;
            gotoXY(8,18);
            cout<<"Medical Alowance : "<<emp[i].MedicalAllowance<<endl;
            gotoXY(8,20);
            cout<<"Transport Allowance : "<<emp[i].TransportAllowance<<endl;
            gotoXY(40,8);
            cout<<"Deductions :- "<<endl<<endl;
            gotoXY(42,10);
            cout<<"PF : "<<emp[i].PF<<endl;
            gotoXY(42,12);
            cout<<"Tax : "<<emp[i].tax<<endl;
            gotoXY(42,14);
            int l = emp[i].AnyLoan;
            char l2 = toupper(l);
            cout<<"Loan Status : "<<l2<<endl;
            gotoXY(42,16);
            cout<<"Loan Debit This Month : "<<emp[i].LoanDebit<<endl;
            gotoXY(42,18);
            cout<<"Loan Balance : "<<emp[i].LoanBalance<<endl;
            gotoXY(32,22);
            cout<<"Total Gross Pay : "<<emp[i].grosspay;
        }
    }
    linux_getch();
}

// Add this function definition
void log_message(const char* message) {
    FILE *log_file = fopen("payroll.log", "a");
    if(log_file != NULL) {
        time_t now = time(NULL);
        char timestamp[26];
        ctime_r(&now, timestamp);
        timestamp[24] = '\0';  // Remove newline
        fprintf(log_file, "[%s] %s\n", timestamp, message);
        fclose(log_file);
    }
}

int main()
{
    try {
        log_message("Program started");
        
        // Initialize terminal and locale
        setlocale(LC_ALL, "");
        initTerminal();
        log_message("Terminal initialized");
        
        // Load existing records
        if(isFilePresent()) {
            getrecords();
            if(emp[0].code == 0) {
                num--;
            }
        }
        
        // Main program sequence
        border();
        intro();
        loading();
        loginFrame();
        login();
        
        // Main program loop
        while(1) {
            menu();
            char option = linux_getch();
            
            // Convert to lowercase for consistency
            option = tolower(option);
            
            switch(option) {
                case 'l':
                    list();
                    break;
                case 'a':
                    insert();
                    break;
                case 'd':
                    deletes();
                    break;
                case 'u':
                    edit();
                    break;
                case 's':
                    search();
                    break;
                case 'p':
                    displayPayslip();
                    break;
                case 'q':
                    resetTerminal();
                    saverecords();
                    return 0;
                default:
                    // Ignore invalid input
                    continue;
            }
        }
    }
    catch(...) {
        log_message("Program crashed with an exception");
        resetTerminal();
        printf("An error occurred. Terminal has been reset.\n");
        return 1;
    }
    
    log_message("Program ended normally");
    return 0;
}
