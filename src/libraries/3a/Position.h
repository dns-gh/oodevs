// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Position_h_
#define __Position_h_

#include "game_asn/asn.h"
#include <string>

// =============================================================================
/** @class  Position
    @brief  Position
*/
// Created: AGE 2007-09-12
// =============================================================================
class Position
{

public:
    //! @name Constructors/Destructor
    //@{     
             Position();
    explicit Position( const ASN1T_CoordUTM& coord );
    virtual ~Position();
    //@}

    //! @name Operations
    //@{
    float Distance( const Position& rhs ) const;
    bool operator<( const Position& rhs ) const;
    bool operator>( const Position& rhs ) const;
    bool operator==( const Position& rhs ) const;
    bool operator!=( const Position& rhs ) const;
    //@}

    //! @name Compilation compatibility
    //@{
    template< typename T >
    Position& operator+=( const T& ) { return *this; }
    template< typename T >
    Position operator+( const T& ) { return *this; }
    explicit Position( unsigned int ) {};
    template< typename T >
    Position operator/( const T& ) { return *this; }
    //@}

private:
    //! @name Member data
    //@{
    std::string mgrs_;
    //@}
};

#endif // __Position_h_
