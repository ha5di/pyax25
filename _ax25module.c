#include <Python.h>

#include <sys/types.h>
#include <netinet/in.h>
#include <sys/ioctl.h>
#include <netdb.h>

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <poll.h>

// #include <curses.h>

#include <sys/socket.h>
#include <net/if.h>
#include <net/ethernet.h>

#include <netax25/ax25.h>
#include <netax25/axconfig.h>
#include <netax25/axlib.h>

static PyObject * config_get_ports_nr(PyObject* self, PyObject* args)
{
    int n;

    n = ax25_config_load_ports();
    return Py_BuildValue("i", n);
}

static PyObject * config_get_first_port(PyObject* self, PyObject* args)
{
    char *nextport;
    
    nextport = ax25_config_get_next(NULL);
    return Py_BuildValue("s", nextport);
}

static PyObject * config_get_next_port(PyObject* self, PyObject* args)
{
    char *port;
    char *nextport;
    
    PyArg_ParseTuple(args, "s", &port);
    nextport = ax25_config_get_next(port);
    return Py_BuildValue("s", nextport);
}

static PyObject * config_get_name(PyObject* self, PyObject* args)
{
    char *device;
    char *port;
    
    PyArg_ParseTuple(args, "s", &device);
    port = ax25_config_get_name(device);
    return Py_BuildValue("s", port);
}

static PyObject * config_get_addr(PyObject* self, PyObject* args)
{
    char *port;
    char *addr;
    
    PyArg_ParseTuple(args, "s", &port);
    addr = ax25_config_get_addr(port);
    return Py_BuildValue("s", addr);
}


static PyObject * config_get_dev(PyObject* self, PyObject* args)
{
    char *device;
    char *port;
    
    PyArg_ParseTuple(args, "s", &port);
    device = ax25_config_get_dev(port);
    return Py_BuildValue("s", device);
}

static PyObject * config_get_window(PyObject* self, PyObject* args)
{
    int window;
    char *port;
    
    PyArg_ParseTuple(args, "s", &port);
    window = ax25_config_get_window(port);
    return Py_BuildValue("i", window);
}

static PyObject * config_get_paclen(PyObject* self, PyObject* args)
{
    int paclen;
    char *port;
    
    PyArg_ParseTuple(args, "s", &port);
    paclen = ax25_config_get_paclen(port);
    return Py_BuildValue("i", paclen);
}

static PyObject * config_get_baud(PyObject* self, PyObject* args)
{
    int baud;
    char *port;
    
    PyArg_ParseTuple(args, "s", &port);
    baud = ax25_config_get_baud(port);
    return Py_BuildValue("i", baud);
}

static PyObject * config_get_desc(PyObject* self, PyObject* args)
{
    char *desc;
    char *port;
    
    PyArg_ParseTuple(args, "s", &port);
    desc = ax25_config_get_desc(port);
    return Py_BuildValue("s", desc);
}


static PyObject *  rawsocket(PyObject* self, PyObject* args)
{
    int fd;
    
    fd = socket(PF_PACKET, SOCK_PACKET, htons(ETH_P_AX25));        
    
    return Py_BuildValue("i", fd);
}    


static PyObject *  rawtx(PyObject* self, PyObject* args)
{
    int fd;
    int result;
    int ln;
    char *buffer;
    char *dst;
    struct sockaddr sa;
    int asize=sizeof(sa);
    unsigned char b[1000];
    int nr;
    int i;
    int k;
    unsigned char ch;

    PyArg_ParseTuple(args, "isis", &fd, &buffer, &ln, &dst);

    nr = strlen(buffer);

    i = 0;
    k = 0;

    while ( i < nr )
    {
        ch = (buffer[i++] & 0x0f) << 4;
        ch = ch | (buffer[i++] & 0x0f);
        b[k++] = ch;
    }

    strcpy(sa.sa_data, dst);
    sa.sa_family = 3;

    result = sendto(fd, b, k, 0, &sa, asize);

    return Py_BuildValue("i", result);
}
    
static PyObject *  rawrx(PyObject* self, PyObject* args)
{
    int fd;
    int result;
    int asize;
    int size;
    
    struct sockaddr sa;
    struct pollfd pfd;
           
    unsigned char rxbuf[1000];
    
    PyArg_ParseTuple(args, "i", &fd);
    
    pfd.fd = fd;
    pfd.events = 0x040;
    
    result = poll(&pfd, 1, 100);

    size = 0;
    sa.sa_family = 0;
    strcpy(sa.sa_data, "");

    if (result == 1) {
        asize = sizeof(sa);
        size = recvfrom(fd, rxbuf, sizeof(rxbuf), 0, &sa, (socklen_t*)&asize);
    }
    
    return Py_BuildValue("(is)s#", sa.sa_family, sa.sa_data, rxbuf, size);
}    

//////////////////////////////////////////
// Define methods
//////////////////////////////////////////

static PyMethodDef Ax25Methods[] =
{
    {"ConfigGetPortsNr", config_get_ports_nr, METH_VARARGS, ""},
    {"ConfigGetFirstPort", config_get_first_port, METH_VARARGS, ""},
    {"ConfigGetNextPort", config_get_next_port, METH_VARARGS, ""},
    {"ConfigGetName", config_get_name, METH_VARARGS, ""},
    {"ConfigGetAddr", config_get_addr, METH_VARARGS, ""},
    {"ConfigGetDev", config_get_dev, METH_VARARGS, ""},
    {"ConfigGetWindow", config_get_window, METH_VARARGS, ""},
    {"ConfigGetPaclen", config_get_paclen, METH_VARARGS, ""},
    {"ConfigGetBaud", config_get_baud, METH_VARARGS, ""},
    {"ConfigGetDesc", config_get_desc, METH_VARARGS, ""},

    {"RawSocket", rawsocket, METH_VARARGS, ""},
    {"RawRx", rawrx, METH_VARARGS, ""},
    {"RawTx", rawtx, METH_VARARGS, ""},

//    {"loadports", ax25_config_load_ports, METH_VARARGS, ""},

    {NULL, NULL, 0, NULL}
};

// Initialize module

PyMODINIT_FUNC

init_ax25(void)
{
    (void) Py_InitModule("_ax25", Ax25Methods);
}




                