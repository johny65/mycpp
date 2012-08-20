/**
 * \file debug.h
 * \brief Utilidad para depuración.
 *
 * Define una macro para mostrar mensajes en tiempo de depuración, `dbg(x)`. Si
 * el programa es compilado con la macro definida `DEBUG`, entonces `dbg(x)`
 * mostrará por salida de error estándar (`stderr`, o `cerr`) el mensaje `x`,
 * donde `x` puede ser cualquier cosa que pueda pasarse a `cerr`. No hace falta
 * incluir salto de línea, es agregado por la macro.
 * Si no se define `DEBUG`, entonces `dbg(x)` es completamente ignorado.
 */
#ifdef DEBUG
	#include <iostream>
	#define dbg(x) std::cerr<<x<<std::endl
#else
	#define dbg(x) //nop
#endif
