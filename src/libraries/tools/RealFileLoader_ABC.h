// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef RealFileLoader_ABC_h
#define RealFileLoader_ABC_h

#include <string>
#include <memory>
#include <boost/noncopyable.hpp>

namespace xml
{
    class xistream;
}

namespace tools
{
    class RealFileLoaderObserver_ABC;

// =============================================================================
/** @class  RealFileLoader_ABC
    @brief  RealFileLoader_ABC
*/
// Created: NLD 2011-02-14
// =============================================================================
class RealFileLoader_ABC : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             RealFileLoader_ABC() {}
    virtual ~RealFileLoader_ABC() {}
    //@}
    
    //! @name Operations
    //@{
    virtual std::auto_ptr< xml::xistream > LoadFile( const std::string& file, RealFileLoaderObserver_ABC& observer ) const = 0;
    //@}
};

}

#endif // RealFileLoader_ABC_h
