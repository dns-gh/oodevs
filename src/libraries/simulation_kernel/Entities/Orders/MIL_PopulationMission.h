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
#include "MIL_MissionType_ABC.h"

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
             MIL_PopulationMission( const MIL_MissionType_ABC& type, MIL_Population& population, const ASN1T_MsgPopulationOrder& asn );
    virtual ~MIL_PopulationMission();
    //@}

    //! @name Operations
    //@{
    virtual void Start               ();
    virtual bool IsFragOrderAvailable( const MIL_FragOrderType& fragOrderType ) const;
    //@}

private:
    //! @name Tools
    //@{
    void Stop();
    //@}

    //! @name Network
    //@{
           void Send         () const;
    static void SendNoMission( const MIL_Population& population );
    //@}

private:
    MIL_Population& population_;
    bool            bDIABehaviorActivated_;
};

#endif // __MIL_PopulationMission_h_
