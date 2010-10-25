#include <gtk/gtk.h>
#include <unistd.h>
#include <libxml/tree.h>
#include <string.h>
#include "notifications.h"

static GtkWidget *my_menu = NULL;

/* gcc -o status status.c `pkg-config --libs --cflags gtk+-2.0` */ 
/* gcc -Wall -o test-notify test-notify.c `pkg-config --libs --cflags glib-2.0 gtk+-2.0` -lnotify */ // THIS WORKS

GtkStatusIcon *change_tray_icon(GtkStatusIcon *tray_icon, int success) {
        if(success)
                gtk_status_icon_set_from_icon_name(tray_icon, GTK_STOCK_YES);
        else
                gtk_status_icon_set_from_icon_name(tray_icon, GTK_STOCK_STOP);

        return tray_icon;
}

char *getProjectBuildStatus(GtkStatusIcon *tray_icon,char *domain_name) {
	xmlChar *status;

        xmlDocPtr doc = xmlParseFile(domain_name);//cctray.xml");

        xmlNodePtr projects = doc->children;
        xmlNodePtr project = projects->children;
	if(project->type == XML_TEXT_NODE)
	while(project->next) {
		project = project->next;
		if(project->type != XML_TEXT_NODE)
			break;
	}

        if(xmlHasProp(project, (xmlChar *)"lastBuildStatus")) {
		status = xmlGetProp(project, (xmlChar *)"lastBuildStatus");
		tray_icon = change_tray_icon(tray_icon, !strcmp((char *)status, "Success"));
		send_notification("Project", (char *)xmlGetProp(project, (xmlChar *)"name"), "Project name");
		xmlFree(status);
	}
	xmlFreeDoc(doc);
	return "";
}

void tray_icon_on_click(GtkStatusIcon *status_icon, gpointer user_data) {
        getProjectBuildStatus(status_icon, "punetwocruise01.thoughtworks.com");
        send_notification("Click", "You have clicked on the system tray icon!!!!", "Clicked on tray icon");
    printf("Clicked on tray icon\n");
}

static void destroy(GtkWidget *widget, gpointer data) {
        gtk_main_quit ();
}

void tray_icon_on_menu(GtkStatusIcon *status_icon, guint button, guint activate_time, gpointer user_data) {
	if (!my_menu)
        {
                GtkWidget *item;
                my_menu = gtk_menu_new();

                item = gtk_menu_item_new_with_label("Update Status");
                gtk_menu_append(my_menu, item);
                /*g_signal_connect(G_OBJECT(item), "activate",
                                G_CALLBACK(on_blink_change),
                                GUINT_TO_POINTER(TRUE));*/

                item = gtk_menu_item_new_with_label("Configure");
                gtk_menu_append(my_menu, item);
                /*g_signal_connect (G_OBJECT(item), "activate",
                                G_CALLBACK(on_blink_change), 
                                GUINT_TO_POINTER(FALSE));*/
                
                item = gtk_menu_item_new_with_label("Quit");
                gtk_menu_append(my_menu, item);
                g_signal_connect (G_OBJECT(item), "activate",
                                G_CALLBACK(destroy), 
                                NULL);
        }
        
        gtk_widget_show_all(my_menu);

        gtk_menu_popup(GTK_MENU(my_menu),
                        NULL,
                        NULL,
                        gtk_status_icon_position_menu,
                        status_icon,
                        button,
                        activate_time);
        printf("Popup menu\n");
}

static GtkStatusIcon *create_tray_icon() {
        GtkStatusIcon *tray_icon;

        tray_icon = gtk_status_icon_new();
        g_signal_connect(G_OBJECT(tray_icon), "activate",
                         G_CALLBACK(tray_icon_on_click), NULL);
        g_signal_connect(G_OBJECT(tray_icon),
                         "popup-menu",
                         G_CALLBACK(tray_icon_on_menu), NULL);
        gtk_status_icon_set_from_icon_name(tray_icon,
                                           GTK_STOCK_STOP);
        gtk_status_icon_set_tooltip(tray_icon,
                                    "Example Tray Icon");
        gtk_status_icon_set_visible(tray_icon, TRUE);

        return tray_icon;
}

int main(int argc, char **argv) { 
        GtkStatusIcon *tray_icon;
	char *domain_name =  "http://punetwocruise01.thoughtworks.com:8080/dashboard/cctray.xml";

        gtk_init(&argc, &argv); 
        tray_icon = create_tray_icon();
        
	getProjectBuildStatus(tray_icon,domain_name);
	gtk_main();

	free(domain_name);
        return 0; 
}

