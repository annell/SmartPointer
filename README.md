# SmartPointer
A simple smart pointer library implementing:
- `smartPtr::Auto<T>`, that does a deep copy of the underlying pointer.
- `smartPtr::Shared<T>`, where the underlying pointer is shared when copied.
- `smartPtr::Unique<T>`, that prevents copying of the underlying resource.

# Usage
Create using the `smartPtr::Make...`
```c++
auto sp1 = smartPtr::MakeAuto<int>(5);
auto sp2 = smartPtr::MakeShared<int>(5);
auto sp3 = smartPtr::MakeUnique<int>(5);
```

Usage like a regular smart pointer
```c++
auto sp1 = smartPtr::MakeAuto<T>(5);
T val = *sp1;
T* val = sp1.get();

auto func = [] (const T& val) {
    ...
};

func(*sp1);

sp1->TFunc();
```
Memory is managed by pointer and is deleted depending on style of pointer.
```c++
{
    auto sp1 = smartPtr::MakeAuto<int>(5);
    auto sp2 = sp1; // Memory is deep copied to sp2 from 1.
}
{
    auto sp1 = smartPtr::MakeShared<int>(5);
    auto sp2 = sp1; // Memory is shared between sp1 and sp2 and released when all shared references are removed.
}
{
    auto sp1 = smartPtr::MakeUnique<int>(5);
    auto sp2 = sp1; // ERROR! will give you a compile error, cannot copy a unique ptr.
    auto sp3 = std::move(sp1) // Ok, need to move ownership.
}
```