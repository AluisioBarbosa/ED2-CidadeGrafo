PROJETO = ted
COMPILADOR = gcc
BIBLIOTECAS = -lm -lgvc -lcgraph
OPCOES_COMPILACAO = -g -O0 -std=c99 -fstack-protector-all -Werror=implicit-function-declaration -Wall
FONTE = digraph.c djikstra.c hashTable.c info.c lista.c main.c processarGeo.c processarQry.c processarVia.c programa.c quadra.c STreap.c svg.c argumentos.c

OBJETOS = $(FONTE:.c=.o)

$(PROJETO): $(OBJETOS)
	$(COMPILADOR) -o $(PROJETO) $(OBJETOS) $(BIBLIOTECAS)

%.o: %.c
	$(COMPILADOR) -c $(OPCOES_COMPILACAO) $< -o $@

digraph.o: digraph.h hashtable.h
djikstra.o: djikstra.h digraph.h lista.h
hashTable.o: hashTable.h lista.h
info.o: info.h
lista.o: lista.h
processarGeo.o: processarGeo.h quadra.h STreap.h hashTable.h
processarQry.o: processarQry.h svg.h quadra.h digraph.h lista.h hashTable.h djikstra.h info.h
processarVia.o: processarVia.h digraph.h hashTable.h info.h
programa.o: programa.h argumentos.h lista.h hashTable.h STreap.h digraph.h processarGeo.h processarVia.h processarQry.h svg.h
quadra.o: quadra.h
STreap.o: STreap.h
svg.o: svg.h quadra.h info.h STreap.h digraph.h
argumentos.o: argumentos.h 
main.o: programa.h

clean:
	rm -f $(OBJETOS) $(PROJETO)