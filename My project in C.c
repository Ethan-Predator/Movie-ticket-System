#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<ctype.h>
#include<time.h>
#include <malloc.h>
#define MAXCHAR 30
#define MAXREC 50
#define TRUE 1
#define FALSE 0
#define MEMOFREC 9
#define FILENAME "movie.txt"
struct movie{
	 char MBN[MAXCHAR], NoC[MAXCHAR], NoM[MAXCHAR];  // MBN==Movie Booking Number NoC==Name of Customer NoM==Name of Movie 
	 char MS[MAXCHAR], Time[MAXCHAR], NoG[MAXCHAR];  // MS==Movie Schedule NoG==Number of Guests
	 char HN[MAXCHAR], TT[MAXCHAR], TF[MAXCHAR];     // HN==House Number TT==Ticket Type TF==Total Fee
};
void TitMenu();                              // Title and Menu
int isValidData(char [], int);               // check for data validation
void UpFile(struct movie []);                // used to update the file
void ReadinStruct();                         // to read the file data into struct movie array for option 3,4,5
int addcount();                              // to return how many records in "movie.txt" file, if this file is not existing, then creat on and return 0
int seatreccheck(const char *,int,int, int );// to check if the seat was occupied                           
void Add();                                  // Add New Movie Ticketing Record<s>
void Display();                              // Display All Movie Ticketing Records
void Modify();                               // Modify Movie Ticketing Record<s>
void Search();                               // Search Movie Ticketing Record<s>
void Delete();                               // Delete Movie Ticketing Record<s>
void ChoDisSeat();                           // Choose(by human or at random) and Display Seats Record<s>(10*15)
void movie_print();
void movie_review();
FILE *in, *out;
struct movie PerInfo[MAXREC];
int add;                                      // A viriable reflected how many records in "movie.txt" file
int main()
{
	int num;
	int end=TRUE;
	do{
		fflush(stdin);
	    system("cls");                      // empty the screen
	    TitMenu();
	    scanf("%d",&num);
	    switch(num){	
		    case 1: Add(); break;
		    case 2: Display(); break;
		    case 3: Modify(); break;
		    case 4: Search(); break;
		    case 5: Delete(); break;
		    case 6: ChoDisSeat(); break;
		    case 8: movie_print(); break;
		    default : printf(" \nYou end this system successfully!"); end=FALSE; break; 
	    }
	}while(end);
	return 0;
}
void TitMenu()
{
	int i;
	char tit[2][100]={"    ***  Welcome to HK GRAND SPACE Movie Ticketing system 2017   *** ",
	                  "    *** This system is developed by CCIT4020 No.CL-06 Group No.1 *** "};
	char fun[9][100]={" 1. Add New Movie Ticketing Record<s>: ",
	                  " 2. Display All Movie Ticketing Records: ",
					  " 3. Modify Movie Ticketing Record<s>: ",
					  " 4. Search Movie Ticketing Record<s>: ",
					  " 5. Delete Movie Ticketing Record<s>: ",
					  " 6. Display and Choose(by human or at random) the Video Hall and Seats Recoed<s>(10*10): ",
					  " 7. Check and input moive rating: ", 
					  " 8. Print your ticket: ",
					  " 9. Snack ordering system: "};
	for(i=0;i<=1;i++) tit[i][strlen(tit[i])-1]='\n';    //Every sentence is separate by two lines
	for(i=0;i<=8;i++) fun[i][strlen(fun[i])-1]='\n';
    puts(tit[0]);puts(tit[1]);printf("\n--<Basic funtion>--\n");
	puts(fun[0]);puts(fun[1]);puts(fun[2]);puts(fun[3]);puts(fun[4]);puts(fun[5]);puts(fun[6]);puts(fun[7]);puts(fun[8]);
    printf("What is your option <1-6>(others for ending this system)?");
}
int isValidData(char Data[MAXCHAR], int Datatype)  //Datetype: 1 for integer  2 for float  3 for character  4 for date  5 for movie booking number which can't be same as before  6 for time
{
	int i,j;
	int valid=TRUE;
	if(Data[0]=='\0') valid=FALSE; // check for an empty string
	if(valid==TRUE && Datatype==1){           //check for integer
		i=0;
		while(valid==TRUE && Data[i]){
			if(Data[i]<'0'||Data[i]>'9')
			   valid=FALSE;
			i++;
		}
    }
	else if(valid==TRUE && Datatype==2){           //check for float
		i=0,j=0;
		while(valid==TRUE && Data[i]){
			if(Data[i]=='.') j++; 
			if((Data[i]<'0' || Data[i]>'9')&& Data[i]!='.'){
			   valid=FALSE; break;
		    }
			i++;
		}
		if(j>1) valid=FALSE;              // check for at most one point		
	}
	else if(valid==TRUE && Datatype==3){         //check for character
	    i=0; 
	    while(valid==TRUE&&Data[i]){
	        if(Data[i]<'A'||(Data[i]>'Z'&&Data[i]<'a')||Data[i]>'z')
	           valid=FALSE;
	        i++;
		}
	}
	else if(valid==TRUE && Datatype==4){           //check for date
		i=0;j=0;
		while(valid==TRUE&&Data[i]){
			if(Data[i]=='-') j++;                 //check for date which must be "__-__-____" form
			if((Data[i]<'0' || Data[i]>'9')&& Data[i]!='-'){
				valid=FALSE;break;
			}
			i++;
		}
		if(j!=2) valid=FALSE;                      // only two '-'
	}
	else if(valid==TRUE && Datatype==5){              //check for a different movie booking number
		i=0;
		while(valid==TRUE && Data[i]){
			if(Data[i]<'0'||Data[i]>'9')
			   valid=FALSE;
			i++;
		}
		if(i!=4) valid =FALSE;
		ReadinStruct();
		add=addcount();
		for(i=0;i<=add-1;i++)                       // if it's the same as before, then return FALSE
			if(strcmp(Data,PerInfo[i].MBN)==0){
			valid=FALSE; break;
		    }
	}
	else if(valid==TRUE && Datatype==6){
		i=0,j=0;
		while(valid==TRUE && Data[i]){
            if(Data[i]==':') j++;                 //check for date which can be "__:__" form
			if((Data[i]<'0' || Data[i]>'9')&& Data[i]!=':'){
				valid=FALSE;break;
			}
			i++;
		}
		if(j>1) valid=FALSE;                     // at most one ":"
	}
	return valid;
}
void UpFile(struct movie newdata[])
{
	int i;
	add=addcount();
	out=fopen(FILENAME,"w");
	for(i=0;i<=add-1;i++)
		fprintf(out,"%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n\n",newdata[i].MBN,newdata[i].NoC,newdata[i].NoM,newdata[i].MS,newdata[i].Time,newdata[i].NoG,newdata[i].HN,newdata[i].TT,newdata[i].TF);
	fclose(out);		
}
void ReadinStruct()
{
	fflush(stdin);
	int i;
	add=addcount(); 
	in=fopen(FILENAME,"r");
	for(i=0;i<=add-1;i++)
		fscanf(in,"%s%s%s%s%s%s%s%s%s",PerInfo[i].MBN,PerInfo[i].NoC,PerInfo[i].NoM,PerInfo[i].MS,PerInfo[i].Time,PerInfo[i].NoG,PerInfo[i].HN,PerInfo[i].TT,PerInfo[i].TF);
	fclose(in);
}
int addcount()
{
	fflush(stdin);
	char str[MAXCHAR];
	int sign=0;                                         // store the return value
	in=fopen(FILENAME,"r");
	if(in==NULL){
		fclose(in);
		in=fopen(FILENAME,"w");                     // creat a new file
		fclose(in);
	}
	else{
		while(fscanf(in,"%s",str)!=EOF)
		   sign++;
		sign=sign/MEMOFREC;
		fclose(in);   
	}
	return sign;
}
void Add()
{
	fflush(stdin);
	int a,b,d,e,f,g,h,i;
	int sign=TRUE;                             // to sign the data vilidation
	int start,skip;                            // "start" for recording original "add","skip" for skipping in manu and getchar() return int
	char arb;
	start=add=addcount();
	system("cls");
	if(add<50){
	    printf("\nPlease enter 1>Movie Booking Number<4 digits>, 2>Name of Customer, 3>Name of Movie, 4>Movie Schedule, 5>Time, 6>Number of Guests, 7>House Number, 8>Ticket Type, 9>Total Fee");
	    printf("\n(1.a 'BLANK SPACE' between two datas 2.no 'BLANK SPACE' within each data 3.use form\"__-__-____\" for Movie Schedule input 4.Movie Booking Number which can't be the same as before 5.use \"__:__\" for time is OK)\n");
        for(;add<=MAXREC-1;add++){
	        while(sign){
			    fflush(stdin);
		        scanf("%s%s%s%s%s%s%s%s%s",PerInfo[add].MBN,PerInfo[add].NoC,PerInfo[add].NoM,PerInfo[add].MS,PerInfo[add].Time,PerInfo[add].NoG,PerInfo[add].HN,PerInfo[add].TT,PerInfo[add].TF);
                a=isValidData(PerInfo[add].MBN,5), b=isValidData(PerInfo[add].NoC,3);                                                                                        
                d=isValidData(PerInfo[add].MS,4), e=isValidData(PerInfo[add].Time,6), f=isValidData(PerInfo[add].NoG,1);
                g=isValidData(PerInfo[add].HN,1), h=isValidData(PerInfo[add].TT,3), i=isValidData(PerInfo[add].TF,2);
                if(a==TRUE&&b==TRUE&&d==TRUE&&e==TRUE&&f==TRUE&&g==TRUE&&h==TRUE&&i==TRUE) sign=FALSE;
                else{
                	if(a==FALSE) printf("\"%s\" is invalid.  ",PerInfo[add].MBN); if(b==FALSE) printf("\"%s\" is invalid.  ",PerInfo[add].NoC);                                                               
                    if(d==FALSE) printf("\"%s\" is invalid.  ",PerInfo[add].MS); if(e==FALSE) printf("\"%s\" is invalid.  ",PerInfo[add].Time); if(f==FALSE) printf("\"%s\" is invalid.  ",PerInfo[add].NoG);
                    if(g==FALSE) printf("\"%s\" is invalid.  ",PerInfo[add].HN); if(h==FALSE) printf("\"%s\" is invalid.  ",PerInfo[add].TT); if(i==FALSE) printf("\"%s\" is invalid.  ",PerInfo[add].TF);					
				    printf("\nYou input invalid data<s>, please input again!\n");
			    }
            }
            out=fopen(FILENAME,"a");                   // add new records to the end of file                                        
		    fprintf(out,"%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n\n",PerInfo[add].MBN,PerInfo[add].NoC,PerInfo[add].NoM,PerInfo[add].MS,PerInfo[add].Time,PerInfo[add].NoG,PerInfo[add].HN,PerInfo[add].TT,PerInfo[add].TF);
	        fclose(out);                                                      
            fflush(stdin);
            printf("\nDo you want to ADD another record<y/n>:");
            if((skip=getchar())==110 || skip==78) break;
            printf("\nPlease enter 1>Movie Booking Number, 2>Name of Customer, 3>Name of Movie, 4>Movie Schedule, 5>Time, 6>Number of Guests, 7>House Number, 8>Ticket Type, 9>Total Fee");
	        printf("\n(1.a 'BLANK SPACE' between two datas 2.no 'BLANK SPACE' within each data 3.use form\"__-__-____\" for Movie Schedule input4.Movie Booking Number which can't be the same as before 5.use \"__:__\" for time is OK)\n");
	        sign=TRUE;
    	}
    }
    if(add>=50){
	printf("It's full. You can't add any more records. ");
    fflush(stdin);               
	printf("\n\nGo back to manu with inputting arbitrary value ");
	scanf("%c",&arb);
    }
}
void Display()
{
	fflush(stdin);
	char rec[MEMOFREC][MAXCHAR];
	int i,j;
	add=addcount();
	char arb;
    system("cls");
	if((in=fopen(FILENAME,"r"))==NULL)
		fprintf(stdout,"The file \"movie.txt\" is not existing , please creat this file first.");
	else{
		if(add==0)
		    printf("There is no records in file, please choose option 1 to add records first!\n");
		else{
		printf("1>Movie Booking Number\n2>Name of Customer\n3>Name of Movie\n4>Movie Schedule\n5>Time\n6>Number of Guests\n7>House Number\n8>Ticket Type\n9>Total Fee\n\n");
		    for(i=0;i<=add-1;i++){
		        for(j=0;j<=8;j++){
			        fscanf(in,"%s",rec[j]);
			        fprintf(stdout,"%s\n",rec[j]);
		        }
		    printf("\n");
	        }
	    }
	}
	fclose(in);                  // if succeed, the input buffer will be update
	fflush(stdin);               //avoid fail to close "in" stream, there still are some data in input buffer 
	printf("\n\nGo back to manu with inputting arbitrary value ");
	scanf("%c",&arb);
}
void Modify()
{
    system("cls");
    int a,b,d,e,f,g,h,i,j;
    int sign1=TRUE,sign2=TRUE,sign3=TRUE;                             // same as before
    char num[MAXCHAR];                               // store movie booking number
    char skip,arb;                                   // same as before
	ReadinStruct();
	add=addcount();
	do{
	    printf("Please input the MOVIE BOOKING NUMBER of the one which you want to MODIFY:");
	    fflush(stdin);
	    gets(num);
	    for(i=0;i<=add-1;i++){
		    if(strcmp(num,PerInfo[i].MBN)==0){
		    	printf("\nFollow this order and requirement,1>Movie Booking Number<4 digits>, 2>Name of Customer, 3>Name of Movie, 4>Movie Schedule, 5>Time, 6>Number of Guests, 7>House Number, 8>Ticket Type, 9>Total Fee");
	            printf("\n(1.A 'BLANK SPACE' between two datas 2.No 'BLANK SPACE' within each data 3.use form\"..-..-....\" for Movie Schedule inputMovie 4.Booking Number which can't be the same as before)\n");
			    printf("\nPlease enter a modified record of number <%s>\n",num);
			    while(sign1){
			        scanf("%s%s%s%s%s%s%s%s%s",PerInfo[i].MBN,PerInfo[i].NoC,PerInfo[i].NoM,PerInfo[i].MS,PerInfo[i].Time,PerInfo[i].NoG,PerInfo[i].HN,PerInfo[i].TT,PerInfo[i].TF);
			        a=isValidData(PerInfo[i].MBN,1), b=isValidData(PerInfo[i].NoC,3);
                    d=isValidData(PerInfo[i].MS,4), e=isValidData(PerInfo[i].Time,6), f=isValidData(PerInfo[i].NoG,1);
                    g=isValidData(PerInfo[i].HN,1), h=isValidData(PerInfo[i].TT,3), j=isValidData(PerInfo[i].TF,2);
                    if(a==TRUE&&b==TRUE&&d==TRUE&&e==TRUE&&f==TRUE&&g==TRUE&&h==TRUE&&j==TRUE) sign1=FALSE;
                    else{
                    	if(a==FALSE) printf("\"%s\" is invalid.  ",PerInfo[i].MBN); if(b==FALSE) printf("\"%s\" is invalid.  ",PerInfo[i].NoC);                                                            
                        if(d==FALSE) printf("\"%s\" is invalid.  ",PerInfo[i].MS); if(e==FALSE) printf("\"%s\" is invalid.  ",PerInfo[i].Time); if(f==FALSE) printf("\"%s\" is invalid.  ",PerInfo[i].NoG);
                        if(g==FALSE) printf("\"%s\" is invalid.  ",PerInfo[i].HN); if(h==FALSE) printf("\"%s\" is invalid.  ",PerInfo[i].TT); if(j==FALSE) printf("\"%s\" is invalid.  ",PerInfo[i].TF);					
				        printf("\nYou input invalid data<s>, please input again!\n");
					}
                }
                sign2=FALSE; break;
		    }
	    }
	    if(sign2) printf("\nSORRY, there is no movie booking number <%s> record\n",num);
	    else printf("\nYou modify the record successfully!\nYou can choose option 2 of manu to check.\n"); 
	    printf("\nDo you want to MODIFY another record<y/n>:");
	    fflush(stdin);
	    if((skip=getchar())==110 || skip==78) sign3=FALSE;
	    sign2=TRUE;
	}while(sign3);
	UpFile(PerInfo);
}
void Search()
{
    system("cls");
    int i;
    int sign1=TRUE,sign2=TRUE;                   // same as before
    char num[MAXCHAR];                           // same as bofore
    char skip;                                   // same as before
	ReadinStruct();
	add=addcount();
	do{
	    printf("Please input the MOVIE BOOKING NUMBER of the one which you want to SEARCH:");
	    fflush(stdin);
	    gets(num);
	    for(i=0;i<=add-1;i++){
		    if(strcmp(num,PerInfo[i].MBN)==0){
		    	puts(PerInfo[i].MBN); puts(PerInfo[i].NoC); puts(PerInfo[i].NoM);
		    	puts(PerInfo[i].MS); puts(PerInfo[i].Time); puts(PerInfo[i].NoG);
		    	puts(PerInfo[i].HN); puts(PerInfo[i].TT); puts(PerInfo[i].TF);
		    	sign2=FALSE; break;
			}
        }
        if(sign2) printf("\nSORRY, there is no movie booking number <%s> record\n",num);
        printf("\nDo you want to SEARCH another record<y/n>:");
	    fflush(stdin);
	    if((skip=getchar())==110 || skip==78) sign1=FALSE;
        system("cls");
        sign2=TRUE;                       // for second searching
    }while(sign1);
}
void Delete()
{
	system("cls");
    int i,j,num1;                                     // store the converted number of "num"
    int sign1=TRUE,sign2=TRUE,sign3=TRUE;                             // same as before
    char num[MAXCHAR];                               // store movie booking number
    char skip,arb;                                   // same as before
	ReadinStruct();
	add=addcount();
	do{
	    printf("Please input the MOVIE BOOKING NUMBER of the record you want to DELETE:");
	    fflush(stdin);
	    gets(num);
	    for(i=0;i<=add-1;i++){
		    if(strcmp(num,PerInfo[i].MBN)==0){            //use the next one to take place of before, which is the same as deleting
		    	for(j=i;j<=add-1;j++){
		    		strcpy(PerInfo[j].MBN,PerInfo[j+1].MBN);  strcpy(PerInfo[j].NoC,PerInfo[j+1].NoC); 
					strcpy(PerInfo[j].NoM,PerInfo[j+1].NoM);  strcpy(PerInfo[j].MS,PerInfo[j+1].MS); 
		    	    strcpy(PerInfo[j].Time,PerInfo[j+1].Time);  strcpy(PerInfo[j].NoG,PerInfo[j+1].NoG);
		    	    strcpy(PerInfo[j].HN,PerInfo[j+1].HN);  strcpy(PerInfo[j].TT,PerInfo[j+1].TT);
					strcpy(PerInfo[j].TF,PerInfo[j+1].TF);
				}
                sign2=FALSE; break;
		    }
	    }
	    if(sign2) printf("\nSORRY, there is no movie booking number <%s> record\n",num);
	    else printf("\nYou delete the record successfully!\nYou can choose option 2 of manu to check.\n");
	    printf("\nDo you want to DELETE another record<y/n>:");
	    fflush(stdin);
	    if((skip=getchar())==110 || skip==78) sign3=FALSE;
	    sign2=TRUE;
	}while(sign3);
	UpFile(PerInfo);
}
void ChoDisSeat()
{
    int a,b,c,i,j,n,hall,sign1=TRUE,sign2=TRUE,sign3=TRUE,sign4=TRUE, sign5=TRUE, sign6=TRUE,sign7=TRUE;
    const char filename[]="seat.txt";  
    char num[MAXCHAR],way;           // store the digit of vedio hall and the way of selecting
	char skip,ch,arb;                             
    int st[11][11], nd[11][11], rd[11][11];
    int row,column;                       // store the number of seats
    long int rec;                        // store the return number of ftell()
    ReadinStruct();
	add=addcount();
	system("cls");
	printf("There three movies playing today! We will open three video hall");
    printf("\nDo you want to display the seat chart <y/n>:");
	fflush(stdin);
	if((skip=getchar())==110 || skip==78) sign6=FALSE;
	if(sign6){
		st[0][0]=nd[0][0]=rd[0][0]=0;
		for(i=1;i<=10;i++){
			st[0][i]=st[0][i-1]+1, nd[0][i]=nd[0][i-1]+1, rd[0][i]=rd[0][i-1]+1;
			st[i][0]=st[i-1][0]+1, nd[i][0]=nd[i-1][0]+1, rd[i][0]=rd[i-1][0]+1;
		}
		for(n=1;n<=3;n++){ 
	    	if(n==1)
	    		for(i=1;i<=10;i++)
	    		    for(j=1;j<=10;j++)
	    		        st[i][j]=0;                                                
	    	if(n==2)
	    		for(i=1;i<=10;i++)                                             
	    		    for(j=1;j<=10;j++)
	    		        nd[i][j]=0;                                                                                                       
			if(n==3)
	    		for(i=1;i<=10;i++)                               
	    		    for(j=1;j<=10;j++)                                                                  
	    		        rd[i][j]=0;                                     
	    }
	    out=fopen(filename,"r");
	    fseek(out,0L,SEEK_END);
	    rec=(ftell(out)+2)/13;
	    rewind(out);
	    for(i=1;i<=rec;i++){
            fscanf(out,"%s %d %d %d",num,&hall,&row,&column);                   
			if(hall==1) {
		        for(j=0;j<=add-1;j++)                                         // if option 5 delete some one, this one will not display in this seat chart
        	        if(strcmp(num,PerInfo[j].MBN)==0){
			            st[row][column]=atoi(num);
						break;   
		            }
		    }
			if(hall==2){
				for(j=0;j<=add-1;j++)
        	        if(strcmp(num,PerInfo[j].MBN)==0){
			            nd[row][column]=atoi(num);
						break;
				    }
		    }
			if(hall==3){
				for(j=0;j<=add-1;j++)
        	        if(strcmp(num,PerInfo[j].MBN)==0){
			            rd[row][column]=atoi(num);
						break; 
					}
		    }
		}
	    for(n=1;n<=3;n++){
	    	if(n==1){
	    		for(i=0;i<=10;i++){
	    		    for(j=0;j<=10;j++)
	    		       printf("%-5d",st[i][j]);                               
	    		    printf("\n");                                        
	    	    }
	    	}                                                           
	    	if(n==2){
	    		for(i=0;i<=10;i++){                                            
	    		    for(j=0;j<=10;j++)
	    		       printf("%-5d",nd[i][j]);
	    		    printf("\n");		
				}
		    }                                                                           
			if(n==3){
	    		for(i=0;i<=10;i++){                                 
	    		    for(j=0;j<=10;j++)                             
	    		       printf("%-5d",rd[i][j]);                        
	    		    printf("\n");		
				}
		    }
	        printf("\n\n");                                            
	    }
	   	fflush(stdin);                                                                             
	    printf("\n\nGo to select seat with inputting arbitrary value ");
	    scanf("%c",&arb);
   }
    do{
    	system("cls");
        printf("\nNow you can enter the MOVIE BOOKING NUMBER for selecting seat:");
        fflush(stdin);
        gets(num);
        for(i=0;i<=add-1;i++)
        	if(strcmp(num,PerInfo[i].MBN)==0){
        		sign1=FALSE; break;
			}
		a=seatreccheck(num,0,0,0);
        if(sign1) printf("\nSORRY, there is no movie booking number <%s> record\n",num);
        else if(!a) {
		    printf("\nSORRY, movie booking number <%s> has selected, you can't select again.\n",num);
		    sign1=TRUE;
		}
        else{
        	printf("\nThere are three video halls today, which video hall does your movie match(1/2/3):");
        	do{
        		fflush(stdin);
        		scanf("%d",&hall);
        		if(hall==1||hall==2||hall==3) sign2=FALSE;
        		else printf("%d is invalid, please enter again:",hall);
			}while(sign2);
			sign2=TRUE;
			printf("\nWhich way seat selected do you want to choose, 1 by you own, 2 by random <1/2>:");
			do{
        		fflush(stdin);
        		way=getchar();
        		if(way=='1'||way=='2') sign3=FALSE;
        		else printf("%c is invalid, please enter again:",way);
			}while(sign3);
			sign3=TRUE;
			if(way=='1'){
				printf("Please enter the row(1 to 10) and column(1 to 10):");
				do{
					scanf("%d%d",&row,&column);
					b=seatreccheck(num,hall,row,column);
					if(row>=1&&row<=10&&column>=1&&column<=10){
						sign5=FALSE;
				        if(!b){
					    sign5=TRUE;                                           // seats reserved can not be selected again
					    printf("SORRY,this seat has been selected, please input again.");
				        }
				    }
					else printf("The number you input is invalid, please input again:");
				}while(sign5);
				sign5=TRUE;
				out=fopen(filename,"a");
				fprintf(out,"%.4s %d %2d %2d\n",num,hall,row,column);                           //store the seat information in the seat.txt
				fclose(out);
			}
			if(way=='2'){
				srand(time(NULL));                                            // this generates the "seed" value 
				do{
				    row=1+(int)rand()%10;
				    column=1+(int)rand()%10;
				    c=seatreccheck(num,hall,row,column);
				    if(c) sign7=FALSE;
				}while(sign7);
				sign7=TRUE;
				out=fopen(filename,"a");
				printf("row: %d and column: %d\n",row,column);
			    fprintf(out,"%.4s %d %2d %2d\n",num,hall,row,column);                           //store the seat information in the seat.txt
			    fclose(out);

			}
		}
		if(!sign1&&a) printf("You have selected the seat successfully!");
		printf("\nDo you want to enter another MOVIE BOOKiNG NUMBER for selecting seat<y/n>:");
	    fflush(stdin);
	    if((skip=getchar())==110 || skip==78){
		sign4=FALSE; fclose(out);
	    }
	    sign1=TRUE;
    }while(sign4);
    fclose(out);
}
int seatreccheck(const char *st,int x,int y,int z)
{
	long int rec;
	char num[MAXCHAR];
	int i,hall,row,column,sign=TRUE;
	char filename[]="seat.txt";
	out=fopen(filename,"r");
	fseek(out,0L,SEEK_END);
	rec=(ftell(out)+2)/13;
	rewind(out);
	fflush(stdin);
	for(i=1;i<=rec;i++){
        fscanf(out,"%s%d%d%d",num,&hall,&row,&column);
		if(strcmp(st,num)==0){
			sign=FALSE;
			break;
		}
		if(x==hall){
			if(y==row&&z==column){
				sign=FALSE;
			    break;
			}
		}
	}
	fclose(out);
	return sign;
}
void movie_print()
{
	system("cls");
	struct info{
	 char username[MAXCHAR];
     char password[MAXCHAR];
     char id[MAXCHAR];
	};
    struct info user[MAXCHAR];
    struct movie PerInfo[MAXCHAR];
    int exit=0;
    int end1=0,h,exit1=0;
    int i,arb;
    
    char judge[MAXCHAR];
    char usernameinput[MAXCHAR],passwordinput[MAXCHAR];
    FILE *inFileu;
    printf("*** Welcome to HK Grand SPACE Movie Tickets Printing system 2017 ***\n");
    inFileu = fopen ("user.txt","r");
    fflush(stdin);
	do
{
	printf("Please enter the your username:");
    gets(usernameinput);
	for(i=0;i<=10;i++)
	{
		fscanf(inFileu,"%s%s%s",user[i].username,user[i].password,user[i].id);
		if (strcmp(user[i].username,usernameinput)==0)
		{
		    exit = 1;
		    end1=1;
		    break;
	    }
	}
	if (exit == 0)
	printf("Sorry,Please enter the right username.\n");
}while (end1==0);
    while(exit1==0)
{
	printf("Please enter your password:");
	fflush(stdin);
	gets(passwordinput);
	if (strcmp(user[i].password,passwordinput)==0)
		{
		    exit1 = 1;
	    }
	else
	printf("Sorry,You entered a wrong password.\n");
}
	printf("Do you want to print your movie ticket now?<y/n>: ");
    gets(judge);
    h=0;
    while (h == 0)
	{
		if (judge[0] == 'n' && judge[1] == '\0')
		h =1;
	    else if (judge[0] == 'y' && judge[1] == '\0')
	    {
	        h =1;
		}
	    else
	    {	
	        printf("You enter a wrong character!\nDo you want to print your movie ticket now?<y/n>: ");
	        fflush(stdin);
	        gets(judge);
	    }
	}
	fclose(inFileu);
	if (judge[0] == 'y')
	printf("%s is your tieckt ordering number, you could print it(them)!",user[i].id);
	if (judge[0] == 'n')
	printf("OK!Byebye!");
	printf("\n\nGo back to manu with inputting arbitrary value ");
	scanf("%c",&arb);
}
void movie_review()
{
	char number[100],review[100];
	int a;			// a for choice function; b for loop 
	char findStr[100],b;
	char *pStr=(char*)malloc(100);
	FILE*fp,*fp2;
	fp = fopen("moive_rating.txt","rt");
	fp2 = fopen("moive_rating.txt", "a");
	
	if(fp==NULL)//
 	{
 		fprintf(stderr,"fail to open file");
 		
 	}
	
	do
	{
		printf("Press '1' to check a moive rating\npress '2' to input a moive rating.\n");
		scanf("%d",&a);
	
		if (a==1)// Movie_check function
		{
			fflush(stdin);
			printf("\nPlease input moive number: ");
			scanf("%s",findStr);
					
			while(fgets(pStr,100,fp)!=NULL)
			{
				if(strstr(pStr,findStr)!=NULL)
				printf("%s",pStr); 
			}
		}
	

		if (a==2)
		{
			fflush (stdin);
			printf("\nplease input moive_rating(eg.#10005.0)(Don't press <Enter> before ')\n"); 
			
			scanf("%s",number);
			gets(review);
			fprintf(fp2,"\n%s%s",number,review);
		
			printf("\nSuccess! Thank for your comment.");
			printf("\nYour review <%s%s> has save in system!",number,review);
		}
		
		printf("\n\nDo you want check or review again?(y/n)\n");
		scanf("%c",&b);
		fclose(fp);
	
		}
	while(b == 'y');
	
	printf("\nGood bye!");
}
