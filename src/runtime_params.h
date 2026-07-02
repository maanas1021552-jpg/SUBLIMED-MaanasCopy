#ifndef RUNTIME_PARAMS_H
#define RUNTIME_PARAMS_H

#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef void (*runtime_param_setter_fn)(const char *, double);

static inline char *runtime_params_trim(char *s){
  char *end;
  while(*s == ' ' || *s == '\t' || *s == '\n' || *s == '\r') s++;
  if(*s == '\0') return s;
  end = s + strlen(s) - 1;
  while(end > s && (*end == ' ' || *end == '\t' || *end == '\n' || *end == '\r')) end--;
  end[1] = '\0';
  return s;
}

static inline void runtime_params_default_path(char *path, size_t pathSize, const char *sourcePath, const char *fileName){
  const char *slash = strrchr(sourcePath, '/');

  if(slash == NULL){
    snprintf(path, pathSize, "%s", fileName);
    return;
  }

  snprintf(path, pathSize, "%.*s/%s", (int)(slash - sourcePath), sourcePath, fileName);
}

static inline int runtime_params_load(const char *sourcePath, const char *defaultFileName, runtime_param_setter_fn setter){
  const char *envPath;
  const char *paramsPath = NULL;
  const char *candidatePaths[3];
  char defaultPath[PATH_MAX];
  FILE *fp;
  char line[256];
  int loaded = 0;
  int i;

  runtime_params_default_path(defaultPath, sizeof(defaultPath), sourcePath, defaultFileName);
  envPath = getenv("SUBLIMED_PARAM_FILE");
  candidatePaths[0] = (envPath != NULL && envPath[0] != '\0') ? envPath : NULL;
  candidatePaths[1] = defaultFileName;
  candidatePaths[2] = defaultPath;

  fp = NULL;
  for(i=0;i<3;i++){
    if(candidatePaths[i] == NULL || candidatePaths[i][0] == '\0') continue;
    fp = fopen(candidatePaths[i], "r");
    if(fp != NULL){
      paramsPath = candidatePaths[i];
      break;
    }
  }

  if(fp == NULL) return 0;

  while(fgets(line, sizeof(line), fp) != NULL){
    char *hash = strchr(line, '#');
    char *eq;
    char *key;
    char *valueStr;
    char *endPtr;
    double value;

    if(hash != NULL) *hash = '\0';
    eq = strchr(line, '=');
    if(eq == NULL) continue;

    *eq = '\0';
    key = runtime_params_trim(line);
    valueStr = runtime_params_trim(eq + 1);
    if(key[0] == '\0' || valueStr[0] == '\0') continue;

    errno = 0;
    value = strtod(valueStr, &endPtr);
    endPtr = runtime_params_trim(endPtr);
    if(errno != 0 || *endPtr != '\0') continue;

    setter(key, value);
    loaded = 1;
  }

  fclose(fp);
  return loaded;
}

#endif
