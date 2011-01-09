// pch.h for precompiled headers
#ifndef PCH_H
#define PCH_H

#include <string>
#include <vector>
#include <map>

#ifdef WIN32

#ifndef WINVER
#define WINVER 0x0400
#endif

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#endif // WIN32

#define _min(x,y) x<y?x:y

#ifdef _MAC_

#include <pthread.h>
#include <time.h>
#include <errno.h>

#include <CoreFoundation/CoreFoundation.h>
#include <CoreFoundation/CFBundle.h>
#include <ApplicationServices/ApplicationServices.h>

class Utf8
{
public:
  Utf8(const wchar_t* wsz): m_utf8(NULL), m_wstring(NULL)
  {
    // OS X uses 32-bit wchar
    const int bytes = wcslen(wsz) * sizeof(wchar_t);
    m_wstring = new wchar_t[bytes+1];
    wcslcpy(m_wstring, wsz, bytes+1);
    // kCFStringEncodingUTF32BE for PowerPC
    CFStringEncoding encoding = kCFStringEncodingUTF32LE;
    
    CFStringRef str = CFStringCreateWithBytesNoCopy(NULL, 
                                                    (const UInt8*)wsz, bytes, 
                                                    encoding, false, 
                                                    kCFAllocatorNull
                                                    );
    
    const int bytesUtf8 = CFStringGetMaximumSizeOfFileSystemRepresentation(str);
    m_utf8 = new char[bytesUtf8];
    CFStringGetFileSystemRepresentation(str, m_utf8, bytesUtf8);
    CFRelease(str);
  }   
  Utf8(const char* sz): m_utf8(NULL), m_wstring(NULL)
  {
    // OS X uses 32-bit wchar
    const int bytes = strlen(sz) * sizeof(char);
    // kCFStringEncodingUTF32BE for PowerPC
    CFStringEncoding encoding = kCFStringEncodingUTF32LE;
    
    CFStringRef cfstring = CFStringCreateWithBytesNoCopy(NULL, 
																												 (const UInt8*)sz, bytes, 
																												 kCFStringEncodingUTF8, false, 
																												 kCFAllocatorNull
																												 );
    
    const int bytesUtf8 = CFStringGetMaximumSizeOfFileSystemRepresentation(cfstring);
    m_utf8 = new char[bytesUtf8];
    strlcpy(m_utf8, sz, bytesUtf8);
    
    CFIndex length = CFStringGetLength(cfstring);
    if (length == 0)
      return;
    
    CFRange whole_string = CFRangeMake(0, length);
    CFIndex out_size;
    CFIndex converted = CFStringGetBytes(cfstring,
                                         whole_string,
                                         encoding,
                                         0,      // lossByte
                                         false,  // isExternalRepresentation
                                         NULL,   // buffer
                                         0,      // maxBufLen
                                         &out_size);
    if (converted == 0 || out_size == 0)
      return;
    
    // out_size is the number of UInt8-sized units needed in the destination.
    // A buffer allocated as UInt8 units might not be properly aligned to
    // contain elements of StringType::value_type.  Use a container for the
    // proper value_type, and convert out_size by figuring the number of
    // value_type elements per UInt8.  Leave room for a NUL terminator.
    const int elements = out_size * sizeof(UInt8) / sizeof(wchar_t) + 1;
    
    m_wstring = new wchar_t[elements];
    converted = CFStringGetBytes(cfstring,
                                 whole_string,
                                 encoding,
                                 0,      // lossByte
                                 false,  // isExternalRepresentation
                                 reinterpret_cast<UInt8*>(&m_wstring[0]),
                                 out_size,
                                 NULL);  // usedBufLen
    if (converted == 0)
      return;
    
    m_wstring[elements - 1] = '\0';
		
  }   
  
  ~Utf8() 
  { 
    if( m_utf8 )
    {
      delete[] m_utf8;
      delete[] m_wstring;
    }
  }
  
public:
  operator const char*() const { return m_utf8; }
  operator const wchar_t*() const { return m_wstring; }
private:
  char* m_utf8;
  wchar_t* m_wstring;
};
#endif

#endif // PCH_H