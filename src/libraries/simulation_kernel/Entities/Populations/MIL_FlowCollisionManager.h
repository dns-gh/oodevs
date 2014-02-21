// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __MIL_FlowCollisionManager_h_
#define __MIL_FlowCollisionManager_h_

#include "MT_Tools/Mt_Vector2DTypes.h"

class MIL_PopulationFlow;
class MIL_FlowCollision;

// =============================================================================
/** @class  MIL_FlowCollisionManager
    @brief  MIL_FlowCollisionManager
*/
// Created: JSR 2014-01-09
// =============================================================================
class MIL_FlowCollisionManager : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
    MIL_FlowCollisionManager();
    virtual ~MIL_FlowCollisionManager();
    //@}

    //! @name Operations
    //@{
    void SetCollisions( MIL_PopulationFlow* flow, const std::vector< std::pair< MIL_PopulationFlow*, MT_Vector2D > >& collisions );
    bool CanMove( const MIL_PopulationFlow* flow ) const;
    void Update();
    void NotifyFlowDestruction( const MIL_PopulationFlow* flow );
    //@}

private:
    //! @name Member data
    //@{
    std::map< MT_Vector2D, std::shared_ptr< MIL_FlowCollision > > flowCollisions_;
    //@}
};

#endif // __MIL_FlowCollisionManager_h_
