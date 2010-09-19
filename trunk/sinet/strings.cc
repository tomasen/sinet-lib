#include "pch.h"
#include "strings.h"

using namespace sinet;

// Convert a std::wstring to a std::string (utf8)
std::string strings::wstring_utf8string(const std::wstring& s)
{
  char* ch;
  UINT bytes = WideCharToMultiByte(CP_UTF8, 0, s.c_str(), -1, NULL, 0,
    NULL, NULL); 
  ch = new char[bytes];
  if(ch)
    bytes = WideCharToMultiByte(CP_UTF8, 0, s.c_str(), -1, ch, bytes,
    NULL, NULL);
  std::string str = ch;
  delete[] ch;
  return str;
}