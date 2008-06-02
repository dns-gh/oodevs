// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __MissionParameterAgent_h_
#define __MissionParameterAgent_h_

#include "MissionParameter_ABC.h"

namespace dispatcher
{
    class Agent;
}

namespace bml
{

// =============================================================================
/** @class  MissionParameterAgent
    @brief  MissionParameterAgent
*/
// Created: SBO 2008-06-02
// =============================================================================
class MissionParameterAgent : public MissionParameter_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             MissionParameterAgent( const kernel::OrderParameter& type, const dispatcher::Agent& agent );
    virtual ~MissionParameterAgent();
    //@}

    //! @name Operations
    //@{
    virtual void Serialize( ASN1T_MissionParameter& parameter ) const;
    virtual void Clean( ASN1T_MissionParameter& parameter ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    MissionParameterAgent( const MissionParameterAgent& );            //!< Copy constructor
    MissionParameterAgent& operator=( const MissionParameterAgent& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    const dispatcher::Agent& agent_;
    //@}
};

}

#endif // __MissionParameterAgent_h_
