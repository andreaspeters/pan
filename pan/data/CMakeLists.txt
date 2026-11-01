set(SOURCES 
 article.cc
 article-cache.cc
 encode-cache.cc
 cert-store.cc
 data.cc
 parts.cc
 xref.cc
)

add_library(data ${SOURCES})

# Regression test for Xref fields with and without whitespace after the colon.
add_executable(xref-test xref-test.cc)
target_compile_options(xref-test PRIVATE "${CXX_STD}" "-Wreorder" "-Wzero-as-null-pointer-constant")
target_link_libraries(xref-test data generalutils)
add_test(NAME xref-test COMMAND xref-test)

# Sanity checking
# https://cmake.org/cmake/help/latest/command/add_compile_options.html
target_compile_options(data PRIVATE "${CXX_STD}" "-Wreorder" "-Wzero-as-null-pointer-constant" )

target_link_libraries(data PUBLIC PkgConfig::GTK3)
target_link_libraries(data PUBLIC PkgConfig::GMIME)

if(WANT_GNUTLS)
  target_link_libraries(data PUBLIC PkgConfig::GNUTLS)
endif()

# this test is broken
# add_executable(article-test article-test.cc)
# add_test(NAME article COMMAND article-test)
