#ifndef _N47_LOKILIKE_HEADER_INCLUDED_120knjvcaasd823mn_
#define _N47_LOKILIKE_HEADER_INCLUDED_120knjvcaasd823mn_


namespace n47 {
    namespace loki {

    struct NullType {};

    template <typename H, class TL>
    struct SO {
        static const int size = sizeof(H) + TL::size;
    };

    template <typename H>
    struct SO<H, NullType> {
        static const int size = sizeof(H);
    };

    #define SIZEOF1(T1) SO<T1, NullType>::size

    #define SIZEOF2(T1, T2) SO<T1, SO<T2, NullType> >::size

    #define SIZEOF3(T1, T2, T3) SO<T1, SO<T2, SO<T3, NullType> > >::size

    #define SIZEOF4(T1, T2, T3, T4) SO<T1, SO<T2, SO<T3, SO<T4, NullType> > > >::size

    #define SIZEOF5(T1, T2, T3, T4, T5) SO<T1, SO<T2, SO<T3, SO<T4, SO<T5, NullType> > > > >::size

    } // namespace loki
} // namesapce n47



#endif // _N47_LOKILIKE_HEADER_INCLUDED_120knjvcaasd823mn_
