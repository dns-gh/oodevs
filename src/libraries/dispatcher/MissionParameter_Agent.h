// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __MissionParameter_Agent_h_
#define __MissionParameter_Agent_h_

#include "MissionParameter_ABC.h"

namespace dispatcher
{

// =============================================================================
/** @class  MissionParameter_Agent
    @brief  MissionParameter_Agent
*/
// Created: NLD 2007-04-20
// =============================================================================
class MissionParameter_Agent : public MissionParameter_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             MissionParameter_Agent( const ASN1T_MissionParameter& asn );
    virtual ~MissionParameter_Agent();
    //@}

    //! @name Operations
    //@{
    virtual void Send     ( ASN1T_MissionParameter& asn ) const;
    virtual void AsnDelete( ASN1T_MissionParameter& asn ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    MissionParameter_Agent( const MissionParameter_Agent& );            //!< Copy constructor
    MissionParameter_Agent& operator=( const MissionParameter_Agent& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    unsigned int agent_;
    //@}
};

}

#endif // __MissionParameter_Agent_h_
