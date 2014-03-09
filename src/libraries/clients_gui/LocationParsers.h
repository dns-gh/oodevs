// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __LocationParsers_h_
#define __LocationParsers_h_

#include <boost/noncopyable.hpp>

namespace kernel
{
    class Controllers;
    class CoordinateConverter_ABC;
}

namespace gui
{
    class LocationParser_ABC;

// =============================================================================
/** @class  LocationParsers
    @brief  Location parsers
*/
// Created: AME 2010-03-10
// =============================================================================
class LocationParsers : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             LocationParsers( kernel::Controllers& controllers, const kernel::CoordinateConverter_ABC& converter );
    virtual ~LocationParsers();
    //@}

    //! @name Operations
    //@{
    const std::shared_ptr< const LocationParser_ABC >& GetParser( int parserId ) const;
    void AddParser( const std::shared_ptr< const LocationParser_ABC >& parser, int id );
    //@}

private:
    //! @name Member data
    //@{
    std::map< int, std::shared_ptr< const LocationParser_ABC > > parsers_;
    //@}
};

}
#endif // __LocationParsers_h_
