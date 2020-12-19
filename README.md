# Shockwave
Shockwave is a game like tic-tac-toe with a twist

Written in C. Shockwave operates much like tic-tac-toe except that the board can have any specified dimensions, the length of a run necessary to win
can be specified, and there is a special feature called "shockwave" which allows a player to send all of their pieces as far as possible in a specified direction.
In addition, the game can be stored in either a matrix representation or bit-packed into a bits representation for greater memory efficiency. 

Initiate a game with commands -w and board width, -h and board height, -r and the value for needed for a winning board run, and either -m or -b for either a matrix
representation or bits representation respectively. To make a move, the current player must type in two characters: either the two coordinates for placing a piece,
or an exclamation mark followed by a capital letter indicating a direction for a shockwave. For example !N would prompt a shockwave going up. 
