#include <Python.h>


using namespace std;


void exec(char *commandes);

int main(int argc,char *argv[])
{
    Py_Initialize();
    exec("print 'test'"); // Affiche "test"

    exec("a=3\n\
print str(a)"); //Waouh ! Ca fonctionne sur plusieurs lignes !

    exec("b=5");
    exec("print str(b)"); //Ca fonctionne même avec un appel différent à exec !
    system("pause");
    return 0;
}

void exec(char *commandes)
{
    PyRun_SimpleStringFlags(reinterpret_cast<const char*>(commandes),NULL);
}
