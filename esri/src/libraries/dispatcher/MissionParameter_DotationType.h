// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __MissionParameter_DotationType_h_
#define __MissionParameter_DotationType_h_

#include "MissionParameter_ABC.h"

namespace dispatcher
{

// =============================================================================
/** @class  MissionParameter_DotationType
    @brief  MissionParameter_DotationType
*/
// Created: NLD 2007-04-20
// =============================================================================
class MissionParameter_DotationType : public MissionParameter_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             MissionParameter_DotationType( const ASN1T_MissionParameter& asn );
    virtual ~MissionParameter_DotationType();
    //@}

    //! @name Operations
    //@{
    virtual void Send     ( ASN1T_MissionParameter& asn ) const;
    virtual void AsnDelete( ASN1T_MissionParameter& asn ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    MissionParameter_DotationType( const MissionParameter_DotationType& );            //!< Copy constructor
    MissionParameter_DotationType& operator=( const MissionParameter_DotationType& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    unsigned int dotationType_;
    //@}
};

}

#endif // __MissionParameter_DotationType_h_
