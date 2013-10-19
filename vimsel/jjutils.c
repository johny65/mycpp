/*
 * jjutils.c
 *
 * Copyright 2012 Juan Bertinetti <juanbertinetti@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 *
 *
 */
#include <geanyplugin.h>

#define SEARCH line[i] == '(' || line[i] == '[' || line[i] == '<' || line[i] == '{'

/****************************************************************************
 * Datos del plugin:
 ***************************************************************************/
GeanyPlugin	*geany_plugin;
GeanyData *geany_data;
GeanyFunctions *geany_functions;

PLUGIN_VERSION_CHECK(211)

PLUGIN_SET_INFO("aaaaaaaa", "Utilidades para acelerar la codificación.",
                "0.1", "Juan Bertinetti <juanbertinetti@gmail.com>");


/****************************************************************************
 * Globales:
 ***************************************************************************/
/* para las teclas rápidas */
enum {
    GO_TO_PARENTESIS_KB,
    GO_TO_CORCHETE_KB,
    GO_TO_LLAVE_KB,
    GO_TO_MENOR_KB,
    GO_TO_COMILLA_KB,
    GO_TO_PARAMETER_KB,
    SELECT_INNER_KB,
    COUNT_KB
};

/* menúes */
static GtkWidget *main_menu = NULL;
static GtkWidget *menu_parentesis = NULL;
static GtkWidget *menu_corchete = NULL;
static GtkWidget *menu_llave = NULL;
static GtkWidget *menu_menor = NULL;
static GtkWidget *menu_comilla = NULL;
static GtkWidget *menu_parametro = NULL;
static GtkWidget *menu_select = NULL;
static GtkWidget *menu_open_terminal = NULL;


/****************************************************************************
 * name: scintilla_get_current
 * @param void
 * @return ScintillaObject *
 *
 * Devuelve una referencia al documento actual.
 * (función sacada de extrasel.c, del plugin Extra Selection de
 * Dimitar Toshkov Zhekov). La uso porque es útil.
 *
 ***************************************************************************/
ScintillaObject *scintilla_get_current()
{
    GeanyDocument *doc = document_get_current();
    return doc ? doc->editor->sci : NULL;
}




/****************************************************************************
 * name: goto_char_generic
 * @param gchar caracter
 * @return gboolean
 *
 * Busca en la línea actual el caracter definido por *caracter* y ubica el
 * cursor a su lado. Devuelve TRUE si lo logra, FALSE en caso contrario.
 *
 ***************************************************************************/
gboolean goto_char_generic(gchar caracter)
{
    ScintillaObject *sci = scintilla_get_current();
    if (sci) {
        int actual = sci_get_current_position(sci);
        int num = sci_get_current_line(sci);
        int n = sci_get_line_length(sci, num);
        int pos = actual - sci_get_position_from_line(sci, num);
        gchar *line = sci_get_line(sci, num);
        int i, c = 0;

        /* primero busca desde donde estoy: */
        for (i = pos; i < n; ++i) {
            if (line[i] == caracter) {
                sci_set_current_position(sci, actual+c+1, TRUE);
                return TRUE;
            }
            c++;
        }

        /* si no encontró, busca desde el comienzo de la línea
         * de esta forma hace una búsqueda circular en la línea */
        c = 0;
        for (i = 0; i < pos; ++i) {
            if (line[i] == caracter) {
                sci_set_current_position(sci, actual-pos+c+1, TRUE);
                return TRUE;
            }
            c++;
        }
    }

    return FALSE;

}

static void select_inner(GtkMenuItem *menuitem, gpointer gdata)
{
    ScintillaObject *sci = scintilla_get_current();
    if (sci) {
        int actual = sci_get_current_position(sci);
        int num = sci_get_current_line(sci);
        int n = sci_get_line_length(sci, num);
        gchar *line = sci_get_line(sci, num);
        int pos_ini_line = sci_get_position_from_line(sci, num);
        int i, a = -1, b = -1, c = 0;

        /*
         * busco para atrás un paréntesis:
         */

        /* si hay una selección, me corro para afuera */
        if (sci_get_selected_text_length(sci) > 1) {
            actual = sci_get_selection_start(sci) - 1;
        }

        int pos = actual - pos_ini_line; /* pos dentro de la línea */

        for (i = pos - 1; i >= 0; --i) {
            if (SEARCH) {
                a = actual - c;
                /* encontré uno, buscar su pareja */
                b = sci_find_matching_brace(sci, a-1);
                /* ver si b está adelante de mí, sino corresponde a otro paréntesis */
                if (b >= actual) {
                    sci_set_selection_start(sci, a);
                    sci_set_selection_end(sci, b);
                    /* listo, todo bien, salir */
                    break;
                }
                /* sino sigue buscando otro */
            }
            c++;
        }

        //prueba: funcion(primer nivel, (acá hay otro), y sigue el primero) listo.
        //prueba2: funcion(primer nivel, (acá hay (y [uno] metido) otro), y sigue) listo.
        //prueba3: f(g((int)a * (int)b))

        //TODO: arreglar cuando está dividido en 2 líneas
    }

}

/* Called when keystroke were pressed */
static void select_inner_kb(G_GNUC_UNUSED guint key_id)
{
    select_inner(NULL, NULL);
}




/****************************************************************************
 * Funciones goto_(caracter) y sus equivalentes para las teclas rápidas
 ***************************************************************************/

static void goto_parentesis(GtkMenuItem *menuitem, gpointer user_data)
{
    goto_char_generic((gchar)'(');
}
static void goto_parentesis_kb(G_GNUC_UNUSED guint key_id)
{
    goto_parentesis(NULL, NULL);
}

static void goto_corchete(GtkMenuItem *menuitem, gpointer user_data)
{
    goto_char_generic((gchar)'[');
}
static void goto_corchete_kb(G_GNUC_UNUSED guint key_id)
{
    goto_corchete(NULL, NULL);
}

static void goto_llave(GtkMenuItem *menuitem, gpointer user_data)
{
    goto_char_generic((gchar)'{');
}
static void goto_llave_kb(G_GNUC_UNUSED guint key_id)
{
    goto_llave(NULL, NULL);
}

static void goto_menor(GtkMenuItem *menuitem, gpointer user_data)
{
    goto_char_generic((gchar)'<');
}
static void goto_menor_kb(G_GNUC_UNUSED guint key_id)
{
    goto_menor(NULL, NULL);
}

static void goto_comilla(GtkMenuItem *menuitem, gpointer user_data)
{
    goto_char_generic((gchar)'"');
}
static void goto_comilla_kb(G_GNUC_UNUSED guint key_id)
{
    goto_comilla(NULL, NULL);
}


/****************************************************************************
 * goto_parameter, igual que las anteriores pero busca una coma (,) y te
 * ubica al principio de la palabra
 ***************************************************************************/
static void goto_parameter(GtkMenuItem *menuitem, gpointer user_data)
{
    if (goto_char_generic((gchar)',')) {
        /* si encontró la coma, ubicarse al comienzo de la palabra (ignorar
         * espacios */
        ScintillaObject *sci = scintilla_get_current();
        if (sci) {
            gchar *line = sci_get_line(sci, sci_get_current_line(sci));
            int actual = sci_get_current_position(sci) - sci_get_position_from_line(sci, sci_get_current_line(sci));
            int i = actual, c = 0;
            while (line[i] == ' ') {
                i++;
                c++;
            }
            sci_set_current_position(sci, sci_get_current_position(sci) + c,  TRUE);
        }
    }

}
static void goto_parameter_kb(G_GNUC_UNUSED guint key_id)
{
    goto_parameter(NULL, NULL);
}

static void open_terminal()
{
    gchar *cmd = "x-terminal-emulator";
    if (!geany->app->project)
        system(cmd);
    else {
        gchar *base_dir = geany->app->project->base_path;
        gchar *cmd_full = malloc(strlen(cmd) + strlen(base_dir) + 2);
        sprintf(cmd_full, "%s %s", cmd, base_dir);
        printf(cmd_full);
        system(cmd_full);
        free(cmd_full);
    }
}


/*
 *
 * name: plugin_init
 * @param GeanyData *data
 * @return void
 *
 * Función llamada al cargarse el plugin dentro de Geany.
 *
 */
void plugin_init(GeanyData *data)
{
    /******************* armar los menús: ************************/

    /* menú abrir terminal: */
    menu_open_terminal = gtk_menu_item_new_with_mnemonic("Abrir terminal");
    gtk_container_add(GTK_CONTAINER(geany->main_widgets->project_menu), menu_open_terminal);
    //ui_add_document_sensitive(main_menu); /* deshabilitarlo si no hay documento */
    //GtkContainer *menu = GTK_CONTAINER(gtk_menu_new());
    //gtk_menu_item_set_submenu(GTK_MENU_ITEM(main_menu), GTK_WIDGET(menu));
    g_signal_connect(menu_open_terminal, "activate", G_CALLBACK(open_terminal), NULL);

    /* menú general: */
    main_menu = gtk_menu_item_new_with_mnemonic("jjUtils");
    gtk_container_add(GTK_CONTAINER(geany->main_widgets->tools_menu), main_menu);
    ui_add_document_sensitive(main_menu); /* deshabilitarlo si no hay documento */
    GtkContainer *menu = GTK_CONTAINER(gtk_menu_new());
    gtk_menu_item_set_submenu(GTK_MENU_ITEM(main_menu), GTK_WIDGET(menu));

    /* menú 1 "(" */
    menu_parentesis = gtk_menu_item_new_with_mnemonic("Ubicarse en el ( (paréntesis)");
    gtk_container_add(menu, menu_parentesis);
    g_signal_connect(menu_parentesis, "activate", G_CALLBACK(goto_parentesis), NULL);

    /* menú 2 "[" */
    menu_corchete = gtk_menu_item_new_with_mnemonic("Ubicarse en el [ (corchete)");
    gtk_container_add(menu, menu_corchete);
    g_signal_connect(menu_corchete, "activate", G_CALLBACK(goto_corchete), NULL);

    /* menú 3 "{" */
    menu_llave = gtk_menu_item_new_with_mnemonic("Ubicarse en la { (llave)");
    gtk_container_add(menu, menu_llave);
    g_signal_connect(menu_llave, "activate", G_CALLBACK(goto_llave), NULL);

    /* menú 4 "<" */
    menu_menor = gtk_menu_item_new_with_mnemonic("Ubicarse en el < (menor)");
    gtk_container_add(menu, menu_menor);
    g_signal_connect(menu_menor, "activate", G_CALLBACK(goto_menor), NULL);

    /* menú 5 """ */
    menu_comilla = gtk_menu_item_new_with_mnemonic("Ubicarse en la \" (comilla doble)");
    gtk_container_add(menu, menu_comilla);
    g_signal_connect(menu_comilla, "activate", G_CALLBACK(goto_comilla), NULL);

    /* separador */
    gtk_container_add(menu, gtk_separator_menu_item_new());

    /* menú parámetro */
    menu_parametro = gtk_menu_item_new_with_mnemonic("Ir al siguiente parámetro");
    gtk_container_add(menu, menu_parametro);
    g_signal_connect(menu_parametro, "activate", G_CALLBACK(goto_parameter), NULL);

    /* menú seleccionar */
    menu_select = gtk_menu_item_new_with_mnemonic("Seleccionar todo lo rodeado");
    gtk_container_add(menu, menu_select);
    g_signal_connect(menu_select, "activate", G_CALLBACK(select_inner), NULL);

    /* mostrar menús */
    gtk_widget_show_all(main_menu);
    gtk_widget_show_all(menu_open_terminal);


    /************************ keybindings: ************************/

    GeanyKeyGroup *key_group = plugin_set_key_group(geany_plugin, "jjUtils",
                               COUNT_KB, NULL);

    keybindings_set_item(key_group, GO_TO_PARENTESIS_KB, goto_parentesis_kb,
                         0, 0, "goto_parentesis", "Ubicarse en el (", menu_parentesis);

    keybindings_set_item(key_group, GO_TO_CORCHETE_KB, goto_corchete_kb,
                         0, 0, "goto_corchete", "Ubicarse en el [", menu_corchete);

    keybindings_set_item(key_group, GO_TO_LLAVE_KB, goto_llave_kb,
                         0, 0, "goto_llave", "Ubicarse en la {", menu_llave);

    keybindings_set_item(key_group, GO_TO_MENOR_KB, goto_menor_kb,
                         0, 0, "goto_menor", "Ubicarse en el <", menu_menor);

    keybindings_set_item(key_group, GO_TO_COMILLA_KB, goto_comilla_kb,
                         0, 0, "goto_comilla", "Ubicarse en la \"", menu_comilla);

    keybindings_set_item(key_group, GO_TO_PARAMETER_KB, goto_parameter_kb,
                         0, 0, "goto_parameter", "Ir al siguiente parámetro", menu_parametro);

    keybindings_set_item(key_group, SELECT_INNER_KB, select_inner_kb,
                         0, 0, "select_inner", "Seleccionar todo lo rodeado", menu_select);


}

/*
 *
 * name: plugin_cleanup
 * @param void
 * @return void
 *
 * Función llamada al descargar el plugin de memoria. Acá se deben hacer todas
 * las limpiezas.
 *
 */
void plugin_cleanup(void)
{
    gtk_widget_destroy(main_menu);
    gtk_widget_destroy(menu_parentesis);
    gtk_widget_destroy(menu_corchete);
    gtk_widget_destroy(menu_llave);
    gtk_widget_destroy(menu_menor);
    gtk_widget_destroy(menu_comilla);
    gtk_widget_destroy(menu_parametro);
    gtk_widget_destroy(menu_select);
    gtk_widget_destroy(menu_open_terminal);
}
