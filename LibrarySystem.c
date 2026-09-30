#include <stdio.h>
#include <stdlib.h>


struct book
{
    int id;
    char title[100];
    char author[60];
    float price;
    int issued;       
    int issuedTo;      
};

struct user
{
    int id;
    char name[60];
    char contact[20];
};


void printBook(struct book *p)
{
    printf("%-8d %-45s %-25s %.2f\n", p->id, p->title, p->author, p->price);
}

void bookHeading()
{
    printf("\n%-8s %-45s %-25s %s\n", "ID", "Title", "Author", "Price");
    printf("--------------------------------------------------------------------------------\n");
}

int loadBooks(struct book list[])
{
    FILE *fp;
    int size, count, i;

    fp = fopen("BookData", "rb");
    if (fp == NULL)
        return 0;

    fseek(fp, 0, SEEK_END);
    size = ftell(fp);
    count = size / sizeof(struct book);
    fseek(fp, 0, SEEK_SET);

    for (i = 0; i < count; i++)
        fread(&list[i], sizeof(struct book), 1, fp);

    fclose(fp);
    return count;
}

void saveBooks(struct book list[], int count)
{
    FILE *fp;
    int i;

    fp = fopen("BookData", "wb");
    if (fp == NULL)
    {
        printf("Error! opening file");
        exit(1);
    }
    for (i = 0; i < count; i++)
        fwrite(&list[i], sizeof(struct book), 1, fp);
    fclose(fp);
}

int findBook(struct book list[], int count, int id)
{
    int i;
    for (i = 0; i < count; i++)
        if (list[i].id == id)
            return i;
    return -1;
}

void insertBook()
{
    struct book list[100], b;
    int count;
    FILE *fp;

    count = loadBooks(list);
    if (count == 100)
    {
        printf("The file is full (100 books).\n");
        return;
    }

    printf("Enter the Book ID     : ");
    scanf("%d", &b.id);

    if (findBook(list, count, b.id) != -1)
    {
        printf("Book ID %d already exists.\n", b.id);
        return;
    }

    printf("Enter the Book Title  : ");
    scanf(" %99[^\n]", b.title);
    printf("Enter the Author Name : ");
    scanf(" %59[^\n]", b.author);
    printf("Enter the Price       : ");
    scanf("%f", &b.price);
    b.issued = 0;
    b.issuedTo = 0;

    fp = fopen("BookData", "ab");
    if (fp == NULL)
    {
        printf("Error! opening file");
        exit(1);
    }
    fwrite(&b, sizeof(struct book), 1, fp);
    fclose(fp);

    printf("\nBOOK ADDED\n");
}

void displayBooks()
{
    struct book list[100];
    int count, i;

    count = loadBooks(list);
    if (count == 0)
    {
        printf("The file is EMPTY\n");
        return;
    }

    bookHeading();
    for (i = 0; i < count; i++)
        printBook(&list[i]);
}

void searchBook()
{
    struct book list[100];
    int count, id, pos;

    count = loadBooks(list);
    printf("Enter the Book ID you want to search : ");
    scanf("%d", &id);

    pos = findBook(list, count, id);
    if (pos == -1)
        printf("Book ID %d is not available in the file\n", id);
    else
    {
        bookHeading();
        printBook(&list[pos]);
    }
}

void deleteBook()
{
    struct book list[100];
    int count, id, pos, i;

    count = loadBooks(list);
    printf("Enter the Book ID you want to delete : ");
    scanf("%d", &id);

    pos = findBook(list, count, id);
    if (pos == -1)
    {
        printf("Book ID %d is not available in the file\n", id);
        return;
    }

    for (i = pos; i < count - 1; i++)
        list[i] = list[i + 1];
    count--;

    saveBooks(list, count);
    printf("\nRECORD DELETED\n");
}

void updateBook()
{
    struct book list[100];
    int count, id, pos, ch;

    count = loadBooks(list);
    printf("Enter the Book ID you want to update : ");
    scanf("%d", &id);

    pos = findBook(list, count, id);
    if (pos == -1)
    {
        printf("Book ID %d is not available in the file\n", id);
        return;
    }

    printf("\n\t1. Update Title");
    printf("\n\t2. Update Author");
    printf("\n\t3. Update Price");
    printf("\n\t4. Update all three");
    printf("\nEnter your choice : ");
    scanf("%d", &ch);

    switch (ch)
    {
    case 1:
        printf("Enter new Title : ");
        scanf(" %99[^\n]", list[pos].title);
        break;
    case 2:
        printf("Enter new Author : ");
        scanf(" %59[^\n]", list[pos].author);
        break;
    case 3:
        printf("Enter new Price : ");
        scanf("%f", &list[pos].price);
        break;
    case 4:
        printf("Enter new Title : ");
        scanf(" %99[^\n]", list[pos].title);
        printf("Enter new Author : ");
        scanf(" %59[^\n]", list[pos].author);
        printf("Enter new Price : ");
        scanf("%f", &list[pos].price);
        break;
    default:
        printf("Invalid Selection\n");
        return;
    }

    saveBooks(list, count);
    printf("\nRECORD UPDATED\n");
}

void sortBooks()
{
    struct book list[100], temp;
    int count, i, j;

    count = loadBooks(list);
    if (count == 0)
    {
        printf("The file is EMPTY\n");
        return;
    }

    for (i = 0; i < count - 1; i++)
        for (j = 0; j < count - 1 - i; j++)
            if (list[j].id > list[j + 1].id)
            {
                temp = list[j];
                list[j] = list[j + 1];
                list[j + 1] = temp;
            }

    printf("\nBOOKS SORTED BY ID");
    bookHeading();
    for (i = 0; i < count; i++)
        printBook(&list[i]);
}


void printUser(struct user *p)
{
    printf("%-8d %-30s %-20s\n", p->id, p->name, p->contact);
}

void userHeading()
{
    printf("\n%-8s %-30s %-20s\n", "ID", "Name", "Contact");
    printf("--------------------------------------------------------------\n");
}

int loadUsers(struct user list[])
{
    FILE *fp;
    int size, count, i;

    fp = fopen("UserData", "rb");
    if (fp == NULL)
        return 0;

    fseek(fp, 0, SEEK_END);
    size = ftell(fp);
    count = size / sizeof(struct user);
    fseek(fp, 0, SEEK_SET);

    for (i = 0; i < count; i++)
        fread(&list[i], sizeof(struct user), 1, fp);

    fclose(fp);
    return count;
}

void saveUsers(struct user list[], int count)
{
    FILE *fp;
    int i;

    fp = fopen("UserData", "wb");
    if (fp == NULL)
    {
        printf("Error! opening file");
        exit(1);
    }
    for (i = 0; i < count; i++)
        fwrite(&list[i], sizeof(struct user), 1, fp);
    fclose(fp);
}

int findUser(struct user list[], int count, int id)
{
    int i;
    for (i = 0; i < count; i++)
        if (list[i].id == id)
            return i;
    return -1;
}

void insertUser()
{
    struct user list[100], u;
    int count;
    FILE *fp;

    count = loadUsers(list);
    if (count == 100)
    {
        printf("The file is full (100 users).\n");
        return;
    }

    printf("Enter the User ID     : ");
    scanf("%d", &u.id);

    if (findUser(list, count, u.id) != -1)
    {
        printf("User ID %d already exists.\n", u.id);
        return;
    }

    printf("Enter the User Name   : ");
    scanf(" %59[^\n]", u.name);
    printf("Enter Contact Number  : ");
    scanf(" %19[^\n]", u.contact);

    fp = fopen("UserData", "ab");
    if (fp == NULL)
    {
        printf("Error! opening file");
        exit(1);
    }
    fwrite(&u, sizeof(struct user), 1, fp);
    fclose(fp);

    printf("\nUSER ADDED\n");
}

void displayUsers()
{
    struct user list[100];
    int count, i;

    count = loadUsers(list);
    if (count == 0)
    {
        printf("The file is EMPTY\n");
        return;
    }

    userHeading();
    for (i = 0; i < count; i++)
        printUser(&list[i]);
}

void searchUser()
{
    struct user list[100];
    int count, id, pos;

    count = loadUsers(list);
    printf("Enter the User ID you want to search : ");
    scanf("%d", &id);

    pos = findUser(list, count, id);
    if (pos == -1)
        printf("User ID %d is not available in the file\n", id);
    else
    {
        userHeading();
        printUser(&list[pos]);
    }
}

void deleteUser()
{
    struct user list[100];
    int count, id, pos, i;

    count = loadUsers(list);
    printf("Enter the User ID you want to delete : ");
    scanf("%d", &id);

    pos = findUser(list, count, id);
    if (pos == -1)
    {
        printf("User ID %d is not available in the file\n", id);
        return;
    }

    for (i = pos; i < count - 1; i++)
        list[i] = list[i + 1];
    count--;

    saveUsers(list, count);
    printf("\nRECORD DELETED\n");
}

void updateUser()
{
    struct user list[100];
    int count, id, pos, ch;

    count = loadUsers(list);
    printf("Enter the User ID you want to update : ");
    scanf("%d", &id);

    pos = findUser(list, count, id);
    if (pos == -1)
    {
        printf("User ID %d is not available in the file\n", id);
        return;
    }

    printf("\n\t1. Update Name");
    printf("\n\t2. Update Contact");
    printf("\n\t3. Update both");
    printf("\nEnter your choice : ");
    scanf("%d", &ch);

    switch (ch)
    {
    case 1:
        printf("Enter new Name : ");
        scanf(" %59[^\n]", list[pos].name);
        break;
    case 2:
        printf("Enter new Contact : ");
        scanf(" %19[^\n]", list[pos].contact);
        break;
    case 3:
        printf("Enter new Name : ");
        scanf(" %59[^\n]", list[pos].name);
        printf("Enter new Contact : ");
        scanf(" %19[^\n]", list[pos].contact);
        break;
    default:
        printf("Invalid Selection\n");
        return;
    }

    saveUsers(list, count);
    printf("\nRECORD UPDATED\n");
}

void sortUsers()
{
    struct user list[100], temp;
    int count, i, j;

    count = loadUsers(list);
    if (count == 0)
    {
        printf("The file is EMPTY\n");
        return;
    }

    for (i = 0; i < count - 1; i++)
        for (j = 0; j < count - 1 - i; j++)
            if (list[j].id > list[j + 1].id)
            {
                temp = list[j];
                list[j] = list[j + 1];
                list[j + 1] = temp;
            }

    printf("\nUSERS SORTED BY ID");
    userHeading();
    for (i = 0; i < count; i++)
        printUser(&list[i]);
}

void issueBook()
{
    struct book blist[100];
    struct user ulist[100];
    int bcount, ucount, bid, uid, bpos, upos;

    bcount = loadBooks(blist);
    ucount = loadUsers(ulist);

    printf("Enter the Book ID to issue : ");
    scanf("%d", &bid);
    bpos = findBook(blist, bcount, bid);
    if (bpos == -1)
    {
        printf("Book ID %d is not available in the file\n", bid);
        return;
    }
    if (blist[bpos].issued == 1)
    {
        printf("This book is already issued.\n");
        return;
    }

    printf("Enter the User ID to issue it to : ");
    scanf("%d", &uid);
    upos = findUser(ulist, ucount, uid);
    if (upos == -1)
    {
        printf("User ID %d is not available in the file\n", uid);
        return;
    }

    blist[bpos].issued = 1;
    blist[bpos].issuedTo = uid;
    saveBooks(blist, bcount);

    printf("\nBOOK ISSUED to %s\n", ulist[upos].name);
}

void returnBook()
{
    struct book blist[100];
    int bcount, bid, bpos;

    bcount = loadBooks(blist);

    printf("Enter the Book ID to return : ");
    scanf("%d", &bid);
    bpos = findBook(blist, bcount, bid);
    if (bpos == -1)
    {
        printf("Book ID %d is not available in the file\n", bid);
        return;
    }
    if (blist[bpos].issued == 0)
    {
        printf("This book was not issued.\n");
        return;
    }

    blist[bpos].issued = 0;
    blist[bpos].issuedTo = 0;
    saveBooks(blist, bcount);

    printf("\nBOOK RETURNED\n");
}

void displayIssuedBooks()
{
    struct book blist[100];
    struct user ulist[100];
    int bcount, ucount, i, upos, found = 0;

    bcount = loadBooks(blist);
    ucount = loadUsers(ulist);

    printf("\n%-8s %-35s %-20s\n", "Book ID", "Title", "Issued To");
    printf("--------------------------------------------------------------\n");

    for (i = 0; i < bcount; i++)
    {
        if (blist[i].issued == 1)
        {
            upos = findUser(ulist, ucount, blist[i].issuedTo);
            found = 1;
            if (upos == -1)
                printf("%-8d %-35s %-20s\n", blist[i].id, blist[i].title, "(unknown user)");
            else
                printf("%-8d %-35s %-20s\n", blist[i].id, blist[i].title, ulist[upos].name);
        }
    }
    if (!found)
        printf("No books are currently issued.\n");
}

void displayAvailableBooks()
{
    struct book blist[100];
    int bcount, i, found = 0;

    bcount = loadBooks(blist);

    bookHeading();
    for (i = 0; i < bcount; i++)
    {
        if (blist[i].issued == 0)
        {
            printBook(&blist[i]);
            found = 1;
        }
    }
    if (!found)
        printf("No books are left. All books are issued.\n");
}


void bookMenu()
{
    int c;
    do
    {
        printf("\n\t------- BOOK LIST -------\n");
        printf("\t1. INSERT\n\t2. DISPLAY\n\t3. SEARCH");
        printf("\n\t4. DELETE\n\t5. UPDATE\n\t6. SORT");
        printf("\n\t7. BACK TO MAIN MENU\n");
        printf("\nEnter your choice : ");
        scanf("%d", &c);
        printf("\n");

        switch (c)
        {
        case 1: insertBook();  break;
        case 2: displayBooks(); break;
        case 3: searchBook();  break;
        case 4: deleteBook();  break;
        case 5: updateBook();  break;
        case 6: sortBooks();   break;
        case 7: printf("Going back...\n"); break;
        default: printf("Your choice is wrong\nPlease try again...\n");
        }
    } while (c != 7);
}

void userMenu()
{
    int c;
    do
    {
        printf("\n\t------- USER LIST -------\n");
        printf("\t1. INSERT\n\t2. DISPLAY\n\t3. SEARCH");
        printf("\n\t4. DELETE\n\t5. UPDATE\n\t6. SORT");
        printf("\n\t7. BACK TO MAIN MENU\n");
        printf("\nEnter your choice : ");
        scanf("%d", &c);
        printf("\n");

        switch (c)
        {
        case 1: insertUser();  break;
        case 2: displayUsers(); break;
        case 3: searchUser();  break;
        case 4: deleteUser();  break;
        case 5: updateUser();  break;
        case 6: sortUsers();   break;
        case 7: printf("Going back...\n"); break;
        default: printf("Your choice is wrong\nPlease try again...\n");
        }
    } while (c != 7);
}

void issueMenu()
{
    int c;
    do
    {
        printf("\n\t------- BOOK ISSUE / RETURN -------\n");
        printf("\t1. ISSUE A BOOK\n\t2. RETURN A BOOK");
        printf("\n\t3. SHOW ISSUED BOOKS\n\t4. SHOW AVAILABLE (LEFT) BOOKS");
        printf("\n\t5. BACK TO MAIN MENU\n");
        printf("\nEnter your choice : ");
        scanf("%d", &c);
        printf("\n");

        switch (c)
        {
        case 1: issueBook();          break;
        case 2: returnBook();         break;
        case 3: displayIssuedBooks(); break;
        case 4: displayAvailableBooks(); break;
        case 5: printf("Going back...\n"); break;
        default: printf("Your choice is wrong\nPlease try again...\n");
        }
    } while (c != 5);
}

int main()
{
    int c;

    do
    {
        printf("\n\t=========================================\n");
        printf("\t   LIBRARY MANAGEMENT SYSTEM\n");
        printf("\t=========================================\n");
        printf("\t1. BOOK LIST\n");
        printf("\t2. USER LIST\n");
        printf("\t3. BOOK ISSUE / RETURN\n");
        printf("\t4. EXIT\n");
        printf("\nEnter your choice : ");
        scanf("%d", &c);
        printf("\n");

        switch (c)
        {
        case 1: bookMenu();  break;
        case 2: userMenu();  break;
        case 3: issueMenu(); break;
        case 4: printf("Thank you!\n"); break;
        default: printf("Your choice is wrong\nPlease try again...\n");
        }
    } while (c != 4);

    return 0;
}
