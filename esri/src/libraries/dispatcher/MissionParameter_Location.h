// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __MissionParameter_Location_h_
#define __MissionParameter_Location_h_

#include "MissionParameter_ABC.h"
#include "Localisation.h"

namespace dispatcher
{

// =============================================================================
/** @class  MissionParameter_Location
    @brief  MissionParameter_Location
*/
// Created: NLD 2007-04-20
// =============================================================================
class MissionParameter_Location : public MissionParameter_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             MissionParameter_Location( const ASN1T_MissionParameter& asn );
    virtual ~MissionParameter_Location();
    //@}

    //! @name Operations
    //@{
    virtual void Send     ( ASN1T_MissionParameter& asn ) const;
    virtual void AsnDelete( ASN1T_MissionParameter& asn ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    MissionParameter_Location( const MissionParameter_Location& );            //!< Copy constructor
    MissionParameter_Location& operator=( const MissionParameter_Location& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    Localisation location_;
    //@}
};

}

#endif // __MissionParameter_Location_h_
