// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __MissionParameter_UrbanBlock_h_
#define __MissionParameter_Agent_h_

#include "MissionParameter_ABC.h"

namespace dispatcher
{

// =============================================================================
/** @class  MissionParameter_UrbanBlock
    @brief  MissionParameter_UrbanBlock
*/
// Created: SLG 2009-11-06
// =============================================================================
class MissionParameter_UrbanBlock : public MissionParameter_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
    MissionParameter_UrbanBlock( const ASN1T_MissionParameter& asn );
    virtual ~MissionParameter_UrbanBlock();
    //@}

    //! @name Operations
    //@{
    virtual void Send     ( ASN1T_MissionParameter& asn ) const;
    virtual void AsnDelete( ASN1T_MissionParameter& asn ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    MissionParameter_UrbanBlock( const MissionParameter_UrbanBlock& );            //!< Copy constructor
    MissionParameter_UrbanBlock& operator=( const MissionParameter_UrbanBlock& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    unsigned int block_;
    //@}
};

}

#endif // __MissionParameter_Agent_h_
