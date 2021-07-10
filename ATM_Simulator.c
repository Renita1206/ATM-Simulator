#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

int accounts=0;

typedef struct client_details
{
	char name[100];
	int pass;
	double baln;
	char baln_stat[100];
	char mobile[100];
}client_t;

void displayMenu();
void print_to_file(client_t*);
int login(client_t*,char*,int);
void moneyDeposit(client_t* data,int);
void moneyWithdraw(client_t* data,int);
void checkBal(client_t*,int);
void pin_change(client_t* data,int);
void mobile_change(client_t* data,int);
void fast_cash_stat(client_t*,int,double);
void fast_cash(client_t*,int);
void balance_enquiry(client_t*,int);

void displayMenu()
{
    printf("---------------- ATM Simulator -----------------\n");
    printf("---- Please choose one of the options below ----\n");
    printf("1)Balance Statement\t2)Deposit\n3)Withdraw Money\t4)Fast Cash\n5)Change Mobile Number\t6)Change Pin\n7)Balance enquiry\n");
}

void print_to_file(client_t* data)
{
	FILE * pfile;
	pfile = fopen("details.txt","w");
	if (pfile==NULL)
		printf("Error in opening the file\n");
	else
	{
		for(int i=0;i<accounts;i++)
		{
			fprintf(pfile,"%s\t%d\t%.3lf\t%s\t%s\n",(data+i)->name,(data+i)->pass,(data+i)->baln,(data+i)->mobile,(data+i)->baln_stat);
		}
	}
	fclose(pfile);
}

int login(client_t* data,char* name1,int pass1)
{
	for(int i=0;i<100;i++)
	{
		if((pass1==(data+i)->pass) && (strcmp(name1,(data+i)->name)==0))
			return i+1;
	}
	return 0;
}

void checkBal(client_t* data,int i)
{
	printf("Account Details - \n");
    printf("Account Name    : %s \n",(data+i)->name);
    printf("Account Number  : %s \n",(data+i)->mobile);
    printf("Account Balance : %.3lf \n",(data+i)->baln);
	FILE *f;
	char s;
	f = fopen((data+i)->baln_stat,"r");
	while((s=fgetc(f))!=EOF)
		printf("%c",s);
	printf("\n\n");
	fclose(f);
}

void moneyDeposit(client_t* data,int i) 
{
    double deposit;
    printf("You choose to Deposit money\n");
    printf("Enter your amount to Deposit\n");
    scanf("%lf", &deposit);
    (data+i)->baln += deposit;
	print_to_file(data);
	FILE * cfile;
	cfile = fopen((data+i)->baln_stat,"a");
	time_t t; // time t gives current time of system
    time(&t);
    char m[1000]=" ";
    strncat(m,ctime(&t),24); // ctime to get string representation
	fprintf(cfile,"%s\t\tDeposit\t\t\t%.3lf\t\t\t%.3lf\n",m,deposit,(data+i)->baln);
	fclose(cfile);
}

void moneyWithdraw(client_t* data,int i) 
{
    double withdraw;
    bool back = true;
    printf("You choose to Withdraw money\n");
    while (back) 
	{
		printf("Enter your amount to withdraw:\n");
		scanf("%lf", &withdraw);
		if (withdraw < (data+i)->baln) 
		{
			back = false;
			(data+i)->baln -= withdraw;
			print_to_file(data);
			FILE * cfile;
			cfile = fopen((data+i)->baln_stat,"a");
			time_t t; // time t gives current time of system
			time(&t);
			char m[1000]=" ";
			strncat(m,ctime(&t),24); // ctime to get string representation
			fprintf(cfile,"%s\t\tWithdraw\t\t%.3lf\t\t\t%.3lf\n",m,withdraw,(data+i)->baln);
			fclose(cfile);
		}
		else  
		{
			printf("You don't have enough money\n");
			printf("Please contact to your Bank Customer Services\n");
		}
    }
}

void pin_change(client_t* data,int i)
{
	int enteredPin,confirmPin;
	printf("Enter the new pin:");
	scanf("%d",&enteredPin);
	printf("\nConfirm the pin: ");
	scanf("%d",&confirmPin);
	char s[10];
	itoa(confirmPin,s,10);
	int l=strlen(s);
	if(l==4)
	{
		if (enteredPin == confirmPin)
		{
			(data+i)->pass = confirmPin;
			print_to_file(data);
			printf("\nYour new pin is %d",confirmPin);
		}
		else
		{
			printf("Pins don't match. Please try again.");
		}
	}
	else
		printf("Please enter a four digit number");
}

void mobile_change(client_t* data,int i)
{
	char n1[100],n2[100];
	printf("Enter the new number:");
	scanf("%s",&n1);
	printf("\nConfirm the number: ");
	scanf("%s",&n2);
	int l=strlen(n1);
	int check=0;
	for(int j=0;j<10;j++)
	{
		if(isdigit(n1[j])!=0)
			check=1;
		else
		{
			check=0;
			break;
		}
	}
	if(l==10  && check==1)
	{
		if (strcmp(n1,n2)==0)
		{
			for(int j=0;j<10;j++)
			{
				((data+i)->mobile)[j]=n2[j];
			}
			print_to_file(data);
			printf("\nYour new number is %s",n2);
		}
		else
		{
			printf("Numbers don't match. Please try again.");
		}
	}
	else
		printf("Please enter a valid phone number");
}

void fast_cash(client_t* data,int i)
{
	printf("1)500\t2)1000\t3)2000\n4)5000\t5)10000\n");
	printf("Enter your option\n");
	int n;
	scanf("%d",&n);
	double amt;

	switch(n)
	{
		case 1 : fast_cash_stat(data,i,500);
				 break;
		
		case 2 : fast_cash_stat(data,i,1000);
				 break;

		case 3 : fast_cash_stat(data,i,2000);
				 break;
		
		case 4 : fast_cash_stat(data,i,5000);
				 break;
		
		case 5 : fast_cash_stat(data,i,10000);
				 break;

		default : printf("Enter a valid option\n");
				  break;
		
	}
}

void fast_cash_stat(client_t* data,int i,double amt)
{
	if ((data+i)->baln > amt)
	{
		int acc=sizeof(data)/sizeof(*data);
		(data+i)->baln -= amt;
		printf(" %.3lf has been withdrawn\n",amt);
		print_to_file(data);
		FILE * cfile;
		cfile = fopen((data+i)->baln_stat,"a");
		time_t t; 
    		time(&t);
   		char m[1000]=" ";
   		strncat(m,ctime(&t),24); 
		fprintf(cfile,"%s\t\tWithdraw\t\t%.3lf\t\t\t%.3lf\n",m,amt,(data+i)->baln);
		fclose(cfile);
	}
	else
		printf("Insufficient balance\n");
}

void balance_enquiry(client_t* data,int i)
{
	printf("Your available balance is %.3lf\n",(data+i)->baln);

}


int main()
{
	client_t data[100];
	FILE* ptr = fopen("details.txt","r");
	int i=0; 
	while(fscanf(ptr,"%s %d %lf %s %s",(data+i)->name,&(data+i)->pass,&(data+i)->baln,(data+i)->mobile,(data+i)->baln_stat)!=EOF)
	{
		i=i+1;
	}
	accounts=i; //accounts to hold number of accounts
	fclose(ptr);

	char name1[100];
	int pass1;
	
	print_to_file(data);

	while(1)//Start application
	{
		printf("Enter the username: ");
		scanf("%s",name1);
		printf("\nEnter the password: ");
		scanf("%d",&pass1);
		
		int res = login(data,name1,pass1);
		int i = res-1;
		if (res)
		{
			printf("\nLogged in successfully\n");
			while(1)
			{
				char ch;
				printf("\nDo you want to continue?(y/n): ");
				scanf("%c",&ch);
				scanf("%c",&ch);
				system("CLS");
			
				if(ch == 'y')
				{
					int option=0;
					displayMenu();
					scanf("%d",&option);
					if(option==0)
						scanf("%c",&ch);
					switch (option) 
					{
						case 1: system("CLS");
								checkBal(data,i);
								break;
							
						case 2:	system("CLS");
								moneyDeposit(data,i);
								printf("Your available balance is: %.3lf\n",(data+i)->baln);
								break;

						case 3:	system("CLS");
								moneyWithdraw(data,i);
								printf("Your available balance is: %.3lf\n",(data+i)->baln);
								break;

						case 4:	system("CLS");
								fast_cash(data,i);
								break;

						case 5:	system("CLS");
								mobile_change(data,i);
								break;
						
						case 6:	system("CLS");
								pin_change(data,i);
								break;

						case 7:	system("CLS");
								balance_enquiry(data,i);
								break;
						
						default:printf("Invalid option. Please try again\n");
								break;
					}//switch

				}//if(ch == 'y')

				else if(ch == 'n')
				{	
					printf("\nLogged out");
					break;
				}
				
			}//inner while loop
		}//if (res)
		else
			printf("Invaid username or password\n");

		char inp;
		printf("\nDo you want to close the application?(y/n): ");
		scanf("%c",&inp);
		scanf("%c",&inp);
		if(inp=='y')
		{
			break;
		}
		system("CLS");
	}//outer while loop
	
	return 0;
}

