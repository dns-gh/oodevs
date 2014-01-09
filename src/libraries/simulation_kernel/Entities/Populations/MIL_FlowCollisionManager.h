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

// =============================================================================
/** @class  MIL_FlowCollisionManager
    @brief  MIL_FlowCollisionManager
*/
// Created: JSR 2014-01-09
// =============================================================================
class MIL_FlowCollisionManager : private boost::noncopyable
{
public:
    //! @name Types
    //@{
    //@}

public:
    //! @name Constructors/Destructor
    //@{
    static MIL_FlowCollisionManager& GetInstance();
    virtual ~MIL_FlowCollisionManager();
    //@}

    //! @name Operations
    //@{
    void AddCollision( MIL_PopulationFlow* flow1, MIL_PopulationFlow* flow2, T_PointList::const_iterator it, MT_Vector2D& point );
    bool CanMove( const MIL_PopulationFlow* flow );
    void Execute();
    //@}


    //! @name CheckPoints
    //@{
//     BOOST_SERIALIZATION_SPLIT_MEMBER()
// 
//     void load( MIL_CheckPointInArchive&, const unsigned int );
//     void save( MIL_CheckPointOutArchive&, const unsigned int ) const;
    //@}

private:
    //! @name Constructor
    //@{
    MIL_FlowCollisionManager();

private:
    //! @name Helpers
    //@{
    //@}

private:
    //! @name Types
    //@{
    //@}

private:
    //! @name Member data
    //@{
    static MIL_FlowCollisionManager* pInstance_;
    std::map< MT_Vector2D, std::vector< MIL_PopulationFlow* > > collidingFlows_;
//     MIL_PopulationFlow* flow1_;
//     MIL_PopulationFlow* flow2_;
    MIL_PopulationFlow* going_;
    bool isFlowing_;
    MT_Vector2D point_;
    int movingIndex_;
    int start_;
    //@}
};

#endif // __MIL_FlowCollisionManager_h_
