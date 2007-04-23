// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __MissionParameter_ObjectKnowledge_h_
#define __MissionParameter_ObjectKnowledge_h_

#include "MissionParameter_ABC.h"

namespace dispatcher
{

// =============================================================================
/** @class  MissionParameter_ObjectKnowledge
    @brief  MissionParameter_ObjectKnowledge
*/
// Created: NLD 2007-04-20
// =============================================================================
class MissionParameter_ObjectKnowledge : public MissionParameter_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             MissionParameter_ObjectKnowledge( const ASN1T_MissionParameter& asn );
    virtual ~MissionParameter_ObjectKnowledge();
    //@}

    //! @name Operations
    //@{
    virtual void Send     ( ASN1T_MissionParameter& asn ) const;
    virtual void AsnDelete( ASN1T_MissionParameter& asn ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    MissionParameter_ObjectKnowledge( const MissionParameter_ObjectKnowledge& );            //!< Copy constructor
    MissionParameter_ObjectKnowledge& operator=( const MissionParameter_ObjectKnowledge& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    unsigned int objectKnowledge_;
    //@}
};

}

#endif // __MissionParameter_ObjectKnowledge_h_
