James Roach
811991542

this project took a long time. Like a really long time. super long
But i did learn a great deal about unix and c++ as i went so i guess
that is good. 
to compile and link and all that just run make all. you can also make each
utility individually such as
make rm

I fully anticipated this taking a super long time so thankfully i had time to 
completly debug all my code so i can say with pretty good certianty that it worksBelow are my reflections on the various utilites

chmod:
this was probably the most annoying thing i have ever programmed. like seriously
screw symbolic mode everyone should just learn octal. 
it wasnt that any specific part was really hard it was just that parsing
symblolic required alot of code

mkdir:
this one was pretty easy. Most challenging part was parsing out the intermediate
directories which wasnt that bad

cp:
This one was actually fun and im proud to say it was the first time ive ever
had to use recursion to solve a problem in my cs classes. The key to understanding
this one was to break the recursive directory stuff into two parts. one to
capture the directory structure in a datastructure and another to write that
data structure

mv: 
there were no suprises in this one not too hard to implement with the rename
utility

ls:
this one was kind of annoying with the time but everything else was pretty
easy to get out of the stat system call

cat:
haha lab6 with the hard stuff taken out

rm:
i discovered the nftw function for this one which made my life much easier
10/10 would recomend nftw

ln:
also trivial

penv:
supa mike gave us this

stat: 
this one was just displaying the results of the stat system call 
the whole device number thing was annoying. why combine major and 
minor with an arbitrary zero inbetween then convert that number to decimal??

pwd:
see entry for penv

kill:
big mike said that this was non trivial but apart from the mapping it seemed
pretty easy but i had no idea how to test it so it may be horribly wrong.
