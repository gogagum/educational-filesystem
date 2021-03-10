/*
 * Заметка:
 * Пусть блоков для файла доступно 15. Тогда размер файла 15 * 4096 = 61440 байт.
 * Быть может, это временное решение.
 * Адресуется 16-битными адресами. 
 * Максимальный размер файловой системы - 2^16 * 4096 байт = 256мб.
 * Помимо всего прочего, файлов всего не более 2^16 = 65536
 * В начале файла будет список inode, записанных подряд.
 * Всего штщву максимум столько же, сколько и файлов - 2^16.
 * Они будут занимать статический размер - 2^16 * 32 байт = 2^21 байт = 2^11 кбайт = 2мб.
 * В нвчале надо будет указать размер от 0 до 256 мб в килобайтах, который обрежется по размеру блока
 * (округлится вниз).
 * UPD: свободные блоки будем поддерживать след. образом: в самом начале файла будем хранить 2 числа: первый блок,
 * после которого все свободны, и указатель на первый блок, который будет "Освободившимся". 
 * "Освободившиеся" будут составлять стек.
 */

struct inode
{
    uint16_t[15] blocks;
    uint8_t type;  
    uint8_t rights;
    // TODO
}

// TODO: убедиться, что структура "Упакуется", то есть будет занимать аккурат 32 бита.