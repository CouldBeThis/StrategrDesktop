#
# Make a version file containing the current version from git.
#

include(GetGitRevisionDescription)
git_describe(VERSION --tags)
git_get_repo_name(REPO_NAME)

# Get appcast url
string(REGEX REPLACE "^[^/]+/" "" GITHUB_REPO "${REPO_NAME}")
string(REGEX REPLACE "/[^/]+$" "" GITHUB_USER "${REPO_NAME}")
set(APPCAST_URL "https://${GITHUB_USER}.github.io/${GITHUB_REPO}/appcast.xml")

# Parse the version information into pieces.
string(REGEX REPLACE "^v([0-9]+)\\..*" "\\1" VERSION_MAJOR "${VERSION}")
string(REGEX REPLACE "^v[0-9]+\\.([0-9]+).*" "\\1" VERSION_MINOR "${VERSION}")
string(REGEX REPLACE "^v[0-9]+\\.[0-9]+\\.([0-9]+).*" "\\1" VERSION_PATCH "${VERSION}")
string(REGEX REPLACE "^v[0-9]+\\.[0-9]+\\.[0-9]+(.*)" "\\1" VERSION_SHA1 "${VERSION}")
string(REGEX REPLACE "^v" "" VERSION "${VERSION}")

set(VERSION_SHORT "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}")

set(VERSION_FILE "${CMAKE_CURRENT_BINARY_DIR}/version.cpp")
set(CORE_VERSION_FILE "${CMAKE_CURRENT_BINARY_DIR}/core_version.cpp")

file(REMOVE ${VERSION_FILE})
file(REMOVE ${CORE_VERSION_FILE})

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules/version.cpp.in ${VERSION_FILE})
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules/core_version.cpp.in ${CORE_VERSION_FILE})

