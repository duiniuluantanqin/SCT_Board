/*
 * FileName: TWGLClient.h
 * Author: TimeLink Inc.
 * Modified: 2012-04-24
 * Email: sdk@timelink.cn
 * Comment: This defines the interface of `TWGLClient` module.
 */

#ifndef __TWGL_TWGLCLIENT_H__
#define __TWGL_TWGLCLIENT_H__


#ifdef WIN32
#	ifdef TWGLCLIENT_EXPORTS
#		define TWGLCLIENT_API //__declspec(dllexport)
#	else
#		define TWGLCLIENT_API __declspec(dllimport)
#	endif
#else /* POSIX */
#	define TWGLCLIENT_API
#	define __stdcall
#endif


enum TWGL_ERRORCODE
{
	TWGL_ERROR_SESSION_NOT_FOUND = -2,
	TWGL_ERROR_COMMUNICATE = -1,
	TWGL_NOERROR,
	TWGL_ERROR_UNKNOWN,
	TWGL_ERROR_CONFIG_FILE_NOT_FOUND,
	TWGL_ERROR_CONFIG_FILE,
	TWGL_ERROR_SERVICE_REFUSED,
	TWGL_ERROR_GESTURE_NOT_FOUND,
	TWGL_ERROR_SCENE_NOT_FOUND,
	TWGL_ERROR_PROFILE_NOT_FOUND,
};


typedef void (__stdcall *twEventHandler) (const char* event);
typedef int (__stdcall *twHitTester) (const float x, const float y);

typedef struct twDeviceInfo
{
	unsigned int size;
	unsigned int index;
	unsigned int maxContacts;
	unsigned int fps;
	unsigned int diagonal;
	unsigned int aspectRatioX;
	unsigned int aspectRatioY;
	const char id[16];
} twDeviceInfo_t;


#ifdef __cplusplus
extern "C"
{
#endif
	TWGLCLIENT_API void __stdcall twSetEventHandler(twEventHandler pfn);
	TWGLCLIENT_API void __stdcall twSetHitTester(twHitTester pfn);
	TWGLCLIENT_API int __stdcall twConnectGestureServer(void);
	TWGLCLIENT_API int __stdcall twDisconnectGestureServer(void);
	TWGLCLIENT_API int __stdcall twSuspendGestureServer(void);
	TWGLCLIENT_API int __stdcall twResumeGestureServer(void);
	TWGLCLIENT_API int __stdcall twIsConnected(void);
	TWGLCLIENT_API int __stdcall twRegisterGesture(const int sceneId, const char* gestureId);
	TWGLCLIENT_API int __stdcall twUnregisterGesture(const int sceneId, const char* gestureId);
	TWGLCLIENT_API int __stdcall twEnableScene(const int sceneId);
	TWGLCLIENT_API int __stdcall twDisableScene(const int sceneId);
	TWGLCLIENT_API int __stdcall twFreeScene(const int sceneId);
	TWGLCLIENT_API int __stdcall twSetProfile(const char* profileId);
	TWGLCLIENT_API int __stdcall twUnsetProfile(void);

	TWGLCLIENT_API const char* __stdcall twGetOemLabel(const int index);
	TWGLCLIENT_API int __stdcall twGetDevicesInfo(twDeviceInfo_t* pDevicesInfo);

	TWGLCLIENT_API int __stdcall twRequest(const char* statement);
	TWGLCLIENT_API const char* __stdcall twGetLastResponse();
#ifdef __cplusplus
};
#endif

#endif
