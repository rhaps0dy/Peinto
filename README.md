Peinto
=======

Segundo proyecto para el curso de Infografía 2014. 

El programa permite dibujar en pantalla. Soporta la creación y cancelación
a medias de líneas rectas, polilíneas, polígonos, círculos y dibujo a mano
alzada. También permite rellenar las áreas que se generen. También tiene un
selector de color RGB.

Utiliza los algoritmos de Bresenham para líneas y círculos, y el algoritmo
"flood fill" para el relleno de áreas. Todos estos están en el archivo 
"draw.cpp".

Dificultades encontradas
------------------------

He tenido diferentes complicaciones con cada uno de los algoritmos. El más fácil
ha sido el círculo, que salió casi a la primera. Las líneas en cambio requerieron
varios intentos, y el uso extensivo de macros para evitar duplicación de código.

El relleno ha sido más complicado. He tenido que utilizar una lista enlazada cómo
cola, y un array de máscaras de bits para indicar qué nodos había metido ya en la
cola.

Para permitir de cancelar una polilínea, forma, o dibujo a mano alzada he usado una
lista enlazada.

Adrià Garriga 158636
