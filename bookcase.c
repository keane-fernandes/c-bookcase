#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include "neillsimplescreen.h"

#define HEIGHT   9
#define WIDTH    9
#define MAXNUM   100000000
#define MAXCHAR  26
#define TESTNUM  10
#define NOPARENT -1
#define NOTFOUND -1
#define SPACE    '.'

enum bool {false, true};
typedef enum bool bool;

struct bookcase{
    char svs[HEIGHT][WIDTH];
    int parentIndex;
};
typedef struct bookcase bookcase;

struct queue{
    int size;
    int h;
    int w;
    int front;
    int end;
    int moves;
};
typedef struct queue queue;

/* Tests functions within this program */
void test(void);
/* Initializes a structure which holds bookcase list parameters */
queue* queue_init();
/* Initializes an array of bookcase structures to null chars */
void bcArrInit(bookcase* bc, int n);
/* Reads input in from a bookcase definition file */
void parseInput(queue* q, bookcase* bc, int nArgs, char** userInput);
/* Checks if an imported bookcase from a *.bc file is valid */
bool isValidBookcase(queue* q, bookcase *bc);
/* Checks if the imported parent bookcase cannot be solved */
bool initialChecks(queue* q, bookcase* bc);
/* Checks the bookcase for invalid characters */
bool validChars(queue* q, bookcase* bc);
/* Checks number of shelves does not exceed colours of books */
bool sufficientShelves(queue* q, bookcase* bc);
/* Ensures no. of single colour books does not exceed spaces on shelf */
bool sufficientSpaces(queue* q, bookcase* bc);
/* Makes a histogram of colours for a bookcase */
void makeHist(queue* q, bookcase* bc, int* hist);
/* Prints a bookcase */
void bcPrint(bookcase* bc, int y, int x);
/* Copies a bookcase */
void bcCopy(bookcase* bc_dst, bookcase* bc_src);
/* Generates children for a parent, returns true if child is happy */
bool generateChildren(queue* q, bookcase* bc);
/* Finds the rightmost book on a shelf */
int findRightmostBook(bookcase bc, int y, int w);
/* Finds the leftmost space on a shelf */
int findLeftmostSpace(bookcase bc, int y, int w);
/* Swaps a book and a space */
void swap(bookcase* bc, int y1, int x1, int y2, int x2);
/* Checks if a bookcase which has passed func initialChecks is happy */ 
bool isHappy(bookcase* bc, int h, int w);
/* Checks if shelf is empty */
bool checkShelfEmpty(bookcase* bc, int y, int w);
/* Checks if same colour books exist on a shelf */
bool checkSameColour(bookcase* bc, int y, int w);
/* Adds a child bookcase on to the end of a list */
void bcEnqueue(queue* q, bookcase* bc, bookcase child);
/* Checks if a character is a valid coloured book */
bool isValidColour(char c);
/* Utility to print a solution depending on verbose flag */
void printSolution(queue* q, bookcase* bc, int nArgs, char** userInput);
/* Borrowed from Dr. Neill Campbell's notes on ADTs */
void* ncalloc(int n, size_t size);
/* Borrowed from Dr. Neill Campbell's notes on ADTs */
void* nfopen(char* fname, char* mode);
/* Borrowed from Dr. Neill Campbell's notes on ADTs */
void on_error(const char* s);


int main(int argc, char *argv[])
{
    queue* bcQ = queue_init();
    static bookcase bcList[MAXNUM];
    
    test();
    parseInput(bcQ, bcList, argc, argv);
    
    if (!isValidBookcase(bcQ, bcList)){
        free(bcQ);
        on_error("Invalid bookcase definition file.");
    }
    
    if(!initialChecks(bcQ, bcList)){
        free(bcQ);
        printf("No Solution?\n");
        return 0;
    }

    while(!generateChildren(bcQ, bcList)){
        bcQ->front++;
    };

    printSolution(bcQ, bcList, argc, argv);
    free(bcQ);
    return 0;
}

void test(void)
{   
    char c1;
    queue* tq = queue_init(1);
    queue* tq2 = queue_init(1);
    queue* tq3 = queue_init(1);
    queue* tq5 = queue_init(1);
    bookcase tbc1;
    bookcase tbc2;
    bookcase tbc3;
    bookcase tbc4;
    bookcase tbc5;
    bookcase tbcs[TESTNUM];

    /* Check return pointers */
    assert(tq != NULL);

    tq->h = 3;
    tq->w = 3;

    /* Check valid book colours */
    c1 = 'r'; assert(isValidColour(c1));
    c1 = 'R'; assert(isValidColour(c1));
    c1 = 'A'; assert(!isValidColour(c1));
    c1 = '!'; assert(!isValidColour(c1));
    c1 = ' '; assert(!isValidColour(c1));
    c1 = SPACE; assert(!isValidColour(c1));
    
    /* Checks for valid characters space or book */
    tbc1.svs[0][0] = 'R';
    tbc1.svs[0][1] = 'G'; 
    tbc1.svs[0][2] = SPACE;
    tbc1.svs[1][0] = 'R';
    tbc1.svs[1][1] = 'G'; 
    tbc1.svs[1][2] = SPACE;  
    tbc1.svs[2][0] = 'R';
    tbc1.svs[2][1] = 'G';
    tbc1.svs[2][2] = SPACE; 

    assert(validChars(tq, &tbc1));
    
    tbc1.svs[0][0] = 'r'; assert(validChars(tq, &tbc1));
    tbc1.svs[0][0] = 'g'; assert(validChars(tq, &tbc1));
    tbc1.svs[0][0] = 'b'; assert(validChars(tq, &tbc1));
    tbc1.svs[0][0] = 'X'; assert(!validChars(tq, &tbc1));
    tbc1.svs[0][0] = '!'; assert(!validChars(tq, &tbc1));
    tbc1.svs[0][0] = ' '; assert(!validChars(tq, &tbc1));

    tbc1.svs[0][0] = 'R'; 
    
    /* Checks for colours > width and colours > height */ 
    assert(sufficientShelves(tq, &tbc1));
    assert(sufficientSpaces(tq, &tbc1));  
    
    tbc1.svs[0][0] = 'G'; assert(!sufficientSpaces(tq, &tbc1));

    tbc1.svs[0][0] = 'R'; assert(initialChecks(tq, &tbc1));
    tbc1.svs[0][0] = 'G'; assert(!initialChecks(tq, &tbc1));

    tbc1.svs[0][0] = 'R';
    
    /* Checks to find rightmost books and leftmost spaces */
    assert((findRightmostBook(tbc1, 0, tq->w)) == 1);
    
    tbc1.svs[0][2] = 'R'; assert((findRightmostBook(tbc1, 0, tq->w)) == 2);

    tbc1.svs[0][0] = SPACE;
    tbc1.svs[0][1] = SPACE;
    tbc1.svs[0][2] = SPACE;
    assert((findRightmostBook(tbc1, 0, tq->w)) == NOTFOUND);
    
    tbc1.svs[0][0] = 'R';
    tbc1.svs[0][1] = 'R';
    tbc1.svs[0][2] = 'R';
    assert((findRightmostBook(tbc1, 0, tq->w)) == 2);
    assert((findLeftmostSpace(tbc1, 0, tq->w)) == NOTFOUND);

    tbc1.svs[0][1] = SPACE;
    assert((findRightmostBook(tbc1, 0, tq->w)) == 2);

    /* Test swap function */
    swap(&tbc1, 0, 2, 2, 2);
    
    assert(tbc1.svs[0][2] == SPACE);
    assert(tbc1.svs[2][2] == 'R');
    
    tbc2.svs[0][0] = SPACE;
    tbc2.svs[0][1] = SPACE;
    tbc2.svs[1][0] = 'G';
    tbc2.svs[1][1] = SPACE;

    tq2->h = 2;
    tq2->w = 2;

    assert(checkShelfEmpty(&tbc2, 0, tq2->w));
    assert(!checkShelfEmpty(&tbc2, 1, tq2->w));
    assert(checkSameColour(&tbc2, 1, tq2->w));
    
    /* Checks for bookcase happiness */
    
    tbc3.svs[0][0] = 'R';
    tbc3.svs[0][1] = 'R';
    tbc3.svs[0][2] = SPACE;
    tbc3.svs[1][0] = 'G';
    tbc3.svs[1][1] = 'G';
    tbc3.svs[1][2] = SPACE;

    tq3->h = 2;
    tq3->w = 3;
    
    assert(isHappy(&tbc3, 2, 3));
    
    tbc3.svs[1][0] = 'R';
    tbc3.svs[1][1] = 'R';

    assert(!isHappy(&tbc3, 2, 3));

    tbc3.svs[1][0] = SPACE;
    tbc3.svs[1][1] = SPACE;    
    
    assert(isHappy(&tbc3, 2, 3));
    
    tbc3.svs[1][0] = SPACE;
    tbc3.svs[1][1] = SPACE;
    
    assert(isHappy(&tbc3, 2, 3));

    tbc4.svs[0][0] = SPACE; assert(isHappy(&tbc4, 1, 1));
    tbc4.svs[0][0] = 'R'; assert(isHappy(&tbc4, 1, 1));
    tbc4.svs[0][0] = 'X'; assert(!isHappy(&tbc4, 1, 1));
    tbc4.svs[0][0] = 'r'; assert(isHappy(&tbc4, 1, 1));

    /* Checks for if books are not left aligned */
    tbc5.svs[0][0] = SPACE;
    tbc5.svs[0][1] = SPACE;
    tbc5.svs[1][0] = SPACE;
    tbc5.svs[1][1] = 'R';
    
    tq5->h = 2;
    tq5->w = 2;

    assert(!isValidBookcase(tq5, &tbc5));

    tbc5.svs[0][1] = 'G'; assert(!isValidBookcase(tq5, &tbc5));
    tbc5.svs[0][1] = 'R'; assert(!isValidBookcase(tq5, &tbc5));
    
    /* Checks for empty shelves and shelves of one colour */
    
    tbc5.svs[0][0] = 'R';
    tbc5.svs[0][1] = SPACE;
    tbc5.svs[0][2] = SPACE;
    tbc5.svs[1][0] = 'G';
    tbc5.svs[1][1] = 'R';
    tbc5.svs[1][2] = SPACE;
    tbc5.svs[2][0] = SPACE;
    tbc5.svs[2][1] = SPACE;
    tbc5.svs[2][2] = SPACE;

    assert(checkSameColour(&tbc5, 0, 3));
    assert(!checkSameColour(&tbc5, 1, 3));
    assert(checkShelfEmpty(&tbc5, 2, 3));
    assert(!checkShelfEmpty(&tbc5, 1, 3));
    assert(!checkShelfEmpty(&tbc5, 0, 3));
    
    /* Checking if children generated for tbc5 returns true */
    tq5->h = 3;
    tq5->w = 3;    
    tq5->size = 1;
    tq5->front = 0;
    tq5->end = 0;

    bcCopy(&tbcs[0], &tbc5);
    assert(generateChildren(tq5, tbcs));
    
    /* Iterates top to bottom, so 3 should be added to queue */
    assert(tq5->size = 4);
    assert(tq5->end = 3); 

    free(tq);
    free(tq2);
    free(tq3);
    free(tq5);
}

queue* queue_init()
{
    queue* q = (queue*) ncalloc(1, sizeof(queue)); 
    q->size = 0;
    q->front = 0;
    q->moves = 0;
    return q;
}

void bcArrInit(bookcase* bc, int n)
{
    int i, j, k;
    for (k = 0; k < n; k++)
        for (j = 0; j < HEIGHT; j++){
            for (i = 0; i < WIDTH; i++){
                bc[k].svs[j][i] = '\0';
                bc[k].parentIndex = NOPARENT;
            }
    }
}

void parseInput(queue* q, bookcase* bc, int nArgs, char** userInput)
{
    int y = 0, x = 0, h, w;
    char c;
    char buffer[WIDTH];
    FILE* fp;
    
    if (nArgs != 2 && nArgs != 3){
        on_error("Usage:./bookcase filename verbose(optional)");
    }

    fp = nfopen(userInput[1], "r");
    
    /* Reads in bookcase dimensions */
    fgets(buffer, sizeof(buffer), fp);
    sscanf(buffer, "%d %d", &h, &w);

    if (h <= 0 || w <= 0 || h > 9 || w > 9){
        on_error("Bookshelf dimensions should be between 1 and 9");
    }

    /* Reads in books and spaces */
    while((c = fgetc(fp)) != EOF){
        if (c != '\n'){
            bc[q->front].svs[y][x++] = c;
        }
        if (c == '\n')
        {
            if (x != w){
                on_error("Bookcase width not correctly defined.");
            }
            x = 0;            
            y++;
        }   
    }
    
    if(y != h){
        on_error("Bookcase height not correctly defined.");
    }
    /* Updates parameters of bookcase list */
    q->h = h;
    q->w = w;
    bc[q->front].parentIndex = NOPARENT;
    q->size++;
}

bool isValidBookcase(queue* q, bookcase *bc)
{
    int y, x, h, w, validBook;
    bookcase temp;    
    
    h = q->h;
    w = q->w;

    bcCopy(&temp, &bc[0]);
    
    /* Checks for chars apart from colours and spaces */
    if (!validChars(q, &temp)){
        return false;
    }
    
    /* Looks for books with spaces in between them */
    for (y = 0; y < h; y++){
        validBook = findRightmostBook(temp, y, w);
        if (validBook > 0){
            for (x = validBook; x >= 0; x--){
                if (temp.svs[y][x] == SPACE){
                    return false;
                }
            }
        }
    }
    return true;
}

bool initialChecks(queue* q, bookcase* bc)
{
    if (sufficientShelves(q, bc) && sufficientSpaces(q, bc)){
            return true;
    }
    return false;    
}

bool validChars(queue* q, bookcase* bc)
{
    int h, w, y, x;
    char c;
    
    h = q->h;
    w = q->w;

    /* Looks for spaces and valid books */
    for (y = 0; y < h; y++){
        for (x = 0; x < w; x++){
            c = bc->svs[y][x];
            if(!isValidColour(c) && c != SPACE){
                return false;
            }
        }
    }
    return true;
}

bool sufficientShelves(queue* q, bookcase* bc)
{
    int h, x;
    int colourCnt = 0;
    int colourHist[MAXCHAR] = {0};
    
    h = q->h;

    makeHist(q, bc, colourHist);
    
    /* Counts the number of book colours in bookcase */
    for (x = 0; x < MAXCHAR; x++){
        if (colourHist[x] != 0){
            colourCnt++;
        }
    }
    if(colourCnt > h){
        return false;
    }
    return true;
}

bool sufficientSpaces(queue* q, bookcase* bc)
{
    int maxW, x;
    int colourHist[MAXCHAR] = {0};
    int maxColour;
    
    maxW = q->w;

    makeHist(q, bc, colourHist);
    maxColour = colourHist[0];
    
    /* Finds the colour with the highest number of books */
    for (x = 0; x < MAXCHAR; x++){
        if (colourHist[x] > maxColour){
            maxColour = colourHist[x];
        }
    }
    if (maxColour > maxW){
        return false;
    }     
    
    return true;
}

void makeHist(queue* q, bookcase* bc, int* hist)
{
    int h, w, y, x;
    char c;
    
    h = q->h;
    w = q->w;

    /* Generates a histogram for lower and uppercase */
    for (y = 0; y < h; y++){
        for (x = 0; x < w; x++){
            c = bc->svs[y][x];
            if(isValidColour(c)){
                c = toupper(c);
                hist[c - 'A']++;
            }
        }
    }
}

void bcPrint(bookcase* bc, int y, int x)
{
    char c;
    int i, j;
    for (j = 0; j < y; j++){
        for (i = 0; i < x; i++){
            c = bc->svs[j][i];
            neillbgcol(black);
            if (c == 'K' || c == 'k'){
                neillbgcol(white);
                neillfgcol(black);
            }
            if (c == 'R' || c == 'r'){
                neillfgcol(red);
            }
            if (c == 'G' || c == 'g'){
                neillfgcol(green);
            }
            if (c == 'Y' || c == 'y'){
                neillfgcol(yellow);
            }
            if (c == 'B' || c == 'b'){
                neillfgcol(blue);
            }
            if (c == 'M' || c == 'm'){
                neillfgcol(magenta);
            }
            if (c == 'C' || c == 'c'){
                neillfgcol(cyan);
            }
            if (c == 'W' || c == 'w'){
                neillfgcol(white);
            }
            if (c == SPACE){
                neillfgcol(white);
            }
            printf("%c", c);
        }
        neillreset();
        printf("\n");
    }

    printf("\n");
}

void bcCopy(bookcase* bc_dst, bookcase* bc_src)
{
    memcpy(bc_dst, bc_src, sizeof(bookcase));
}

bool generateChildren(queue* q, bookcase* bc)
{
    int y, j, validBook, freeSpace, h, w;

    bookcase parent;
    bookcase child;
    
    bcCopy(&parent, &bc[q->front]);
    bcCopy(&child, &parent);
    
    h = q->h;
    w = q->w;

    if (isHappy(&parent, h, w)){
        return true; 
    }

    for (y = 0; y < h; y++){
        validBook = findRightmostBook(parent, y, w);
        if (validBook != NOTFOUND){
            for (j = 0; j < h; j++){
                if (j != y){
                    freeSpace = findLeftmostSpace(parent, j, w);
                    if (freeSpace != NOTFOUND) {
                        swap(&child, y, validBook, j, freeSpace);
                        bcEnqueue(q, bc, child);
                        if (isHappy(&child, h, w)){
                            return true;
                        }
                        swap(&child, y, validBook, j, freeSpace);
                    }   
                }
            }
        }    
    }
    return false;
}

int findRightmostBook(bookcase bc, int y, int w)
{
    int x;

    for (x = w-1; x >= 0; x--){    
        if (isValidColour(bc.svs[y][x])){
            return x;
        }                
    }
    return NOTFOUND;
}

int findLeftmostSpace(bookcase bc, int y, int w)
{
    int x;

    for (x = 0; x < w; x++){
        if (bc.svs[y][x] == SPACE){
            return x;
        }
    }
    return NOTFOUND;
}

void swap(bookcase* bc, int y1, int x1, int y2, int x2)
{
    char tmp;
    tmp = bc->svs[y1][x1];
    bc->svs[y1][x1] = bc->svs[y2][x2];
    bc->svs[y2][x2] = tmp;
}

bool isHappy(bookcase* bc, int h, int w)
{
    int y;
    char c;
    int str[MAXCHAR] = {0};
    
    /* This procedure assumes that invalid bookcases and
       bookcases with no solutions have been trapped 
       earlier in the program */ 
    
    for (y = 0; y < h; y++){
        if(!checkShelfEmpty(bc, y, w)){
            if(!checkSameColour(bc, y, w)){
                return false;
            }
            else{
                c = toupper(bc->svs[y][0]);
                if (str[c -'A'] != 0){
                    return false;
                }
                else{
                    str[c - 'A']++;
                }
            }
        }
    }
    return true;
}

bool checkShelfEmpty(bookcase* bc, int y, int w)
{
    int x;

    for (x = 0; x < w; x++){
        if (bc->svs[y][x] != SPACE){
            return false;
        }
    }
    return true;
}

bool checkSameColour(bookcase* bc, int y, int w)
{
    int x;
    char bookColour;
    char currColour;

    bookColour = bc->svs[y][0];

    /* If the bookcase has a width of 1 */
    if (w == 1){
        if (bookColour == SPACE){
            return true;
        }
        if(!isValidColour(bookColour)){
            return false;
        }
    }    
    
    for(x = 0; x < w; x++){
        currColour = bc->svs[y][x];
        if(isValidColour(currColour)){
            if(currColour != bookColour){
                return false;
            }
        }
    }
    return true;
}

void bcEnqueue(queue* q, bookcase* bc, bookcase child)
{   
    if (q->size >= MAXNUM){
        free(q);
        printf("No Solution?\n");
        exit(EXIT_SUCCESS);
    }
    
    child.parentIndex = q->front;
    
    bcCopy(&bc[q->size], &child);
    
    q->size++;
    q->end++;
}

bool isValidColour(char c)
{
    if (islower(c)){
        c = toupper(c);
    }
    switch(c)
    {
        case 'K':
            return true;
            break;
        case 'R':
            return true;
            break;        
        case 'G':
            return true;
            break;
        case 'Y':
            return true;
            break;
        case 'B':
            return true;
            break;
        case 'M':
            return true;
            break;
        case 'C':
            return true;
            break;
        case 'W':
            return true;
            break;
        default:
            return false;
    }
}

void printSolution(queue* q, bookcase* bc, int nArgs, char** userInput)
{
    int i, h, w;  
    
    h = q->h;
    w = q->w;

    i = q->end;
    
    while (i != NOPARENT){
        i = bc[i].parentIndex;
        q->moves++;
    }
    printf("%d\n", q->moves);

    /* If verbose option selected */
    if (nArgs == 3 && !strcmp("verbose", userInput[2])){
        i = q->end;
        printf("\n");
        while (i != NOPARENT){
            bcPrint(&bc[i], h, w);
            i = bc[i].parentIndex;
        }    
    }
}

void* ncalloc(int n, size_t size)
{
    void* v = calloc(n, size);
    if (v == NULL){
        on_error("Cannot calloc() space");
    }
    return v;
}

void* nfopen(char* fname, char* mode)
{
    FILE* fp;
    fp = fopen(fname, mode);
    if (!fp){
        on_error("Cannot open file.");
    }
    return fp;
}

void on_error(const char* s)
{
   fprintf(stderr, "%s\n", s);
   exit(EXIT_FAILURE);
}
