import sys

'''
Idea general: 
Para la solución, calculamos con la lista de movidas el estado del tablero para cada iteración. 
Hacemos lo mismo para la solución del alumno y las comparamos. 100% si tiene todas las iteraciones iguales
0% si no, y si tiene algunas buenas, se calculara el puntaje de manera lineal
'''


def copy_board(board_a, m, n):
    board_b = [[-1 for x in range(0, n)] for y in range(0, m)]
    for row in range(0, m):
        for col in range(0, n):
            board_b[row][col] = board_a[row][col]
    return board_b


def load_board(path):
    all_boards = []
    with open(path) as f:
        m, n = [int(x) for x in f.readline().split()] # sacamos los valores n y m que vienen del WATCHER_OPEN
        board = [[-1 for x in range(0, n)] for y in range(0, m)]
        for line in f:
            line = line.rstrip('\n')
            if line == "REFRESH": # guardamos el tablero actual y nos movemos a la siguiente iteración
                all_boards.append(copy_board(board, m, n))
            elif line == "CLOSE":
                f.close()
                break
            else:
                row, col, value = [int(x) for x in line.split()]
                board[row][col] = value
    return all_boards

output_board   = load_board(sys.argv[1])
solution_board = load_board(sys.argv[2])

count = 0
iteration = 0
# Comparamos los dos tableros por cada iteración y contamos cuantas son iguales
for board_s, board_o in zip(solution_board, output_board):
    if (board_s == board_o):
        count += 1
        iteration += 1
    else:
        print("Iteración {} es distinta!".format(iteration))
    

# 1 pto por tener el estado inicial (leer bien el archivo)
# 1 pto por cada una de las siguientes 10 generaciones 
# Nota 7 es tener los 11 tableros correctos (el tablero incial + los 10 siguientes)
# Para tener puntaje, el programa tiene que correr en <10 segundos, si no no hay puntaje
print(round(1 + count*6/11, 2))

 
 
