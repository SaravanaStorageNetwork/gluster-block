/*
  Copyright (c) 2016 Red Hat, Inc. <http://www.redhat.com>
  This file is part of gluster-block.

  This file is licensed to you under your choice of the GNU Lesser
  General Public License, version 3 or any later version (LGPLv3 or
  later), or the GNU General Public License, version 2 (GPLv2), in all
  cases as published by the Free Software Foundation.
*/


# include "utils.h"
# include "config.h"


const char *argp_program_version = ""                                 \
  PACKAGE_NAME" ("PACKAGE_VERSION")"                                  \
  "\nRepository rev: https://github.com/gluster/gluster-block.git\n"  \
  "Copyright (c) 2016 Red Hat, Inc. <https://redhat.com/>\n"          \
  "gluster-block comes with ABSOLUTELY NO WARRANTY.\n"                \
  "It is licensed to you under your choice of the GNU Lesser\n"       \
  "General Public License, version 3 or any later version (LGPLv3\n"  \
  "or later), or the GNU General Public License, version 2 (GPLv2),\n"\
  "in all cases as published by the Free Software Foundation.";


int
glusterBlockCLIOptEnumParse(const char *opt)
{
  int i;


  if (!opt) {
    return GB_CLI_OPT_MAX;
  }

  for (i = 0; i < GB_CLI_OPT_MAX; i++) {
    if (!strcmp(opt, gbCliCmdlineOptLookup[i])) {
      return i;
    }
  }

  return i;
}


int
glusterBlockDaemonOptEnumParse(const char *opt)
{
  int i;


  if (!opt) {
    return GB_DAEMON_OPT_MAX;
  }

  for (i = 0; i < GB_DAEMON_OPT_MAX; i++) {
    /* clip '--' from option */
    while (*opt == '-') {
      opt++;
    }
    if (!strcmp(opt, gbDaemonCmdlineOptLookup[i])) {
      return i;
    }
  }

  return i;
}


int
blockMetaKeyEnumParse(const char *opt)
{
  int i;


  if (!opt) {
    return GB_METAKEY_MAX;
  }

  for (i = 0; i < GB_METAKEY_MAX; i++) {
    if (!strcmp(opt, MetakeyLookup[i])) {
      return i;
    }
  }

  return i;
}


int
blockMetaStatusEnumParse(const char *opt)
{
  int i;


  if (!opt) {
    return GB_METASTATUS_MAX;
  }

  for (i = 0; i < GB_METASTATUS_MAX; i++) {
    if (!strcmp(opt, MetaStatusLookup[i])) {
      return i;
    }
  }

  return i;
}

int blockRemoteCreateRespEnumParse(const char *opt)
{
  int i;


  if (!opt) {
    return GB_REMOTE_CREATE_RESP_MAX;
  }

  for (i = 0; i < GB_REMOTE_CREATE_RESP_MAX; i++) {
    if (strstr(opt, RemoteCreateRespLookup[i])) {
      return i;
    }
  }

  return i;
}


int
gbAlloc(void *ptrptr, size_t size,
        const char *filename, const char *funcname, size_t linenr)
{
  *(void **)ptrptr = calloc(1, size);

  if (*(void **)ptrptr == NULL) {
    errno = ENOMEM;
    return -1;
  }

  return 0;
}


int
gbAllocN(void *ptrptr, size_t size, size_t count,
         const char *filename, const char *funcname, size_t linenr)
{
  *(void**)ptrptr = calloc(count, size);

  if (*(void**)ptrptr == NULL) {
    errno = ENOMEM;
    return -1;
  }

  return 0;
}


int
gbReallocN(void *ptrptr, size_t size, size_t count,
         const char *filename, const char *funcname, size_t linenr)
{
  void *tmp;


  if (xalloc_oversized(count, size)) {
    errno = ENOMEM;
    return -1;
  }
  tmp = realloc(*(void**)ptrptr, size * count);
  if (!tmp && (size * count)) {
    errno = ENOMEM;
    return -1;
  }
  *(void**)ptrptr = tmp;

  return 0;
}


void
gbFree(void *ptrptr)
{
  int save_errno = errno;


  if(*(void**)ptrptr == NULL) {
   return;
  }

  free(*(void**)ptrptr);
  *(void**)ptrptr = NULL;
  errno = save_errno;
}


int
gbStrdup(char **dest, const char *src,
         const char *filename, const char *funcname, size_t linenr)
{
  *dest = NULL;

  if (!src) {
    return 0;
  }

  if (!(*dest = strdup(src))) {
    return -1;
  }

  return 0;
}
