/* Autor: Rafal Uzarowicz, 300282
 * Zawartosc: Program wypisujacy skrypt potrzebny do usuniecia duplikatow z jednego folderu i podfolderow
 * i utworzenia na ich miejsce twardych linkow.*/

#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <stdlib.h>
#include <errno.h>

#define MAX_NAME 50

// FilesList: Lista jednokierunkowa wykorzystywana do zapisu sciezek, nazw i wielkosci plikow w liscie.
struct FilesList{
    char* file;
    char* path;
    long long size;
    struct FilesList* next;
};
typedef struct FilesList FilesList;

/* listFiles: Funkcja przyjmuje sciezke do glownego folderu, wskaznik na wskaznik na glowe listy, wskaznik na
 * glowe listy nowej oraz zmienna "flag", ktora decyduje o rozpatrywaniu danego folderu jako folder bazowy
 * lub jako folder nowy. Funkcja wpisuje do kolejnych elementow listy plikow odpowiednie wartosci i w
 * sposob rekurencyjny przeszukuje wszystkie podfoldery.*/
void listFiles( char *basePath, FilesList** headCurr, FilesList* new, int flag );
/* addToList: Funkcja ktora przyjmuje wskaznik na wskaznik na glowe listy, folder w ktorym znajduje sie plik
 * oraz jego nazwe a nastepnie dodaje go do odpowiedniej listy plikow FilesList.*/
void addToList( FilesList** head, char path[], char name[] );
/* findDupli: Funkcja przyjmuje dwa wskazniki na glowy do dwoch listy plikow FilesList, a nastepnie
 * bierze pierwszy element z listy plikow nowych i porownuje z plikami bazowymi. Jesli plik sa takie same
 * pod wzgledem zawartosci to funkcja pisze na standardowe wyjscie odpowiedni skrypt.*/
void findDupli( FilesList* main, FilesList* new );
/* writeOut: Funkcja przyjmuje sciezki do pliku bazowego i do jego duplikatu a nastepnie wypisuje odpowiednie komendy
 * do usuwania duplikatu i tworzenia twardego linku na standardowe wyjscie.*/
void writeOut( char* pathMain, char* pathNew, int flag );
/*freeList: Funkcja zwalnia zaalokowana na liste jednokoerunkowa pamiec.*/
void freeList( FilesList* head );
/* checkDir: Funkcja przyjmuje sciezke bazowa i nowa, a nastepnie sprawdza czy sa to poprawne sciezki oraz czy istnieja.
 * W przypadku niepoprawnosci funkcja wypisuje odpowiedni komunikat.*/
int checkDir( char* pathMain, char* pathNew );
/* checkPath: Funkcja przyjmuje sciezke oraz glowe listy plikow doleru nowego, a nastepnie sprawdza czy podana sicezka
 * nie wystepuje juz na tej liscie. Funkcja zabezpiecza przed sytuacja w ktorej folder nowy jest podfolderem
 * folderu bazowego. Zwraca 0 jesli folder wystepuje, a 1 gdy nie wystepuje.*/
int checkPath( char* path, FilesList* head);
/* compareFiles: Funkcja przyjmuje dwa wskazniki na elementy listy plikow, a nastepnie porownuje binarnie pliki
 * ktore sa opisane w danych elementach.*/
int compareFiles( FilesList* main, FilesList* new );

int main(int argc, char *argv[])
{
    // Glowy dwoch list plikow - folderu nowego i bazowego.
    FilesList* headMain=NULL;
    FilesList* headNew=NULL;
    if( argc==3 ){
        if( checkDir( argv[1], argv[2] ) ){
            // Funkcja przyjmuje 1 gdy ma być uzyta dla folderu nowego i 0 gdy dla bazowego.
            listFiles( argv[2], &headNew, headNew, 1 );
            listFiles( argv[1], &headMain, headNew, 0 );
            findDupli( headMain, headNew );
            if( ENOENT == errno ){
                puts( "Nie udalo sie otworzyc sciezki." );
            }
            // prawdzamy czy po uzyciu funkcji findDupli lista bazowa jest pusta. Jesli tak to znaczy, ze
            // folder bazowy byl podfolderem folderu nowego.
            if( !headMain && headNew ){
                puts( "Folder bazowy nie moze byc podfolderem folderu nowego." );
            }
            // Zwalnianie zaalokowanej pamieci.
            freeList( headNew );
            freeList( headMain );
        }
    }else{
        puts( "Podano nieprawidlowa ilosc sciezek." );
    }
    return 0;
}

void listFiles( char *basePath, FilesList** headCurr, FilesList* new, int flag )
{
    char* path = malloc( sizeof( char ) * ( strlen( basePath ) + MAX_NAME + 2 ) );
    if( !path ){
        puts( "Brak pamieci." );
        exit( EXIT_FAILURE );
    }
    struct dirent *dp = NULL;
    DIR *dir = opendir( basePath );
    if ( dir ){
        while ( ( dp = readdir( dir ) ) != NULL )
        {
            if( strcmp( dp->d_name, "." ) != 0 && strcmp( dp->d_name, ".." ) != 0)
            {
                strcpy( path, basePath );
                if( flag && dp->d_type != DT_DIR ){
                    // Dodawanie plikow z folderu nowego.
                    addToList( headCurr, path, dp->d_name );
                }else{
                    if( dp->d_type != DT_DIR && checkPath( path, new ) ){
                        // Dodawanie plikow z folderu bazowego.
                        addToList( headCurr, path, dp->d_name );
                    }
                }
                strcat( path, "/" );
                strcat( path, dp->d_name );
                listFiles( path, headCurr, new, flag );
            }
        }
    }
    closedir( dir );
}
void addToList( FilesList** head, char path[], char name[] ){
    char* fileDir = malloc( ( strlen( path ) + strlen( name ) + 2 ) * sizeof( char ) );
    if( !fileDir ){
        puts( "Brak pamieci." );
        exit( EXIT_FAILURE );
    }
    strcpy( fileDir, path );
    strcat( fileDir, "/" );
    strcat( fileDir, name );
    long long size = 0;
    // Ustalanie wielkosci pliku.
    FILE *file = fopen( fileDir, "rb" );
    if ( file )
    {
        fseek( file, 0, SEEK_END );
        size = ftell( file );
        if( -1 == size ){
            printf( "\nBlad ustalania wielkosci pliku: %s\n", fileDir );
        }
        fclose( file );
    }
    // Do listy wpisujemy tylko pliki. Foldery maja wielkosc 0.
    if( size > 0 ){
        char* filePath = malloc( ( strlen( path ) + 2 ) * sizeof( char ) );
        if( !filePath ){
            puts( "Brak pamieci." );
            exit( EXIT_FAILURE );
        }
        strcpy( filePath, path );
        strcat( filePath, "/" );
        FilesList* new = malloc( sizeof( FilesList ) );
        if( !new ){
            puts( "Brak pamieci." );
            exit( EXIT_FAILURE );
        }
        new->size = size;
        new->path = filePath;
        new->file = fileDir;
        new->next = *head;
        *head = new;
    }else{
        free( fileDir );
    }
	fclose( file );
}
void findDupli( FilesList* main, FilesList* new ){
    FilesList* current = main;
    // Zmienna kontrolujaca aby wypisany skrypt byl prawidlowo napisany czyli nie zaczynal sie od "&&".
    int flag = 0;
    // Przechodzimy po wszystkich elementach listy new i szukamy duplikatow.
    while( new ){
        while( current ){
            if( new->size == current->size && compareFiles( current, new ) ){
                writeOut( current->file, new->file, flag );
                flag = 1;
            }
            current = current->next;
        }
        current = main;
        new = new->next;
    }
    puts( "" );
}
void writeOut( char* pathMain, char* pathNew, int flag ){
    if( flag ){
        printf(" && ");
    }
    printf( "rm %s && ln %s %s", pathNew, pathMain, pathNew );
}
void freeList( FilesList* head ){
    FilesList* tmp = NULL;
    while( head ){
        tmp = head;
        head = head->next;
        free( tmp->file );
        free( tmp );
    }
}
int checkDir( char* pathMain, char* pathNew ){
    // Zmienna kontrolujaca wystapienie bledu.
    int errInt = 1;
    // Podane foldery musza zaczynac sie od "." po ktorej musi nastapic "/" albo nic.
    if( pathMain[0] == '.' && pathNew[0] == '.' ){
        if( strlen( pathMain ) > 1 && pathMain[1] != '.' && pathMain[1] != '/' ){
            puts( "Sciezka bazowa jest niepoprawna." );
            errInt = 0;
        }
        if( strlen( pathNew ) > 1 && pathNew[1] != '.' && pathNew[1] != '/' ){
            puts( "Sciezka nowa jest niepoprawna." );
            errInt = 0;
        }
        if( errInt != 0){
            DIR* dir = opendir( pathMain );
            if( ENOENT == errno )
            {
                puts( "Sciezka folderu bazowego nie istnieje." );
                errno = 0;
                errInt = 0;
            }
            closedir( dir );
            dir = opendir( pathNew );
            if( ENOENT == errno )
            {
                puts( "Sciezka folderu nowego nie istnieje." );
                errno = 0;
                errInt = 0;
            }
            closedir( dir );
        }
    }else{
        puts( "Podano nieprawidlowe sciezki." );
        errInt = 0;
    }
    return errInt;
}
int checkPath( char* path, FilesList* head ){
    // Zmienna do kontrolowania czy dany folder wystepuje.
    int flag = 1;
    char* pathExt = malloc( ( strlen( path ) + 2 ) * sizeof( char ) );
    if( !pathExt ){
        puts( "Brak pamieci." );
        exit( EXIT_FAILURE );
    }
    strcpy( pathExt, path );
    strcat( pathExt, "/" );
    while( head && flag ){
        if( !strcmp( pathExt, head->path ) ){
            flag = 0;
        }
        head = head->next;
    }
    free( pathExt );
    return flag;
}
int compareFiles( FilesList* main, FilesList* new )
{
    FILE *fileMain,*fileNew;
    // Zmienne pomocnicze do porownywania aktualnych bitow.
    char byteMain, byteNew;
    fileMain = fopen( main->file, "rb" );
    fileNew = fopen( new->file, "rb" );
    for( long long i=0 ; i < main->size ; ++i ) {;
        // Czytamy po jednym bicie po jednym elemencie.
        fread( &byteMain, 1, 1, fileMain );
        fread( &byteNew, 1, 1, fileNew );
        if ( byteMain != byteNew ) {
            // Pliki sa rozne.
            return 0;
        }
    }
    fclose( fileMain );
    fclose( fileNew );
    // Pliki sa takie same.
    return 1;
}
