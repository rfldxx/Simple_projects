# ЗАДАЧИ с Yandex Intern Week Offer 2024 backend:
>[A. Газировка](#a-газировка) \
[B. Сборщики огурцов](#b-сборщики-огурцов) \
[D. Домашнее задание](#d-домашнее-задание) \
[C. Графы](#c-графы) \
[E. Пекарня](#e-пекарня)



<!-- A -------------------------------------------------------------------- -->
$\text{}$

## A. Газировка
Олег работает на производстве известной газировки. Рецепт газировки тщательно охраняется, а неповторимый вкус получается в том числе из-за идеальных пропорций ингредиентов. \
У Олега есть $n$ бесконечно больших чанов с газировкой, расположенных в один ряд. Изначально в каждом чане есть некоторое количество концентрата, из которого делают газировку. Для стабильного вкуса нужно, чтобы во всех резервуарах содержался одинаковый объем концентрата. Но аппарат, который разливает концентрат несовершенен, так что за одну операцию способен разлить по одному литру концентрата в каждый из первых  $k$ $(1 \le k \le n)$ чанов. Обратите внимание, что для разных операций $k$ могут быть разными. \
Поскольку концентрат очень ценный, Олегу не разрешается выливать концентрат из чанов. Олег просит вас узнать, можно ли уравнять объемы концентрата в чанах, и, если это возможно, то посчитать минимальное количество операций, которое потребуется, чтобы этого достичь.

#### Формат ввода
Первая строка содержит число $n$ $( 1 \le n \le 10^5)$ — количество чанов. \
Во второй строке содержатся $n$ целых чисел $a_i$ $( 1 \le a_i \le 10^9)$, где $a_i$ означает исходный объём концентрата в $i$-м чане в литрах.

#### Формат вывода
Если объемы концентрата в чанах можно уравнять, выведите минимальное количество операций, необходимых для этого. \
Если это невозможно, выведите $-1$.



<!-- B -------------------------------------------------------------------- -->
$\text{}$

## B. Сборщики огурцов
> *Идентичную задачу можно решить по [ссылке](https://codeforces.com/problemset/gymProblem/102262/A) (спасибо М\*)*

Валентина Аркадьевна очень любит свой огород, поэтому много сил вложила в выращивание всех возможных овощей и фруктов на своем участке. Пришло время собирать урожай огурцов, поэтому Валентина Аркадьевна позвала на дачу всех своих детей и внуков, чтобы это сделать. \
Все родственники, задействованные в сборе урожая, поделились на $n$ групп. Каждый участник каждой группы собрал некоторое количество $P_{ij}$ огурцов, из которых потом сделали $T$ банок с солеными огурцами. \
Валентина Аркадьевна хочет распределить банки «честно» между всеми участниками процесса. Поэтому она приняла решение распределить награду так, чтобы сумма абсолютных разностей по всем участникам между собранными ими огурцами и полученным количеством банок на группу была минимальной. \
Распределите банки между группами таким образом, что должна быть минимизирована величина $\sum\limits_{i=1}^n \sum\limits_{j=1}^{m_i} | P_{ij} - T_i |$, где $i$-я группа получит $T_i$ банок.

#### Формат ввода
В первой строке задано число $1 \le n \le 1000$ — число групп. \
В следующих $n$ строках записаны описания групп родственников. Первое число в строке $1 \le m \le 100$ — размер группы. Оставшиеся $m_i$ целых чисел $0 \le P_{ij} \le 10^6$ — количество единиц огурцов, собранных $j$-м другом из $i$-й группы. \
В последней строке записано единственное целое число $1 \le T \le 10^9$ — количество банок с солеными огурцами.

#### Формат вывода
Выведите в одной строке $n$ целых чисел $0 \le T_i \le 10^9$ — банки для $i$-й группы. Все банки должны быть полностью распределены, то есть $\sum\limits_{i=1}^n T_i = T$. Также должна быть минимизирована величина $\sum\limits_{i=1}^n \sum\limits_{j=1}^{m_i} | P_{ij} - T_i |$. \
Если есть несколько оптимальных распределений банок, выведите любое из них.



<!-- D -------------------------------------------------------------------- -->
$\text{}$

## D. Домашнее задание
Борис прошел в школе сразу две новых темы - наибольший общий делитель $(\text{НОД})$ и простые числа. В качестве домашнего задания Борису необходимо решить $T$ заданий следующего вида:
> «Даны два целых числа $A$ и $B$, можно ровно один раз умножить либо 
$A$, либо $B$ на любое простое число. Какого наибольшего значения $\text{НОД}$ можно добиться с помощью такого умножения?» 

Пример:
* Пусть $A = 20$, $B = 45$. НОД($A$, $B$) = 5.
* При умножении числа $A$ на простое число $3$ итоговый $\text{НОД} (20 \cdot 3,45) = 15$.
* При умножении числа $B$ на простое число $2$ итоговый $\text{НОД}( 20, 45 \cdot 2) = 10$. 

Пожалуйста, ознакомьтесь с примерами тестов и пояснениями к ним для лучшего понимания условия.

#### Формат ввода
В первой строке дано целое число $T$ $(1 \le T \le 20)$ — количество заданий. \
В каждой из следующих $T$ строк содержатся два целых числа $A$ и $B$ $(1 \le A, B \le 10^{12})$ — параметры задания.

#### Формат вывода
Для каждого задания выведите единственное целое число — наибольшее значение $\text{НОД}$, которого можно добиться умножением одного из чисел $A$ и $B$ на любое простое число.



<!-- C -------------------------------------------------------------------- -->
$\text{}$

## C. Графы
Назовём неориентированный граф простым, если в нём нет петель и кратных рёбер. \
Назовём простой неориентированный граф красивым, если в нём есть ровно одна вершина, соединённая со всеми остальными ребром.

Дано число $n$, требуется посчитать количество красивых графов на $n$ вершинах. \
Два графа называются различными, если существует пара вершин $(u, v)$ такая, что в одном графе есть ребро $(u, v)$, а в другом нет.

Так как ответ может быть крайне большим, выведите отстаток от его деления на $10^9 + 7$.

#### Формат ввода
В единственной строке задано одно целое число 
$n$  $(1 \le n \le 5000)$.

#### Формат вывода
Выведите одно число — ответ на задачу по модулю $10^9 + 7$.



<!-- E -------------------------------------------------------------------- -->
$\text{}$

## E. Пекарня
Ярослав уже давно работает в пекарне, поэтому решил открыть свою собственную. Для того, чтобы получить больше покупателей, он решает прорекламироваться в Индиректе. Таргетирование в данном случае - это нацеливание на жителей, находящихся сейчас в некотором прямоугольнике со сторонами, параллельными осям координат.

В своём интересующем районе Ярослав знает все $n$ пекарен. Он также, каким-то образом, знает, на какую территорию они таргетируются. Ярослав хочет оценить уровень конкуренции, для этого ему необходимо для каждого магазина понять, со сколькими другими магазинами он пересекается по нацеливанию.

Определение: два прямоугольника пересекаются, если существует область ненулевой площади, принадлежащая обоим прямоугольникам. Внешнее касание по стороне образует общую область нулевой площади, поэтому пересечением не является.

#### Формат ввода
В первой строке дано целое число $n$ $(1 \le n \le 10^5)$  — количество прямоугольников.

В следующих $n$ строках заданы описания прямоугольников: целые числа $x_L$, $y_L$, $x_R$, $y_R$ $(-10^9 \le x_L, y_L, x_R, y_R \le 10^9 \text{ ; } x_L < x_R \text{ ;  } y_L < y_R)$ — координаты левого нижнего и правого верхнего углов.

#### Формат вывода
В единственной строке выведите через пробел $n$ чисел: $i$-е число равно количество прямоугольников, пересекающихся с $i$-м в порядке ввода прямоугольником.