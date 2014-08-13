// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#ifndef __Translations_h_
#define __Translations_h_

#include <boost/shared_ptr.hpp>
#include <vector>
#include <unordered_map>

namespace kernel
{
    class LocalizedString;

// =============================================================================
/** @class  Context
    @brief  Context
*/
// Created: ABR 2013-07-15
// =============================================================================
class Context : public std::unordered_multimap< std::string, boost::shared_ptr< LocalizedString > >
{
public:
    //! @name Constructors/Destructor
    //@{
             Context();
    virtual ~Context();
    //@}

    //! @name Operations
    //@{
    bool Apply( const std::function< bool( LocalizedString& ) >& functor );

    boost::shared_ptr< LocalizedString > operator[]( const std::string& key );
    const boost::shared_ptr< LocalizedString >& operator[]( const std::string& key ) const;

    boost::shared_ptr< LocalizedString > CreateNew( const std::string& key );
    //@}
};

} //! Namespace kernel

#endif // __Translations_h_
