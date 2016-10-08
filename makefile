all: servidor cliente

servidor: servidor.c funcoes.c
	gcc servidor.c funcoes.c -o servidor

cliente: cliente.c funcoes.c
	gcc cliente.c funcoes.c -o cliente
