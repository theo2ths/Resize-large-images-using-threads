
1) Am retinut matricea de pixeli ca un vector de pixeli cu weight * height elemente, dupa ce am facut citirea am apelat 
functiile de creere de threduri iar apoi fiecarui thread i am atribuit diferiti pixeli din matricea de output. Pentru pixeul 
i j am calculez suma din submatricea corespunzatoare din input(daca resize era 3 faceam o suma ponderata bazata pe matricea gaussian). 
Am tratat separat cazurile cu alb-negru/color si resize==3/resize!=3.
2) Mai intai am facut o schimbare de coordonate pt fiecare pixel i j deoarece linia data avea varful 0 0 in stanga jos iar matricea 
avea varful 0 0 in stanga sus. Dupa ce am facut asta am aplicat formula de distanta de la un punct la o dreapta si daca era mai mica 
decat 3 coloram pixelul respectiv cu negru, altfel cu alb.
