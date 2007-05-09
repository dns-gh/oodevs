// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __MissionParameter_Enumeration_h_
#define __MissionParameter_Enumeration_h_

#include "MissionParameter_ABC.h"

namespace dispatcher
{

// =============================================================================
/** @class  MissionParameter_Enumeration
    @brief  MissionParameter_Enumeration
*/
// Created: NLD 2007-04-20
// =============================================================================
class MissionParameter_Enumeration : public MissionParameter_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             MissionParameter_Enumeration( const ASN1T_MissionParameter& asn );
    virtual ~MissionParameter_Enumeration();
    //@}

    //! @name Operations
    //@{
    virtual void Send     ( ASN1T_MissionParameter& asn ) const;
    virtual void AsnDelete( ASN1T_MissionParameter& asn ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    MissionParameter_Enumeration( const MissionParameter_Enumeration& );            //!< Copy constructor
    MissionParameter_Enumeration& operator=( const MissionParameter_Enumeration& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    unsigned int enumeration_;
    //@}
};

}

#endif // __MissionParameter_Enumeration_h_
