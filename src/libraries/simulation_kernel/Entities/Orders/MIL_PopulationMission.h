// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __MIL_PopulationMission_h_
#define __MIL_PopulationMission_h_

#include "MIL_Mission_ABC.h"

namespace sword
{
    class CrowdOrder;
}

class MIL_Population;

// =============================================================================
/** @class  MIL_PopulationMission
    @brief  MIL_PopulationMission
*/
// Created: NLD 2006-11-14
// =============================================================================
class MIL_PopulationMission : public MIL_Mission_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             MIL_PopulationMission( const MIL_MissionType_ABC& type, MIL_Population& population, const sword::CrowdOrder& asn );
    virtual ~MIL_PopulationMission();
    //@}

    //! @name Operations
    //@{
    virtual void Start( boost::shared_ptr< MIL_Mission_ABC > self );
    virtual void Stop( boost::shared_ptr< MIL_Mission_ABC > self );
    virtual bool IsFragOrderAvailable( const MIL_FragOrderType& fragOrderType ) const;
    virtual void Send() const;
    //@}

private:
    //! @name Helpers
    //@{
    static void SendNoMission( const MIL_Population& population );
    //@}

private:
    //! @name Member data
    //@{
    MIL_Population& population_;
    bool            bDIABehaviorActivated_;
    //@}
};

#endif // __MIL_PopulationMission_h_
