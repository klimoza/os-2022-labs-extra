# Лабораторная работа № 3
Автор: Климов Иван, 20.Б10-мкн

## **Часть I**

### **Задание 1**
Очевидно FIFO просто выполнит последовательно все три job-а, начиная в моменты 0, 200, 400. Поэтому response-time для них будут равны соответственно 0, 200, 400, а turnaround-time будут равны 200, 400, 600. Запуск scheduler-а подтверждает эту догадку:
```console
lab3 git:(main) ✗ ./scheduler.py -p FIFO -l 200,200,200 -c

Final statistics:
  Job   0 -- Response: 0.00  Turnaround 200.00  Wait 0.00
  Job   1 -- Response: 200.00  Turnaround 400.00  Wait 200.00
  Job   2 -- Response: 400.00  Turnaround 600.00  Wait 400.00
```

Для SJF будет аналогичный результат, так как все задачи приходят в одно и то же время(в момент времи 0) и при этом имют одинаковое время до окончания(а значит все будут выполняться в том же порядке что и в FIFO). Проверим, запустив scheduler:
```console
lab3 git:(main) ✗ ./scheduler.py -p SJF -l 200,200,200 -c

Final statistics:
  Job   0 -- Response: 0.00  Turnaround 200.00  Wait 0.00
  Job   1 -- Response: 200.00  Turnaround 400.00  Wait 200.00
  Job   2 -- Response: 400.00  Turnaround 600.00  Wait 400.00
```

### **Задание 2**
Очевидно, что так как все задачи приходят в одно и то же время, то в FIFO responce-time для задачи равен сумме всех длин задач, которые были завершены строго до текущей. В данном случае это будет соответственно 0, 100, 300. При это turnaround-time будет равен responce-time + время выполнения конкретной задачи, так как выполнение задачи в FIFO не прерывается:
```console
➜  lab3 git:(main) ✗ ./scheduler.py -p FIFO -l 100,200,300 -c

Final statistics:
  Job   0 -- Response: 0.00  Turnaround 100.00  Wait 0.00
  Job   1 -- Response: 100.00  Turnaround 300.00  Wait 100.00
  Job   2 -- Response: 300.00  Turnaround 600.00  Wait 300.00
```

Заметим, что для SJF результаты будут аналогичные. Так как задачи даны в порядке возрастания времени выполнения, а значит ровно в таком порядке они и будут выполняться:
```console
➜  lab3 git:(main) ✗ ./scheduler.py -p SJF -l 100,200,300 -c

Final statistics:
  Job   0 -- Response: 0.00  Turnaround 100.00  Wait 0.00
  Job   1 -- Response: 100.00  Turnaround 300.00  Wait 100.00
  Job   2 -- Response: 300.00  Turnaround 600.00  Wait 300.00
```

### **Задание 3**
Очевидно, что раз длина задач одинакова, то выполнение наших задач выглядит как ABCABC...ABC, поэтому response-time равны 0, 1, 2, так как ровно так мы запускаем их в RR, а turnaround-time равны 598, 599, 600, так как turnaround-time для последней выполненной задачи всегда равно сумме длин всех задач, а все остальные задачи идут перед ней в понятном порядке(последовательно), а значит для вычисления их turnaround-time мы можем просто повычитать столько квантумов сколько нужно:
```console
➜  lab3 git:(main) ✗ ./scheduler.py -p RR -l 200,200,200 -q 1 -c

Final statistics:
  Job   0 -- Response: 0.00  Turnaround 598.00  Wait 398.00
  Job   1 -- Response: 1.00  Turnaround 599.00  Wait 399.00
  Job   2 -- Response: 2.00  Turnaround 600.00  Wait 400.00
```

### **Задание 4**
Очевидно одинаковый turnaround-time происходит, когда работы приходят в порядке неубывания времени выполнения, так как в таком случае порядок их выполнения в FIFO и в SJF совпадает.

В обратную сторону, если монотонность не выполнена, то какая-то из работ в SJF будет выполнена раньше, чем в FIFO, а значит turnaroud-time будет отличаться.

### **Задание 5**
Заметим, что в RR response-time мы знаем однозначно, это i * quantum для i-ый по порядку задачи(в 0-индексации). Тогда во-первых раз в RR задачи начинают выполняться в порядке увеличения номера, то для совпадения с SJF и подаваться они должны в порядке неубывания времени работы.

При этом заметим, что SJF выполняет все работы всегда до конца, поэтому для того чтобы response-time совпадал с RR длины всех работ кроме последней должны быть равны quantum length(время работы последней неважно, кроме конечно условия на то что оно должно быть не меньше чем время работы предпоследней).

### **Задание 6**
Так как SJF по определению выполняет задачи по неубыванию времени работы, то при увеличении длины разумеется место задачи в очереди выполнения увеличивается(или, по крайней мере, не уменьшается), поэтому и response-time, то есть время первого обращения к задаче не уменьшается(а иногда даже увеличивается). При этом разумеется для последующих задач response-time увеличивается на ту дельту, на которую изменилась длина нашей.

Посмотрим на примерах, изначальные длины:
```console
➜  lab3 git:(main) ✗ ./scheduler.py -p SJF -l 100,200,300 -c

Final statistics:
  Job   0 -- Response: 0.00  Turnaround 100.00  Wait 0.00
  Job   1 -- Response: 100.00  Turnaround 300.00  Wait 100.00
  Job   2 -- Response: 300.00  Turnaround 600.00  Wait 300.00
```
Теперь увеличим длину первой работы:
```console
➜  lab3 git:(main) ✗ ./scheduler.py -p SJF -l 250,200,300 -c

Final statistics:
  Job   1 -- Response: 0.00  Turnaround 200.00  Wait 0.00
  Job   0 -- Response: 200.00  Turnaround 450.00  Wait 200.00
  Job   2 -- Response: 450.00  Turnaround 750.00  Wait 450.00
```
Видим, что теперь первой выполняется вторая работа, а первая выполняется второй. Снова изменим длину для Job 0:
```console
➜  lab3 git:(main) ✗ ./scheduler.py -p SJF -l 300,200,300 -c

Final statistics:
  Job   1 -- Response: 0.00  Turnaround 200.00  Wait 0.00
  Job   0 -- Response: 200.00  Turnaround 500.00  Wait 200.00
  Job   2 -- Response: 500.00  Turnaround 800.00  Wait 500.00
```
А теперь порядок выполнения работ не изменился(и даже response-time для первой работы), зато изменился response-time для последней работы, которая в абсолютных(но не относительных) величинах начала выполняться позже.

### **Задание 7**
Мы знаем response-time для i-ой работы, оно равно i * (quantum length). Тогда очевидно, что при увеличении quantum legnth на x, response-time для i-ой работы увеличивается на i * x. Отвечая на второй вопрос задачи: response-time очевидно максимален у задачи, имеющей максимальный номер, то есть равен (N - 1) * (quantum length).

## **Часть II**

### **Задание 1**
Решим с сидом 1:
```console
➜  os-2022-labs-klimoza git:(main) ✗ ./lottery.py -j 3 -s 1
ARG jlist
ARG jobs 3
ARG maxlen 10
ARG maxticket 100
ARG quantum 1
ARG seed 1

Here is the job list, with the run time of each job:
  Job 0 ( length = 1, tickets = 84 )
  Job 1 ( length = 7, tickets = 25 )
  Job 2 ( length = 4, tickets = 44 )


Here is the set of random numbers you will need (at most):
Random 651593
Random 788724
Random 93859
Random 28347
Random 835765
Random 432767
Random 762280
Random 2106
Random 445387
Random 721540
Random 228762
Random 945271
```
Буду выписывать шаги алгоритма в следующем формате:
```
total = n, ticket = a % n = x
Run i => ans = (l1, l2, l3);
```
Где ```total``` -- количество билетов на текущем шаге, ```ticket``` -- билет который победил, ```a``` -- рандомное число на текущем шаге, ```i``` -- номер задачи, которую будем запускать на текущем шаге, ```ans``` -- тройка чисел, равных соответствующим длинам(оставшимся) каждой из работ.

```
1. total = 153, ticket = 651593 % 153 = 119
Run 2 => ans = (1, 7, 3)

2. total = 153, ticket = 788724 % 153 = 9
Run 0 => ans = (0, 7, 3)

3. total = 69, ticket = 93859 % 69 = 19
Run 1 => ans = (0, 6, 3)

4. total = 69, ticket = 28347 % 69 = 57
Run 2 => ans = (0, 6, 2)

5. total = 69, ticket = 835765 % 69 = 37
Run 2 => ans = (0, 6, 1)

6. total = 69, ticket = 432767 % 69 = 68
Run 2 => ans = (0, 6, 0)

7. total = 25, ticket = 762280 % 25 = 5
Run 1 => ans = (0, 5, 0)

8. total = 25, ticket = 2106 % 25 = 6
Run 1 => ans = (0, 4, 0)

9. total = 25, ticket = 445387 % 25 = 12
Run 1 => ans = (0, 3, 0)

10. total = 25, ticket = 721540 % 25 = 15
Run 1 => ans = (0, 2, 0)

11. total = 25, ticket = 228762 % 25 = 12
Run 1 => ans = (0, 1, 0)

12. total = 25, ticket = 945271 % 25 = 21
Run 1 => ans = (0, 0, 0)
```
Таким образом работа 0 закончилась после шага 2, работа 2 закончилась после шага 6, а работа 1 закончилась после шага 12.

Решим с сидом 2:
```console
➜  os-2022-labs-klimoza git:(main) ✗ ./lottery.py -j 3 -s 2
ARG jlist
ARG jobs 3
ARG maxlen 10
ARG maxticket 100
ARG quantum 1
ARG seed 2

Here is the job list, with the run time of each job:
  Job 0 ( length = 9, tickets = 94 )
  Job 1 ( length = 8, tickets = 73 )
  Job 2 ( length = 6, tickets = 30 )


Here is the set of random numbers you will need (at most):
Random 605944
Random 606802
Random 581204
Random 158383
Random 430670
Random 393532
Random 723012
Random 994820
Random 949396
Random 544177
Random 444854
Random 268241
Random 35924
Random 27444
Random 464894
Random 318465
Random 380015
Random 891790
Random 525753
Random 560510
Random 236123
Random 23858
Random 325143
```
Будем выписывать шаги алгоритма:
```
1. total = 197, ticket = 605944 % 197 = 169
Run 2 => ans = (9, 8, 5)

2. total = 197, ticket = 606802 % 197 = 42
Run 0 => ans = (8, 8, 5)

3. total = 197, ticket = 581204 % 197 = 54
Run 0 => ans = (7, 8, 5)

4. total = 197, ticket = 158383 % 197 = 192
Run 2 => ans = (7, 8, 4)

5. total = 197, ticket = 430670 % 197 = 28
Run 0 => ans = (6, 8, 4)

6. total = 197, ticket = 393532 % 197 = 123
Run 1 => ans = (6, 7, 4)

7. total = 197, ticket = 723012 % 197 = 22
Run 0 => ans = (5, 7, 4)

8. total = 197, ticket = 994820 % 197 = 167
Run 2 => ans = (5, 7, 3)

9. total = 197, ticket = 949396 % 197 = 53
Run 0 => ans = (4, 7, 3)

10. total = 197, ticket = 544177 % 197 = 63
Run 0 => ans = (3, 7, 3)

11. total = 197, ticket = 444854 % 197 = 28
Run 0 => ans = (2, 7, 3)

12. total = 197, ticket = 268241 % 197 = 124
Run 1 => ans = (2, 6, 3)

13. total = 197, ticket = 35924 % 197 = 70
Run 0 => ans = (1, 6, 3)

14. total = 197, ticket = 27444 % 197 = 61
Run 0 => ans = (0, 6, 3)

15. total = 103, ticket = 464894 % 103 = 55
Run 1 => ans = (0, 5, 3)

16. total = 103, ticket = 318465 % 103 = 92
Run 2 => ans = (0, 5, 2)

17. total = 103, ticket = 380015 % 103 = 48
Run 1 => ans = (0, 4, 2)

18. total = 103, ticket = 891790 % 103 = 16
Run 1 => ans = (0, 3, 2)

19. total = 103, ticket = 525753 % 103 = 41
Run 1 => ans = (0, 2, 2)

20. total = 103, ticket = 560510 % 103 = 87
Run 2 => ans = (0, 2, 1)

21. total = 103, ticket = 236123 % 103 = 47
Run 1 => ans = (0, 1, 1)

22. total = 103, ticket = 23858 % 103 = 65
Run 1 => ans = (0, 0, 1)

23. total = 30, ticket = 325143 % 30 = 3
Run 2 => ans = (0, 0, 0)
```
Таким образом работа 0 закончилась после шага 14, работа 1 закончилась после шага 22, а работа 2 закончилась после шага 23.

Решим с сидом 3:
```console
➜  os-2022-labs-klimoza git:(main) ✗ ./lottery.py -j 3 -s 3
ARG jlist
ARG jobs 3
ARG maxlen 10
ARG maxticket 100
ARG quantum 1
ARG seed 3

Here is the job list, with the run time of each job:
  Job 0 ( length = 2, tickets = 54 )
  Job 1 ( length = 3, tickets = 60 )
  Job 2 ( length = 6, tickets = 6 )


Here is the set of random numbers you will need (at most):
Random 13168
Random 837469
Random 259354
Random 234331
Random 995645
Random 470263
Random 836462
Random 476353
Random 639068
Random 150616
Random 634861
```
Будем выписывать шаги алгоритма:
```
1. total = 120, ticket = 13168 % 120 = 88
Run 1 => ans = (2, 2, 6)

2. total = 120, ticket = 837469 % 120 = 109
Run 1 => ans = (2, 1, 6)

3. total = 120, ticket = 259354 % 120 = 34
Run 0 => ans = (1, 1, 6)

4. total = 120, ticket = 234331 % 120 = 91
Run 1 => ans = (1, 0, 6)

5. total = 60, ticket = 995645 % 60 = 5
Run 0 => ans = (0, 0, 6)

6. total = 6, ticket = 470263 % 6 = 1
Run 2 => ans = (0, 0, 5)

7. total = 6, ticket = 836462 % 6 = 2
Run 2 => ans = (0, 0, 4)

8. total = 6, ticket = 476353 % 6 = 1
Run 2 => ans = (0, 0, 3)

9. total = 6, ticket = 639068 % 6 = 2
Run 2 => ans = (0, 0, 2)

10. total = 6, ticket = 150616 % 6 = 4
Run 2 => ans = (0, 0, 1)

11. total = 6, ticket = 634861 % 6 = 1
Run 2 => ans = (0, 0, 0)
```
Таким образом работа 1 закончилась после шага 4, работа 0 закончилась после шага 5, а работа 2 закончилась после шага 11.

### **Задание 2**
При таком распределении билетов почти всегда(пока задача 1 не завершена) мы будем выполнять именно её, шанс того что задача 0 сможет хотя бы запуститься до того как будет выполнена задача 1 равен 1 - (100/101)^10 ~ 0.095, то есть примерно 9%. При таком распределении билетов наш scheduler начинает вести себя "нечестным" образом, выполняя работы почти в строго заданном порядке.

### **Задание 3**
Давайте нечестностью считать отношение разницы между моментами, когда были завершены задачи и суммарного времени работы всех задач. Тогда:
```
1. ./lottery.py -l 100:100,100:100 -s 1 -c
JOB 0 DONE at time 200
JOB 1 DONE at time 196
unfairness = 4/200

2. ./lottery.py -l 100:100,100:100 -s 2 -c
JOB 0 DONE at time 200
JOB 1 DONE at time 190
unfairness = 10/200

3. ./lottery.py -l 100:100,100:100 -s 3 -c
JOB 0 DONE at time 196
JOB 1 DONE at time 200
unfairness = 4/200

4. ./lottery.py -l 100:100,100:100 -s 4 -c
JOB 0 DONE at time 200
JOB 1 DONE at time 199
unfairness = 1/200

5. ./lottery.py -l 100:100,100:100 -s 5 -c
JOB 0 DONE at time 200
JOB 1 DONE at time 181
unfairness = 19/200

6. ./lottery.py -l 100:100,100:100 -s 6 -c
JOB 0 DONE at time 200
JOB 1 DONE at time 193
unfairness = 7/200

7. ./lottery.py -l 100:100,100:100 -s 7 -c
JOB 0 DONE at time 200
JOB 1 DONE at time 185
unfairness = 15/200

8. ./lottery.py -l 100:100,100:100 -s 8 -c
JOB 0 DONE at time 200
JOB 1 DONE at time 191
unfairness = 9/200

9. ./lottery.py -l 100:100,100:100 -s 9 -c
JOB 0 DONE at time 200
JOB 1 DONE at time 192
unfairness = 8/200

10. ./lottery.py -l 100:100,100:100 -s 10 -c
JOB 0 DONE at time 200
JOB 1 DONE at time 197
unfairness = 3/200
```
Таким образом нечестность в среднем равна (4 + 10 + 4 + 1 + 19 + 7 + 15 + 9 + 8 + 3)/2000 ~ 0.04. Таким образом равное количество билетов обеспечивает достаточно честное распределение времени работы задач.

### **Задание 4**
Посчитаем для разных ```quantum-size```:
```
1. -q 1
unfairness ~ 0.04
2. -q 2
unfairness ~ 0.036
3. -q 3
unfairness ~ 0.085
4. -q 5
unfairness ~ 0.135
5. -q 10
unfairness ~ 0.16
```
Таким образом среднее значение нечестности увеличивается при увеличении значения ```quantum-size```. Оно и понятно, при увеличении этой величины уменьшается количество шагов, необходимых для завершения всех задач, то есть у каждого рандомного числа увеличивается вклад, которое оно привности в порядок выполнения задач, ну а это разумеется приводит к увеличению разброса между временем выполнения работ.

---
Completely Fair Scheduler для последнего задания реализован в файлах ```cfs.c``` и ```cfs.h```