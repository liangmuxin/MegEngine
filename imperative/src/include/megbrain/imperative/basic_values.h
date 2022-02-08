/**
 * \file imperative/src/include/megbrain/imperative/basic_values.h
 * MegEngine is Licensed under the Apache License, Version 2.0 (the "License")
 *
 * Copyright (c) 2014-2021 Megvii Inc. All rights reserved.
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT ARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 */

#pragma once

#include <future>
#include <iomanip>

#include "megbrain/imperative/utils/helper.h"
#include "megbrain/imperative/utils/value_shape.h"
#include "megbrain/imperative/value.h"

namespace mgb {
namespace imperative {

class GradKey;

using GenericFunction = std::function<ValueRefList(Span<ValueRef>)>;

class ShapeValue final
        : public MixinValueImpl<ShapeValue, ValueKind::Primitive, ValueShape> {
public:
    using MixinValueImpl::MixinValueImpl;

    std::string to_string() const override;
};

class CompNodeValue final
        : public MixinValueImpl<CompNodeValue, ValueKind::Primitive, CompNode> {
public:
    using MixinValueImpl::MixinValueImpl;

    std::string to_string() const override;
};

// TODO: override factory method
class BoolValue final : public ValueImpl<BoolValue, ValueKind::Primitive> {
private:
    std::optional<bool> m_value;

public:
    BoolValue(bool value) : m_value{value} {}
    operator bool() const { return *m_value; }

    std::string to_string() const override;

    void clear() override { m_value.reset(); }
};

class HostStorage final
        : public MixinValueImpl<HostStorage, ValueKind::Primitive, HostTensorStorage> {
public:
    using MixinValueImpl::MixinValueImpl;

    std::string to_string() const override;
};

class DeviceStorage final
        : public MixinValueImpl<
                  DeviceStorage, ValueKind::Primitive, DeviceTensorStorage> {
public:
    using MixinValueImpl::MixinValueImpl;

    std::string to_string() const override;
};

/**
 * \brief like HostTensorND mixin, but allow scalar value
 *
 */
class HostValue final : public ValueImpl<HostValue, ValueKind::Primitive> {
private:
    DType m_dtype;
    ValueShape m_shape;
    HostTensorStorage m_storage;

public:
    HostValue(DType dtype, ValueShape shape, HostTensorStorage storage)
            : m_dtype(dtype), m_shape(shape), m_storage(storage) {}
    HostValue(HostTensorND value)
            : HostValue(
                      value.dtype(), ValueShape::from(value.shape()), value.storage()) {
    }

    std::string to_string() const override;

    void clear() override {
        m_dtype = {};
        m_shape = {};
        m_storage = {};
    }

    DType dtype() const { return m_dtype; }
    const ValueShape& shape() const { return m_shape; }
    CompNode device() const { return m_storage.comp_node(); }
    const HostTensorStorage& storage() const { return m_storage; }
    DTypeScalar item() const {
        // FIXME: check scalar
        mgb_assert(m_shape.total_nr_elems());
        return DTypeScalar::make_from_raw(m_dtype, m_storage.ptr());
    }

    HostTensorND as_nd(bool allow_scalar = false) const;
};

/**
 * \brief like DeviceTensorND mixin, but allow scalar value
 *
 */
class DeviceValue final : public ValueImpl<DeviceValue, ValueKind::Primitive> {
private:
    DType m_dtype;
    ValueShape m_shape;
    DeviceTensorStorage m_storage;

public:
    DeviceValue(DType dtype, ValueShape shape, DeviceTensorStorage storage)
            : m_dtype(dtype), m_shape(shape), m_storage(std::move(storage)) {}
    DeviceValue(const DeviceTensorND& value)
            : DeviceValue(
                      value.dtype(), ValueShape::from(value.shape()), value.storage()) {
    }

    std::string to_string() const override;

    void clear() override {
        m_dtype = {};
        m_shape = {};
        m_storage = {};
    }

    DType dtype() const { return m_dtype; }
    const ValueShape& shape() const { return m_shape; }
    CompNode device() const { return m_storage.comp_node(); }
    const DeviceTensorStorage& storage() const { return m_storage; }

    DeviceTensorND as_nd(bool allow_scalar = false) const;
};

class FunctionValue final
        : public MixinValueImpl<FunctionValue, ValueKind::Primitive, GenericFunction> {
public:
    using MixinValueImpl::MixinValueImpl;

    std::string to_string() const override;
};

class DTypeValue final
        : public MixinValueImpl<DTypeValue, ValueKind::Primitive, DType> {
public:
    using MixinValueImpl::MixinValueImpl;

    std::string to_string() const override;
};

class StringValue final
        : public MixinValueImpl<StringValue, ValueKind::Primitive, std::string> {
public:
    using MixinValueImpl::MixinValueImpl;

    std::string to_string() const override;
};

class Error {
protected:
    std::string m_message;

public:
    Error() = default;
    Error(std::string message) : m_message(message) {}

    std::string message() const { return m_message; }
};

class ErrorValue final
        : public MixinValueImpl<ErrorValue, ValueKind::Primitive, Error> {
public:
    using MixinValueImpl::MixinValueImpl;

    std::string to_string() const override;
};

}  // namespace imperative
}  // namespace mgb
