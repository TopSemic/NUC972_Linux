
#include <stdio.h>
#include <string.h>
 
int main(int argc, char *argv[])
{
    printf("Content-type:text/html\n\n");
    printf("<html>\n");
    printf("<head><title>An html page from a cgi</title></head>\n");
    printf("<body>\n");
    printf("<h1>Board.Info</h1>\n");
    printf("</body>\n");
    printf("</html>\n");
    fflush(stdout);
    return 0;
}

