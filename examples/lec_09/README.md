### Пример 09.01. Выбор шаблона (специализации) подстановкой параметров (инстанцирование).
В каждом случае программа выводит:
```
1.5
1.5
```
Последние примеры надо компилировать с флагами более поздних стандартов, например: `-std=c++20`.

### Пример 09.02. Использование простого концепта.
Аналогично, у меня всё скопмилировалось с `-std=c++20`.
Во всех случаях выводит:
```
1
2
3
4
5
```

### Пример 09.03. Варианты использования концепта.
Выводит `i = 1`. С раскомменченой последней строкой не компилится ни в одном из примеров.
###### TODO

### Пример 09.04. Концепты с составными ограничениями по типу выражений.
Выводит `false`.
###### TODO

### Пример 09.05. Использование концепта с несколькими параметрами.
Выводит:
```
true
true
```

### Пример 09.06. Концепт с вариативным количеством параметров.
Выводит `15`.

### Пример 09.07. Концепты и инстанцирование. 
```
/tmp/ccxpRrl9.s: Assembler messages:
/tmp/ccxpRrl9.s:4403: Error: symbol `_Z1fISt6vectorIiSaIiEEEvT_' is already defined
```
###### TODO

### Пример 09.08. Инстанцирование шаблонов методов класса.
Выводит: `Creating float object`

### Пример 09.09. Ограничения для шаблонов классов, использование дизъюнкции.
Выводит:
```
true
false
```

### Пример 09.10. Шаблон Holder.
```
Function f of class A is called
```

### Пример 09.11. Применение unique\_ptr.
```
Constructor
Constructor
Destructor
Function f
Destructor
```

### Пример 09.12. Установка deleter для unique\_ptr на примере закрытия файла.
Если файл `test.txt` существует, выводит:
```
file is open
file is closed
```
иначе:
```
File not found!
```

### Пример 09.13. Утечка памяти при использовании shared\_ptr.
```
Example 1
BadWidget 1 First
BadWidget 1 Second
1 First now points to 1 Second
1 Second now points to 1 First
Example 2
GoodWidget 2 First
GoodWidget 2 Second
2 First now points to 2 Second
2 Second now points to 2 First
~GoodWidget 2 Second
~GoodWidget 2 First
```

### Пример 09.14. Возврат shared\_ptr на себя.
```
Constructor
good1.use_count() = 2
Constructor
Destructor
Destructor
bad_weak_ptr
```

### Пример 09.15. Возврат shared\_ptr на член данное объекта.
```
value = 10
```

### Пример 09.16. Реализация хранителя unique\_ptr.
```
Calling the constructor of class A (obj1);
Calling the constructor of class A (obj2);
Method f;
Method f;
Calling a class A destructor (obj2);
Method f;
Calling a class A destructor (obj1);
```

### Пример 09.17. Реализация shared\_ptr и weak\_ptr.
```
Calling the constructor of class A (obj1);
Method f;
Method f;
2
4
Calling the constructor of class A (obj2);
Method f;
Calling a class A destructor (obj2);
Calling a class A destructor (obj1);
```
