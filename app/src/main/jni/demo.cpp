#include <stdio.h>
#include <stdlib.h>
#include <jni.h>
#include <assert.h>
#include <sys/types.h>
#include <android/log.h>

#define LOG_TAG "crashdemo"
#define LOGE(...)  __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)
#define LOGI(...)  __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)


#define JNIREG_CLASS "example/com/bugrptdemo/MainActivity"

void* g_handle = NULL;

extern "C" {
	JNIEXPORT void msg(JNIEnv *env, jobject  clazz, jstring str);
};

//jstring to char* 
char* jstringTostring(JNIEnv* env, jstring jstr) 
{ 
	char* rtn = NULL; 
	jclass clsstring = env->FindClass("java/lang/String"); 
	jstring strencode = env->NewStringUTF("utf-8"); 
	jmethodID mid = env->GetMethodID(clsstring, "getBytes", "(Ljava/lang/String;)[B"); 
	jbyteArray barr= (jbyteArray)env->CallObjectMethod(jstr, mid, strencode); 
	jsize alen = env->GetArrayLength(barr); 
	jbyte* ba = env->GetByteArrayElements(barr, JNI_FALSE); 
	if (alen > 0) 
	{ 
		rtn = (char*)malloc(alen + 1); 
		memcpy(rtn, ba, alen); 
		rtn[alen] = 0; 
	} 
	env->ReleaseByteArrayElements(barr, ba, 0); 
	return rtn; 
}

JNIEXPORT void msg(JNIEnv *env, jobject  clazz, jstring str)
{
	char *pszstr = NULL; 

	pszstr = jstringTostring(env, str);

	LOGI("%s", pszstr);

	//free(pszstr);
	free(pszstr);
	pszstr = NULL;
	*pszstr = 0;
}

/**
* Table of methods associated with a single class.
*/
static JNINativeMethod gMethods[] = {
	{ "msg", "(Ljava/lang/String;)V", (void*)msg},
};

/*
* Register native methods for all classes we know about.
*/
static int registerNativeMethods(JNIEnv* env)
{
	int nError = 0;
	jclass clazz = NULL;

	clazz = env->FindClass(JNIREG_CLASS);
	if (clazz == NULL) {

		LOGE("clazz is null");
		return JNI_FALSE;
	}

	nError = env->RegisterNatives(clazz, gMethods, sizeof(gMethods) / sizeof(gMethods[0]) );
	if ( nError < 0 ) {

		LOGE("RegisterNatives error: %d num: %d",nError, (int)(sizeof(gMethods) / sizeof(gMethods[0])) );

		return JNI_FALSE;
	}

	return JNI_TRUE;
}

/*
* Set some test stuff up.
*
* Returns the JNI version on success, -1 on failure.
*/
JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM* vm, void* reserved)
{
	JNIEnv* env = NULL;
	jint result = -1;

	if(vm->GetEnv((void**) &env,JNI_VERSION_1_4) != JNI_OK){
		return -1;
	}
	assert(env != NULL);

	if (!registerNativeMethods(env)) {

		LOGE("registerNativeMethods failed");

		return -1;
	}

	/* success -- return valid version number */
	result = JNI_VERSION_1_4;

	return result;
}