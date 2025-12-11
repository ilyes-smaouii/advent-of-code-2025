# advent-of-code-2025
Personal attempt at Advent of Code 2025

## Intro/context
After finishing the 2024 edition a few weeks ago, I'll try participating in the
2025 edition, this time as it's being released, rather than months later.

## Journal/misc
### 2025/11/30
Created the repo a few hours before the release of first Day's instructions.
\
Will report back tomorrow when I've had my look at the first Day.

### 2025/12/01
Look at Day 1, solved it pretty quickly, currently reading part 2 of Day 1.
\
Update : had a bit of trouble on part 2 cause I forgot to use `abs()` on an int variable I had,
but I got it in the end.
\
Will report back tomorrow for Day 2.

### 2025/12/02
Just read Part 1 of Day 2, and I already have some ideas besides the bruteforce approach -
which, if I'm trying to anticipate could be useful for Part 2 -, e.g. using the fact that
invalid IDs will be multiples of 1 + 10^n where n > 0;
\
Update : had a bit of trouble, switched to the simple approach of using strings, and after
fishing out a but, I figured it out. Now onto Part 2.

### 2025/12/03
Couldn't finish Day 2 last night as I was busy, and I was trying to think of an efficient
approach, but I just ended up using a slow solution.
\
I'll try to read and do Day 3 later today, I've got other stuff to take care of right now.

### 2025/12/04
Update on Part 1 of Day 03 : didn't have time to code it yesterday as I was out, but I took
some notes on my phone, and after a minor correction it worked straight away. Now onto
Part 2.

### 2025/12/05
Update on yesterday : wrote the code for Day 03 fairly quickly, but it kept crashing. It turned out
my test data wasn't properly initialized, which caused the continous crashes. Got the right answer
once I fixed that.
\
I'd also started to look at Day 04 and to think about it, although I couldn't really write code yet.
I'm now starting to write the code.
\
Update : made a lot of stupid mistake, but I'm now done with Part 1. Moving on to Part 2.
\
Update : solved it, although I'm not too happy with the speed of my code. Anyway, I'm not wasting
any more time on this. Moving on to Day 05.
\
Update : solved Day 5. Surprisingly enough, Part 2 kinda felt easier than Part 1. I'm also pretty
happy with the algorithm I've used to "fuse" the ranges, which reminds of a technical test I'd once
had to do, which I'd failed at, but I feel I'd be able to better approach today. Ironically enough,
failing that techical test was also what prompted me to get into Advent of Code in the first place
(and also started me on a lot of other projects). Anyway, will report back tomorrow - or when I'm
available - for Day 6.

### 2025/12/09
Been struggling for for hours with Day 6 using C++, and I just realized that a big part of that is
because Windows ends its lines differently from Unix/Linux systems, which left an extra `'\r'` at the
end of all my lines - but the last one -, and made my entire program bug. SMH.
\
Anyway, Day finished, moving on to Day 7 (probably not now though, maybe later).
\
Update : went ahead and looked at Day 7 anyway. Part 1 was relatively easy. I tried a naive
recursive approach for Part 2 and, unsurprisingly, it didn't work. I'm going to have to use
memoization (by starting from the bottom layer, and making the program determine the number of
possibilities starting from each cell in the layer above, then the layer above that, etc.).
I'm gonna do that later though, I don't have time right now.
\
Update : nvm it was easier/quicker than I though,
I went to prepare some code and ended up writing the whole thing in a few minutes.
\
I'm gonna try not to read the instructions for Day 8 straight away because I really have to stop
this and go do other things right now.
\
Update : just read the instructions for Day 8, and damn. Will try to think about it while I make
myself dinner.

### 2025/12/11
Update on Day 8 : didn't do much, and I may not allocate too much time for it in the next few days
as I'm a little busy, but I might still give it a go.