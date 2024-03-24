#pragma once

#include <Eigen/Dense>
#include <memory>
#include "Mesh.h"
namespace renderer {

// Object for potential polymorphic use cases
class AnyObject {
public:
    class InnerBase;
    AnyObject() = default;

    AnyObject(const AnyObject &other) : inner_((other.inner_ ? other.inner_->Clone() : nullptr)) {
    }

    template <typename T>
    AnyObject(T &&object) : inner_(new Inner<std::remove_reference_t<T>>(std::forward<T>(object))) {
    }
    template <typename T>
    AnyObject(const T &object)
        : inner_(std::make_unique<Inner<std::remove_reference_t<T>>>(object)) {
    }
    AnyObject(AnyObject &&) noexcept = default;
    AnyObject &operator=(AnyObject &&) noexcept = default;

    AnyObject &operator=(const AnyObject &val) {
        return *this = AnyObject(val);
    }

    const InnerBase *operator->() const {
        return inner_.get();
    }
    InnerBase *operator->() {
        return inner_.get();
    }

    class InnerBase {
    public:
        virtual ~InnerBase() = default;

        virtual const Mesh &GetMesh() const = 0;
        friend class AnyObject;

    private:
        virtual std::unique_ptr<InnerBase> Clone() const = 0;
    };

private:
    template <typename T>
    class Inner : public InnerBase {
    public:
        /*Inner(const T &value) : value_(value) {
        }*/
        Inner(T &&value) : value_(std::forward<T>(value)) {
        }
        Inner(const T &value) : value_(value) {
        }
        const Mesh &GetMesh() const override {
            return value_.GetMesh();
        }

    private:
        std::unique_ptr<InnerBase> Clone() const override {
            return std::make_unique<Inner>(value_);
        }
        T value_;
    };

    std::unique_ptr<InnerBase> inner_;
};
}  // namespace renderer
