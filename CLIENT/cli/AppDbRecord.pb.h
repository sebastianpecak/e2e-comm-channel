// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: AppDbRecord.proto

#ifndef GOOGLE_PROTOBUF_INCLUDED_AppDbRecord_2eproto
#define GOOGLE_PROTOBUF_INCLUDED_AppDbRecord_2eproto

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
// @@protoc_insertion_point(includes)
#include <google/protobuf/port_def.inc>
#define PROTOBUF_INTERNAL_EXPORT_AppDbRecord_2eproto
PROTOBUF_NAMESPACE_OPEN
namespace internal {
class AnyMetadata;
}  // namespace internal
PROTOBUF_NAMESPACE_CLOSE

// Internal implementation detail -- do not use these members.
struct TableStruct_AppDbRecord_2eproto {
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
extern const ::PROTOBUF_NAMESPACE_ID::internal::DescriptorTable descriptor_table_AppDbRecord_2eproto;
class AppDbRecord;
class AppDbRecordDefaultTypeInternal;
extern AppDbRecordDefaultTypeInternal _AppDbRecord_default_instance_;
PROTOBUF_NAMESPACE_OPEN
template<> ::AppDbRecord* Arena::CreateMaybeMessage<::AppDbRecord>(Arena*);
PROTOBUF_NAMESPACE_CLOSE

// ===================================================================

class AppDbRecord :
    public ::PROTOBUF_NAMESPACE_ID::Message /* @@protoc_insertion_point(class_definition:AppDbRecord) */ {
 public:
  AppDbRecord();
  virtual ~AppDbRecord();

  AppDbRecord(const AppDbRecord& from);
  AppDbRecord(AppDbRecord&& from) noexcept
    : AppDbRecord() {
    *this = ::std::move(from);
  }

  inline AppDbRecord& operator=(const AppDbRecord& from) {
    CopyFrom(from);
    return *this;
  }
  inline AppDbRecord& operator=(AppDbRecord&& from) noexcept {
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
  static const AppDbRecord& default_instance();

  static void InitAsDefaultInstance();  // FOR INTERNAL USE ONLY
  static inline const AppDbRecord* internal_default_instance() {
    return reinterpret_cast<const AppDbRecord*>(
               &_AppDbRecord_default_instance_);
  }
  static constexpr int kIndexInFileMessages =
    0;

  friend void swap(AppDbRecord& a, AppDbRecord& b) {
    a.Swap(&b);
  }
  inline void Swap(AppDbRecord* other) {
    if (other == this) return;
    InternalSwap(other);
  }

  // implements Message ----------------------------------------------

  inline AppDbRecord* New() const final {
    return CreateMaybeMessage<AppDbRecord>(nullptr);
  }

  AppDbRecord* New(::PROTOBUF_NAMESPACE_ID::Arena* arena) const final {
    return CreateMaybeMessage<AppDbRecord>(arena);
  }
  void CopyFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) final;
  void MergeFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) final;
  void CopyFrom(const AppDbRecord& from);
  void MergeFrom(const AppDbRecord& from);
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
  void InternalSwap(AppDbRecord* other);
  friend class ::PROTOBUF_NAMESPACE_ID::internal::AnyMetadata;
  static ::PROTOBUF_NAMESPACE_ID::StringPiece FullMessageName() {
    return "AppDbRecord";
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
    ::PROTOBUF_NAMESPACE_ID::internal::AssignDescriptors(&::descriptor_table_AppDbRecord_2eproto);
    return ::descriptor_table_AppDbRecord_2eproto.file_level_metadata[kIndexInFileMessages];
  }

  public:

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  enum : int {
    kUserIdFieldNumber = 1,
    kPublicKeyFieldNumber = 2,
    kPrivateKeyFieldNumber = 3,
  };
  // string userId = 1;
  void clear_userid();
  const std::string& userid() const;
  void set_userid(const std::string& value);
  void set_userid(std::string&& value);
  void set_userid(const char* value);
  void set_userid(const char* value, size_t size);
  std::string* mutable_userid();
  std::string* release_userid();
  void set_allocated_userid(std::string* userid);

  // bytes publicKey = 2;
  void clear_publickey();
  const std::string& publickey() const;
  void set_publickey(const std::string& value);
  void set_publickey(std::string&& value);
  void set_publickey(const char* value);
  void set_publickey(const void* value, size_t size);
  std::string* mutable_publickey();
  std::string* release_publickey();
  void set_allocated_publickey(std::string* publickey);

  // bytes privateKey = 3;
  void clear_privatekey();
  const std::string& privatekey() const;
  void set_privatekey(const std::string& value);
  void set_privatekey(std::string&& value);
  void set_privatekey(const char* value);
  void set_privatekey(const void* value, size_t size);
  std::string* mutable_privatekey();
  std::string* release_privatekey();
  void set_allocated_privatekey(std::string* privatekey);

  // @@protoc_insertion_point(class_scope:AppDbRecord)
 private:
  class _Internal;

  ::PROTOBUF_NAMESPACE_ID::internal::InternalMetadataWithArena _internal_metadata_;
  ::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr userid_;
  ::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr publickey_;
  ::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr privatekey_;
  mutable ::PROTOBUF_NAMESPACE_ID::internal::CachedSize _cached_size_;
  friend struct ::TableStruct_AppDbRecord_2eproto;
};
// ===================================================================


// ===================================================================

#ifdef __GNUC__
  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wstrict-aliasing"
#endif  // __GNUC__
// AppDbRecord

// string userId = 1;
inline void AppDbRecord::clear_userid() {
  userid_.ClearToEmptyNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
}
inline const std::string& AppDbRecord::userid() const {
  // @@protoc_insertion_point(field_get:AppDbRecord.userId)
  return userid_.GetNoArena();
}
inline void AppDbRecord::set_userid(const std::string& value) {
  
  userid_.SetNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), value);
  // @@protoc_insertion_point(field_set:AppDbRecord.userId)
}
inline void AppDbRecord::set_userid(std::string&& value) {
  
  userid_.SetNoArena(
    &::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), ::std::move(value));
  // @@protoc_insertion_point(field_set_rvalue:AppDbRecord.userId)
}
inline void AppDbRecord::set_userid(const char* value) {
  GOOGLE_DCHECK(value != nullptr);
  
  userid_.SetNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), ::std::string(value));
  // @@protoc_insertion_point(field_set_char:AppDbRecord.userId)
}
inline void AppDbRecord::set_userid(const char* value, size_t size) {
  
  userid_.SetNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(),
      ::std::string(reinterpret_cast<const char*>(value), size));
  // @@protoc_insertion_point(field_set_pointer:AppDbRecord.userId)
}
inline std::string* AppDbRecord::mutable_userid() {
  
  // @@protoc_insertion_point(field_mutable:AppDbRecord.userId)
  return userid_.MutableNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
}
inline std::string* AppDbRecord::release_userid() {
  // @@protoc_insertion_point(field_release:AppDbRecord.userId)
  
  return userid_.ReleaseNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
}
inline void AppDbRecord::set_allocated_userid(std::string* userid) {
  if (userid != nullptr) {
    
  } else {
    
  }
  userid_.SetAllocatedNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), userid);
  // @@protoc_insertion_point(field_set_allocated:AppDbRecord.userId)
}

// bytes publicKey = 2;
inline void AppDbRecord::clear_publickey() {
  publickey_.ClearToEmptyNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
}
inline const std::string& AppDbRecord::publickey() const {
  // @@protoc_insertion_point(field_get:AppDbRecord.publicKey)
  return publickey_.GetNoArena();
}
inline void AppDbRecord::set_publickey(const std::string& value) {
  
  publickey_.SetNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), value);
  // @@protoc_insertion_point(field_set:AppDbRecord.publicKey)
}
inline void AppDbRecord::set_publickey(std::string&& value) {
  
  publickey_.SetNoArena(
    &::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), ::std::move(value));
  // @@protoc_insertion_point(field_set_rvalue:AppDbRecord.publicKey)
}
inline void AppDbRecord::set_publickey(const char* value) {
  GOOGLE_DCHECK(value != nullptr);
  
  publickey_.SetNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), ::std::string(value));
  // @@protoc_insertion_point(field_set_char:AppDbRecord.publicKey)
}
inline void AppDbRecord::set_publickey(const void* value, size_t size) {
  
  publickey_.SetNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(),
      ::std::string(reinterpret_cast<const char*>(value), size));
  // @@protoc_insertion_point(field_set_pointer:AppDbRecord.publicKey)
}
inline std::string* AppDbRecord::mutable_publickey() {
  
  // @@protoc_insertion_point(field_mutable:AppDbRecord.publicKey)
  return publickey_.MutableNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
}
inline std::string* AppDbRecord::release_publickey() {
  // @@protoc_insertion_point(field_release:AppDbRecord.publicKey)
  
  return publickey_.ReleaseNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
}
inline void AppDbRecord::set_allocated_publickey(std::string* publickey) {
  if (publickey != nullptr) {
    
  } else {
    
  }
  publickey_.SetAllocatedNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), publickey);
  // @@protoc_insertion_point(field_set_allocated:AppDbRecord.publicKey)
}

// bytes privateKey = 3;
inline void AppDbRecord::clear_privatekey() {
  privatekey_.ClearToEmptyNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
}
inline const std::string& AppDbRecord::privatekey() const {
  // @@protoc_insertion_point(field_get:AppDbRecord.privateKey)
  return privatekey_.GetNoArena();
}
inline void AppDbRecord::set_privatekey(const std::string& value) {
  
  privatekey_.SetNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), value);
  // @@protoc_insertion_point(field_set:AppDbRecord.privateKey)
}
inline void AppDbRecord::set_privatekey(std::string&& value) {
  
  privatekey_.SetNoArena(
    &::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), ::std::move(value));
  // @@protoc_insertion_point(field_set_rvalue:AppDbRecord.privateKey)
}
inline void AppDbRecord::set_privatekey(const char* value) {
  GOOGLE_DCHECK(value != nullptr);
  
  privatekey_.SetNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), ::std::string(value));
  // @@protoc_insertion_point(field_set_char:AppDbRecord.privateKey)
}
inline void AppDbRecord::set_privatekey(const void* value, size_t size) {
  
  privatekey_.SetNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(),
      ::std::string(reinterpret_cast<const char*>(value), size));
  // @@protoc_insertion_point(field_set_pointer:AppDbRecord.privateKey)
}
inline std::string* AppDbRecord::mutable_privatekey() {
  
  // @@protoc_insertion_point(field_mutable:AppDbRecord.privateKey)
  return privatekey_.MutableNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
}
inline std::string* AppDbRecord::release_privatekey() {
  // @@protoc_insertion_point(field_release:AppDbRecord.privateKey)
  
  return privatekey_.ReleaseNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited());
}
inline void AppDbRecord::set_allocated_privatekey(std::string* privatekey) {
  if (privatekey != nullptr) {
    
  } else {
    
  }
  privatekey_.SetAllocatedNoArena(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), privatekey);
  // @@protoc_insertion_point(field_set_allocated:AppDbRecord.privateKey)
}

#ifdef __GNUC__
  #pragma GCC diagnostic pop
#endif  // __GNUC__

// @@protoc_insertion_point(namespace_scope)


// @@protoc_insertion_point(global_scope)

#include <google/protobuf/port_undef.inc>
#endif  // GOOGLE_PROTOBUF_INCLUDED_GOOGLE_PROTOBUF_INCLUDED_AppDbRecord_2eproto
