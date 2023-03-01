# Dish washing

Передача посуды от процесса к процессу реализована через разделяемую память

## Запуск
Для запуска программы нужно указать 3 параметра (в указанном порядке): `path_to_file_with_dirty_dish`, `path_to_file_with_washer_process` и `path_to_file_with_wiper_process`

## Условие задачи
На мойке посуды в ресторане работают два человека. Один из них моет посуду, второй вытирает уже вымытую. Времена выполнения операций мытья и вытирания посуды меняются в зависимости от того, что моется. Стол для вымытой, но не вытертой посуды имеет ограниченные размеры. Смоделируйте процесс работы персонала следующим образом: каждому работнику соответствует свой процесс. Времена выполнения операций содержатся в двух файлах. Каждый файл имеет формат записей:
<тип посуды> : <время операции>
Стол вмещает N предметов независимо от их наименования. Значение N задается как параметр среды TABLE_LIMIT перед стартом процессов. Грязная посуда, поступающая на мойку, описывается файлом с форматом записи:
<тип посуды> : <количество предметов>
Записи с одинаковым типом посуды могут встречаться неоднократно
