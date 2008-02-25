 // *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Position_h_
#define __Position_h_

#include "game_asn/Asn.h"

namespace dispatcher
{
    class Automat;
    class Model;

// =============================================================================
/** @class  Position
    @brief  Position
*/
// Created: NLD 2006-09-19
// =============================================================================
class Position
{
public:
    //! @name Constructors/Destructor
    //@{
     Position();
     Position( const ASN1T_CoordUTM& asn ); 
    ~Position();
    //@}

    //! @name Operations
    //@{
    void Update( const ASN1T_CoordUTM& asn );
    void Send  ( ASN1T_CoordUTM& asn ) const;
    //@}

public:
    std::string strPosition_;
};

}

#endif // __Position_h_
