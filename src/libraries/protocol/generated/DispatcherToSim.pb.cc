// Generated by the protocol buffer compiler.  DO NOT EDIT!

#define INTERNAL_SUPPRESS_PROTOBUF_FIELD_DEPRECATION
#include "DispatcherToSim.pb.h"
#include <google/protobuf/stubs/once.h>
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/wire_format_lite_inl.h>
#include <google/protobuf/descriptor.h>
#include <google/protobuf/reflection_ops.h>
#include <google/protobuf/wire_format.h>

namespace MsgsDispatcherToSim {

namespace {

const ::google::protobuf::Descriptor* MsgCtrlClientAnnouncement_descriptor_ = NULL;
const ::google::protobuf::internal::GeneratedMessageReflection*
  MsgCtrlClientAnnouncement_reflection_ = NULL;
const ::google::protobuf::Descriptor* MsgDispatcherToSim_descriptor_ = NULL;
const ::google::protobuf::internal::GeneratedMessageReflection*
  MsgDispatcherToSim_reflection_ = NULL;
const ::google::protobuf::Descriptor* MsgDispatcherToSim_Content_descriptor_ = NULL;
const ::google::protobuf::internal::GeneratedMessageReflection*
  MsgDispatcherToSim_Content_reflection_ = NULL;

}  // namespace


void protobuf_AssignDesc_DispatcherToSim_2eproto() {
  protobuf_AddDesc_DispatcherToSim_2eproto();
  const ::google::protobuf::FileDescriptor* file =
    ::google::protobuf::DescriptorPool::generated_pool()->FindFileByName(
      "DispatcherToSim.proto");
  GOOGLE_CHECK(file != NULL);
  MsgCtrlClientAnnouncement_descriptor_ = file->message_type(0);
  static const int MsgCtrlClientAnnouncement_offsets_[1] = {
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(MsgCtrlClientAnnouncement, announce_),
  };
  MsgCtrlClientAnnouncement_reflection_ =
    new ::google::protobuf::internal::GeneratedMessageReflection(
      MsgCtrlClientAnnouncement_descriptor_,
      MsgCtrlClientAnnouncement::default_instance_,
      MsgCtrlClientAnnouncement_offsets_,
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(MsgCtrlClientAnnouncement, _has_bits_[0]),
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(MsgCtrlClientAnnouncement, _unknown_fields_),
      -1,
      ::google::protobuf::DescriptorPool::generated_pool(),
      ::google::protobuf::MessageFactory::generated_factory(),
      sizeof(MsgCtrlClientAnnouncement));
  MsgDispatcherToSim_descriptor_ = file->message_type(1);
  static const int MsgDispatcherToSim_offsets_[2] = {
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(MsgDispatcherToSim, context_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(MsgDispatcherToSim, message_),
  };
  MsgDispatcherToSim_reflection_ =
    new ::google::protobuf::internal::GeneratedMessageReflection(
      MsgDispatcherToSim_descriptor_,
      MsgDispatcherToSim::default_instance_,
      MsgDispatcherToSim_offsets_,
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(MsgDispatcherToSim, _has_bits_[0]),
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(MsgDispatcherToSim, _unknown_fields_),
      -1,
      ::google::protobuf::DescriptorPool::generated_pool(),
      ::google::protobuf::MessageFactory::generated_factory(),
      sizeof(MsgDispatcherToSim));
  MsgDispatcherToSim_Content_descriptor_ = MsgDispatcherToSim_descriptor_->nested_type(0);
  static const int MsgDispatcherToSim_Content_offsets_[1] = {
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(MsgDispatcherToSim_Content, control_client_announcement_),
  };
  MsgDispatcherToSim_Content_reflection_ =
    new ::google::protobuf::internal::GeneratedMessageReflection(
      MsgDispatcherToSim_Content_descriptor_,
      MsgDispatcherToSim_Content::default_instance_,
      MsgDispatcherToSim_Content_offsets_,
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(MsgDispatcherToSim_Content, _has_bits_[0]),
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(MsgDispatcherToSim_Content, _unknown_fields_),
      -1,
      ::google::protobuf::DescriptorPool::generated_pool(),
      ::google::protobuf::MessageFactory::generated_factory(),
      sizeof(MsgDispatcherToSim_Content));
}

namespace {

GOOGLE_PROTOBUF_DECLARE_ONCE(protobuf_AssignDescriptors_once_);
inline void protobuf_AssignDescriptorsOnce() {
  ::google::protobuf::GoogleOnceInit(&protobuf_AssignDescriptors_once_,
                 &protobuf_AssignDesc_DispatcherToSim_2eproto);
}

void protobuf_RegisterTypes(const ::std::string&) {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedMessage(
    MsgCtrlClientAnnouncement_descriptor_, &MsgCtrlClientAnnouncement::default_instance());
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedMessage(
    MsgDispatcherToSim_descriptor_, &MsgDispatcherToSim::default_instance());
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedMessage(
    MsgDispatcherToSim_Content_descriptor_, &MsgDispatcherToSim_Content::default_instance());
}

}  // namespace

void protobuf_ShutdownFile_DispatcherToSim_2eproto() {
  delete MsgCtrlClientAnnouncement::default_instance_;
  delete MsgCtrlClientAnnouncement_reflection_;
  delete MsgDispatcherToSim::default_instance_;
  delete MsgDispatcherToSim_reflection_;
  delete MsgDispatcherToSim_Content::default_instance_;
  delete MsgDispatcherToSim_Content_reflection_;
}

void protobuf_AddDesc_DispatcherToSim_2eproto() {
  static bool already_here = false;
  if (already_here) return;
  already_here = true;
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  ::google::protobuf::DescriptorPool::InternalAddGeneratedFile(
    "\n\025DispatcherToSim.proto\022\023MsgsDispatcherT"
    "oSim\"-\n\031MsgCtrlClientAnnouncement\022\020\n\010ann"
    "ounce\030\001 \001(\010\"\307\001\n\022MsgDispatcherToSim\022\017\n\007co"
    "ntext\030\001 \001(\005\022@\n\007message\030\002 \002(\0132/.MsgsDispa"
    "tcherToSim.MsgDispatcherToSim.Content\032^\n"
    "\007Content\022S\n\033control_client_announcement\030"
    "\001 \001(\0132..MsgsDispatcherToSim.MsgCtrlClien"
    "tAnnouncement", 293);
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedFile(
    "DispatcherToSim.proto", &protobuf_RegisterTypes);
  MsgCtrlClientAnnouncement::default_instance_ = new MsgCtrlClientAnnouncement();
  MsgDispatcherToSim::default_instance_ = new MsgDispatcherToSim();
  MsgDispatcherToSim_Content::default_instance_ = new MsgDispatcherToSim_Content();
  MsgCtrlClientAnnouncement::default_instance_->InitAsDefaultInstance();
  MsgDispatcherToSim::default_instance_->InitAsDefaultInstance();
  MsgDispatcherToSim_Content::default_instance_->InitAsDefaultInstance();
  ::google::protobuf::internal::OnShutdown(&protobuf_ShutdownFile_DispatcherToSim_2eproto);
}

// Force AddDescriptors() to be called at static initialization time.
struct StaticDescriptorInitializer_DispatcherToSim_2eproto {
  StaticDescriptorInitializer_DispatcherToSim_2eproto() {
    protobuf_AddDesc_DispatcherToSim_2eproto();
  }
} static_descriptor_initializer_DispatcherToSim_2eproto_;


// ===================================================================

#ifndef _MSC_VER
const int MsgCtrlClientAnnouncement::kAnnounceFieldNumber;
#endif  // !_MSC_VER

MsgCtrlClientAnnouncement::MsgCtrlClientAnnouncement() {
  SharedCtor();
}

void MsgCtrlClientAnnouncement::InitAsDefaultInstance() {
}

MsgCtrlClientAnnouncement::MsgCtrlClientAnnouncement(const MsgCtrlClientAnnouncement& from) {
  SharedCtor();
  MergeFrom(from);
}

void MsgCtrlClientAnnouncement::SharedCtor() {
  _cached_size_ = 0;
  announce_ = false;
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
}

MsgCtrlClientAnnouncement::~MsgCtrlClientAnnouncement() {
  SharedDtor();
}

void MsgCtrlClientAnnouncement::SharedDtor() {
  if (this != default_instance_) {
  }
}

const ::google::protobuf::Descriptor* MsgCtrlClientAnnouncement::descriptor() {
  protobuf_AssignDescriptorsOnce();
  return MsgCtrlClientAnnouncement_descriptor_;
}

const MsgCtrlClientAnnouncement& MsgCtrlClientAnnouncement::default_instance() {
  if (default_instance_ == NULL) protobuf_AddDesc_DispatcherToSim_2eproto();  return *default_instance_;
}

MsgCtrlClientAnnouncement* MsgCtrlClientAnnouncement::default_instance_ = NULL;

MsgCtrlClientAnnouncement* MsgCtrlClientAnnouncement::New() const {
  return new MsgCtrlClientAnnouncement;
}

void MsgCtrlClientAnnouncement::Clear() {
  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    announce_ = false;
  }
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
  mutable_unknown_fields()->Clear();
}

bool MsgCtrlClientAnnouncement::MergePartialFromCodedStream(
    ::google::protobuf::io::CodedInputStream* input) {
#define DO_(EXPRESSION) if (!(EXPRESSION)) return false
  ::google::protobuf::uint32 tag;
  while ((tag = input->ReadTag()) != 0) {
    switch (::google::protobuf::internal::WireFormatLite::GetTagFieldNumber(tag)) {
      // optional bool announce = 1;
      case 1: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) !=
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_VARINT) {
          goto handle_uninterpreted;
        }
        DO_(::google::protobuf::internal::WireFormatLite::ReadBool(
              input, &announce_));
        _set_bit(0);
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

void MsgCtrlClientAnnouncement::SerializeWithCachedSizes(
    ::google::protobuf::io::CodedOutputStream* output) const {
  ::google::protobuf::uint8* raw_buffer = output->GetDirectBufferForNBytesAndAdvance(_cached_size_);
  if (raw_buffer != NULL) {
    MsgCtrlClientAnnouncement::SerializeWithCachedSizesToArray(raw_buffer);
    return;
  }
  
  // optional bool announce = 1;
  if (_has_bit(0)) {
    ::google::protobuf::internal::WireFormatLite::WriteBool(1, this->announce(), output);
  }
  
  if (!unknown_fields().empty()) {
    ::google::protobuf::internal::WireFormat::SerializeUnknownFields(
        unknown_fields(), output);
  }
}

::google::protobuf::uint8* MsgCtrlClientAnnouncement::SerializeWithCachedSizesToArray(
    ::google::protobuf::uint8* target) const {
  // optional bool announce = 1;
  if (_has_bit(0)) {
    target = ::google::protobuf::internal::WireFormatLite::WriteBoolToArray(1, this->announce(), target);
  }
  
  if (!unknown_fields().empty()) {
    target = ::google::protobuf::internal::WireFormat::SerializeUnknownFieldsToArray(
        unknown_fields(), target);
  }
  return target;
}

int MsgCtrlClientAnnouncement::ByteSize() const {
  int total_size = 0;
  
  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    // optional bool announce = 1;
    if (has_announce()) {
      total_size += 1 + 1;
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

void MsgCtrlClientAnnouncement::MergeFrom(const ::google::protobuf::Message& from) {
  GOOGLE_CHECK_NE(&from, this);
  const MsgCtrlClientAnnouncement* source =
    ::google::protobuf::internal::dynamic_cast_if_available<const MsgCtrlClientAnnouncement*>(
      &from);
  if (source == NULL) {
    ::google::protobuf::internal::ReflectionOps::Merge(from, this);
  } else {
    MergeFrom(*source);
  }
}

void MsgCtrlClientAnnouncement::MergeFrom(const MsgCtrlClientAnnouncement& from) {
  GOOGLE_CHECK_NE(&from, this);
  if (from._has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    if (from._has_bit(0)) {
      set_announce(from.announce());
    }
  }
  mutable_unknown_fields()->MergeFrom(from.unknown_fields());
}

void MsgCtrlClientAnnouncement::CopyFrom(const ::google::protobuf::Message& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void MsgCtrlClientAnnouncement::CopyFrom(const MsgCtrlClientAnnouncement& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool MsgCtrlClientAnnouncement::IsInitialized() const {
  
  return true;
}

void MsgCtrlClientAnnouncement::Swap(MsgCtrlClientAnnouncement* other) {
  if (other != this) {
    std::swap(announce_, other->announce_);
    std::swap(_has_bits_[0], other->_has_bits_[0]);
    _unknown_fields_.Swap(&other->_unknown_fields_);
    std::swap(_cached_size_, other->_cached_size_);
  }
}

::google::protobuf::Metadata MsgCtrlClientAnnouncement::GetMetadata() const {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::Metadata metadata;
  metadata.descriptor = MsgCtrlClientAnnouncement_descriptor_;
  metadata.reflection = MsgCtrlClientAnnouncement_reflection_;
  return metadata;
}


// ===================================================================

#ifndef _MSC_VER
const int MsgDispatcherToSim_Content::kControlClientAnnouncementFieldNumber;
#endif  // !_MSC_VER

MsgDispatcherToSim_Content::MsgDispatcherToSim_Content() {
  SharedCtor();
}

void MsgDispatcherToSim_Content::InitAsDefaultInstance() {
  control_client_announcement_ = const_cast< ::MsgsDispatcherToSim::MsgCtrlClientAnnouncement*>(&::MsgsDispatcherToSim::MsgCtrlClientAnnouncement::default_instance());
}

MsgDispatcherToSim_Content::MsgDispatcherToSim_Content(const MsgDispatcherToSim_Content& from) {
  SharedCtor();
  MergeFrom(from);
}

void MsgDispatcherToSim_Content::SharedCtor() {
  _cached_size_ = 0;
  control_client_announcement_ = NULL;
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
}

MsgDispatcherToSim_Content::~MsgDispatcherToSim_Content() {
  SharedDtor();
}

void MsgDispatcherToSim_Content::SharedDtor() {
  if (this != default_instance_) {
    delete control_client_announcement_;
  }
}

const ::google::protobuf::Descriptor* MsgDispatcherToSim_Content::descriptor() {
  protobuf_AssignDescriptorsOnce();
  return MsgDispatcherToSim_Content_descriptor_;
}

const MsgDispatcherToSim_Content& MsgDispatcherToSim_Content::default_instance() {
  if (default_instance_ == NULL) protobuf_AddDesc_DispatcherToSim_2eproto();  return *default_instance_;
}

MsgDispatcherToSim_Content* MsgDispatcherToSim_Content::default_instance_ = NULL;

MsgDispatcherToSim_Content* MsgDispatcherToSim_Content::New() const {
  return new MsgDispatcherToSim_Content;
}

void MsgDispatcherToSim_Content::Clear() {
  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    if (_has_bit(0)) {
      if (control_client_announcement_ != NULL) control_client_announcement_->::MsgsDispatcherToSim::MsgCtrlClientAnnouncement::Clear();
    }
  }
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
  mutable_unknown_fields()->Clear();
}

bool MsgDispatcherToSim_Content::MergePartialFromCodedStream(
    ::google::protobuf::io::CodedInputStream* input) {
#define DO_(EXPRESSION) if (!(EXPRESSION)) return false
  ::google::protobuf::uint32 tag;
  while ((tag = input->ReadTag()) != 0) {
    switch (::google::protobuf::internal::WireFormatLite::GetTagFieldNumber(tag)) {
      // optional .MsgsDispatcherToSim.MsgCtrlClientAnnouncement control_client_announcement = 1;
      case 1: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) !=
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_LENGTH_DELIMITED) {
          goto handle_uninterpreted;
        }
        DO_(::google::protobuf::internal::WireFormatLite::ReadMessageNoVirtual(
             input, mutable_control_client_announcement()));
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

void MsgDispatcherToSim_Content::SerializeWithCachedSizes(
    ::google::protobuf::io::CodedOutputStream* output) const {
  ::google::protobuf::uint8* raw_buffer = output->GetDirectBufferForNBytesAndAdvance(_cached_size_);
  if (raw_buffer != NULL) {
    MsgDispatcherToSim_Content::SerializeWithCachedSizesToArray(raw_buffer);
    return;
  }
  
  // optional .MsgsDispatcherToSim.MsgCtrlClientAnnouncement control_client_announcement = 1;
  if (_has_bit(0)) {
    ::google::protobuf::internal::WireFormatLite::WriteMessageNoVirtual(
      1, this->control_client_announcement(), output);
  }
  
  if (!unknown_fields().empty()) {
    ::google::protobuf::internal::WireFormat::SerializeUnknownFields(
        unknown_fields(), output);
  }
}

::google::protobuf::uint8* MsgDispatcherToSim_Content::SerializeWithCachedSizesToArray(
    ::google::protobuf::uint8* target) const {
  // optional .MsgsDispatcherToSim.MsgCtrlClientAnnouncement control_client_announcement = 1;
  if (_has_bit(0)) {
    target = ::google::protobuf::internal::WireFormatLite::
      WriteMessageNoVirtualToArray(
        1, this->control_client_announcement(), target);
  }
  
  if (!unknown_fields().empty()) {
    target = ::google::protobuf::internal::WireFormat::SerializeUnknownFieldsToArray(
        unknown_fields(), target);
  }
  return target;
}

int MsgDispatcherToSim_Content::ByteSize() const {
  int total_size = 0;
  
  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    // optional .MsgsDispatcherToSim.MsgCtrlClientAnnouncement control_client_announcement = 1;
    if (has_control_client_announcement()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::MessageSizeNoVirtual(
          this->control_client_announcement());
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

void MsgDispatcherToSim_Content::MergeFrom(const ::google::protobuf::Message& from) {
  GOOGLE_CHECK_NE(&from, this);
  const MsgDispatcherToSim_Content* source =
    ::google::protobuf::internal::dynamic_cast_if_available<const MsgDispatcherToSim_Content*>(
      &from);
  if (source == NULL) {
    ::google::protobuf::internal::ReflectionOps::Merge(from, this);
  } else {
    MergeFrom(*source);
  }
}

void MsgDispatcherToSim_Content::MergeFrom(const MsgDispatcherToSim_Content& from) {
  GOOGLE_CHECK_NE(&from, this);
  if (from._has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    if (from._has_bit(0)) {
      mutable_control_client_announcement()->::MsgsDispatcherToSim::MsgCtrlClientAnnouncement::MergeFrom(from.control_client_announcement());
    }
  }
  mutable_unknown_fields()->MergeFrom(from.unknown_fields());
}

void MsgDispatcherToSim_Content::CopyFrom(const ::google::protobuf::Message& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void MsgDispatcherToSim_Content::CopyFrom(const MsgDispatcherToSim_Content& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool MsgDispatcherToSim_Content::IsInitialized() const {
  
  return true;
}

void MsgDispatcherToSim_Content::Swap(MsgDispatcherToSim_Content* other) {
  if (other != this) {
    std::swap(control_client_announcement_, other->control_client_announcement_);
    std::swap(_has_bits_[0], other->_has_bits_[0]);
    _unknown_fields_.Swap(&other->_unknown_fields_);
    std::swap(_cached_size_, other->_cached_size_);
  }
}

::google::protobuf::Metadata MsgDispatcherToSim_Content::GetMetadata() const {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::Metadata metadata;
  metadata.descriptor = MsgDispatcherToSim_Content_descriptor_;
  metadata.reflection = MsgDispatcherToSim_Content_reflection_;
  return metadata;
}


// -------------------------------------------------------------------

#ifndef _MSC_VER
const int MsgDispatcherToSim::kContextFieldNumber;
const int MsgDispatcherToSim::kMessageFieldNumber;
#endif  // !_MSC_VER

MsgDispatcherToSim::MsgDispatcherToSim() {
  SharedCtor();
}

void MsgDispatcherToSim::InitAsDefaultInstance() {
  message_ = const_cast< ::MsgsDispatcherToSim::MsgDispatcherToSim_Content*>(&::MsgsDispatcherToSim::MsgDispatcherToSim_Content::default_instance());
}

MsgDispatcherToSim::MsgDispatcherToSim(const MsgDispatcherToSim& from) {
  SharedCtor();
  MergeFrom(from);
}

void MsgDispatcherToSim::SharedCtor() {
  _cached_size_ = 0;
  context_ = 0;
  message_ = NULL;
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
}

MsgDispatcherToSim::~MsgDispatcherToSim() {
  SharedDtor();
}

void MsgDispatcherToSim::SharedDtor() {
  if (this != default_instance_) {
    delete message_;
  }
}

const ::google::protobuf::Descriptor* MsgDispatcherToSim::descriptor() {
  protobuf_AssignDescriptorsOnce();
  return MsgDispatcherToSim_descriptor_;
}

const MsgDispatcherToSim& MsgDispatcherToSim::default_instance() {
  if (default_instance_ == NULL) protobuf_AddDesc_DispatcherToSim_2eproto();  return *default_instance_;
}

MsgDispatcherToSim* MsgDispatcherToSim::default_instance_ = NULL;

MsgDispatcherToSim* MsgDispatcherToSim::New() const {
  return new MsgDispatcherToSim;
}

void MsgDispatcherToSim::Clear() {
  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    context_ = 0;
    if (_has_bit(1)) {
      if (message_ != NULL) message_->::MsgsDispatcherToSim::MsgDispatcherToSim_Content::Clear();
    }
  }
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
  mutable_unknown_fields()->Clear();
}

bool MsgDispatcherToSim::MergePartialFromCodedStream(
    ::google::protobuf::io::CodedInputStream* input) {
#define DO_(EXPRESSION) if (!(EXPRESSION)) return false
  ::google::protobuf::uint32 tag;
  while ((tag = input->ReadTag()) != 0) {
    switch (::google::protobuf::internal::WireFormatLite::GetTagFieldNumber(tag)) {
      // optional int32 context = 1;
      case 1: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) !=
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_VARINT) {
          goto handle_uninterpreted;
        }
        DO_(::google::protobuf::internal::WireFormatLite::ReadInt32(
              input, &context_));
        _set_bit(0);
        if (input->ExpectTag(18)) goto parse_message;
        break;
      }
      
      // required .MsgsDispatcherToSim.MsgDispatcherToSim.Content message = 2;
      case 2: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) !=
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_LENGTH_DELIMITED) {
          goto handle_uninterpreted;
        }
       parse_message:
        DO_(::google::protobuf::internal::WireFormatLite::ReadMessageNoVirtual(
             input, mutable_message()));
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

void MsgDispatcherToSim::SerializeWithCachedSizes(
    ::google::protobuf::io::CodedOutputStream* output) const {
  ::google::protobuf::uint8* raw_buffer = output->GetDirectBufferForNBytesAndAdvance(_cached_size_);
  if (raw_buffer != NULL) {
    MsgDispatcherToSim::SerializeWithCachedSizesToArray(raw_buffer);
    return;
  }
  
  // optional int32 context = 1;
  if (_has_bit(0)) {
    ::google::protobuf::internal::WireFormatLite::WriteInt32(1, this->context(), output);
  }
  
  // required .MsgsDispatcherToSim.MsgDispatcherToSim.Content message = 2;
  if (_has_bit(1)) {
    ::google::protobuf::internal::WireFormatLite::WriteMessageNoVirtual(
      2, this->message(), output);
  }
  
  if (!unknown_fields().empty()) {
    ::google::protobuf::internal::WireFormat::SerializeUnknownFields(
        unknown_fields(), output);
  }
}

::google::protobuf::uint8* MsgDispatcherToSim::SerializeWithCachedSizesToArray(
    ::google::protobuf::uint8* target) const {
  // optional int32 context = 1;
  if (_has_bit(0)) {
    target = ::google::protobuf::internal::WireFormatLite::WriteInt32ToArray(1, this->context(), target);
  }
  
  // required .MsgsDispatcherToSim.MsgDispatcherToSim.Content message = 2;
  if (_has_bit(1)) {
    target = ::google::protobuf::internal::WireFormatLite::
      WriteMessageNoVirtualToArray(
        2, this->message(), target);
  }
  
  if (!unknown_fields().empty()) {
    target = ::google::protobuf::internal::WireFormat::SerializeUnknownFieldsToArray(
        unknown_fields(), target);
  }
  return target;
}

int MsgDispatcherToSim::ByteSize() const {
  int total_size = 0;
  
  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    // optional int32 context = 1;
    if (has_context()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::Int32Size(
          this->context());
    }
    
    // required .MsgsDispatcherToSim.MsgDispatcherToSim.Content message = 2;
    if (has_message()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::MessageSizeNoVirtual(
          this->message());
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

void MsgDispatcherToSim::MergeFrom(const ::google::protobuf::Message& from) {
  GOOGLE_CHECK_NE(&from, this);
  const MsgDispatcherToSim* source =
    ::google::protobuf::internal::dynamic_cast_if_available<const MsgDispatcherToSim*>(
      &from);
  if (source == NULL) {
    ::google::protobuf::internal::ReflectionOps::Merge(from, this);
  } else {
    MergeFrom(*source);
  }
}

void MsgDispatcherToSim::MergeFrom(const MsgDispatcherToSim& from) {
  GOOGLE_CHECK_NE(&from, this);
  if (from._has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    if (from._has_bit(0)) {
      set_context(from.context());
    }
    if (from._has_bit(1)) {
      mutable_message()->::MsgsDispatcherToSim::MsgDispatcherToSim_Content::MergeFrom(from.message());
    }
  }
  mutable_unknown_fields()->MergeFrom(from.unknown_fields());
}

void MsgDispatcherToSim::CopyFrom(const ::google::protobuf::Message& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void MsgDispatcherToSim::CopyFrom(const MsgDispatcherToSim& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool MsgDispatcherToSim::IsInitialized() const {
  if ((_has_bits_[0] & 0x00000002) != 0x00000002) return false;
  
  return true;
}

void MsgDispatcherToSim::Swap(MsgDispatcherToSim* other) {
  if (other != this) {
    std::swap(context_, other->context_);
    std::swap(message_, other->message_);
    std::swap(_has_bits_[0], other->_has_bits_[0]);
    _unknown_fields_.Swap(&other->_unknown_fields_);
    std::swap(_cached_size_, other->_cached_size_);
  }
}

::google::protobuf::Metadata MsgDispatcherToSim::GetMetadata() const {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::Metadata metadata;
  metadata.descriptor = MsgDispatcherToSim_descriptor_;
  metadata.reflection = MsgDispatcherToSim_reflection_;
  return metadata;
}


}  // namespace MsgsDispatcherToSim
