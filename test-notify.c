#include <gtk/gtk.h> 
#include <libnotify/notify.h>  
#include <unistd.h>  

/* gcc -o status status.c `pkg-config --libs --cflags gtk+-2.0` */ 
/* gcc -Wall -o test-notify test-notify.c `pkg-config --libs --cflags glib-2.0 gtk+-2.0` -lnotify */ // THIS WORKS

void tray_icon_on_click(GtkStatusIcon *status_icon, 
                        gpointer user_data) 
{ 
	NotifyNotification *n;  
        notify_init("Clicked on tray icon\n");  
        n = notify_notification_new ("Click","You have clicked on the sysem tray icon!!!", NULL, NULL);  
        notify_notification_set_timeout(n, 3000); //3 seconds  
        if (!notify_notification_show (n, NULL)) {  
            g_error("Failed to send notification.\n");  
            return;  
        }  
        g_object_unref(G_OBJECT(n));
        printf("Clicked on tray icon\n"); 
} 

void tray_icon_on_menu(GtkStatusIcon *status_icon, guint button, 
                       guint activate_time, gpointer user_data) 
{ 
	NotifyNotification *n;  
        notify_init("Popup menu\n");  
        n = notify_notification_new ("Popup","Popup menu has been popped up!!!!", NULL, NULL);  
        notify_notification_set_timeout(n, 3000); //3 seconds  
        if (!notify_notification_show (n, NULL)) {  
            g_error("Failed to send notification.\n");  
            return;  
        }  
        g_object_unref(G_OBJECT(n));
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
                                           GTK_STOCK_MEDIA_STOP); 
        gtk_status_icon_set_tooltip(tray_icon, 
                                    "Example Tray Icon"); 
        gtk_status_icon_set_visible(tray_icon, TRUE); 

        return tray_icon; 
} 

int main(int argc, char **argv) { 
        GtkStatusIcon *tray_icon; 

        gtk_init(&argc, &argv); 
        tray_icon = create_tray_icon(); 
        gtk_main(); 

        return 0; 
} 

