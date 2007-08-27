 // *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Localisation_h_
#define __Localisation_h_

#include "game_asn/Asn.h"
#include "Position.h"

namespace dispatcher
{
    class Automat;
    class Model;

// =============================================================================
/** @class  Localisation
    @brief  Localisation
*/
// Created: NLD 2006-09-19
// =============================================================================
class Localisation
{
public:
    //! @name Constructors/Destructor
    //@{
     Localisation( const ASN1T_Location& asn ); 
     Localisation(); 
    ~Localisation();
    //@}

    //! @name Main
    //@{
    void Update( const ASN1T_Location& asn );
    void Send  ( ASN1T_Location& asn ) const;

    static void AsnDelete( ASN1T_Location& asn, bool bOptionalValue = true );
    //@}

private:
    //! @name Types
    //@{
    typedef std::vector< Position > T_PositionVector;
    //@}

private:
    ASN1T_EnumLocationType nType_;
    T_PositionVector       points_;
};

}

#endif // __Localisation_h_
