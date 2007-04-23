// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __MissionParameter_PopulationKnowledge_h_
#define __MissionParameter_PopulationKnowledge_h_

#include "MissionParameter_ABC.h"

namespace dispatcher
{

// =============================================================================
/** @class  MissionParameter_PopulationKnowledge
    @brief  MissionParameter_PopulationKnowledge
*/
// Created: NLD 2007-04-20
// =============================================================================
class MissionParameter_PopulationKnowledge : public MissionParameter_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             MissionParameter_PopulationKnowledge( const ASN1T_MissionParameter& asn );
    virtual ~MissionParameter_PopulationKnowledge();
    //@}

    //! @name Operations
    //@{
    virtual void Send     ( ASN1T_MissionParameter& asn ) const;
    virtual void AsnDelete( ASN1T_MissionParameter& asn ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    MissionParameter_PopulationKnowledge( const MissionParameter_PopulationKnowledge& );            //!< Copy constructor
    MissionParameter_PopulationKnowledge& operator=( const MissionParameter_PopulationKnowledge& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    unsigned int populationKnowledge_;
    //@}
};

}

#endif // __MissionParameter_PopulationKnowledge_h_
