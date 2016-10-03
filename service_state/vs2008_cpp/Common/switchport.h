#ifndef _SWITCHPORT_H_
#define _SWITCHPORT_H_

// 現状はスタティックリンクとする
#define API_STATIC_LINK

#ifdef _MSC_VER
	#ifdef API_STATIC_LINK
		#define API
	#else
		#ifdef API_EXPORTS
			#define API __declspec(dllexport)
		#else
			#define API __declspec(dllimport)
		#endif
	#endif
#else
	#define API
#endif
#endif	// _SWITCHPORT_H_
