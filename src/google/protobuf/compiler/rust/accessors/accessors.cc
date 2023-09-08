// Protocol Buffers - Google's data interchange format
// Copyright 2023 Google LLC.  All rights reserved.
// https://developers.google.com/protocol-buffers/
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are
// met:
//
//     * Redistributions of source code must retain the above copyright
// notice, this list of conditions and the following disclaimer.
//     * Redistributions in binary form must reproduce the above
// copyright notice, this list of conditions and the following disclaimer
// in the documentation and/or other materials provided with the
// distribution.
//     * Neither the name of Google LLC. nor the names of its
// contributors may be used to endorse or promote products derived from
// this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
// OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#include "google/protobuf/compiler/rust/accessors/accessors.h"

#include <memory>

#include "google/protobuf/compiler/rust/accessors/accessor_generator.h"
#include "google/protobuf/compiler/rust/context.h"
#include "google/protobuf/descriptor.h"
#include "google/protobuf/descriptor.pb.h"

namespace google {
namespace protobuf {
namespace compiler {
namespace rust {

namespace {

std::unique_ptr<AccessorGenerator> AccessorGeneratorFor(
    const FieldDescriptor& desc) {
  // We do not support [ctype=FOO] (used to set the field type in C++ to
  // cord or string_piece) in V0 API.
  if (desc.options().has_ctype()) {
    return std::make_unique<UnsupportedField>();
  }

  if (desc.is_repeated()) {
    return std::make_unique<UnsupportedField>();
  }

  switch (desc.type()) {
    case FieldDescriptor::TYPE_INT32:
    case FieldDescriptor::TYPE_INT64:
    case FieldDescriptor::TYPE_FIXED32:
    case FieldDescriptor::TYPE_FIXED64:
    case FieldDescriptor::TYPE_SFIXED32:
    case FieldDescriptor::TYPE_SFIXED64:
    case FieldDescriptor::TYPE_SINT32:
    case FieldDescriptor::TYPE_SINT64:
    case FieldDescriptor::TYPE_UINT32:
    case FieldDescriptor::TYPE_UINT64:
    case FieldDescriptor::TYPE_FLOAT:
    case FieldDescriptor::TYPE_DOUBLE:
    case FieldDescriptor::TYPE_BOOL:
      return std::make_unique<SingularScalar>();
    case FieldDescriptor::TYPE_BYTES:
    case FieldDescriptor::TYPE_STRING:
      return std::make_unique<SingularString>();
    case FieldDescriptor::TYPE_MESSAGE:
      return std::make_unique<SingularMessage>();

    default:
      return std::make_unique<UnsupportedField>();
  }
}

}  // namespace

void GenerateAccessorMsgImpl(Context<FieldDescriptor> field) {
  AccessorGeneratorFor(field.desc())->GenerateMsgImpl(field);
}

void GenerateAccessorExternC(Context<FieldDescriptor> field) {
  AccessorGeneratorFor(field.desc())->GenerateExternC(field);
}

void GenerateAccessorThunkCc(Context<FieldDescriptor> field) {
  AccessorGeneratorFor(field.desc())->GenerateThunkCc(field);
}

}  // namespace rust
}  // namespace compiler
}  // namespace protobuf
}  // namespace google
