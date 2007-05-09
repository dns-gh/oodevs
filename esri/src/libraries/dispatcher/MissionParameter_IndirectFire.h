// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __MissionParameter_IndirectFire_h_
#define __MissionParameter_IndirectFire_h_

#include "MissionParameter_ABC.h"

namespace dispatcher
{

// =============================================================================
/** @class  MissionParameter_IndirectFire
    @brief  MissionParameter_IndirectFire
*/
// Created: NLD 2007-04-20
// =============================================================================
class MissionParameter_IndirectFire : public MissionParameter_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             MissionParameter_IndirectFire( const ASN1T_MissionParameter& asn );
    virtual ~MissionParameter_IndirectFire();
    //@}

    //! @name Operations
    //@{
    virtual void Send     ( ASN1T_MissionParameter& asn ) const;
    virtual void AsnDelete( ASN1T_MissionParameter& asn ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    MissionParameter_IndirectFire( const MissionParameter_IndirectFire& );            //!< Copy constructor
    MissionParameter_IndirectFire& operator=( const MissionParameter_IndirectFire& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    unsigned int fire_;
    //@}
};

}

#endif // __MissionParameter_IndirectFire_h_
