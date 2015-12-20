#include <stdio.h>
#include <string.h>
#include <stdarg.h>

#include <log.h>

extern void CAOS_OutputDebugString(const char *pzStr);

static char tmpStr[80];//,tmpStr2[1024];

void __logi(const char *pzFunc,const char *pzFile,int line,const char *pzMessage,...) {
	va_list ap;

	if( strlen(pzMessage) > 128 ) {
	  CAOS_OutputDebugString("Log output too long. Skipping");
	  return;
	}

//	snprintf(tmpStr,1023,"%s@%s:%i: ",pzFunc,pzFile,line);

	va_start(ap,pzMessage);
	vsnprintf(tmpStr,80,pzMessage,ap);
	va_end(ap);

//	strcat(tmpStr,tmpStr2);
	strcat(tmpStr,"\r\n");

	CAOS_OutputDebugString(tmpStr);
}
