## Detta projekt implementerar en hash-tabell och länkad lista i C, inklusive funktioner för insättning, borttagning, sökning och iteration. Programmet inkluderar även en ord-frekvensräknare som läser in text från filer och räknar hur många gånger varje ord förekommer.

## Bygg programmet:

Att bygga och köra programmet görs genom 'make' i terminalen. För att hitta specifikt test du vill köra, öppna make-filen.
T.ex körs test_hash_table med valgrind genom "make valgrind_test_hash_table".

För att få fram data kring running-time och performance för freq_count, så kan du köra "gprof freq_count gmon.out > gprof_output.txt"
i terminalen.

För att testa line coverage skrivs "make coverage_test". Line Coverage % hittas längre ner i readme filen.

För att testa branch coverage skrivs "make branch_coverage_test". Branch Coverage % hittas längre ner i readme filen.

## Hash Tabell:

Skapa och lagra element i ett hash table. Vid fel returneras ett Failure-objekt, och vid framgång ett Success-objekt.

## Linked List:

Den länkade listan används för kollisioner i hash-tabellen. Den har funktioner för insättning, borttagning och iteration.

## Iteratorer:

Iteratorer används för att gå igenom elementen i både hash-tabeller och länkade listor.

## Antaganden:

    - Nycklar i hash-tabellen är unika
    - Dynamiskt allokerat minne(stacken) frigörs vid behov.

**How failure is handled** - Fel hanteras genom att använda en option_t-struktur, där varje operation returnerar ett objekt som indikerar om den lyckades (Success) eller misslyckades (Failure). - Vid misslyckade uppslagningar i hash-tabellen returneras ett Failure()-objekt, och länkade listor hanterar fel genom att returnera standardvärden som int_elem(-1) vid ogiltiga operationer. - Minnesallokering hanteras genom calloc och free, och allt dynamiskt allokerat minne frigörs korrekt vid behov.

**Code Test Coverage**

## Line Coverage

<<<<<<< Updated upstream
hash_table.c: 84% Coverage
=======
hash_table.c: 89% Coverage

> > > > > > > Stashed changes
> > > > > > > test_hash_table.c: 98% Coverage
> > > > > > > iterator.c: 95% Coverage
> > > > > > > test_iterator.c: 94% Coverage
> > > > > > > linked_list.c: 90% Coverage
> > > > > > > test_linked_list.c: 97% Coverage

Average Coverage: 94%

## Branch Coverage:

hash_table.c: 78% Coverage
test_hash_table.c: 64% Coverage
iterator.c: 72% Coverage
test_iterator.c: 50% Coverage
linked_list.c: 79% Coverage
test_linked_list.c: 40% Coverage

-   Alot of branches never get executed since we had as a standard to always implement edegcase handling in our functions, e.g return -1 if negative index and etc.
    Hence, when functions call upon other functions and further on the edgecase handling doesn't get executed since we already handled it in functions before.

## Branch Coverage

hash_table.c: 73%
test_hash_table.c 64%
iterator.c: 72%
test_iterator.c: 50%
linked_list.c: 77%
test_linked.c: 50%

Average Coverage: 68%

**Frågor Assignment 1 step 14**

## For each input, what are the top 3 functions?

-   1k-long-words: - string_eq, find_previous_entry_for_key, string_sum_hash
-   10k-words - string_eq, find_previous_entry_for_key, string_sum_hash
-   16k-words - ioopm_linked_list_insert, find_previous_entry_for_key, string_eq

## For each input, are the top 3 functions in your code (that you have written), or is it in library functions?

-   1k-long-words:
    -   All three are our functions and not library functions.
-   10k-words
    -   All three are our functions and not library functions.
-   16k-words:
    -   All three are our functions and not library functions.

## **Are the top 3 functions in your code consistent across the inputs? Why? Why not?**

    - The top 3 functions are mostly consistent across the 1k-long-words and 10k-words inputs. However, for the 16k-w   ords input, insert affected the performance more than the others and was not even top 3 in smaller text docs.

    I might also add that \_init shows up on my computer, but that's more individual for each computer and not something I'd count for this.

## Is there some kind of trend? (Possibly several)

    - A trend is that `find_previous_entry_for_key` appears in the top 3 for all input sizes, which means that lookup has a big role in the performance.

    -   For larger inputs like 16k-words, \_init stands for the nr 1 spot. Probably because of increased input sizes and my computers performance.

## Do the results correspond with your expectations?

    - Yes, since we're processing big amount of words I guessed before hand that lookup_hash and string_sum_hash would be used a lot. find previous makes sense aswell since it's called upon when lookup_hash is called.

## Based on these results, do you see a way to make your program go faster?

    -   We would try to optimize find_previous_entry_for_key since it would affect two of the most demanding functions.
    -   We could also increase the efficiency of the string_eq and string_sum_hash by finding a better algorithm. - If we pre allocate more memory we could probably reduce the init time, but also by how many running applications I have on my computer.
