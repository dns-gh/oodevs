// Generated by the protocol buffer compiler.  DO NOT EDIT!

#define INTERNAL_SUPPRESS_PROTOBUF_FIELD_DEPRECATION
#include "Version.pb.h"
#include <google/protobuf/stubs/once.h>
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/wire_format_lite_inl.h>
#include <google/protobuf/descriptor.h>
#include <google/protobuf/reflection_ops.h>
#include <google/protobuf/wire_format.h>

namespace Version {

namespace {

const ::google::protobuf::Descriptor* ProtocolVersion_descriptor_ = NULL;
const ::google::protobuf::internal::GeneratedMessageReflection*
  ProtocolVersion_reflection_ = NULL;

}  // namespace


void protobuf_AssignDesc_Version_2eproto() {
  protobuf_AddDesc_Version_2eproto();
  const ::google::protobuf::FileDescriptor* file =
    ::google::protobuf::DescriptorPool::generated_pool()->FindFileByName(
      "Version.proto");
  GOOGLE_CHECK(file != NULL);
  ProtocolVersion_descriptor_ = file->message_type(0);
  static const int ProtocolVersion_offsets_[1] = {
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(ProtocolVersion, value_),
  };
  ProtocolVersion_reflection_ =
    new ::google::protobuf::internal::GeneratedMessageReflection(
      ProtocolVersion_descriptor_,
      ProtocolVersion::default_instance_,
      ProtocolVersion_offsets_,
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(ProtocolVersion, _has_bits_[0]),
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(ProtocolVersion, _unknown_fields_),
      -1,
      ::google::protobuf::DescriptorPool::generated_pool(),
      ::google::protobuf::MessageFactory::generated_factory(),
      sizeof(ProtocolVersion));
}

namespace {

GOOGLE_PROTOBUF_DECLARE_ONCE(protobuf_AssignDescriptors_once_);
inline void protobuf_AssignDescriptorsOnce() {
  ::google::protobuf::GoogleOnceInit(&protobuf_AssignDescriptors_once_,
                 &protobuf_AssignDesc_Version_2eproto);
}

void protobuf_RegisterTypes(const ::std::string&) {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedMessage(
    ProtocolVersion_descriptor_, &ProtocolVersion::default_instance());
}

}  // namespace

void protobuf_ShutdownFile_Version_2eproto() {
  delete ProtocolVersion::default_instance_;
  delete ProtocolVersion_reflection_;
}

void protobuf_AddDesc_Version_2eproto() {
  static bool already_here = false;
  if (already_here) return;
  already_here = true;
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  ::google::protobuf::DescriptorPool::InternalAddGeneratedFile(
    "\n\rVersion.proto\022\007Version\"\'\n\017ProtocolVers"
    "ion\022\024\n\005value\030\001 \002(\t:\0054.2.4", 65);
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedFile(
    "Version.proto", &protobuf_RegisterTypes);
  ProtocolVersion::default_instance_ = new ProtocolVersion();
  ProtocolVersion::default_instance_->InitAsDefaultInstance();
  ::google::protobuf::internal::OnShutdown(&protobuf_ShutdownFile_Version_2eproto);
}

// Force AddDescriptors() to be called at static initialization time.
struct StaticDescriptorInitializer_Version_2eproto {
  StaticDescriptorInitializer_Version_2eproto() {
    protobuf_AddDesc_Version_2eproto();
  }
} static_descriptor_initializer_Version_2eproto_;


// ===================================================================

const ::std::string ProtocolVersion::_default_value_("4.2.4");
#ifndef _MSC_VER
const int ProtocolVersion::kValueFieldNumber;
#endif  // !_MSC_VER

ProtocolVersion::ProtocolVersion() {
  SharedCtor();
}

void ProtocolVersion::InitAsDefaultInstance() {
}

ProtocolVersion::ProtocolVersion(const ProtocolVersion& from) {
  SharedCtor();
  MergeFrom(from);
}

void ProtocolVersion::SharedCtor() {
  _cached_size_ = 0;
  value_ = const_cast< ::std::string*>(&_default_value_);
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
}

ProtocolVersion::~ProtocolVersion() {
  SharedDtor();
}

void ProtocolVersion::SharedDtor() {
  if (value_ != &_default_value_) {
    delete value_;
  }
  if (this != default_instance_) {
  }
}

const ::google::protobuf::Descriptor* ProtocolVersion::descriptor() {
  protobuf_AssignDescriptorsOnce();
  return ProtocolVersion_descriptor_;
}

const ProtocolVersion& ProtocolVersion::default_instance() {
  if (default_instance_ == NULL) protobuf_AddDesc_Version_2eproto();  return *default_instance_;
}

ProtocolVersion* ProtocolVersion::default_instance_ = NULL;

ProtocolVersion* ProtocolVersion::New() const {
  return new ProtocolVersion;
}

void ProtocolVersion::Clear() {
  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    if (_has_bit(0)) {
      if (value_ != &_default_value_) {
        value_->assign(_default_value_);
      }
    }
  }
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
  mutable_unknown_fields()->Clear();
}

bool ProtocolVersion::MergePartialFromCodedStream(
    ::google::protobuf::io::CodedInputStream* input) {
#define DO_(EXPRESSION) if (!(EXPRESSION)) return false
  ::google::protobuf::uint32 tag;
  while ((tag = input->ReadTag()) != 0) {
    switch (::google::protobuf::internal::WireFormatLite::GetTagFieldNumber(tag)) {
      // required string value = 1 [default = "4.2.4"];
      case 1: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) !=
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_LENGTH_DELIMITED) {
          goto handle_uninterpreted;
        }
        DO_(::google::protobuf::internal::WireFormatLite::ReadString(
              input, this->mutable_value()));
        ::google::protobuf::internal::WireFormat::VerifyUTF8String(
          this->value().data(), this->value().length(),
          ::google::protobuf::internal::WireFormat::PARSE);
        if (input->ExpectAtEnd()) return true;
        break;
      }
      
      default: {
      handle_uninterpreted:
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_END_GROUP) {
          return true;
        }
        DO_(::google::protobuf::internal::WireFormat::SkipField(
              input, tag, mutable_unknown_fields()));
        break;
      }
    }
  }
  return true;
#undef DO_
}

void ProtocolVersion::SerializeWithCachedSizes(
    ::google::protobuf::io::CodedOutputStream* output) const {
  ::google::protobuf::uint8* raw_buffer = output->GetDirectBufferForNBytesAndAdvance(_cached_size_);
  if (raw_buffer != NULL) {
    ProtocolVersion::SerializeWithCachedSizesToArray(raw_buffer);
    return;
  }
  
  // required string value = 1 [default = "4.2.4"];
  if (_has_bit(0)) {
    ::google::protobuf::internal::WireFormat::VerifyUTF8String(
      this->value().data(), this->value().length(),
      ::google::protobuf::internal::WireFormat::SERIALIZE);
    ::google::protobuf::internal::WireFormatLite::WriteString(
      1, this->value(), output);
  }
  
  if (!unknown_fields().empty()) {
    ::google::protobuf::internal::WireFormat::SerializeUnknownFields(
        unknown_fields(), output);
  }
}

::google::protobuf::uint8* ProtocolVersion::SerializeWithCachedSizesToArray(
    ::google::protobuf::uint8* target) const {
  // required string value = 1 [default = "4.2.4"];
  if (_has_bit(0)) {
    ::google::protobuf::internal::WireFormat::VerifyUTF8String(
      this->value().data(), this->value().length(),
      ::google::protobuf::internal::WireFormat::SERIALIZE);
    target =
      ::google::protobuf::internal::WireFormatLite::WriteStringToArray(
        1, this->value(), target);
  }
  
  if (!unknown_fields().empty()) {
    target = ::google::protobuf::internal::WireFormat::SerializeUnknownFieldsToArray(
        unknown_fields(), target);
  }
  return target;
}

int ProtocolVersion::ByteSize() const {
  int total_size = 0;
  
  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    // required string value = 1 [default = "4.2.4"];
    if (has_value()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::StringSize(
          this->value());
    }
    
  }
  if (!unknown_fields().empty()) {
    total_size +=
      ::google::protobuf::internal::WireFormat::ComputeUnknownFieldsSize(
        unknown_fields());
  }
  _cached_size_ = total_size;
  return total_size;
}

void ProtocolVersion::MergeFrom(const ::google::protobuf::Message& from) {
  GOOGLE_CHECK_NE(&from, this);
  const ProtocolVersion* source =
    ::google::protobuf::internal::dynamic_cast_if_available<const ProtocolVersion*>(
      &from);
  if (source == NULL) {
    ::google::protobuf::internal::ReflectionOps::Merge(from, this);
  } else {
    MergeFrom(*source);
  }
}

void ProtocolVersion::MergeFrom(const ProtocolVersion& from) {
  GOOGLE_CHECK_NE(&from, this);
  if (from._has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    if (from._has_bit(0)) {
      set_value(from.value());
    }
  }
  mutable_unknown_fields()->MergeFrom(from.unknown_fields());
}

void ProtocolVersion::CopyFrom(const ::google::protobuf::Message& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void ProtocolVersion::CopyFrom(const ProtocolVersion& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool ProtocolVersion::IsInitialized() const {
  if ((_has_bits_[0] & 0x00000001) != 0x00000001) return false;
  
  return true;
}

void ProtocolVersion::Swap(ProtocolVersion* other) {
  if (other != this) {
    std::swap(value_, other->value_);
    std::swap(_has_bits_[0], other->_has_bits_[0]);
    _unknown_fields_.Swap(&other->_unknown_fields_);
    std::swap(_cached_size_, other->_cached_size_);
  }
}

::google::protobuf::Metadata ProtocolVersion::GetMetadata() const {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::Metadata metadata;
  metadata.descriptor = ProtocolVersion_descriptor_;
  metadata.reflection = ProtocolVersion_reflection_;
  return metadata;
}


}  // namespace Version
