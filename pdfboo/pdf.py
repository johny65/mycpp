# -*- coding: utf-8 -*-

import sys
import csv

def analizar_linea(s):
	# contar tabs:
	i = 0
	while s[i] == '\t':
		i += 1
	t = i + 1

	# buscar página:
	i = s.find('|')
	pag = s[i+1:].strip()

	# cadena:
	nom = s[0:i].strip()

	return (t, nom, pag)


def outline(txt):
	salida = csv.writer(open("indice.txt", "wb"), delimiter=';')
	for l in txt:
		(tabs, nombre, pag) = analizar_linea(l)
		salida.writerow([tabs, '', nombre, pag])
		#salida.write("{};{};{};{}\n".format(tabs, 0, nombre, pag))


if __name__ == "__main__":
	if len(sys.argv) != 3:
		print "Error. Uso: {} in out".format(sys.argv[0])
		sys.exit(1)
	txt = []
	with open(sys.argv[1]) as f:
		l = f.readline()
		while l:
			txt.append(l)
			l = f.readline()
		outline(txt)

