/*=============================================================================
    Copyright (c) 2001-2006 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying 
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#if !defined(FUSION_ERASE_KEY_10022005_1851)
#define FUSION_ERASE_KEY_10022005_1851

#include <boost/fusion/algorithm/query/detail/assoc_find.hpp>
#include <boost/fusion/algorithm/transformation/erase.hpp>
#include <boost/mpl/not.hpp>
#include <boost/type_traits/is_same.hpp>

namespace boost { namespace fusion
{
    namespace result_of
    {
        template <typename Sequence, typename Key>
        struct erase_key
        {
            typedef detail::assoc_find<Sequence, Key> filter;
            typedef typename erase<Sequence, typename filter::type>::type type;
        };
    }

    template <typename Key, typename Sequence>
    inline typename result_of::erase_key<Sequence const, Key>::type
    erase_key(Sequence const& seq)
    {
        typedef typename result_of::erase_key<Sequence const, Key>::filter filter;
        return erase(seq, filter::call(seq));
    }
}}

#endif

