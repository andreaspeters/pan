
file(GLOB public_icons hicolor_apps_*.pan.png)

# Embed all Pan PNG icons so the application does not depend on its working
# directory or on the separately installed icon files at runtime.
file(GLOB icon_resources CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/*.png")
set(icon_resource_manifest "${CMAKE_CURRENT_BINARY_DIR}/pan-icons.gresource.xml")
file(WRITE "${icon_resource_manifest}"
  "<gresources>\n  <gresource prefix=\"/org/gnome/pan/icons\">\n")
foreach(icon_file IN LISTS icon_resources)
  cmake_path(GET icon_file FILENAME icon_name)
  file(APPEND "${icon_resource_manifest}" "    <file>${icon_name}</file>\n")
endforeach()
file(APPEND "${icon_resource_manifest}" "  </gresource>\n</gresources>\n")

find_program(GLIB_COMPILE_RESOURCES glib-compile-resources REQUIRED)
set(PAN_ICONS_RESOURCE_SOURCE "${CMAKE_CURRENT_BINARY_DIR}/pan-icons-resource.c")
set(PAN_ICONS_RESOURCE_SOURCE "${PAN_ICONS_RESOURCE_SOURCE}" PARENT_SCOPE)
add_custom_command(
  OUTPUT "${PAN_ICONS_RESOURCE_SOURCE}"
  COMMAND "${GLIB_COMPILE_RESOURCES}" "${icon_resource_manifest}"
          --sourcedir "${CMAKE_CURRENT_SOURCE_DIR}"
          --generate-source --manual-register --c-name pan_icons
          --target "${PAN_ICONS_RESOURCE_SOURCE}"
  DEPENDS "${icon_resource_manifest}" ${icon_resources}
  VERBATIM
)
add_custom_target(pan-icons-resource DEPENDS "${PAN_ICONS_RESOURCE_SOURCE}")

# keep .png otherwise dependency check fails
list(APPEND stock_images
	icon_article_read.png
	icon_article_unread.png
	icon_binary_complete.png
	icon_binary_complete_read.png
	icon_binary_incomplete.png
	icon_binary_incomplete_read.png
	icon_bluecheck.png
	icon_red_flag.png
	icon_by_me.png
	icon_by_others.png
	icon_compose_followup.png
	icon_compose_post.png
	icon_disk.png
	icon_empty.png
	icon_filter.png
	icon_filter_only_attachments.png
	icon_filter_only_cached.png
	icon_filter_only_me.png
	icon_filter_only_read.png
	icon_filter_only_unread.png
	icon_filter_only_watched.png
	icon_get_dialog.png
	icon_get_flagged.png
	icon_get_selected.png
	icon_get_subscribed.png
	icon_layout_1.png
	icon_layout_2.png
	icon_layout_3.png
	icon_layout_4.png
	icon_layout_5.png
	icon_pan_about_logo.png
	icon_pan.png
	icon_read_group.png
	icon_read_more.png
	icon_read_less.png
	icon_read_unread_article.png
	icon_read_unread_thread.png
	icon_score.png
	icon_search_pulldown.png
	icon_x.png
	icon_mozilla_frown.png
	icon_mozilla_smile.png
	icon_mozilla_surprised.png
	icon_mozilla_tongueout.png
	icon_mozilla_wink.png
	icon_plain.png
	icon_cert.png
	icon_status_online.png
  icon_status_offline.png
  icon_status_active.png
  icon_status_queue_empty.png
  icon_status_error.png
  icon_status_idle.png
  icon_status_new_articles.png
  icon_sig_ok.png
  icon_sig_fail.png
  icon_show_signature.png
  icon_prefs_applications.png
  icon_prefs_extras.png
  icon_prefs_hotkeys.png
  icon_prefs_behavior.png
  icon_prefs_fonts.png
  icon_prefs_upload.png
  icon_prefs_colors.png
  icon_prefs_headers.png
  icon_prefs_layout.png
  icon_prefs_actions.png
  icon_prefs_panes.png
  icon_expand_thread.png
  icon_collapse_thread.png
)

foreach(image_file IN LISTS stock_images)
  string(REPLACE "\.png" "" image "${image_file}")
  # install png files
  install(FILES ${image_file} DESTINATION "${CMAKE_INSTALL_DATAROOTDIR}/pan/icons/")
endforeach()

# install hicolor icons
foreach (icon_path ${public_icons})
  cmake_path(GET icon_path FILENAME icon_file)
  string(REPLACE "_" ";" words ${icon_file})
  list(GET words 0 theme)
  list(GET words 1 context)
  list(GET words 2 size)
  list(GET words 3 target)
  set(destdir "${CMAKE_INSTALL_DATAROOTDIR}/icons/${theme}/${size}/${context}/")
  install(FILES ${icon_file} DESTINATION ${destdir} RENAME ${target})
endforeach()
