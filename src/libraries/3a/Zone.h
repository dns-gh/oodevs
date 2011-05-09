// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Zone_h_
#define __Zone_h_

#include "Zone_ABC.h"
#include <string>
#include <sstream>
#include <boost/shared_ptr.hpp>

// =============================================================================
/** @class  Zone
    @brief  Zone
*/
// Created: AGE 2007-10-09
// =============================================================================
class Zone
{
public:
    //! @name Constructors/Destructor
    //@{
             Zone();
    virtual ~Zone();
    //@}

    //! @name Operations
    //@{
    bool Contains( const Position& position ) const
    {
        return zone_->Contains( position );
    }
    //@}

    //! @name Compilation compatibility
    //@{
    operator double() const;

    Zone( double );
    Zone& operator+=( const Zone& ) { return *this; }
    Zone  operator/( const Zone& ) const { return *this; }
    Zone  operator-( const Zone& ) const { return *this; }
    //@}

private:
    //! @name Operations
    //@{
    void Read( const std::string& );
    friend std::istream& operator>>( std::istream&, Zone& zone );
    //@}

private:
    //! @name Member data
    //@{
    boost::shared_ptr< Zone_ABC > zone_;
    //@}
};

std::istream& operator>>( std::istream&, Zone& zone );

#endif // __Zone_h_
