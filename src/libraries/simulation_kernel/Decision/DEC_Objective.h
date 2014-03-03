// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __DEC_Objective_h_
#define __DEC_Objective_h_

#include "MIL.h"
#include "simulation_terrain/TER_Localisation.h"

namespace sword
{
    class MissionObjective;
}

// =============================================================================
/** @class  DEC_Objective
    @brief  DEC_Objective
*/
// Created: NLD 2007-05-14
// =============================================================================
class DEC_Objective
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit DEC_Objective( const sword::MissionObjective& asn );
    virtual ~DEC_Objective();
    //@}

    //! @name Accessors
    //@{
    const TER_Localisation& GetLocalisation() const;
    unsigned int GetSchedule() const;
    bool IsFlagged() const;
    //@}

    //! @name Operations
    //@{
    void operator=( const DEC_Objective& rhs );
    void Serialize( sword::MissionObjective& asn ) const;
    void Flag     ( bool bValue );

    MT_Vector2D ComputerBarycenter() const;
    //@}

private:
    //! @name Member data
    //@{
    TER_Localisation localisation_;
    unsigned int nSchedule_;
    bool bFlag_;
    //@}
};

#endif // __DEC_Objective_h_
