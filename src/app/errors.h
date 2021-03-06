#ifndef ERRORS_H
#define ERRORS_H

/// Error Codes
#define _ERR_NO_ERROR   0x00
#define _ERR_UNKNOW     0xff
#define _ERR_CANTOPEN   0x01

// DNamespace
#define _ERR_NS_SYNTAX_PRO  0x0101
#define _ERR_NS_NOROOT      0x0102
#define _ERR_NS_NOCNFNODE   0x0103
#define _ERR_NS_TYPE        0x0104
#define _ERR_NS_CNFNOINIT   0x0105
#define _ERR_NS_SQLNOINIT   0x0106

// URI
#define _ERR_URI_SYNTAX     0x0201

// Database
#define _ERR_DB_CONNECT     0x0301

/// Thread Out
#define _TRD_ERR_UNKNOW         0x0101
#define _TRD_ERR_CONF_CANTOPEN  0x0102
#define _TRD_ERR_NS_SYNTAX_PRO  0x0103
#define _TRD_ERR_NS_ROOT        0x0104
#define _TRD_ERR_NS_NOCNFNODE   0x0105
#define _TRD_ERR_RX_SYNTAX      0x0106
#define _TRD_ERR_TYPE           0x0107


// Types
#define _TRD_TYPE_TEXT      0x0201
#define _TRD_TYPE_INT       0x0202
#define _TRD_TYPE_TABLE     0x0203
#define _TRD_TYPE_IMAGE     0x0204
#define _TRD_TYPE_DATETIME  0x0205
#define _TRD_TYPE_ARRAY     0x0206

#endif // ERRORS_H
