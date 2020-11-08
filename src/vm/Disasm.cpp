#include "vm/Disasm.h"

Disasm::Disasm() = default;

Disasm::Disasm(const DisasmOptions & options) : Disasm() {
    this->options = options;
}

void Disasm::eval(const Chunk & chunk) {
    this->chunk = chunk;

    std::cout << "=== Chunk ===" << std::endl;

    if (options.pure_dump) {
        std::cout << "-- Pure code --" << std::endl;
        int div = 0;
        for (const auto & byte : chunk.code) {
            std::cout << std::hex << static_cast<int>(byte) << " ";
            div++;
            if (div == 4) {
                div = 0;
                std::cout << std::endl;
            }
        }
        if (div != 0) {
            std::cout << std::endl;
        }
    }

    std::cout << "-- Constant Pool --" << std::endl;
    for (const auto & constant : chunk.constants) {
        // Note: Now only type((
        std::cout << static_cast<int>(constant->type) << " - " << constant->to_string() << std::endl;
    }

    std::cout << "-- Code --" << std::endl;
    while (ip < chunk.code.size()) {
        const auto & opcode = static_cast<OpCode>(read());
        std::cout << opcode_names.at(opcode) << " ";
        switch (opcode) {
            case OpCode::NOP: break;
            case OpCode::Pop: {
                pop();
            } break;
            case OpCode::NullConst: {
                push(Null);
            } break;
            case OpCode::FalseConst: {
                push(False);
            } break;
            case OpCode::TrueConst: {
                push(True);
            } break;
            case OpCode::IntConst: {
                const auto & int_const = read_int_const();
                push(std::make_shared<Int>(int_const));
                std::cout << top()->to_string();
            } break;
            case OpCode::FloatConst: {
                const auto & float_const = read_float_const();
                push(std::make_shared<Float>(float_const));
                std::cout << float_const->value;
            } break;
            case OpCode::StringConst: {
                const auto & string_const = read_string_const();
                push(std::make_shared<String>(string_const));
                std::cout << string_const->value;
            } break;
            case OpCode::DefineGlobal: {
                const auto & global_name = read_string_const();
                std::cout << global_name->value;
                globals[global_name->value] = pop();
                pop();
            } break;
            case OpCode::LoadGlobal: {
                const auto & global_name = read_string_const();
                std::cout << global_name->value;
                const auto & found = globals.find(global_name->value);
                if (found == globals.end()) {
                    std::cout << " (UNDEFINED)";
                } else {
                    push(found->second);
                    std::cout << " (" << found->second->to_string() << ")";
                }
            } break;
            case OpCode::StoreGlobal: {
                const auto & global_name = read_string_const();
                std::cout << global_name->value;
                const auto & found = globals.find(global_name->value);
                if (found == globals.end()) {
                    std::cout << " (UNDEFINED)";
                } else {
                    std::cout << " " << top()->to_string();
                }
            } break;
            case OpCode::LoadLocal: {
                const auto & slot = read8();
                std::cout << slot;
                // TODO: Use real values
                push(Null);
            } break;
            case OpCode::StoreLocal: {
                const auto & slot = read8();
                std::cout << slot;
                std::cout << " ";
                std::cout << top()->to_string();
            } break;
            case OpCode::Jump: {
                const auto & offset = read8();
                std::cout << offset;
            } break;
            case OpCode::JumpFalse: {
                const auto & offset = read8();
                std::cout << offset;
                std::cout << top()->to_string() << " - maybe false:)";
            } break;
            case OpCode::Invoke:
            case OpCode::InvokeNF: {
                uint64_t arg_count = read8();
                value_ptr func = top(arg_count);
                std::cout << func->to_string() << "(";
                for (uint64_t i = 0; i < arg_count; i++) {
                    std::cout << top(arg_count - i - 1)->to_string();
                }
                std::cout << ")";
            } break;
            case OpCode::GetProperty: {
                std::cout << top()->to_string() << "." << read_string_const()->value;
            } break;
            case OpCode::SetProperty: {
                std::cout << top(1)->to_string() << "." << read_string_const()->value << " = " << top()->to_string();
            } break;
            default: break;
        }
        std::cout << std::endl;
    }
}
