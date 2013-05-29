#ifndef _N47_LOKILIKE_HEADER_INCLUDED_120knjvcaasd823mn_
#define _N47_LOKILIKE_HEADER_INCLUDED_120knjvcaasd823mn_


namespace n47 {
    namespace loki {

    struct NullType {};

    template <typename H, class TL>
    struct TypeList {
        static const int size = sizeof(H) + TL::size;
    };

    template <typename H>
    struct TypeList <H, NullType> {
        static const size_t size = sizeof(H);
    };

    #define SIZEOF1(T1) <T1, NullType>::size

    #define SIZEOF2(T1, T2) TypeList<T1, TypeList<T2, NullType> >::size

    #define SIZEOF3(T1, T2, T3) TypeList<T1, TypeList<T2, TypeList<T3, NullType> > >::size

    #define SIZEOF4(T1, T2, T3, T4) TypeList<T1, TypeList<T2, TypeList<T3, TypeList<T4, NullType> > > >::size

    #define SIZEOF5(T1, T2, T3, T4, T5) TypeList<T1, TypeList<T2, TypeList<T3, TypeList<T4, TypeList<T5, NullType> > > > >::size

    } // namespace loki
} // namesapce n47



#endif // _N47_LOKILIKE_HEADER_INCLUDED_120knjvcaasd823mn_
