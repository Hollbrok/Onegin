#include "Sortline.h"

int main()//int main(int argc, char *argv[]) для запуска через консоль
{

    //FILE *onegin = fopen(argv[1], "r"); //чтобы через консоль указать какой файл открыть
    //FILE *res = fopen(argv[2], "w");    //чтобы через консоль указать в какой файл записать

    //setlocale(LC_ALL, "Russian");

    FILE *onegin = fopen("source.txt", "r");
    FILE *res    = fopen("res.txt",    "w");

    assert(res != NULL);
    assert(onegin != NULL);

    LineProcessing(onegin, res, "decrease_end", "yes"); //yes - делать результат бредогенерации
    //LineProcessing(onegin, res, argv[3], argv[4]);    //для того, чтобы через консоль выбрать как сортировать

    fclose(onegin);
    fclose(res);

    printf("ONEGIN COMPLITE");
    //Sort_test(); // нужно будет добавить проверку

    return 0;
}
