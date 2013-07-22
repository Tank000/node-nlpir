#include <node.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "NLPIR.h"
using namespace v8;

Handle<Value> initNLPIR(const Arguments& args) {
  HandleScope scope;
  Handle<Value> arg = args[0];
  String::Utf8Value initDir(arg);
  bool initRst = NLPIR_Init(*initDir,UTF8_CODE);
  if(!initRst){
    ThrowException(Exception::TypeError(String::New("initial failure")));
  }
  return scope.Close(Undefined());
}

Handle<Value> importDictFile(const Arguments& args) {
  HandleScope scope;
  Handle<Value> arg = args[0];
  String::Utf8Value dictFile(arg);
  FILE *fp;
  fp = fopen(*dictFile, "r");
  if(fp == NULL){
    ThrowException(Exception::TypeError(String::New("can not open the dict file")));
    return scope.Close(False());
  }
  unsigned wordCount = NLPIR_ImportUserDict(*dictFile);
  return scope.Close(Number::New(wordCount));
}

Handle<Value> segment(const Arguments& args) {
  HandleScope scope;
  Local<Function> cb = Local<Function>::Cast(args[1]);
  Handle<Value> arg0 = args[0];
  String::Utf8Value txt(arg0);
  const char* sRst = NLPIR_ParagraphProcess(*txt,1);
  const unsigned argc = 1;
  Local<Value> argv[argc] = { Local<Value>::New(String::New(sRst)) };
  cb->Call(Context::GetCurrent()->Global(), argc, argv);
  return scope.Close(Undefined());
}

void Init(Handle<Object> target) {
  target->Set(String::NewSymbol("init"),
      FunctionTemplate::New(initNLPIR)->GetFunction());

  target->Set(String::NewSymbol("segment"),
      FunctionTemplate::New(segment)->GetFunction());

  target->Set(String::NewSymbol("importDictFile"),
      FunctionTemplate::New(importDictFile)->GetFunction());
}

NODE_MODULE(nlpir, Init)