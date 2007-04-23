// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __MissionParameter_Automat_h_
#define __MissionParameter_Automat_h_

#include "MissionParameter_ABC.h"

namespace dispatcher
{

// =============================================================================
/** @class  MissionParameter_Automat
    @brief  MissionParameter_Automat
*/
// Created: NLD 2007-04-20
// =============================================================================
class MissionParameter_Automat : public MissionParameter_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             MissionParameter_Automat( const ASN1T_MissionParameter& asn );
    virtual ~MissionParameter_Automat();
    //@}

    //! @name Operations
    //@{
    virtual void Send     ( ASN1T_MissionParameter& asn ) const;
    virtual void AsnDelete( ASN1T_MissionParameter& asn ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    MissionParameter_Automat( const MissionParameter_Automat& );            //!< Copy constructor
    MissionParameter_Automat& operator=( const MissionParameter_Automat& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    unsigned int automat_;
    //@}
};

}

#endif // __MissionParameter_Automat_h_
