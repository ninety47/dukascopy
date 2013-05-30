#ifndef _N47_LOKI_HEADER_INCLUDED_
#define _N47_LOKI_HEADER_INCLUDED_


namespace n47 {
    namespace loki {

    struct NullType {};

    template <class T, class U>
    struct TypeList {
        typedef T Head;
        typedef U Tail;
    };

    template <class TList> struct SizeOf;

    template <> struct SizeOf<NullType> {
        enum { value = 0 };
    };

    template <class T, class U>
    struct SizeOf< TypeList<T, U> > {
        enum { value = sizeof(T) + SizeOf<U>::value };
    };

    } // namespace loki
} // namesapce n47

#ifndef DISABLE_TYPELIST_MACROS

#define TYPELIST_1(T1) ::n47::loki::TypeList<T1, ::n47::loki::NullType>

#define TYPELIST_2(T1, T2) ::n47::loki::TypeList<T1, TYPELIST_1(T2) >

#define TYPELIST_3(T1, T2, T3) ::n47::loki::TypeList<T1, TYPELIST_2(T2, T3) >

#define TYPELIST_4(T1, T2, T3, T4) \
    ::n47::loki::TypeList<T1, TYPELIST_3(T2, T3, T4) >

#define TYPELIST_5(T1, T2, T3, T4, T5) \
    ::n47::loki::TypeList<T1, TYPELIST_4(T2, T3, T4, T5) >

#endif // DISABLE_TYPELIST_MACROS

#endif // _N47_LOKI_HEADER_INCLUDED_
