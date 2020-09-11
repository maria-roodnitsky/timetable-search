/*Made by Maria Roodnitsky
Purpose: Program for reading and writing timetable data.*/


#include <stdio.h>
#include <string.h>

typedef struct { /*holds information for a Dartmouth course*/
    int year;
    int ID;
    char department[5];
    int number;
    char title[35];
    char time[4];
    char prof_first[15];
    char prof_last[15];
    char distrib[4];
    int cap;
    int enrolled;
  } course_t;

void menu(void); /*displays basic menu once a file has been chosen*/
void sortmenu(void); /*displays the menu for how to sort the data*/
void searchmenu(void); /*displays the menu for how to search the data*/
int read_data (course_t info[], FILE *ifile); /*reads data into info array*/
FILE *load_file(void); /*stores file pointer to user file*/
void print_data(course_t info[],int n); /*displays data to the screen*/
void add_data (course_t info[], int *n); /*adds a class to an array*/
void save_data(course_t info[], int n); /*writes a file*/
void delete_data (course_t info[],int *n); /*deletes a class from an array by ID*/
void sort_ID(course_t info[],int n); /*sorts data by ID*/
void sort_coursenumber(course_t info[],int n); /*sorts data by course number*/
void sort_time(course_t info[],int n); /*sorts data by time*/
void sort_title(course_t info[],int n); /*sorts data by title*/
void sort_distrib(course_t info[],int n); /*sorts data by distributive*/
void sort_prof(course_t info[],int n); /*sorts data by professor last name*/
void replace (int i, int placeholder, int m, char placeholderarray[], course_t info[]); 
/*this is the heart of the sorting mechanism, it goes through and rearranges all of the data*/
void search_ID(course_t info[],int n); /*searches data by ID*/
void search_number(course_t info[],int n); /*searches data by course number*/
void search_title(course_t info[],int n); /*searches data by course title*/
void search_time(course_t info[],int n); /*searches data by course time*/
void search_prof(course_t info[],int n); /*searches data by professor*/
void search_distrib(course_t info[],int n); /*searches data by distributive*/


int main(void)
{ 
  course_t info[100]; /*100 is a reasonable course limit*/
  FILE *ifile;
  int selection, n, fileselection, sort_type, search_type;
  
  printf("\n\nWelcome to the course catalog.\n\n");
  printf("[1] Use the default file.\n");
  printf("[2] Use your own timetable file.\n\n");
  printf("Please select an option.  > ");
  
  do{ /*asks user to select their own file or use the default; loops until a valid integar is selected*/
   scanf("%d", &fileselection);
   switch(fileselection)
   {case 1: ifile = fopen("timetable.txt", "r");
    if(ifile == NULL)
    {printf("You do not have the default time table. Exiting program.\n\n");
    return(0);}
   n=read_data(info, ifile);
   break;
   case 2: ifile = load_file();
   n=read_data(info, ifile);
   break;
   default: printf("You didn't choose a valid option! Try again.  > ");
   break;
   }
    }while ((fileselection != 1) && (fileselection != 2)); 
  
  do{ /*loops a menu until user quits the program*/
   menu();
   scanf("%d", &selection);
   
   switch(selection)
   {case 1: print_data(info, n);
    break;
    
    case 2: 
    sortmenu();
    scanf("%d", &sort_type); 
    
     switch(sort_type)
     {case 1: sort_ID(info, n);
     break;
     case 2: sort_coursenumber(info, n);
     break;
     case 3: sort_title(info, n);
     break;
     case 4: sort_time(info, n);
     break;
     case 5: sort_prof(info, n);
     break;
     case 6:sort_distrib(info, n);
     break;
     default: printf("'%d' is not a valid sorting option. Exiting to main menu.", sort_type);
     }
     if((sort_type<7)&&(sort_type>0))
     printf("\n\nYour data has been sorted. Press [1] when prompted by the menu to view it.");
     break;
   
   case 3: add_data(info, &n);
   break;
   
   case 4: delete_data(info,&n);
   break;
   
   case 5: save_data(info, n);
   printf("\n\nThank you for using this program!\n\n");
   return(0);
   break;
   
  case 6: 
  searchmenu();
  scanf("%d", &search_type); 
    
     switch(search_type)
     {case 1: search_ID(info, n);
     break;
     case 2: search_number(info, n);
     break;
     case 3: search_title(info, n);
     break;
     case 4: search_time(info, n);
     break;
     case 5: search_prof(info, n);
     break;
     case 6:search_distrib(info, n);
     break;
     default: printf("'%d' is not a valid search option. Exiting to main menu.", search_type);
     }
   
   break;
   
   case 0: printf("\nThank you for using this program!\n");
   return(0);
   break;
   
   default: printf("\n'%d' is not a valid selection option. Enter a valid selection choice.", selection);
   break;
  }
  } while (selection!=0);
  
}

void menu(void) /*prints basic menu*/
{ printf("\n\n\n\n        MENU\n--------------------\n[1] DISPLAY the timetable to the terminal.\n");
  printf("[2] SORT the timetable.\n");
  printf("[3] ADD a course to the timetable. (Note: Your timetable may contain up to 100 courses.)\n");
  printf("[4] DELETE a course from the timetable.\n");
  printf("[5] SAVE changes to a new file.\n");
  printf("[6] SEARCH.\n");
  printf("[0] QUIT without saving.\n");
  printf("\nSelect an option. > ");
}  


FILE *load_file(void) /*allows a user to indicate a file from which the timetable will be read*/
  {char filename[50];
  
  FILE *ifile;
  
  printf("Enter a filename: > ");
  scanf("%s", filename);
  
  ifile = fopen(filename, "r");
  
  while (ifile == NULL) {
    printf("File does not exist.\nEnter a new filename: > ");
    scanf("%s", filename);
    ifile = fopen(filename, "r"); 
  }
   return(ifile);
  }

int read_data (course_t info[], FILE *ifile){ /*reads data from file into structure*/
  int year, ID, number, cap, enrolled;
  
  int i;
  i = 0;
  
  while (fscanf(ifile, "%d", &year)!=EOF)
  { info[i].year = year;
    
    fscanf(ifile, "%d", &ID);
    info[i].ID = ID;
    
    fscanf(ifile, "%s", info[i].department);
  
    fscanf(ifile, "%d", &number);
    info[i].number = number;
  
    fscanf(ifile, "%s", info[i].title);
    fscanf(ifile, "%s", info[i].time);
    fscanf(ifile, "%s", info[i].prof_first);
    fscanf(ifile, "%s", info[i].prof_last);
    fscanf(ifile, "%s", info[i].distrib);
    
    fscanf(ifile, "%d", &cap);
    info[i].cap = cap;
    
    fscanf(ifile, "%d", &enrolled);
    info[i].enrolled = enrolled;
    
    i++;
  }
    fclose(ifile);
    
    return(i);
}

void print_data(course_t info[], int n){ /*prints the timetable to terminal with program-made adjustments (if applicable)*/
  int i;
  printf("\n\n     TIME TABLE\n--------------------\n");
  for (i=0; i<n; i++)
    printf("%d %d %s %03d %35s %4s %15s %15s %4s %2d %2d\n", info[i].year,info[i].ID,info[i].department,info[i].number,info[i].title,info[i].time,info[i].prof_first,info[i].prof_last,info[i].distrib,info[i].cap,info[i].enrolled);
}

void add_data (course_t info[], int *n) /*allows the user to add a course to the next element of the array*/
{ 
  printf("\n\n\n\n\n    ADD A COURSE\n--------------------\n");
  
  printf("Enter the year and month of the course in YYYYMM format.  > ");
  scanf("%d", &info[*n].year);
  
  printf("Enter the five digit ID of the course.  > ");
  scanf("%d", &info[*n].ID);
  
  printf("Enter the 4 character department code of the course. Example: 'ENGS' or 'MATH'. > ");
  scanf("%s", info[*n].department);
  
  printf("Enter the number of the course. > ");
  scanf("%d", &info[*n].number);
  
  printf("Enter the title of the course. (Please enter '_' for spaces.  > ");
  scanf("%s", info[*n].title);
  
  printf("Enter the time of the course.  > ");
  scanf("%s", info[*n].time);
  
  printf("Enter the first name of the professor of the course.  > ");
  scanf("%s", info[*n].prof_first);
  
  printf("Enter the last name of the professor of the course.  > ");
  scanf("%s", info[*n].prof_last);
  
  printf("Enter the 3 character distributive for this course.(Enter '-' if there is none.)  > ");
  scanf("%s", info[*n].distrib);
  
  printf("Enter the cap for this course. > ");
  scanf("%d", &info[*n].cap);
  
  printf("Enter the number of enrolled students in this course. > ");
  scanf("%d", &info[*n].enrolled);
  
  printf("\n\n\nYour new course shall be displayed as follows.\n%6d %5d %5s %03d %35s %3s %15s %15s %3s %2d %2d\n", info[*n].year,info[*n].ID,info[*n].department,info[*n].number,info[*n].title,info[*n].time,info[*n].prof_first,info[*n].prof_last,info[*n].distrib,info[*n].cap,info[*n].enrolled);

  (*n) += 1; /*increases the amount of courses by 1*/
}
   
void save_data(course_t info[], int n){ /*writes a file with the data the user has entered*/
  int i;
  FILE *newfile;
  char filename[50];
  printf("\n\n\n\n\n  SAVE AS NEW FILE\n--------------------\n");
  printf("What would you like to name your new file? (Note: Your file must end with'.txt') > ");
  scanf("%s", filename);
 
  newfile = fopen(filename,"w");
 
  for (i=0; i<n; i++)
    fprintf(newfile,"%d %d %s %03d %35s %4s %15s %15s %4s %2d %2d\n", info[i].year,info[i].ID,info[i].department,info[i].number,info[i].title,info[i].time,info[i].prof_first,info[i].prof_last,info[i].distrib,info[i].cap,info[i].enrolled);
  
  printf("\n\n\nDone writing file '%s'!\n", filename);  
}

void delete_data (course_t info[],int *n) /*deletes a course by shifting the previous courses up and printing one less course*/
{
  int course, i, index;
  index = -1;
  i = 0;
  printf("\n\n\n\n\n  REMOVE A COURSE\n--------------------\n");
  printf("Enter the course ID of the course you would like to remove from the catalog.");
  printf("\nIf you need to exit back to the current time table for this information, type [-1].  > ");
  
  scanf("%d", &course);
 
  if (course == -1)
  {print_data(info, *n);}
  
  else
  {for(i=0; i<(*n); i++)
  
    {if(info[i].ID == course)
    {index = i;
     *n-=1;}}  
   
  int m;
  m = 0;
  
  if (index == -1)
  printf("\n\n\n\nSorry, course ID not found. Exiting back to menu.");
  else  
  {while((index+m)<*n) 
  { 
    info[index+m].year = info[index+m+1].year;
    info[index+m].ID = info[index+m+1].ID;
    strcpy(info[index+m].department, info[index+m+1].department);
    info[index+m].number = info[index+m+1].number;
    strcpy(info[index+m].title, info[index+m+1].title);
    strcpy(info[index+m].time, info[index+m+1].time);
    strcpy(info[index+m].prof_first, info[index+m+1].prof_first);
    strcpy(info[index+m].prof_last, info[index+m+1].prof_last);
    strcpy(info[index+m].distrib, info[index+m+1].distrib);
    info[index+m].cap = info[index+m+1].cap;
    info[index+m].enrolled = info[index+m+1].enrolled;
    
   m++;
  }
  printf("\n\n\n\nThe course with ID %d has been deleted.\n\n", course);
  }
  }
}

void sortmenu(void){ /*prints the sorting menu*/
  printf("\n\n\n\n\n      SORT BY\n--------------------\n");
  printf("[1] Course ID.\n");
  printf("[2] Course Number.\n");
  printf("[3] Title.\n");
  printf("[4] Time.\n");
  printf("[5] Professor last name.\n");
  printf("[6] Distributive.\n");
  printf("\nSelect an option. > ");
}

void sort_ID(course_t info[],int n) /*sorts data by ID*/
{
  int i, placeholder,m;
  char placeholderarray[35];
  placeholder = 0;
 
  for(i=0;i<n;i++){
    for(m=0;m<n;m++){
    if(info[i].ID<info[m].ID)
    replace(i, placeholder, m, placeholderarray, info);}
  }
  
}
 
 void sort_coursenumber(course_t info[],int n) /*sorts data by course number*/
{
  int i, placeholder,m;
  char placeholderarray[35];
  placeholder = 0;
 
  for(i=0;i<n;i++){
    for(m=0;m<n;m++){
    if(info[i].number<info[m].number)
    replace(i, placeholder, m, placeholderarray, info);}
  }
  
}
 
 void sort_time(course_t info[],int n) /*sorts data by time*/
{
  int i, placeholder,m;
  char placeholderarray[35];
  placeholder = 0;
 
  for(i=0;i<n;i++){
    for(m=0;m<n;m++){
    if(strcmp(info[i].time,info[m].time)<0)
   replace(i, placeholder, m, placeholderarray, info);}
  }
  
}

void sort_title(course_t info[],int n)
{
  int i, placeholder,m;
  char placeholderarray[35];
  placeholder = 0;
 
  for(i=0;i<n;i++){
    for(m=0;m<n;m++){
    if(strcmp(info[i].title,info[m].title)<0)
    replace(i, placeholder, m, placeholderarray, info);}
  }
  
}
 
 
void sort_distrib(course_t info[],int n) /*sorts data by distributive*/
{
  int i, placeholder,m;
  char placeholderarray[35];
  placeholder = 0;
 
  for(i=0;i<n;i++){
    for(m=0;m<n;m++){
    if(strcmp(info[i].distrib,info[m].distrib)<0)
    replace(i, placeholder, m, placeholderarray, info);}
  }
  
}
    
void sort_prof(course_t info[],int n) /*sorts data by professor last name*/
{ int i, placeholder,m;
  char placeholderarray[35];
  placeholder = 0;
  
  for(i=0;i<n;i++){
    for(m=0;m<n;m++){
    if(strcmp(info[i].prof_last,info[m].prof_last)<0)
    replace(i, placeholder, m, placeholderarray, info);}
  }
  
}

void replace (int i, int placeholder, int m, char placeholderarray[], course_t info[]) /*does the data reorganizing*/
    {placeholder=info[i].ID;
    info[i].ID=info[m].ID;
    info[m].ID=placeholder;
    
    placeholder=info[i].year;
    info[i].year=info[m].year;
    info[m].year=placeholder;
    
    placeholder=info[i].number;
    info[i].number=info[m].number;
    info[m].number=placeholder;
    
    placeholder=info[i].cap;
    info[i].cap=info[m].cap;
    info[m].cap=placeholder;
    
    placeholder=info[i].enrolled;
    info[i].enrolled=info[m].enrolled;
    info[m].enrolled=placeholder;
    
    strcpy(placeholderarray, info[i].department);
    strcpy(info[i].department, info[m].department);
    strcpy(info[m].department, placeholderarray);
    
    strcpy(placeholderarray, info[i].title);
    strcpy(info[i].title, info[m].title);
    strcpy(info[m].title, placeholderarray);
    
    strcpy(placeholderarray, info[i].time);
    strcpy(info[i].time, info[m].time);
    strcpy(info[m].time, placeholderarray);
    
    strcpy(placeholderarray, info[i].prof_first);
    strcpy(info[i].prof_first, info[m].prof_first);
    strcpy(info[m].prof_first, placeholderarray);
    
    strcpy(placeholderarray, info[i].prof_last);
    strcpy(info[i].prof_last, info[m].prof_last);
    strcpy(info[m].prof_last, placeholderarray);
    
    strcpy(placeholderarray, info[i].distrib);
    strcpy(info[i].distrib, info[m].distrib);
    strcpy(info[m].distrib, placeholderarray);
    }
    
    
  void searchmenu(void){ /*prints the search menu*/
  printf("\n\n\n\n\n     SEARCH BY\n--------------------\n");
  printf("[1] Course ID.\n");
  printf("[2] Course Number.\n");
  printf("[3] Title.\n");
  printf("[4] Time.\n");
  printf("[5] Professor last name.\n");
  printf("[6] Distributive.\n");
  printf("\nSelect an option. > ");
}

void search_ID(course_t info[],int n) /*search by ID*/
{ int i, course, found;
  found=0;
  
  printf("Enter the course ID.  > ");
  
  do{ 
  scanf("%d", &course);
  for(i=0;i<n;i++){
    if(info[i].ID==course)
    {printf("\n\n\n\n COURSE INFORMATION\n--------------------\n%d %d %s %03d %35s %4s %15s %15s %4s %2d %2d\n", info[i].year,info[i].ID,info[i].department,info[i].number,info[i].title,info[i].time,info[i].prof_first,info[i].prof_last,info[i].distrib,info[i].cap,info[i].enrolled);
    found=1;}
    }
  if((found==0) && (course!=-1))
    printf("\n\n\n\nCourse ID not found. Type a new ID or type [-1] to exit back to main menu. > ");     
  }while((found==0)&&(course!=-1)); 
  
}

void search_number(course_t info[],int n) /*search by number*/
{ int i, course, found;
  found=0;
  
  printf("Enter the course number.  > ");
  
  do{ 
  scanf("%d", &course);
  for(i=0;i<n;i++){
      if(info[i].number==course)
      {printf("\n\n\n\nCOURSE INFORMATION\n--------------------\n");
       break;}}
  
  for(i=0;i<n;i++){
      if(info[i].number==course)
      {printf("%d %d %s %03d %35s %4s %15s %15s %4s %2d %2d\n", info[i].year,info[i].ID,info[i].department,info[i].number,info[i].title,info[i].time,info[i].prof_first,info[i].prof_last,info[i].distrib,info[i].cap,info[i].enrolled);
      found=1;}}
  if((found==0) && (course!=-1))
    printf("\n\n\n\nCourse number not found. Type a new number or type [-1] to exit back to main menu. > ");     
  }while((found==0)&&(course!=-1)); 
  
}

void search_title(course_t info[],int n) /*search by title*/
{ int i, found;
  char title[50];
  found=0;
  
  printf("Enter the course title. Use '_' for spaces.  > ");
  
  scanf("%s", title);
  for(i=0;i<n;i++){
      if(strcmp(info[i].title, title)==0)
      {printf("\n\n\n\nCOURSE INFORMATION\n--------------------\n");
       break;}}
  for(i=0;i<n;i++){
    if(strcmp(info[i].title, title)==0)
    {printf("%d %d %s %03d %35s %4s %15s %15s %4s %2d %2d\n", info[i].year,info[i].ID,info[i].department,info[i].number,info[i].title,info[i].time,info[i].prof_first,info[i].prof_last,info[i].distrib,info[i].cap,info[i].enrolled);
    found=1;}
  }
  if(found==0)
    printf("\n\n\n\nCourse title not found. Exiting back to the main menu.");      
}

void search_time(course_t info[],int n) /*search by time*/
{ int i, found;
  char time[5];
  found=0;
  
  printf("Enter the course time.  > ");
  
  scanf("%s", time);
  for(i=0;i<n;i++){
      if(strcmp(info[i].time, time)==0)
      {printf("\n\n\n\nCOURSE INFORMATION\n--------------------\n");
       break;}}
  for(i=0;i<n;i++){
      if(strcmp(info[i].time, time)==0)
      {printf("%d %d %s %03d %35s %4s %15s %15s %4s %2d %2d\n", info[i].year,info[i].ID,info[i].department,info[i].number,info[i].title,info[i].time,info[i].prof_first,info[i].prof_last,info[i].distrib,info[i].cap,info[i].enrolled);
      found=1;}}
  if(found==0)
    printf("\n\n\n\nInvalid course time. Exiting back to the main menu.");      
}

void search_prof(course_t info[],int n) /*search by prof*/
{ int i, found;
  char prof[15];
  found=0;
  
  printf("Enter the professor's last name.  > ");
  
  scanf("%s", prof);
  for(i=0;i<n;i++){
      if(strcmp(info[i].prof_last, prof)==0)
      {printf("\n\n\n\nCOURSE INFORMATION\n--------------------\n");
       break;}}
  for(i=0;i<n;i++){
    if(strcmp(info[i].prof_last, prof)==0)
    {printf("%d %d %s %03d %35s %4s %15s %15s %4s %2d %2d\n", info[i].year,info[i].ID,info[i].department,info[i].number,info[i].title,info[i].time,info[i].prof_first,info[i].prof_last,info[i].distrib,info[i].cap,info[i].enrolled);
    found=1;}}
  if(found==0)
    printf("\n\n\n\nThere are no professors with the last name '%s' teaching in the given course period.\nExiting back to the main menu.", prof);      
}

void search_distrib(course_t info[],int n) /*search by distrib*/
{ int i, found;
  char distrib[5];
  found=0;
  
  printf("Enter the distributive.(Example:'TAS')  > ");
  
  scanf("%s", distrib);
  for(i=0;i<n;i++){
      if(strcmp(info[i].distrib, distrib)==0)
      {printf("\n\n\n\nCOURSE INFORMATION\n--------------------\n");
       break;}}
  for(i=0;i<n;i++){
    if(strcmp(info[i].distrib, distrib)==0)
    {printf("%d %d %s %03d %35s %4s %15s %15s %4s %2d %2d\n", info[i].year,info[i].ID,info[i].department,info[i].number,info[i].title,info[i].time,info[i].prof_first,info[i].prof_last,info[i].distrib,info[i].cap,info[i].enrolled);
    found=1;}}
  if(found==0)
    printf("\n\n\n\nThere are no classes with the distributive '%s' being taught in the given course period.\nExiting back to the main menu.", distrib);      
}