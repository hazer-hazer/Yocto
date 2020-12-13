#include "vm/VM.h"

namespace jc::vm {
    VM::VM() : log("VM", options.log) {
        call_frames.push_back({});
        frame = call_frames.begin();
    }

    void VM::_nop() {}

    void VM::_pop() {
        pop();
    }

    void VM::_null_const() {
        push(Null);
    }

    void VM::_false_const() {
        push(False);
    }

    void VM::_true_const() {
        push(True);
    }

    void VM::_int_const() {
        const auto & int_const = read_int_const();
        push(std::make_shared<IntObject>(int_const));
    }

    void VM::_float_const() {
        const auto & float_const = read_float_const();
        push(std::make_shared<FloatObject>(float_const));
    }

    void VM::_string_const() {
        const auto & string_const = read_string_const();
        push(std::make_shared<StringObject>(string_const));
    }

    void VM::_define_global() {
        const auto & global_name = read_string_const();
        globals.at(global_name->value) = nullptr;
    }

    void VM::_load_global() {
        const auto & global_name = read_string_const();
        try {
            push(globals.at(global_name->value));
        } catch (std::out_of_range & e) {
            error(global_name->value + " is not defined");
        }
    }

    void VM::_store_global() {
        const auto & global_name = read_string_const();
        try {
            globals.at(global_name->value) = top();
        } catch (std::out_of_range & e) {
            error(global_name->value + " is not defined");
        }
    }

    void VM::_load_local() {
        const auto & slot = read8();
        try {
            push(frame->slots.at(slot));
        } catch (std::out_of_range & e) {
            // TODO: Update when locals names will be done
            error("Unable to resolve local");
        }
    }

    void VM::_store_local() {
        const auto & slot = read8();
        try {
            frame->slots.at(slot) = top();
        } catch (std::out_of_range & e) {
            error("Unable to resolve local");
        }
    }

    void VM::_jump() {
        const auto & offset = read8();
        ip += offset;
    }

    void VM::_jump_false() {
        const auto & offset = read8();
        if (!top()->to_b()) {
            ip += offset;
        }
    }

    void VM::_invoke() {

    }

    void VM::_invoke_nf() {
        uint32_t arg_count = read8();
        const auto & func = std::static_pointer_cast<NativeFunc>(top(arg_count));
        const auto & args = read_args(arg_count);
        auto value = func->body(args);
        // TODO: !!! Create `void`, DO NOT USE NULL
        //  Void is not about run-time, so it must be caught on compile-time and push nothing on run-time
        value = value ? value : Null;
        push(value);
    }

    void VM::_invoke_method() {
        // TODO
    }

    void VM::_invoke_nf_method() {
        uint32_t arg_count = read8();
        const auto & method_name = read_string_const()->value;
        const auto & object = top(arg_count);
        const auto & args = read_args(arg_count);
        const auto & method = std::static_pointer_cast<NativeFunc>(object->_class->methods.at(method_name));
        auto value = method->body(args);
        // TODO: Look above (about void)
        value = value ? value : Null;
        push(value);
    }

    void VM::_get_property() {
        // TODO: Rewrite for functions
        const auto & object = top();
        const auto & prop_name = read_string_const();
        pop();
        push(object->fields.at(prop_name->value));
    }

    void VM::_set_property() {
        // TODO
    }

    ////////////
    // Errors //
    ////////////
    void VM::error(const std::string & msg) {
        // TODO: Current file
        // TODO: Position tracking
        throw RuntimeError(msg, {});
    }
}
