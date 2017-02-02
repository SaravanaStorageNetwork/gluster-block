/*
 * Please do not edit this file.
 * It was generated using rpcgen.
 */

#include "block.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <rpc/pmap_clnt.h>
#include <string.h>
#include <memory.h>
#include <sys/socket.h>
#include <netinet/in.h>

#ifndef SIG_PF
#define SIG_PF void(*)(int)
#endif



static void
gluster_block_cli_1(struct svc_req *rqstp, register SVCXPRT *transp)
{
	union {
		blockCreateCli block_create_cli_1_arg;
		blockListCli block_list_cli_1_arg;
		blockInfoCli block_info_cli_1_arg;
		blockDeleteCli block_delete_cli_1_arg;
	} argument;
	char *result;
	xdrproc_t _xdr_argument, _xdr_result;
	char *(*local)(char *, struct svc_req *);

	switch (rqstp->rq_proc) {
	case NULLPROC:
		(void) svc_sendreply (transp, (xdrproc_t) xdr_void, (char *)NULL);
		return;

	case BLOCK_CREATE_CLI:
		_xdr_argument = (xdrproc_t) xdr_blockCreateCli;
		_xdr_result = (xdrproc_t) xdr_blockResponse;
		local = (char *(*)(char *, struct svc_req *)) block_create_cli_1_svc;
		break;

	case BLOCK_LIST_CLI:
		_xdr_argument = (xdrproc_t) xdr_blockListCli;
		_xdr_result = (xdrproc_t) xdr_blockResponse;
		local = (char *(*)(char *, struct svc_req *)) block_list_cli_1_svc;
		break;

	case BLOCK_INFO_CLI:
		_xdr_argument = (xdrproc_t) xdr_blockInfoCli;
		_xdr_result = (xdrproc_t) xdr_blockResponse;
		local = (char *(*)(char *, struct svc_req *)) block_info_cli_1_svc;
		break;

	case BLOCK_DELETE_CLI:
		_xdr_argument = (xdrproc_t) xdr_blockDeleteCli;
		_xdr_result = (xdrproc_t) xdr_blockResponse;
		local = (char *(*)(char *, struct svc_req *)) block_delete_cli_1_svc;
		break;

	default:
		svcerr_noproc (transp);
		return;
	}
	memset ((char *)&argument, 0, sizeof (argument));
	if (!svc_getargs (transp, (xdrproc_t) _xdr_argument, (caddr_t) &argument)) {
		svcerr_decode (transp);
		return;
	}
	result = (*local)((char *)&argument, rqstp);
	if (result != NULL && !svc_sendreply(transp, (xdrproc_t) _xdr_result, result)) {
		svcerr_systemerr (transp);
	}
	if (!svc_freeargs (transp, (xdrproc_t) _xdr_argument, (caddr_t) &argument)) {
		fprintf (stderr, "%s", "unable to free arguments");
		exit (1);
	}
	return;
}

static void
gluster_block_1(struct svc_req *rqstp, register SVCXPRT *transp)
{
	union {
		blockCreate block_create_1_arg;
		blockDelete block_delete_1_arg;
	} argument;
	char *result;
	xdrproc_t _xdr_argument, _xdr_result;
	char *(*local)(char *, struct svc_req *);

	switch (rqstp->rq_proc) {
	case NULLPROC:
		(void) svc_sendreply (transp, (xdrproc_t) xdr_void, (char *)NULL);
		return;

	case BLOCK_CREATE:
		_xdr_argument = (xdrproc_t) xdr_blockCreate;
		_xdr_result = (xdrproc_t) xdr_blockResponse;
		local = (char *(*)(char *, struct svc_req *)) block_create_1_svc;
		break;

	case BLOCK_DELETE:
		_xdr_argument = (xdrproc_t) xdr_blockDelete;
		_xdr_result = (xdrproc_t) xdr_blockResponse;
		local = (char *(*)(char *, struct svc_req *)) block_delete_1_svc;
		break;

	default:
		svcerr_noproc (transp);
		return;
	}
	memset ((char *)&argument, 0, sizeof (argument));
	if (!svc_getargs (transp, (xdrproc_t) _xdr_argument, (caddr_t) &argument)) {
		svcerr_decode (transp);
		return;
	}
	result = (*local)((char *)&argument, rqstp);
	if (result != NULL && !svc_sendreply(transp, (xdrproc_t) _xdr_result, result)) {
		svcerr_systemerr (transp);
	}
	if (!svc_freeargs (transp, (xdrproc_t) _xdr_argument, (caddr_t) &argument)) {
		fprintf (stderr, "%s", "unable to free arguments");
		exit (1);
	}
	return;
}

void *
cli_thread_proc (void *vargp)
{
  register SVCXPRT *transp;
  struct sockaddr_un saun;
  int sockfd, len;

  if ((sockfd = socket(AF_UNIX, SOCK_STREAM, IPPROTO_IP)) < 0) {
    perror("server: socket");
    exit(1);
  }

  saun.sun_family = AF_UNIX;
  strcpy(saun.sun_path, ADDRESS);

  unlink(ADDRESS);
  len = sizeof(saun.sun_family) + strlen(saun.sun_path);

  if (bind(sockfd, (struct sockaddr *) &saun, len) < 0) {
    perror("server: bind");
    exit(1);
  }

  transp = svcunix_create(sockfd, 0, 0, ADDRESS);
  if (transp == NULL) {
    fprintf (stderr, "%s", "cannot create tcp service");
    exit(1);
  }
	
  if (!svc_register(transp, GLUSTER_BLOCK_CLI, GLUSTER_BLOCK_CLI_VERS, gluster_block_cli_1, IPPROTO_IP)) {
		fprintf (stderr, "%s", "unable to register (GLUSTER_BLOCK_CLI, GLUSTER_BLOCK_CLI_VERS, unix|local).");
		exit(1);
	}

  svc_run ();

  return NULL;
}

void *
server_thread_proc(void *vargp)
{
  register SVCXPRT *transp;
  struct sockaddr_in sain;
  int sockfd;

  if ((sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
    perror("server: socket");
    exit(1);
  }

  sain.sin_family = AF_INET;
  sain.sin_addr.s_addr = INADDR_ANY;
  sain.sin_port = htons(24006);

  if (bind(sockfd, (struct sockaddr *) &sain, sizeof (sain)) < 0) {
    perror("server: bind");
    exit(1);
  }

  transp = svctcp_create(sockfd, 0, 0);
  if (transp == NULL) {
    fprintf (stderr, "%s", "cannot create tcp service");
    exit(1);
  }

	if (!svc_register(transp, GLUSTER_BLOCK, GLUSTER_BLOCK_VERS, gluster_block_1, IPPROTO_TCP)) {
		fprintf (stderr, "%s", "unable to register (GLUSTER_BLOCK, GLUSTER_BLOCK_VERS, tcp).");
		exit(1);
	}

  svc_run ();

  return NULL;
}


int
main (int argc, char **argv)
{
  pthread_t cli_thread;
  pthread_t server_thread;


	pmap_unset (GLUSTER_BLOCK_CLI, GLUSTER_BLOCK_CLI_VERS);
  pmap_unset (GLUSTER_BLOCK, GLUSTER_BLOCK_VERS);

  pthread_create(&cli_thread, NULL, cli_thread_proc , NULL);
  pthread_create(&server_thread, NULL, server_thread_proc , NULL);

  pthread_join(cli_thread, NULL);
  pthread_join(server_thread, NULL);


  fprintf (stderr, "%s", "svc_run returned");
  exit (0);
  /* NOTREACHED */
}
