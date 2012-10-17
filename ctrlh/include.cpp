#include <geanyplugin.h>
#include <string>
#include <sstream>
#include <fstream>

using namespace std;


/****************************************************************************
 * MACROS:
 ***************************************************************************/
 
/**
 * Simple macro for \c scintilla_send_message that reduces typing.
 */
#define SSM(s, m, w, l) scintilla_send_message(s, m, w, l)

/**
 * Macro for identifying the \c listType in the \c SCNotification when the
 * user clicks the desired header in the drop-down list.
 */
#define AUTOHEADER_LIST_TYPE 65


/****************************************************************************
 * Plugin info:
 ***************************************************************************/
GeanyPlugin *geany_plugin;
GeanyData *geany_data;
GeanyFunctions *geany_functions;

PLUGIN_VERSION_CHECK(211)

PLUGIN_SET_INFO(_("Auto-headers"), _("Insert headers automatically"),
				"0.1", "Juan Bertinetti <juanbertinetti@gmail.com>");



/****************************************************************************
 * Menu item:
 ***************************************************************************/
static GtkWidget *main_menu_item = NULL;


enum {
	AUTO_HEADER_KB,
	COUNT_KB
};



/****************************************************************************
 * FUNCTIONS:
 ***************************************************************************/


/**
 * \brief Current ScintillaObject.
 * 
 * Returns the current ScintillaObject object from the document (pointer).
 * \return NULL in case of error.
 */
ScintillaObject *scintilla_get_current()
{
	GeanyDocument *doc = document_get_current();
	return doc ? doc->editor->sci : NULL;
}


/**
 * \brief Current GeanyEditor.
 * 
 * Returns the current GeanyEditor object from the document (pointer).
 * \return NULL in case of error.
 */
GeanyEditor *geany_editor_get_current()
{
	GeanyDocument *doc = document_get_current();
	return doc ? doc->editor : NULL;
}


/**
 * \brief Current word.
 * 
 * Gets and returns the current word from the current document (the word
 * under the position of the cursor, even though it is not completely selected).
 *
 * \return The current word or \c NULL if there isn't any word under the cursor.
 */
gchar *get_current_word()
{
	GeanyEditor *editor = geany_editor_get_current();
	ScintillaObject *sci = scintilla_get_current();
	int pos = sci_get_current_position(sci);
	gchar *palabra = editor_get_word_at_pos(editor, pos, NULL);
	return palabra;
}


///Insert header #include.
/**
 * Insert the '#include <header>' directive
 */
void insert_header(string header)
{
	ScintillaObject *sci = scintilla_get_current();
	GeanyEditor *editor = geany_editor_get_current();
	int num = 0;
	int cantidad_lineas = sci_get_line_count(sci);
	bool buscando_fin = false;
	while (num <= cantidad_lineas){
		string line = sci_get_line(sci, num);
		if (line.find(header) != string::npos){
			//ya está incluída
			ui_set_statusbar(false, _("The %s header is already included."), header.c_str());
			return;
		}
		if (line.find("#include") != string::npos){
			if (!buscando_fin)
				buscando_fin = true;
		}
		else {
			if (buscando_fin){
				stringstream ss;
				ss<<"#include "<<header<<endl;
				editor_insert_text_block(editor, ss.str().c_str(),
					sci_get_position_from_line(sci, num), 0, -1, true); ///<\todo que no mueva el cursor
				ui_set_statusbar(false, _("Added '%s' header."), header.c_str());
				return;
			}
		}
		num++;
	}
	///\todo ver cuando no hay ninguno ya incluido.
}


static void buscar()
{
	string file = geany->app->configdir;
	file += "/include.txt";
	
	ifstream archivo(file.c_str());
	if (!archivo.good()){
		dialogs_show_msgbox(GTK_MESSAGE_ERROR, "Error al abrir archivo.");
		return;
	}

	string funcion, cabecera, buffer;

	gchar *p = get_current_word();
	if (p == NULL)
		return;
	else
		funcion = p;
		
	GString *headers = g_string_sized_new(150); ///<\todo usar vector<string> u otra cosa
	int count = 0;
	
	while (archivo>>buffer){
		if (buffer[0] == '<'){
			cabecera = buffer;
			continue;
		}
		if (funcion == buffer){
			g_string_append(headers, cabecera.c_str());
			g_string_append_c(headers, '\n');
			count++;
		}
	}
	archivo.close();

	if (count == 1){
		insert_header(headers->str); ///<\todo ver el \n al final
	}
	else if (count > 1){
		SSM(scintilla_get_current(), SCI_USERLISTSHOW, AUTOHEADER_LIST_TYPE,
			(sptr_t) headers->str);
	}
	else {
		ui_set_statusbar(false, _("Corresponding header for '%s' not found."), p);
	}

}



static gboolean on_editor_notify(GObject *object, GeanyEditor *editor,
								 SCNotification *nt, gpointer data)
{
	/* For detailed documentation about the SCNotification struct, please see
	 * http://www.scintilla.org/ScintillaDoc.html#Notifications. */
	switch (nt->nmhdr.code)
	{
		case SCN_USERLISTSELECTION:
			if (nt->wParam == AUTOHEADER_LIST_TYPE){
				insert_header(nt->text);
				break;
			}
	}

	return FALSE;
}

PluginCallback plugin_callbacks[] =
{
	/* Set 'after' (third field) to TRUE to run the callback @a after the default handler.
	 * If 'after' is FALSE, the callback is run @a before the default handler, so the plugin
	 * can prevent Geany from processing the notification. Use this with care. */
	{ "editor-notify", (GCallback) &on_editor_notify, FALSE, NULL },
	{ NULL, NULL, FALSE, NULL }
};


static void auto_header_kb(G_GNUC_UNUSED guint key_id)
{
	buscar();
}


/****************************************************************************
 * PLUGIN INTERFACE:
 ***************************************************************************/

extern "C"
void plugin_init(GeanyData *data)
{
	main_menu_item = gtk_menu_item_new_with_mnemonic("Actual...");
	gtk_widget_show(main_menu_item);
	//gtk_container_add(GTK_CONTAINER(geany->main_widgets->tools_menu),
	gtk_container_add(GTK_CONTAINER(geany->main_widgets->editor_menu),
		main_menu_item);
	g_signal_connect(main_menu_item, "activate", G_CALLBACK(buscar), NULL);


	/* kebindings */
	
	GeanyKeyGroup *key_group = plugin_set_key_group(geany_plugin, "Auto-header",
		COUNT_KB, NULL);
	
	keybindings_set_item(key_group, AUTO_HEADER_KB, auto_header_kb,
		(GdkModifierType)0, (GdkModifierType)0, "auto_header", "Insert corresponding include", main_menu_item);

}


/**
 * \brief Plugin cleanup.
 * 
 * Do the cleanup (remove the plugin widgets).
 */
extern "C"
void plugin_cleanup(void)
{
	gtk_widget_destroy(main_menu_item);
}
