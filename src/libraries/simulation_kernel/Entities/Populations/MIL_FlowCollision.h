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
    void AddCollision( MIL_PopulationFlow* flow1, MIL_PopulationFlow* flow2 );
    bool CanMove( const MIL_PopulationFlow* flow );
    void Update();
    //@}


    //! @name CheckPoints
    //@{
//     BOOST_SERIALIZATION_SPLIT_MEMBER()
// 
//     void load( MIL_CheckPointInArchive&, const unsigned int );
//     void save( MIL_CheckPointOutArchive&, const unsigned int ) const;
    //@}

private:
    //! @name Helpers
    //@{
    void DoSplit();
    //@}

private:
    //! @name Member data
    //@{
    MT_Vector2D point_;
    std::vector< MIL_PopulationFlow* > collidingFlows_;
    MIL_PopulationFlow* going_; // remplacer par index ?
    bool isFlowing_;
    int movingIndex_;
    int start_;
    //@}
};

#endif // __MIL_FlowCollision_h_
