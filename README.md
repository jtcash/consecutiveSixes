################################
#   Created by Jeffrey Cash    #
# ---------------------------- #
#   Copying/Modifying/Using    #
#   this file is encouraged.   #
# Feel free to contact me at:  #
#       jtcash@ucsd.edu        #
#    jeffreytcash@gmail.com    #
################################

This program counts the number of consecutive sixes in all n digit numbers.
It makes two different counts: the number of numbers with three or more sixes in
a row, and the number of numbers with three and only three sixes in a row.

To build this, run 'make'.
then run './sixes n'
where n is the number of digits you want to scan through.

It takes less than a second for n<9, and it took my computer 40 minutes for n=12.

Here is the data I have acquired from this program:
________________________________________________________________
n |  '666'        |    only counting exactly 3 consecutive sixes
________________________________________________________________
3	1		1
4	18		17
5	261		243
6	3420		3159
7	42291		38871
8	503748		461466
9	5845131		5341626
10	66520530	60680016
11	745824681	679378752
12	8263713078	7518984165
