// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __MIL_FlowCollision_h_
#define __MIL_FlowCollision_h_

#include "MT_Tools/Mt_Vector2DTypes.h"
class MIL_PopulationFlow;
class MT_Line;

// =============================================================================
/** @class  MIL_FlowCollision
    @brief  MIL_FlowCollision
*/
// Created: JSR 2014-01-10
// =============================================================================
class MIL_FlowCollision : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit MIL_FlowCollision( const MT_Vector2D& point );
    virtual ~MIL_FlowCollision();
    //@}

    //! @name Operations
    //@{
    void SetCollision( MIL_PopulationFlow* flow1, MIL_PopulationFlow* flow2 );
    bool CanMove( const MIL_PopulationFlow* flow ) const;
    bool HasCollision( const MIL_PopulationFlow* flow1, const MIL_PopulationFlow* flow2 ) const;
    void NotifyFlowDestruction( const MIL_PopulationFlow* flow );
    bool MarkedForDestruction() const;
    void Update();
    //@}

private:
    //! @name Helpers
    //@{
    void Split();
    bool SplitOnSegment( const MT_Line& line, std::size_t& segmentIndex, double& cumulatedMagnitude );
    void RemovedPassedOverFlows();
    bool IsTimerOver();
    bool HasFlowPassedOver( const MIL_PopulationFlow* flow ) const;
    //@}

private:
    //! @name Member data
    //@{
    MT_Vector2D point_;
    std::vector< MIL_PopulationFlow* > collidingFlows_;
    MIL_PopulationFlow* going_;
    bool markedForDestruction_;
    std::size_t movingIndex_;
    double randomThreshold_;
    unsigned short deadlockTimer_;
    //@}
};

#endif // __MIL_FlowCollision_h_
