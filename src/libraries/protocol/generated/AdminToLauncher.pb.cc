// Generated by the protocol buffer compiler.  DO NOT EDIT!

#define INTERNAL_SUPPRESS_PROTOBUF_FIELD_DEPRECATION
#include "AdminToLauncher.pb.h"
#include <google/protobuf/stubs/once.h>
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/wire_format_lite_inl.h>
#include <google/protobuf/descriptor.h>
#include <google/protobuf/reflection_ops.h>
#include <google/protobuf/wire_format.h>

namespace MsgsAdminToLauncher {

namespace {


}  // namespace


void protobuf_AssignDesc_AdminToLauncher_2eproto() {
  protobuf_AddDesc_AdminToLauncher_2eproto();
  const ::google::protobuf::FileDescriptor* file =
    ::google::protobuf::DescriptorPool::generated_pool()->FindFileByName(
      "AdminToLauncher.proto");
  GOOGLE_CHECK(file != NULL);
}

namespace {

GOOGLE_PROTOBUF_DECLARE_ONCE(protobuf_AssignDescriptors_once_);
inline void protobuf_AssignDescriptorsOnce() {
  ::google::protobuf::GoogleOnceInit(&protobuf_AssignDescriptors_once_,
                 &protobuf_AssignDesc_AdminToLauncher_2eproto);
}

void protobuf_RegisterTypes(const ::std::string&) {
  protobuf_AssignDescriptorsOnce();
}

}  // namespace

void protobuf_ShutdownFile_AdminToLauncher_2eproto() {
}

void protobuf_AddDesc_AdminToLauncher_2eproto() {
  static bool already_here = false;
  if (already_here) return;
  already_here = true;
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  ::google::protobuf::DescriptorPool::InternalAddGeneratedFile(
    "\n\025AdminToLauncher.proto\022\023MsgsAdminToLaun"
    "cher", 44);
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedFile(
    "AdminToLauncher.proto", &protobuf_RegisterTypes);
  ::google::protobuf::internal::OnShutdown(&protobuf_ShutdownFile_AdminToLauncher_2eproto);
}

// Force AddDescriptors() to be called at static initialization time.
struct StaticDescriptorInitializer_AdminToLauncher_2eproto {
  StaticDescriptorInitializer_AdminToLauncher_2eproto() {
    protobuf_AddDesc_AdminToLauncher_2eproto();
  }
} static_descriptor_initializer_AdminToLauncher_2eproto_;


}  // namespace MsgsAdminToLauncher
