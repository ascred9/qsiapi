// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME MyMainFrame
#define R__NO_DEPRECATION

/*******************************************************************/
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#define G__DICTIONARY
#include "ROOT/RConfig.hxx"
#include "TClass.h"
#include "TDictAttributeMap.h"
#include "TInterpreter.h"
#include "TROOT.h"
#include "TBuffer.h"
#include "TMemberInspector.h"
#include "TInterpreter.h"
#include "TVirtualMutex.h"
#include "TError.h"

#ifndef G__ROOT
#define G__ROOT
#endif

#include "RtypesImp.h"
#include "TIsAProxy.h"
#include "TFileMergeInfo.h"
#include <algorithm>
#include "TCollectionProxyInfo.h"
/*******************************************************************/

#include "TDataMember.h"

// Header files passed as explicit arguments
#include "MyMainFrame.h"

// Header files passed via #pragma extra_include

// The generated code does not explicitly qualify STL entities
namespace std {} using namespace std;

namespace ROOT {
   static void delete_MyMainFrame(void *p);
   static void deleteArray_MyMainFrame(void *p);
   static void destruct_MyMainFrame(void *p);
   static void streamer_MyMainFrame(TBuffer &buf, void *obj);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::MyMainFrame*)
   {
      ::MyMainFrame *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::MyMainFrame >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("MyMainFrame", ::MyMainFrame::Class_Version(), "MyMainFrame.h", 20,
                  typeid(::MyMainFrame), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::MyMainFrame::Dictionary, isa_proxy, 16,
                  sizeof(::MyMainFrame) );
      instance.SetDelete(&delete_MyMainFrame);
      instance.SetDeleteArray(&deleteArray_MyMainFrame);
      instance.SetDestructor(&destruct_MyMainFrame);
      instance.SetStreamerFunc(&streamer_MyMainFrame);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::MyMainFrame*)
   {
      return GenerateInitInstanceLocal(static_cast<::MyMainFrame*>(nullptr));
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal(static_cast<const ::MyMainFrame*>(nullptr)); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

//______________________________________________________________________________
atomic_TClass_ptr MyMainFrame::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *MyMainFrame::Class_Name()
{
   return "MyMainFrame";
}

//______________________________________________________________________________
const char *MyMainFrame::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::MyMainFrame*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int MyMainFrame::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::MyMainFrame*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *MyMainFrame::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::MyMainFrame*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *MyMainFrame::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::MyMainFrame*)nullptr)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
void MyMainFrame::Streamer(TBuffer &R__b)
{
   // Stream an object of class MyMainFrame.

   TGMainFrame::Streamer(R__b);
}

namespace ROOT {
   // Wrapper around operator delete
   static void delete_MyMainFrame(void *p) {
      delete (static_cast<::MyMainFrame*>(p));
   }
   static void deleteArray_MyMainFrame(void *p) {
      delete [] (static_cast<::MyMainFrame*>(p));
   }
   static void destruct_MyMainFrame(void *p) {
      typedef ::MyMainFrame current_t;
      (static_cast<current_t*>(p))->~current_t();
   }
   // Wrapper around a custom streamer member function.
   static void streamer_MyMainFrame(TBuffer &buf, void *obj) {
      ((::MyMainFrame*)obj)->::MyMainFrame::Streamer(buf);
   }
} // end of namespace ROOT for class ::MyMainFrame

namespace {
  void TriggerDictionaryInitialization_MyMainFrame_Impl() {
    static const char* headers[] = {
"MyMainFrame.h",
nullptr
    };
    static const char* includePaths[] = {
"/home/ascred9/root_install/include/",
"/home/ascred9/qsiapi-7.6.0/myproject/",
nullptr
    };
    static const char* fwdDeclCode = R"DICTFWDDCLS(
#line 1 "MyMainFrame dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_AutoLoading_Map;
class __attribute__((annotate("$clingAutoload$MyMainFrame.h")))  MyMainFrame;
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "MyMainFrame dictionary payload"


#define _BACKWARD_BACKWARD_WARNING_H
// Inline headers
#include "MyMainFrame.h"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[] = {
"MyMainFrame", payloadCode, "@",
nullptr
};
    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("MyMainFrame",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_MyMainFrame_Impl, {}, classesHeaders, /*hasCxxModule*/false);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_MyMainFrame_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_MyMainFrame() {
  TriggerDictionaryInitialization_MyMainFrame_Impl();
}
