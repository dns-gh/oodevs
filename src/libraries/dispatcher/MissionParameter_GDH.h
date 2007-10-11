// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __MissionParameter_GDH_h_
#define __MissionParameter_GDH_h_

#include "MissionParameter_ABC.h"
#include "game_asn/Asn.h"

namespace dispatcher
{

// =============================================================================
/** @class  MissionParameter_GDH
    @brief  MissionParameter_GDH
*/
// Created: NLD 2007-04-20
// =============================================================================
class MissionParameter_GDH : public MissionParameter_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             MissionParameter_GDH( const ASN1T_MissionParameter& asn );
    virtual ~MissionParameter_GDH();
    //@}

    //! @name Operations
    //@{
    virtual void Send     ( ASN1T_MissionParameter& asn ) const;
    virtual void AsnDelete( ASN1T_MissionParameter& asn ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    MissionParameter_GDH( const MissionParameter_GDH& );            //!< Copy constructor
    MissionParameter_GDH& operator=( const MissionParameter_GDH& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    ASN1T_DateTime datation_;
    //@}
};

}

#endif // __MissionParameter_GDH_h_
