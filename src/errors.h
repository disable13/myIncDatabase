#ifndef ERRORS_H
#define ERRORS_H

#define _ERR_NO_ERROR 0x00
#define _ERR_UNKNOW 0xff
#define _ERR_CANTOPEN 0x01

// DNamespace
#define _ERR_NS_SYNTAX_PRO 0x0101
#define _ERR_NS_NOROOT 0x0102
#define _ERR_NS_NOCNFNODE 0x0103
#define _ERR_NS_TYPE 0x0104
#define _ERR_NS_CNFNOINIT 0x0105
#define _ERR_NS_SQLNOINIT 0x0106

// URI
#define _ERR_URI_SYNTAX 0x0201

// Database
#define _ERR_DB_CONNECT 0x0301

#endif // ERRORS_H
