#pragma once

class exp {
public:
    virtual const char* what() const noexcept = 0;
}; 

class out_of_range : public exp {
public:
    out_of_range() = delete;
    out_of_range(const char* msg) {}
    const char* what() const noexcept override {
        return "out of range";
    }
};