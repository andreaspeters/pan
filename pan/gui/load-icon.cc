#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "load-icon.h"
#include "pan/general/debug.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <gdk-pixbuf/gdk-pixbuf.h>
#include <glib/gi18n-lib.h>

namespace pan {

extern "C" void pan_icons_register_resource(void);

GdkPixbuf *load_icon(gchar const *file_name)
{
  static gsize resources_registered = 0;
  if (g_once_init_enter(&resources_registered)) {
    pan_icons_register_resource();
    g_once_init_leave(&resources_registered, 1);
  }

  // Prefer the copy embedded in the executable over paths relative to the
  // current working directory or the installation prefix.
  gchar *resource_path = g_strdup_printf("/org/gnome/pan/icons/%s", file_name);
  GError *error = NULL;
  GdkPixbuf *pixbuf = gdk_pixbuf_new_from_resource(resource_path, &error);
  g_free(resource_path);
  if (pixbuf != nullptr) {
    return pixbuf;
  }
  if (error != NULL) {
    pan_debug("Unable to load embedded icon " << file_name << ": "
                                              << error->message);
    g_error_free(error);
  }

  // try local icon
  pixbuf = load_icon_from_path(file_name, "pan/icons");
  if (pixbuf != nullptr) {
    return pixbuf;
  }

  // try system icon
  pixbuf = load_icon_from_path(file_name, PAN_SYSTEM_ICON_PATH);
  if (pixbuf != nullptr)
  {
    return pixbuf;
  }

  std::cerr << "Unable to load " << file_name
            << " icon. Use --debug flag for more details" << std::endl;
  return nullptr;
}

GdkPixbuf *load_icon_from_path(gchar const *file_name, gchar const *icon_dir)
{
  GError *error = NULL;
  gchar *icon_path = g_build_filename(icon_dir, file_name, NULL);
  GdkPixbuf *pixbuf = gdk_pixbuf_new_from_file(icon_path, &error);

  if (error != NULL)
  {
    pan_debug("Unable to load icon " << file_name << " from " << icon_dir
                                     << ": " << error->message);
    g_error_free(error);
  }

  g_free(icon_path);
  return pixbuf;
}

} // namespace pan
