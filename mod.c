#include "redismodule.h"
#include <Python.h>
#include "mod_wrap.c"

/* ECHO <string> - Echo back a string sent from the client */
int EchoCommand(RedisModuleCtx *ctx, RedisModuleString **argv, int argc) {
  if (argc < 2) return RedisModule_WrongArity(ctx);
  size_t sz;
  const char *str = RedisModule_StringPtrLen(argv[1], &sz);
  //RedisModule_Log(ctx, "warning", argv[1]);
  PyRun_SimpleString(str);
  if (PyErr_Occurred()) PyErr_Print();
  return RedisModule_ReplyWithString(ctx, argv[1]);
}

//
//static PyMethodDef EmbMethods[] = {
//  {"numargs", emb_numargs, METH_VARARGS,
//    "Return the number of arguments received by the process."},
//  {NULL, NULL, 0, NULL}
//};


/* Registering the module */
int RedisModule_OnLoad(RedisModuleCtx *ctx) {
  if (RedisModule_Init(ctx, "example", 1, REDISMODULE_APIVER_1) == REDISMODULE_ERR) {
     return REDISMODULE_ERR;
  }
  if (RedisModule_CreateCommand(ctx, "example.echo", EchoCommand, "readonly", 1,1,1) == REDISMODULE_ERR) {
    return REDISMODULE_ERR;
  }
  Py_Initialize();
  // Py_InitModule("redismod", methods);
  SWIG_init();
  return 0;
}
