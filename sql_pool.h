
#ifndef SQL_POOL_H_
#define SQL_POOL_H_

#include <stdio.h>
#include <string.h>
#include <mysql/mysql.h>
#include <sys/socket.h>
#include <netdb.h>
#include "g_net_global.h"

#define DB_LEN_IP			15
#define DB_LEN_PORT			8
#define DB_LEN_DBNAME		64
#define DB_LEN_DBUSER		64
#define DB_LEN_PASSWD		64

#define POOL_NUMBER			20
/* 连接节点 */
typedef struct _SQL_SOCK_NODE{
	MYSQL            fd;                  /* MYSQL对象文件描述符 */
	MYSQL            *mysql_sock;         /* 指向已经连接的MYSQL的指针 */
	pthread_mutex_t  sql_lock;            /* 互斥锁; 用在线程对数据的操作限制*/
	int              used;                /* 使用标志 */
	int              index;               /* 下标 */
	enum{DB_DISCONN, DB_CONN}sql_state;   /* 连接状态 */         
} SQL_SOCK_NODE;
/* 连接池 */
typedef struct _POOL_SQL_SOCK{
	SQL_SOCK_NODE sql_pool[POOL_NUMBER];

    char ip[DB_LEN_IP + 1];
	int  port;
	char dbname[DB_LEN_DBNAME + 1];
	char user[DB_LEN_DBUSER + 1];
	char passwd[DB_LEN_PASSWD + 1];

    int pool_number;/* 连接数量 */
} POOL_SQL_SOCK;
/*
	MYSQL *mysql_real_connect (MYSQL *mysql,
								const char *host,
								const char *user, 
								const char *passwd, 
								const char *db, 
								unsigned int port,
								const char *unix_socket,
	//使用 unix连接方式，unix_socket为NULL时，表明不使用socket或管道机制，最后一个参数经常设置为0
								unsigned long client_flag)
*/
void sql_pool_destroy();
SQL_SOCK_NODE *get_db_connect_from_pool();/*取出一个未使用的连接*/
void release_sock_to_sql_pool(SQL_SOCK_NODE *n);
void check_sql_sock_normal(SQL_SOCK_NODE *n);
int sql_pool_create(int connect_pool_number);



#endif /* SQL_POOL_H_ */
