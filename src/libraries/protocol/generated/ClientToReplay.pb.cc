// Generated by the protocol buffer compiler.  DO NOT EDIT!

#define INTERNAL_SUPPRESS_PROTOBUF_FIELD_DEPRECATION
#include "ClientToReplay.pb.h"
#include <google/protobuf/stubs/once.h>
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/wire_format_lite_inl.h>
#include <google/protobuf/descriptor.h>
#include <google/protobuf/reflection_ops.h>
#include <google/protobuf/wire_format.h>

namespace MsgsClientToReplay {

namespace {

const ::google::protobuf::Descriptor* MsgControlSkipToTick_descriptor_ = NULL;
const ::google::protobuf::internal::GeneratedMessageReflection*
  MsgControlSkipToTick_reflection_ = NULL;
const ::google::protobuf::Descriptor* MsgClientToReplay_descriptor_ = NULL;
const ::google::protobuf::internal::GeneratedMessageReflection*
  MsgClientToReplay_reflection_ = NULL;
const ::google::protobuf::Descriptor* MsgClientToReplay_Content_descriptor_ = NULL;
const ::google::protobuf::internal::GeneratedMessageReflection*
  MsgClientToReplay_Content_reflection_ = NULL;

}  // namespace


void protobuf_AssignDesc_ClientToReplay_2eproto() {
  protobuf_AddDesc_ClientToReplay_2eproto();
  const ::google::protobuf::FileDescriptor* file =
    ::google::protobuf::DescriptorPool::generated_pool()->FindFileByName(
      "ClientToReplay.proto");
  GOOGLE_CHECK(file != NULL);
  MsgControlSkipToTick_descriptor_ = file->message_type(0);
  static const int MsgControlSkipToTick_offsets_[1] = {
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(MsgControlSkipToTick, tick_),
  };
  MsgControlSkipToTick_reflection_ =
    new ::google::protobuf::internal::GeneratedMessageReflection(
      MsgControlSkipToTick_descriptor_,
      MsgControlSkipToTick::default_instance_,
      MsgControlSkipToTick_offsets_,
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(MsgControlSkipToTick, _has_bits_[0]),
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(MsgControlSkipToTick, _unknown_fields_),
      -1,
      ::google::protobuf::DescriptorPool::generated_pool(),
      ::google::protobuf::MessageFactory::generated_factory(),
      sizeof(MsgControlSkipToTick));
  MsgClientToReplay_descriptor_ = file->message_type(1);
  static const int MsgClientToReplay_offsets_[2] = {
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(MsgClientToReplay, context_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(MsgClientToReplay, message_),
  };
  MsgClientToReplay_reflection_ =
    new ::google::protobuf::internal::GeneratedMessageReflection(
      MsgClientToReplay_descriptor_,
      MsgClientToReplay::default_instance_,
      MsgClientToReplay_offsets_,
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(MsgClientToReplay, _has_bits_[0]),
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(MsgClientToReplay, _unknown_fields_),
      -1,
      ::google::protobuf::DescriptorPool::generated_pool(),
      ::google::protobuf::MessageFactory::generated_factory(),
      sizeof(MsgClientToReplay));
  MsgClientToReplay_Content_descriptor_ = MsgClientToReplay_descriptor_->nested_type(0);
  static const int MsgClientToReplay_Content_offsets_[5] = {
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(MsgClientToReplay_Content, control_skip_to_tick_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(MsgClientToReplay_Content, control_change_time_factor_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(MsgClientToReplay_Content, control_stop_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(MsgClientToReplay_Content, control_pause_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(MsgClientToReplay_Content, control_resume_),
  };
  MsgClientToReplay_Content_reflection_ =
    new ::google::protobuf::internal::GeneratedMessageReflection(
      MsgClientToReplay_Content_descriptor_,
      MsgClientToReplay_Content::default_instance_,
      MsgClientToReplay_Content_offsets_,
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(MsgClientToReplay_Content, _has_bits_[0]),
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(MsgClientToReplay_Content, _unknown_fields_),
      -1,
      ::google::protobuf::DescriptorPool::generated_pool(),
      ::google::protobuf::MessageFactory::generated_factory(),
      sizeof(MsgClientToReplay_Content));
}

namespace {

GOOGLE_PROTOBUF_DECLARE_ONCE(protobuf_AssignDescriptors_once_);
inline void protobuf_AssignDescriptorsOnce() {
  ::google::protobuf::GoogleOnceInit(&protobuf_AssignDescriptors_once_,
                 &protobuf_AssignDesc_ClientToReplay_2eproto);
}

void protobuf_RegisterTypes(const ::std::string&) {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedMessage(
    MsgControlSkipToTick_descriptor_, &MsgControlSkipToTick::default_instance());
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedMessage(
    MsgClientToReplay_descriptor_, &MsgClientToReplay::default_instance());
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedMessage(
    MsgClientToReplay_Content_descriptor_, &MsgClientToReplay_Content::default_instance());
}

}  // namespace

void protobuf_ShutdownFile_ClientToReplay_2eproto() {
  delete MsgControlSkipToTick::default_instance_;
  delete MsgControlSkipToTick_reflection_;
  delete MsgClientToReplay::default_instance_;
  delete MsgClientToReplay_reflection_;
  delete MsgClientToReplay_Content::default_instance_;
  delete MsgClientToReplay_Content_reflection_;
}

void protobuf_AddDesc_ClientToReplay_2eproto() {
  static bool already_here = false;
  if (already_here) return;
  already_here = true;
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  ::Common::protobuf_AddDesc_Common_2eproto();
  ::google::protobuf::DescriptorPool::InternalAddGeneratedFile(
    "\n\024ClientToReplay.proto\022\022MsgsClientToRepl"
    "ay\032\014Common.proto\"$\n\024MsgControlSkipToTick"
    "\022\014\n\004tick\030\001 \002(\005\"\220\003\n\021MsgClientToReplay\022\017\n\007"
    "context\030\001 \001(\005\022>\n\007message\030\002 \002(\0132-.MsgsCli"
    "entToReplay.MsgClientToReplay.Content\032\251\002"
    "\n\007Content\022F\n\024control_skip_to_tick\030\001 \001(\0132"
    "(.MsgsClientToReplay.MsgControlSkipToTic"
    "k\022F\n\032control_change_time_factor\030\002 \001(\0132\"."
    "Common.MsgControlChangeTimeFactor\022,\n\014con"
    "trol_stop\030\003 \001(\0132\026.Common.MsgControlStop\022"
    ".\n\rcontrol_pause\030\004 \001(\0132\027.Common.MsgContr"
    "olPause\0220\n\016control_resume\030\005 \001(\0132\030.Common"
    ".MsgControlResume", 497);
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedFile(
    "ClientToReplay.proto", &protobuf_RegisterTypes);
  MsgControlSkipToTick::default_instance_ = new MsgControlSkipToTick();
  MsgClientToReplay::default_instance_ = new MsgClientToReplay();
  MsgClientToReplay_Content::default_instance_ = new MsgClientToReplay_Content();
  MsgControlSkipToTick::default_instance_->InitAsDefaultInstance();
  MsgClientToReplay::default_instance_->InitAsDefaultInstance();
  MsgClientToReplay_Content::default_instance_->InitAsDefaultInstance();
  ::google::protobuf::internal::OnShutdown(&protobuf_ShutdownFile_ClientToReplay_2eproto);
}

// Force AddDescriptors() to be called at static initialization time.
struct StaticDescriptorInitializer_ClientToReplay_2eproto {
  StaticDescriptorInitializer_ClientToReplay_2eproto() {
    protobuf_AddDesc_ClientToReplay_2eproto();
  }
} static_descriptor_initializer_ClientToReplay_2eproto_;


// ===================================================================

#ifndef _MSC_VER
const int MsgControlSkipToTick::kTickFieldNumber;
#endif  // !_MSC_VER

MsgControlSkipToTick::MsgControlSkipToTick() {
  SharedCtor();
}

void MsgControlSkipToTick::InitAsDefaultInstance() {
}

MsgControlSkipToTick::MsgControlSkipToTick(const MsgControlSkipToTick& from) {
  SharedCtor();
  MergeFrom(from);
}

void MsgControlSkipToTick::SharedCtor() {
  _cached_size_ = 0;
  tick_ = 0;
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
}

MsgControlSkipToTick::~MsgControlSkipToTick() {
  SharedDtor();
}

void MsgControlSkipToTick::SharedDtor() {
  if (this != default_instance_) {
  }
}

const ::google::protobuf::Descriptor* MsgControlSkipToTick::descriptor() {
  protobuf_AssignDescriptorsOnce();
  return MsgControlSkipToTick_descriptor_;
}

const MsgControlSkipToTick& MsgControlSkipToTick::default_instance() {
  if (default_instance_ == NULL) protobuf_AddDesc_ClientToReplay_2eproto();  return *default_instance_;
}

MsgControlSkipToTick* MsgControlSkipToTick::default_instance_ = NULL;

MsgControlSkipToTick* MsgControlSkipToTick::New() const {
  return new MsgControlSkipToTick;
}

void MsgControlSkipToTick::Clear() {
  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    tick_ = 0;
  }
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
  mutable_unknown_fields()->Clear();
}

bool MsgControlSkipToTick::MergePartialFromCodedStream(
    ::google::protobuf::io::CodedInputStream* input) {
#define DO_(EXPRESSION) if (!(EXPRESSION)) return false
  ::google::protobuf::uint32 tag;
  while ((tag = input->ReadTag()) != 0) {
    switch (::google::protobuf::internal::WireFormatLite::GetTagFieldNumber(tag)) {
      // required int32 tick = 1;
      case 1: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) !=
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_VARINT) {
          goto handle_uninterpreted;
        }
        DO_(::google::protobuf::internal::WireFormatLite::ReadInt32(
              input, &tick_));
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

void MsgControlSkipToTick::SerializeWithCachedSizes(
    ::google::protobuf::io::CodedOutputStream* output) const {
  ::google::protobuf::uint8* raw_buffer = output->GetDirectBufferForNBytesAndAdvance(_cached_size_);
  if (raw_buffer != NULL) {
    MsgControlSkipToTick::SerializeWithCachedSizesToArray(raw_buffer);
    return;
  }
  
  // required int32 tick = 1;
  if (_has_bit(0)) {
    ::google::protobuf::internal::WireFormatLite::WriteInt32(1, this->tick(), output);
  }
  
  if (!unknown_fields().empty()) {
    ::google::protobuf::internal::WireFormat::SerializeUnknownFields(
        unknown_fields(), output);
  }
}

::google::protobuf::uint8* MsgControlSkipToTick::SerializeWithCachedSizesToArray(
    ::google::protobuf::uint8* target) const {
  // required int32 tick = 1;
  if (_has_bit(0)) {
    target = ::google::protobuf::internal::WireFormatLite::WriteInt32ToArray(1, this->tick(), target);
  }
  
  if (!unknown_fields().empty()) {
    target = ::google::protobuf::internal::WireFormat::SerializeUnknownFieldsToArray(
        unknown_fields(), target);
  }
  return target;
}

int MsgControlSkipToTick::ByteSize() const {
  int total_size = 0;
  
  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    // required int32 tick = 1;
    if (has_tick()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::Int32Size(
          this->tick());
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

void MsgControlSkipToTick::MergeFrom(const ::google::protobuf::Message& from) {
  GOOGLE_CHECK_NE(&from, this);
  const MsgControlSkipToTick* source =
    ::google::protobuf::internal::dynamic_cast_if_available<const MsgControlSkipToTick*>(
      &from);
  if (source == NULL) {
    ::google::protobuf::internal::ReflectionOps::Merge(from, this);
  } else {
    MergeFrom(*source);
  }
}

void MsgControlSkipToTick::MergeFrom(const MsgControlSkipToTick& from) {
  GOOGLE_CHECK_NE(&from, this);
  if (from._has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    if (from._has_bit(0)) {
      set_tick(from.tick());
    }
  }
  mutable_unknown_fields()->MergeFrom(from.unknown_fields());
}

void MsgControlSkipToTick::CopyFrom(const ::google::protobuf::Message& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void MsgControlSkipToTick::CopyFrom(const MsgControlSkipToTick& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool MsgControlSkipToTick::IsInitialized() const {
  if ((_has_bits_[0] & 0x00000001) != 0x00000001) return false;
  
  return true;
}

void MsgControlSkipToTick::Swap(MsgControlSkipToTick* other) {
  if (other != this) {
    std::swap(tick_, other->tick_);
    std::swap(_has_bits_[0], other->_has_bits_[0]);
    _unknown_fields_.Swap(&other->_unknown_fields_);
    std::swap(_cached_size_, other->_cached_size_);
  }
}

::google::protobuf::Metadata MsgControlSkipToTick::GetMetadata() const {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::Metadata metadata;
  metadata.descriptor = MsgControlSkipToTick_descriptor_;
  metadata.reflection = MsgControlSkipToTick_reflection_;
  return metadata;
}


// ===================================================================

#ifndef _MSC_VER
const int MsgClientToReplay_Content::kControlSkipToTickFieldNumber;
const int MsgClientToReplay_Content::kControlChangeTimeFactorFieldNumber;
const int MsgClientToReplay_Content::kControlStopFieldNumber;
const int MsgClientToReplay_Content::kControlPauseFieldNumber;
const int MsgClientToReplay_Content::kControlResumeFieldNumber;
#endif  // !_MSC_VER

MsgClientToReplay_Content::MsgClientToReplay_Content() {
  SharedCtor();
}

void MsgClientToReplay_Content::InitAsDefaultInstance() {
  control_skip_to_tick_ = const_cast< ::MsgsClientToReplay::MsgControlSkipToTick*>(&::MsgsClientToReplay::MsgControlSkipToTick::default_instance());
  control_change_time_factor_ = const_cast< ::Common::MsgControlChangeTimeFactor*>(&::Common::MsgControlChangeTimeFactor::default_instance());
  control_stop_ = const_cast< ::Common::MsgControlStop*>(&::Common::MsgControlStop::default_instance());
  control_pause_ = const_cast< ::Common::MsgControlPause*>(&::Common::MsgControlPause::default_instance());
  control_resume_ = const_cast< ::Common::MsgControlResume*>(&::Common::MsgControlResume::default_instance());
}

MsgClientToReplay_Content::MsgClientToReplay_Content(const MsgClientToReplay_Content& from) {
  SharedCtor();
  MergeFrom(from);
}

void MsgClientToReplay_Content::SharedCtor() {
  _cached_size_ = 0;
  control_skip_to_tick_ = NULL;
  control_change_time_factor_ = NULL;
  control_stop_ = NULL;
  control_pause_ = NULL;
  control_resume_ = NULL;
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
}

MsgClientToReplay_Content::~MsgClientToReplay_Content() {
  SharedDtor();
}

void MsgClientToReplay_Content::SharedDtor() {
  if (this != default_instance_) {
    delete control_skip_to_tick_;
    delete control_change_time_factor_;
    delete control_stop_;
    delete control_pause_;
    delete control_resume_;
  }
}

const ::google::protobuf::Descriptor* MsgClientToReplay_Content::descriptor() {
  protobuf_AssignDescriptorsOnce();
  return MsgClientToReplay_Content_descriptor_;
}

const MsgClientToReplay_Content& MsgClientToReplay_Content::default_instance() {
  if (default_instance_ == NULL) protobuf_AddDesc_ClientToReplay_2eproto();  return *default_instance_;
}

MsgClientToReplay_Content* MsgClientToReplay_Content::default_instance_ = NULL;

MsgClientToReplay_Content* MsgClientToReplay_Content::New() const {
  return new MsgClientToReplay_Content;
}

void MsgClientToReplay_Content::Clear() {
  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    if (_has_bit(0)) {
      if (control_skip_to_tick_ != NULL) control_skip_to_tick_->::MsgsClientToReplay::MsgControlSkipToTick::Clear();
    }
    if (_has_bit(1)) {
      if (control_change_time_factor_ != NULL) control_change_time_factor_->::Common::MsgControlChangeTimeFactor::Clear();
    }
    if (_has_bit(2)) {
      if (control_stop_ != NULL) control_stop_->::Common::MsgControlStop::Clear();
    }
    if (_has_bit(3)) {
      if (control_pause_ != NULL) control_pause_->::Common::MsgControlPause::Clear();
    }
    if (_has_bit(4)) {
      if (control_resume_ != NULL) control_resume_->::Common::MsgControlResume::Clear();
    }
  }
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
  mutable_unknown_fields()->Clear();
}

bool MsgClientToReplay_Content::MergePartialFromCodedStream(
    ::google::protobuf::io::CodedInputStream* input) {
#define DO_(EXPRESSION) if (!(EXPRESSION)) return false
  ::google::protobuf::uint32 tag;
  while ((tag = input->ReadTag()) != 0) {
    switch (::google::protobuf::internal::WireFormatLite::GetTagFieldNumber(tag)) {
      // optional .MsgsClientToReplay.MsgControlSkipToTick control_skip_to_tick = 1;
      case 1: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) !=
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_LENGTH_DELIMITED) {
          goto handle_uninterpreted;
        }
        DO_(::google::protobuf::internal::WireFormatLite::ReadMessageNoVirtual(
             input, mutable_control_skip_to_tick()));
        if (input->ExpectTag(18)) goto parse_control_change_time_factor;
        break;
      }
      
      // optional .Common.MsgControlChangeTimeFactor control_change_time_factor = 2;
      case 2: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) !=
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_LENGTH_DELIMITED) {
          goto handle_uninterpreted;
        }
       parse_control_change_time_factor:
        DO_(::google::protobuf::internal::WireFormatLite::ReadMessageNoVirtual(
             input, mutable_control_change_time_factor()));
        if (input->ExpectTag(26)) goto parse_control_stop;
        break;
      }
      
      // optional .Common.MsgControlStop control_stop = 3;
      case 3: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) !=
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_LENGTH_DELIMITED) {
          goto handle_uninterpreted;
        }
       parse_control_stop:
        DO_(::google::protobuf::internal::WireFormatLite::ReadMessageNoVirtual(
             input, mutable_control_stop()));
        if (input->ExpectTag(34)) goto parse_control_pause;
        break;
      }
      
      // optional .Common.MsgControlPause control_pause = 4;
      case 4: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) !=
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_LENGTH_DELIMITED) {
          goto handle_uninterpreted;
        }
       parse_control_pause:
        DO_(::google::protobuf::internal::WireFormatLite::ReadMessageNoVirtual(
             input, mutable_control_pause()));
        if (input->ExpectTag(42)) goto parse_control_resume;
        break;
      }
      
      // optional .Common.MsgControlResume control_resume = 5;
      case 5: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) !=
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_LENGTH_DELIMITED) {
          goto handle_uninterpreted;
        }
       parse_control_resume:
        DO_(::google::protobuf::internal::WireFormatLite::ReadMessageNoVirtual(
             input, mutable_control_resume()));
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

void MsgClientToReplay_Content::SerializeWithCachedSizes(
    ::google::protobuf::io::CodedOutputStream* output) const {
  ::google::protobuf::uint8* raw_buffer = output->GetDirectBufferForNBytesAndAdvance(_cached_size_);
  if (raw_buffer != NULL) {
    MsgClientToReplay_Content::SerializeWithCachedSizesToArray(raw_buffer);
    return;
  }
  
  // optional .MsgsClientToReplay.MsgControlSkipToTick control_skip_to_tick = 1;
  if (_has_bit(0)) {
    ::google::protobuf::internal::WireFormatLite::WriteMessageNoVirtual(
      1, this->control_skip_to_tick(), output);
  }
  
  // optional .Common.MsgControlChangeTimeFactor control_change_time_factor = 2;
  if (_has_bit(1)) {
    ::google::protobuf::internal::WireFormatLite::WriteMessageNoVirtual(
      2, this->control_change_time_factor(), output);
  }
  
  // optional .Common.MsgControlStop control_stop = 3;
  if (_has_bit(2)) {
    ::google::protobuf::internal::WireFormatLite::WriteMessageNoVirtual(
      3, this->control_stop(), output);
  }
  
  // optional .Common.MsgControlPause control_pause = 4;
  if (_has_bit(3)) {
    ::google::protobuf::internal::WireFormatLite::WriteMessageNoVirtual(
      4, this->control_pause(), output);
  }
  
  // optional .Common.MsgControlResume control_resume = 5;
  if (_has_bit(4)) {
    ::google::protobuf::internal::WireFormatLite::WriteMessageNoVirtual(
      5, this->control_resume(), output);
  }
  
  if (!unknown_fields().empty()) {
    ::google::protobuf::internal::WireFormat::SerializeUnknownFields(
        unknown_fields(), output);
  }
}

::google::protobuf::uint8* MsgClientToReplay_Content::SerializeWithCachedSizesToArray(
    ::google::protobuf::uint8* target) const {
  // optional .MsgsClientToReplay.MsgControlSkipToTick control_skip_to_tick = 1;
  if (_has_bit(0)) {
    target = ::google::protobuf::internal::WireFormatLite::
      WriteMessageNoVirtualToArray(
        1, this->control_skip_to_tick(), target);
  }
  
  // optional .Common.MsgControlChangeTimeFactor control_change_time_factor = 2;
  if (_has_bit(1)) {
    target = ::google::protobuf::internal::WireFormatLite::
      WriteMessageNoVirtualToArray(
        2, this->control_change_time_factor(), target);
  }
  
  // optional .Common.MsgControlStop control_stop = 3;
  if (_has_bit(2)) {
    target = ::google::protobuf::internal::WireFormatLite::
      WriteMessageNoVirtualToArray(
        3, this->control_stop(), target);
  }
  
  // optional .Common.MsgControlPause control_pause = 4;
  if (_has_bit(3)) {
    target = ::google::protobuf::internal::WireFormatLite::
      WriteMessageNoVirtualToArray(
        4, this->control_pause(), target);
  }
  
  // optional .Common.MsgControlResume control_resume = 5;
  if (_has_bit(4)) {
    target = ::google::protobuf::internal::WireFormatLite::
      WriteMessageNoVirtualToArray(
        5, this->control_resume(), target);
  }
  
  if (!unknown_fields().empty()) {
    target = ::google::protobuf::internal::WireFormat::SerializeUnknownFieldsToArray(
        unknown_fields(), target);
  }
  return target;
}

int MsgClientToReplay_Content::ByteSize() const {
  int total_size = 0;
  
  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    // optional .MsgsClientToReplay.MsgControlSkipToTick control_skip_to_tick = 1;
    if (has_control_skip_to_tick()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::MessageSizeNoVirtual(
          this->control_skip_to_tick());
    }
    
    // optional .Common.MsgControlChangeTimeFactor control_change_time_factor = 2;
    if (has_control_change_time_factor()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::MessageSizeNoVirtual(
          this->control_change_time_factor());
    }
    
    // optional .Common.MsgControlStop control_stop = 3;
    if (has_control_stop()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::MessageSizeNoVirtual(
          this->control_stop());
    }
    
    // optional .Common.MsgControlPause control_pause = 4;
    if (has_control_pause()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::MessageSizeNoVirtual(
          this->control_pause());
    }
    
    // optional .Common.MsgControlResume control_resume = 5;
    if (has_control_resume()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::MessageSizeNoVirtual(
          this->control_resume());
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

void MsgClientToReplay_Content::MergeFrom(const ::google::protobuf::Message& from) {
  GOOGLE_CHECK_NE(&from, this);
  const MsgClientToReplay_Content* source =
    ::google::protobuf::internal::dynamic_cast_if_available<const MsgClientToReplay_Content*>(
      &from);
  if (source == NULL) {
    ::google::protobuf::internal::ReflectionOps::Merge(from, this);
  } else {
    MergeFrom(*source);
  }
}

void MsgClientToReplay_Content::MergeFrom(const MsgClientToReplay_Content& from) {
  GOOGLE_CHECK_NE(&from, this);
  if (from._has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    if (from._has_bit(0)) {
      mutable_control_skip_to_tick()->::MsgsClientToReplay::MsgControlSkipToTick::MergeFrom(from.control_skip_to_tick());
    }
    if (from._has_bit(1)) {
      mutable_control_change_time_factor()->::Common::MsgControlChangeTimeFactor::MergeFrom(from.control_change_time_factor());
    }
    if (from._has_bit(2)) {
      mutable_control_stop()->::Common::MsgControlStop::MergeFrom(from.control_stop());
    }
    if (from._has_bit(3)) {
      mutable_control_pause()->::Common::MsgControlPause::MergeFrom(from.control_pause());
    }
    if (from._has_bit(4)) {
      mutable_control_resume()->::Common::MsgControlResume::MergeFrom(from.control_resume());
    }
  }
  mutable_unknown_fields()->MergeFrom(from.unknown_fields());
}

void MsgClientToReplay_Content::CopyFrom(const ::google::protobuf::Message& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void MsgClientToReplay_Content::CopyFrom(const MsgClientToReplay_Content& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool MsgClientToReplay_Content::IsInitialized() const {
  
  if (has_control_skip_to_tick()) {
    if (!this->control_skip_to_tick().IsInitialized()) return false;
  }
  if (has_control_change_time_factor()) {
    if (!this->control_change_time_factor().IsInitialized()) return false;
  }
  return true;
}

void MsgClientToReplay_Content::Swap(MsgClientToReplay_Content* other) {
  if (other != this) {
    std::swap(control_skip_to_tick_, other->control_skip_to_tick_);
    std::swap(control_change_time_factor_, other->control_change_time_factor_);
    std::swap(control_stop_, other->control_stop_);
    std::swap(control_pause_, other->control_pause_);
    std::swap(control_resume_, other->control_resume_);
    std::swap(_has_bits_[0], other->_has_bits_[0]);
    _unknown_fields_.Swap(&other->_unknown_fields_);
    std::swap(_cached_size_, other->_cached_size_);
  }
}

::google::protobuf::Metadata MsgClientToReplay_Content::GetMetadata() const {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::Metadata metadata;
  metadata.descriptor = MsgClientToReplay_Content_descriptor_;
  metadata.reflection = MsgClientToReplay_Content_reflection_;
  return metadata;
}


// -------------------------------------------------------------------

#ifndef _MSC_VER
const int MsgClientToReplay::kContextFieldNumber;
const int MsgClientToReplay::kMessageFieldNumber;
#endif  // !_MSC_VER

MsgClientToReplay::MsgClientToReplay() {
  SharedCtor();
}

void MsgClientToReplay::InitAsDefaultInstance() {
  message_ = const_cast< ::MsgsClientToReplay::MsgClientToReplay_Content*>(&::MsgsClientToReplay::MsgClientToReplay_Content::default_instance());
}

MsgClientToReplay::MsgClientToReplay(const MsgClientToReplay& from) {
  SharedCtor();
  MergeFrom(from);
}

void MsgClientToReplay::SharedCtor() {
  _cached_size_ = 0;
  context_ = 0;
  message_ = NULL;
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
}

MsgClientToReplay::~MsgClientToReplay() {
  SharedDtor();
}

void MsgClientToReplay::SharedDtor() {
  if (this != default_instance_) {
    delete message_;
  }
}

const ::google::protobuf::Descriptor* MsgClientToReplay::descriptor() {
  protobuf_AssignDescriptorsOnce();
  return MsgClientToReplay_descriptor_;
}

const MsgClientToReplay& MsgClientToReplay::default_instance() {
  if (default_instance_ == NULL) protobuf_AddDesc_ClientToReplay_2eproto();  return *default_instance_;
}

MsgClientToReplay* MsgClientToReplay::default_instance_ = NULL;

MsgClientToReplay* MsgClientToReplay::New() const {
  return new MsgClientToReplay;
}

void MsgClientToReplay::Clear() {
  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    context_ = 0;
    if (_has_bit(1)) {
      if (message_ != NULL) message_->::MsgsClientToReplay::MsgClientToReplay_Content::Clear();
    }
  }
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
  mutable_unknown_fields()->Clear();
}

bool MsgClientToReplay::MergePartialFromCodedStream(
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
      
      // required .MsgsClientToReplay.MsgClientToReplay.Content message = 2;
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

void MsgClientToReplay::SerializeWithCachedSizes(
    ::google::protobuf::io::CodedOutputStream* output) const {
  ::google::protobuf::uint8* raw_buffer = output->GetDirectBufferForNBytesAndAdvance(_cached_size_);
  if (raw_buffer != NULL) {
    MsgClientToReplay::SerializeWithCachedSizesToArray(raw_buffer);
    return;
  }
  
  // optional int32 context = 1;
  if (_has_bit(0)) {
    ::google::protobuf::internal::WireFormatLite::WriteInt32(1, this->context(), output);
  }
  
  // required .MsgsClientToReplay.MsgClientToReplay.Content message = 2;
  if (_has_bit(1)) {
    ::google::protobuf::internal::WireFormatLite::WriteMessageNoVirtual(
      2, this->message(), output);
  }
  
  if (!unknown_fields().empty()) {
    ::google::protobuf::internal::WireFormat::SerializeUnknownFields(
        unknown_fields(), output);
  }
}

::google::protobuf::uint8* MsgClientToReplay::SerializeWithCachedSizesToArray(
    ::google::protobuf::uint8* target) const {
  // optional int32 context = 1;
  if (_has_bit(0)) {
    target = ::google::protobuf::internal::WireFormatLite::WriteInt32ToArray(1, this->context(), target);
  }
  
  // required .MsgsClientToReplay.MsgClientToReplay.Content message = 2;
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

int MsgClientToReplay::ByteSize() const {
  int total_size = 0;
  
  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    // optional int32 context = 1;
    if (has_context()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::Int32Size(
          this->context());
    }
    
    // required .MsgsClientToReplay.MsgClientToReplay.Content message = 2;
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

void MsgClientToReplay::MergeFrom(const ::google::protobuf::Message& from) {
  GOOGLE_CHECK_NE(&from, this);
  const MsgClientToReplay* source =
    ::google::protobuf::internal::dynamic_cast_if_available<const MsgClientToReplay*>(
      &from);
  if (source == NULL) {
    ::google::protobuf::internal::ReflectionOps::Merge(from, this);
  } else {
    MergeFrom(*source);
  }
}

void MsgClientToReplay::MergeFrom(const MsgClientToReplay& from) {
  GOOGLE_CHECK_NE(&from, this);
  if (from._has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    if (from._has_bit(0)) {
      set_context(from.context());
    }
    if (from._has_bit(1)) {
      mutable_message()->::MsgsClientToReplay::MsgClientToReplay_Content::MergeFrom(from.message());
    }
  }
  mutable_unknown_fields()->MergeFrom(from.unknown_fields());
}

void MsgClientToReplay::CopyFrom(const ::google::protobuf::Message& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void MsgClientToReplay::CopyFrom(const MsgClientToReplay& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool MsgClientToReplay::IsInitialized() const {
  if ((_has_bits_[0] & 0x00000002) != 0x00000002) return false;
  
  if (has_message()) {
    if (!this->message().IsInitialized()) return false;
  }
  return true;
}

void MsgClientToReplay::Swap(MsgClientToReplay* other) {
  if (other != this) {
    std::swap(context_, other->context_);
    std::swap(message_, other->message_);
    std::swap(_has_bits_[0], other->_has_bits_[0]);
    _unknown_fields_.Swap(&other->_unknown_fields_);
    std::swap(_cached_size_, other->_cached_size_);
  }
}

::google::protobuf::Metadata MsgClientToReplay::GetMetadata() const {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::Metadata metadata;
  metadata.descriptor = MsgClientToReplay_descriptor_;
  metadata.reflection = MsgClientToReplay_reflection_;
  return metadata;
}


}  // namespace MsgsClientToReplay
