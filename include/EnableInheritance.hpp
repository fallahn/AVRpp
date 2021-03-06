//enables inheritance with the Atmel GCC toolchain

//see http://jaxcoder.com/Projects.aspx?id=788781549

#ifndef ENABLEINHERITANCE_H_
#define ENABLEINHERITANCE_H_

/*
    This is applicable if using virtual inheritance.
*/
__extension__ typedef int __guard __attribute__((mode (__DI__)));
 
extern "C" int __cxa_guard_acquire(__guard *);
extern "C" void __cxa_guard_release (__guard *);
extern "C" void __cxa_guard_abort (__guard *);
 
/*
    This is applicable if using pure virtual inheritance.
*/
extern "C" void __cxa_pure_virtual(void);

/*
    Operators required for ctors / dtors when using inheritance
*/
void* operator new(unsigned size);
void operator delete(void* size);


#endif /* ENABLEINHERITANCE_H_ */