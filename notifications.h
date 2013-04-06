#include <libnotify/notify.h>

void send_notification(const gchar *summary, const gchar *body, const char *app_name) {
	gint timeout = 3000;
	NotifyNotification *n;
	notify_init(g_strjoin(app_name,"\n",(char *)NULL));  
	n = notify_notification_new (summary,body, NULL, NULL);
	notify_notification_set_timeout(n, timeout);
	if (!notify_notification_show (n, NULL)) {  
		g_error("Failed to send notification.\n");  
	    return;
	}
	g_object_unref(G_OBJECT(n));
}
