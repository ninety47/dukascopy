#ifndef INCLUDE_NINETY47_DUKASCOPY_LOKI_H_
#define INCLUDE_NINETY47_DUKASCOPY_LOKI_H_

/*
Copyright 2013 Michael O'Keeffe (a.k.a. ninety47).

This file is part of ninety47 Dukascopy toolbox.

The "ninety47 Dukascopy toolbox" is free software: you can redistribute it
and/or modify it under the terms of the GNU General Public License as
published by the Free Software Foundation, either version 3 of the License,
or any later version.

"ninety47 Dukascopy toolbox" is distributed in the hope that it will be
useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General
Public License for more details.

You should have received a copy of the GNU General Public License along with
"ninety47 Dukascopy toolbox".  If not, see <http://www.gnu.org/licenses/>.
*/

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

}  // namespace loki
}  // namespace n47

#ifndef DISABLE_TYPELIST_MACROS

#define TYPELIST_1(T1) ::n47::loki::TypeList<T1, ::n47::loki::NullType>

#define TYPELIST_2(T1, T2) ::n47::loki::TypeList<T1, TYPELIST_1(T2) >

#define TYPELIST_3(T1, T2, T3) ::n47::loki::TypeList<T1, TYPELIST_2(T2, T3) >

#define TYPELIST_4(T1, T2, T3, T4) \
    ::n47::loki::TypeList<T1, TYPELIST_3(T2, T3, T4) >

#define TYPELIST_5(T1, T2, T3, T4, T5) \
    ::n47::loki::TypeList<T1, TYPELIST_4(T2, T3, T4, T5) >

#endif  // DISABLE_TYPELIST_MACROS

#endif  // INCLUDE_NINETY47_DUKASCOPY_LOKI_H_
