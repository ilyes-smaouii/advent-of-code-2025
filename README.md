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

### 2025/12/13
Took me days to figure out an algorithm, implement it, and then debug it. Turns out
using `std::uint32_t` caused wrong values to be computed for my square distances, which
falsified all my results. I straight up had to re-implement `std::hash` and `std::less`
and try all kinds of algorithms and data structures, just for it to be caused by a single line of
code in the end.
Once I figured that out, it didn't take me too long to do Part 2.
\
Update : finished Part 1 of Day 09 and read instruction for Part 2. Will try to think about
it and do it later.
\
Update : seems pretty complex, I don't really have any promising leads in mind right now.
I don't even see how I could determine whether, for a given loop, a certain tile is inside
that loop or not, assuming I could find the loop in the first place.
\
Update : I might have some idea after all. The basic idea is this : first, trace lines between
consecutive red tiles from the list; then, fill out the enclosed areas by "propagating" from
the outer lines into the inside, layer by layer. Then, you could iterate through the possible
pairs of corner red tiles, and check whether the corresponding rectangle is filled with red/green
tiles.
\
Update : I realized it's much more complicated than I thought. Here's maybe a simpler approach to
implement : trace only border lines, then iterate through the possible pairs, and for each pair
try to "grow out" the corresponding rectangle line by line, untill it either gets stopped by the
border lines, or "spills out" into at least one of the edges of the whole table.
\
Update : just spent so much time just on tracing/determining the borders, and takes so long to
execute too. This honestly feels
impossible, or like it's going to be really messy and long.
\
Update : currently at 500 lines of code, and I'm not even finished.
\
Update : currently testing out some code for "growing" areas into borders, and it takes a lot
(a looot) of RAM - I think it hit somewhere close to 35 GB at some point - and it's clearly too slow.
Update : it was mostly because of the constant creation of `TableView` objects, which took a lot
of time + memory. It now runs a lot faster (grew area from 1 to like 2 million cells in a few seconds),
but that's probably still not near fast enough, considering the rectangle I found in Part 1 was about
4 billion cells (and it already took a lot longer than just a few seconds to get to 7 millions, for
example).
\
Update : after some long thought, I think I have an idea :
\
- Go through the set of `Rectangle`'s orderd by descending area
- For each on try to grow the area to check if it's all "inside" the borders, except :
  - here's the catch : instead of growing the area in all directions, I modify
  `getNextlayer()` so it returns a set that only contains the "northwest"-most cell (or whichever of the
  three other "diagonal" directions I feel like using). This makes computation much faster, and stills
  allows the program  to go "look for" the edges of the general map.
- If it finds an edge of the general map, move on to next `Rectangle`, otherwise we found our largest
eligible `Rectangle`

### 2025/12/16
Update : small idea for improvement : try to go in all four directions, to avoid getting "stuck"
if we happen to on the wrong side of our map (too complex to explain properly with words, but whatever).
\
Anyway, I've been working on this on and off for a few days, and I thought of a much better approach :
Just check, for each rectangle, wether or not a line between consecutive boxes - or set of such lines -
"splits" it in two. This also solves the problem of getting "stuck"/spending too long going in a
direction. I still wanted to work on implementing my previous idea, if only for personal satisfaction,
but I'm now switching to the latest approach, which should be much quicker to implement + to execute.
\
Update : had a lot of mistypes/bugs to fix, but the core code of my idea - `findLargestUnsplitRectangle()`
, `checkIfRectangleIsSplitAux()` and `checkIfRangesIntersect()` -, worked right away, which I find kind of
impressive considering the number of similarly named variables I had to deal with in those lines,
especially considering I made mistakes on stuff that was much simpler (e.g. forgot to properly switch
`col` with `row` in `computeArea()`).
\
Anyway, Day 09 finished. I might wanna clean up my code a little - or leave it as is, for future reference
-, and then I'm moving on to Day 10.

### 2025/12/19
Did Part 1 of Day 10 yesterday, and looked at instructions for Part 2. I haven't really figured out
how to approach this yet.