// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: AppDb.proto

#ifndef GOOGLE_PROTOBUF_INCLUDED_AppDb_2eproto
#define GOOGLE_PROTOBUF_INCLUDED_AppDb_2eproto

#include <limits>
#include <string>

#include <google/protobuf/port_def.inc>
#if PROTOBUF_VERSION < 3009000
#error This file was generated by a newer version of protoc which is
#error incompatible with your Protocol Buffer headers. Please update
#error your headers.
#endif
#if 3009002 < PROTOBUF_MIN_PROTOC_VERSION
#error This file was generated by an older version of protoc which is
#error incompatible with your Protocol Buffer headers. Please
#error regenerate this file with a newer version of protoc.
#endif

#include <google/protobuf/port_undef.inc>
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/arena.h>
#include <google/protobuf/arenastring.h>
#include <google/protobuf/generated_message_table_driven.h>
#include <google/protobuf/generated_message_util.h>
#include <google/protobuf/inlined_string_field.h>
#include <google/protobuf/metadata.h>
#include <google/protobuf/generated_message_reflection.h>
#include <google/protobuf/message.h>
#include <google/protobuf/repeated_field.h>  // IWYU pragma: export
#include <google/protobuf/extension_set.h>  // IWYU pragma: export
#include <google/protobuf/unknown_field_set.h>
#include "AppDbRecord.pb.h"
// @@protoc_insertion_point(includes)
#include <google/protobuf/port_def.inc>
#define PROTOBUF_INTERNAL_EXPORT_AppDb_2eproto
PROTOBUF_NAMESPACE_OPEN
namespace internal {
class AnyMetadata;
}  // namespace internal
PROTOBUF_NAMESPACE_CLOSE

// Internal implementation detail -- do not use these members.
struct TableStruct_AppDb_2eproto {
  static const ::PROTOBUF_NAMESPACE_ID::internal::ParseTableField entries[]
    PROTOBUF_SECTION_VARIABLE(protodesc_cold);
  static const ::PROTOBUF_NAMESPACE_ID::internal::AuxillaryParseTableField aux[]
    PROTOBUF_SECTION_VARIABLE(protodesc_cold);
  static const ::PROTOBUF_NAMESPACE_ID::internal::ParseTable schema[1]
    PROTOBUF_SECTION_VARIABLE(protodesc_cold);
  static const ::PROTOBUF_NAMESPACE_ID::internal::FieldMetadata field_metadata[];
  static const ::PROTOBUF_NAMESPACE_ID::internal::SerializationTable serialization_table[];
  static const ::PROTOBUF_NAMESPACE_ID::uint32 offsets[];
};
extern const ::PROTOBUF_NAMESPACE_ID::internal::DescriptorTable descriptor_table_AppDb_2eproto;
class AppDb;
class AppDbDefaultTypeInternal;
extern AppDbDefaultTypeInternal _AppDb_default_instance_;
PROTOBUF_NAMESPACE_OPEN
template<> ::AppDb* Arena::CreateMaybeMessage<::AppDb>(Arena*);
PROTOBUF_NAMESPACE_CLOSE

// ===================================================================

class AppDb :
    public ::PROTOBUF_NAMESPACE_ID::Message /* @@protoc_insertion_point(class_definition:AppDb) */ {
 public:
  AppDb();
  virtual ~AppDb();

  AppDb(const AppDb& from);
  AppDb(AppDb&& from) noexcept
    : AppDb() {
    *this = ::std::move(from);
  }

  inline AppDb& operator=(const AppDb& from) {
    CopyFrom(from);
    return *this;
  }
  inline AppDb& operator=(AppDb&& from) noexcept {
    if (GetArenaNoVirtual() == from.GetArenaNoVirtual()) {
      if (this != &from) InternalSwap(&from);
    } else {
      CopyFrom(from);
    }
    return *this;
  }

  static const ::PROTOBUF_NAMESPACE_ID::Descriptor* descriptor() {
    return GetDescriptor();
  }
  static const ::PROTOBUF_NAMESPACE_ID::Descriptor* GetDescriptor() {
    return GetMetadataStatic().descriptor;
  }
  static const ::PROTOBUF_NAMESPACE_ID::Reflection* GetReflection() {
    return GetMetadataStatic().reflection;
  }
  static const AppDb& default_instance();

  static void InitAsDefaultInstance();  // FOR INTERNAL USE ONLY
  static inline const AppDb* internal_default_instance() {
    return reinterpret_cast<const AppDb*>(
               &_AppDb_default_instance_);
  }
  static constexpr int kIndexInFileMessages =
    0;

  friend void swap(AppDb& a, AppDb& b) {
    a.Swap(&b);
  }
  inline void Swap(AppDb* other) {
    if (other == this) return;
    InternalSwap(other);
  }

  // implements Message ----------------------------------------------

  inline AppDb* New() const final {
    return CreateMaybeMessage<AppDb>(nullptr);
  }

  AppDb* New(::PROTOBUF_NAMESPACE_ID::Arena* arena) const final {
    return CreateMaybeMessage<AppDb>(arena);
  }
  void CopyFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) final;
  void MergeFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) final;
  void CopyFrom(const AppDb& from);
  void MergeFrom(const AppDb& from);
  PROTOBUF_ATTRIBUTE_REINITIALIZES void Clear() final;
  bool IsInitialized() const final;

  size_t ByteSizeLong() const final;
  #if GOOGLE_PROTOBUF_ENABLE_EXPERIMENTAL_PARSER
  const char* _InternalParse(const char* ptr, ::PROTOBUF_NAMESPACE_ID::internal::ParseContext* ctx) final;
  #else
  bool MergePartialFromCodedStream(
      ::PROTOBUF_NAMESPACE_ID::io::CodedInputStream* input) final;
  #endif  // GOOGLE_PROTOBUF_ENABLE_EXPERIMENTAL_PARSER
  void SerializeWithCachedSizes(
      ::PROTOBUF_NAMESPACE_ID::io::CodedOutputStream* output) const final;
  ::PROTOBUF_NAMESPACE_ID::uint8* InternalSerializeWithCachedSizesToArray(
      ::PROTOBUF_NAMESPACE_ID::uint8* target) const final;
  int GetCachedSize() const final { return _cached_size_.Get(); }

  private:
  inline void SharedCtor();
  inline void SharedDtor();
  void SetCachedSize(int size) const final;
  void InternalSwap(AppDb* other);
  friend class ::PROTOBUF_NAMESPACE_ID::internal::AnyMetadata;
  static ::PROTOBUF_NAMESPACE_ID::StringPiece FullMessageName() {
    return "AppDb";
  }
  private:
  inline ::PROTOBUF_NAMESPACE_ID::Arena* GetArenaNoVirtual() const {
    return nullptr;
  }
  inline void* MaybeArenaPtr() const {
    return nullptr;
  }
  public:

  ::PROTOBUF_NAMESPACE_ID::Metadata GetMetadata() const final;
  private:
  static ::PROTOBUF_NAMESPACE_ID::Metadata GetMetadataStatic() {
    ::PROTOBUF_NAMESPACE_ID::internal::AssignDescriptors(&::descriptor_table_AppDb_2eproto);
    return ::descriptor_table_AppDb_2eproto.file_level_metadata[kIndexInFileMessages];
  }

  public:

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  enum : int {
    kRecordsFieldNumber = 1,
  };
  // repeated .AppDbRecord records = 1;
  int records_size() const;
  void clear_records();
  ::AppDbRecord* mutable_records(int index);
  ::PROTOBUF_NAMESPACE_ID::RepeatedPtrField< ::AppDbRecord >*
      mutable_records();
  const ::AppDbRecord& records(int index) const;
  ::AppDbRecord* add_records();
  const ::PROTOBUF_NAMESPACE_ID::RepeatedPtrField< ::AppDbRecord >&
      records() const;

  // @@protoc_insertion_point(class_scope:AppDb)
 private:
  class _Internal;

  ::PROTOBUF_NAMESPACE_ID::internal::InternalMetadataWithArena _internal_metadata_;
  ::PROTOBUF_NAMESPACE_ID::RepeatedPtrField< ::AppDbRecord > records_;
  mutable ::PROTOBUF_NAMESPACE_ID::internal::CachedSize _cached_size_;
  friend struct ::TableStruct_AppDb_2eproto;
};
// ===================================================================


// ===================================================================

#ifdef __GNUC__
  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wstrict-aliasing"
#endif  // __GNUC__
// AppDb

// repeated .AppDbRecord records = 1;
inline int AppDb::records_size() const {
  return records_.size();
}
inline ::AppDbRecord* AppDb::mutable_records(int index) {
  // @@protoc_insertion_point(field_mutable:AppDb.records)
  return records_.Mutable(index);
}
inline ::PROTOBUF_NAMESPACE_ID::RepeatedPtrField< ::AppDbRecord >*
AppDb::mutable_records() {
  // @@protoc_insertion_point(field_mutable_list:AppDb.records)
  return &records_;
}
inline const ::AppDbRecord& AppDb::records(int index) const {
  // @@protoc_insertion_point(field_get:AppDb.records)
  return records_.Get(index);
}
inline ::AppDbRecord* AppDb::add_records() {
  // @@protoc_insertion_point(field_add:AppDb.records)
  return records_.Add();
}
inline const ::PROTOBUF_NAMESPACE_ID::RepeatedPtrField< ::AppDbRecord >&
AppDb::records() const {
  // @@protoc_insertion_point(field_list:AppDb.records)
  return records_;
}

#ifdef __GNUC__
  #pragma GCC diagnostic pop
#endif  // __GNUC__

// @@protoc_insertion_point(namespace_scope)


// @@protoc_insertion_point(global_scope)

#include <google/protobuf/port_undef.inc>
#endif  // GOOGLE_PROTOBUF_INCLUDED_GOOGLE_PROTOBUF_INCLUDED_AppDb_2eproto