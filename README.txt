README - CLAUSEN JOHNN - GALHAUD VICTOR


⚠️⚠️⚠️⚠️⚠️⚠️⚠️⚠️⚠️⚠️⚠️⚠️⚠️⚠️⚠️⚠️⚠️⚠️⚠️⚠️⚠️⚠️⚠️⚠️⚠️⚠️⚠️⚠️⚠️⚠️⚠️⚠️⚠️⚠️⚠️⚠️⚠️⚠️⚠️⚠️⚠️⚠️⚠️⚠️⚠️⚠️⚠️⚠️⚠️⚠️⚠️⚠️⚠️⚠️⚠️⚠️⚠️⚠️⚠️⚠️⚠️⚠️⚠️⚠️⚠️⚠️⚠️⚠️⚠️⚠️⚠️⚠️⚠️⚠️⚠️⚠️⚠️⚠️⚠️⚠️⚠️⚠️⚠️
⚠️⚠️⚠️																			⚠️⚠️⚠️
⚠️⚠️⚠️     If the program doesn't work, you'll need to change the struct "Node" to modify 'double distance_to_target' and 'double costs' into : 		⚠️⚠️⚠️
⚠️⚠️⚠️	  		 'long double distance_to_target' and 'long double costs';									⚠️⚠️⚠️
⚠️⚠️⚠️																			⚠️⚠️⚠️
⚠️⚠️⚠️							This problem happens on windowsOS								⚠️⚠️⚠️
⚠️⚠️⚠️																			⚠️⚠️⚠️
⚠️⚠️⚠️⚠️⚠️⚠️⚠️⚠️⚠️⚠️⚠️⚠️⚠️⚠️⚠️⚠️⚠️⚠️⚠️⚠️⚠️⚠️⚠️⚠️⚠️⚠️⚠️⚠️⚠️⚠️⚠️⚠️⚠️⚠️⚠️⚠️⚠️⚠️⚠️⚠️⚠️⚠️⚠️⚠️⚠️⚠️⚠️⚠️⚠️⚠️⚠️⚠️⚠️⚠️⚠️⚠️⚠️⚠️⚠️⚠️⚠️⚠️⚠️⚠️⚠️⚠️⚠️⚠️⚠️⚠️⚠️⚠️⚠️⚠️⚠️⚠️⚠️⚠️⚠️⚠️⚠️⚠️⚠️


1) Getcol, getrow, getid used in the part 3 (create graph). We chose to modularize our program. This functions are really helpful to make part 3 shorter and easier to understand.


2) Horizontal application of the algorithm : 

The functions : create_horizontal_graph, shortest_horizontal_path and find_horizontal_seam are used to apply the same algortih except its horizontal, from left to right.
We also created functions (test_highlight_horizontal_seam and highlight_horizontal_seam) to test if our program works.