#include <geanyplugin.h>

#define SEARCH line[i] == '(' || line[i] == '[' || line[i] == '<' || line[i] == '{'

GeanyPlugin	*geany_plugin;
GeanyData *geany_data;
GeanyFunctions *geany_functions;

PLUGIN_VERSION_CHECK(211)

PLUGIN_SET_INFO("aaaaaaaa", "Just another tool to say hello world",
				"1.0", "John Doe <john.doe@example.org>");

ScintillaObject *scintilla_get_current(void)
{
	GeanyDocument *doc = document_get_current();
	return doc ? doc->editor->sci : NULL;
}

/* Keybinding(s) */
enum {
	GO_TO_PARENTESIS_KB,
	SELECT_INNER_KB,
	COUNT_KB
};

static GtkWidget *main_menu = NULL;
static GtkWidget *main_menu_item1 = NULL;
static GtkWidget *main_menu_item2 = NULL;

static void goto_parentesis(GtkMenuItem *menuitem, gpointer gdata)
{
	ScintillaObject *sci = scintilla_get_current();
	if (sci){
		int actual = sci_get_current_position(sci);
		int num = sci_get_current_line(sci);
		int n = sci_get_line_length(sci, num);
		int pos = actual - sci_get_position_from_line(sci, num);
		gchar *line = sci_get_line(sci, num);
		gboolean found = FALSE;
		int i, c = 0;

		//TODO: hacer la búsqueda de una sola parte
		
		//primero busca desde donde estoy:
		for (i = pos; i < n; ++i){
			if (SEARCH){
				sci_set_current_position(sci, actual+c+1, TRUE);
				found = TRUE;
				break;
			}
			c++;
		}
		//si no encontró, busca desde el comienzo de la línea
		//de esta forma hace una búsqueda circular en la línea
		if (!found){
			c = 0;
			for (i = 0; i < pos; ++i){
				if (SEARCH){
					sci_set_current_position(sci, actual-pos+c+1, TRUE);
					break;
				}
				c++;
			}
		}
	}

}

static void select_inner(GtkMenuItem *menuitem, gpointer gdata)
{
	ScintillaObject *sci = scintilla_get_current();
	if (sci){
		int actual = sci_get_current_position(sci);
		int num = sci_get_current_line(sci);
		int n = sci_get_line_length(sci, num);
		gchar *line = sci_get_line(sci, num);
		int pos_ini_line = sci_get_position_from_line(sci, num);
		int i, a = -1, b = -1, c = 0;
		
		/*
		 * busco para atrás un paréntesis:
		 */
		
		//si hay una selección, me corro para afuera
		if (sci_get_selected_text_length(sci) > 1)
			actual = sci_get_selection_start(sci) - 2;
			
		int pos = actual - pos_ini_line; //pos dentro de la línea
		
		for (i = pos - 1; i >= 0; --i){
			if (SEARCH){
				a = actual - c;
				//encontré uno, buscar su pareja
				b = sci_find_matching_brace(sci, a-1);
				//ver si b está adelante de mí, sino corresponde a otro paréntesis
				if (b >= actual){
					sci_set_selection_start(sci, a);
					sci_set_selection_end(sci, b);
					//listo, todo bien, salir
					break;
				}
				//sino sigue buscando otro
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
static void goto_parentesis_kb(G_GNUC_UNUSED guint key_id)
{
	goto_parentesis(NULL, NULL);
}


void plugin_init(GeanyData *data)
{
	//menú general:
	main_menu = gtk_menu_item_new_with_mnemonic("Vimsel");
	gtk_container_add(GTK_CONTAINER(geany->main_widgets->tools_menu), main_menu);
	ui_add_document_sensitive(main_menu);
	GtkContainer *menu = GTK_CONTAINER(gtk_menu_new());
	gtk_menu_item_set_submenu(GTK_MENU_ITEM(main_menu), GTK_WIDGET(menu));

	//menú 1
	main_menu_item1 = gtk_menu_item_new_with_mnemonic("Ubicarse en el (");
	gtk_container_add(menu, main_menu_item1);
	g_signal_connect(main_menu_item1, "activate", G_CALLBACK(goto_parentesis), NULL);

	//menú 2
	main_menu_item2 = gtk_menu_item_new_with_mnemonic("Seleccionar todo adentro del (");
	gtk_container_add(menu, main_menu_item2);
	g_signal_connect(main_menu_item2, "activate", G_CALLBACK(select_inner), NULL);

	gtk_widget_show_all(main_menu);
	

	//keybindings:
	GeanyKeyGroup *key_group = plugin_set_key_group(geany_plugin, "vimsel", COUNT_KB, NULL);
	
	keybindings_set_item(key_group, GO_TO_PARENTESIS_KB, goto_parentesis_kb,
		0, 0, "goto_vimsel", "Ubicarse (", main_menu_item1);
		
	keybindings_set_item(key_group, SELECT_INNER_KB, select_inner_kb,
		0, 0, "select_vimsel", "Seleccionar todo dentro del (", main_menu_item2);
	
}

void plugin_cleanup(void)
{
	gtk_widget_destroy(main_menu);
	gtk_widget_destroy(main_menu_item1);
	gtk_widget_destroy(main_menu_item2);
}
