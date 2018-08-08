Controls: 
w - up
s - down 
a - left
d - right

Overall design:
The snake grows in length by adding a list (tail) inside the snake structure.
Each time the snake eats a fruit, an element gets pushed into the tail list.

The tail is designed in such a way that it keeps track of the history of where the snake has moved. Everytime the snake moves a unit, the tail pushfront where the snake moved FROM, and popback the last item in the list. If the snake eats a fruit. We simply skip the popback step 5 times, since the tail grows in units of 5. 

Use the make file to compile the code 
