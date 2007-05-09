// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __MissionParameter_AgentKnowledge_h_
#define __MissionParameter_AgentKnowledge_h_

#include "MissionParameter_ABC.h"

namespace dispatcher
{

// =============================================================================
/** @class  MissionParameter_AgentKnowledge
    @brief  MissionParameter_AgentKnowledge
*/
// Created: NLD 2007-04-20
// =============================================================================
class MissionParameter_AgentKnowledge : public MissionParameter_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             MissionParameter_AgentKnowledge( const ASN1T_MissionParameter& asn );
    virtual ~MissionParameter_AgentKnowledge();
    //@}

    //! @name Operations
    //@{
    virtual void Send     ( ASN1T_MissionParameter& asn ) const;
    virtual void AsnDelete( ASN1T_MissionParameter& asn ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    MissionParameter_AgentKnowledge( const MissionParameter_AgentKnowledge& );            //!< Copy constructor
    MissionParameter_AgentKnowledge& operator=( const MissionParameter_AgentKnowledge& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    unsigned int agentKnowledge_;
    //@}
};

}

#endif // __MissionParameter_AgentKnowledge_h_
