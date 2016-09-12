LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

LOCAL_MODULE := demo
LOCAL_LDFLAGS := -Wl,--build-id
LOCAL_LDLIBS := \
	-llog \
	-lz \
	-lm \

LOCAL_SRC_FILES := \
	E:\client\yunbu\Android\src\bugrpt_demo\app\src\main\jni\demo.cpp \
	E:\client\yunbu\Android\src\bugrpt_demo\app\src\main\jni\empty.cpp \

LOCAL_C_INCLUDES += E:\client\yunbu\Android\src\bugrpt_demo\app\src\main\jni
LOCAL_C_INCLUDES += E:\client\yunbu\Android\src\bugrpt_demo\app\src\debug\jni

include $(BUILD_SHARED_LIBRARY)
