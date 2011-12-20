// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __PHY_RoleInterface_TerrainAnalysis_h_
#define __PHY_RoleInterface_TerrainAnalysis_h_

#include "MT_Tools/Role_ABC.h"

class MT_Vector2D;

// =============================================================================
/** @class  PHY_RoleInterface_TerrainAnalysis
    @brief  PHY_RoleInterface_TerrainAnalysis
*/
// Created: MGD 2010-04-20
// =============================================================================
class PHY_RoleInterface_TerrainAnalysis : public tools::Role_ABC
{
public:
    //! @name Types
    //@{
    typedef PHY_RoleInterface_TerrainAnalysis RoleInterface;
    //@}

public:
    //! @name Constructors/Destructor
    //@{
             PHY_RoleInterface_TerrainAnalysis() {}
    virtual ~PHY_RoleInterface_TerrainAnalysis() {}
    //@}

    //! @name Operations
    //@{
    virtual void GetCrossroads                  ( std::vector< boost::shared_ptr< MT_Vector2D > >& points ) = 0;
    virtual void FindSafetyPositionsWithinCircle( std::vector< boost::shared_ptr< MT_Vector2D > >& points, float radius, float safetyDistance ) = 0;
    virtual bool CanMoveOnUrbanBlock            ( const std::vector< MT_Vector2D >& points ) const = 0;
    virtual bool CanMoveOnBurningCells          ( const std::vector< MT_Vector2D >& points ) const = 0;
    virtual bool CanMoveOnKnowledgeObject       ( const std::vector< MT_Vector2D >& points ) const = 0;
    virtual bool CanMoveOn                      ( boost::shared_ptr< MT_Vector2D > position ) const = 0;
    //@}
};

#endif // __PHY_RoleInterface_TerrainAnalysis_h_
