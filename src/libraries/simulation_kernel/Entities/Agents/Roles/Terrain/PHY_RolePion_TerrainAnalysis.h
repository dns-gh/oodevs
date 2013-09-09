// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __PHY_RolePion_TerrainAnalysis_h_
#define __PHY_RolePion_TerrainAnalysis_h_

#include "PHY_RoleInterface_TerrainAnalysis.h"
#include "MovementHandler_ABC.h"
#include "Entities/Orders/MIL_Fuseau.h"
#include "MT_Tools/MT_Vector2D.h"

class MIL_Agent_ABC;

// =============================================================================
/** @class  PHY_RolePion_TerrainAnalysis
    @brief  PHY Role pion terrain analysis
*/
// Created: MGD 2010-04-20
// =============================================================================
class PHY_RolePion_TerrainAnalysis : public PHY_RoleInterface_TerrainAnalysis
                                   , public location::MovementHandler_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit PHY_RolePion_TerrainAnalysis( MIL_Agent_ABC& pion );
    virtual ~PHY_RolePion_TerrainAnalysis();
    //@}

    //! @name Operations
    //@{
    virtual void GetCrossroads                  ( std::vector< boost::shared_ptr< MT_Vector2D > >& points );
    virtual void FindSafetyPositionsWithinCircle( std::vector< boost::shared_ptr< MT_Vector2D > >& points, float radius, float safetyDistance );
    virtual void NotifyHasMoved                  ( const MT_Vector2D& newPos );
    virtual bool CanMoveOnUrbanBlock            ( const std::vector< MT_Vector2D >& points ) const;
    virtual bool CanMoveOnBurningCells          ( const std::vector< MT_Vector2D >& points ) const;
    virtual bool CanMoveOnKnowledgeObject       ( const std::vector< MT_Vector2D >& points ) const;
    virtual bool CanMoveOn                      ( const MT_Vector2D& position ) const;

    //@}

private:
    //! @name Helpers
    //@{
    void CheckFuseau();
    void UpdateCrossroads();
    void UpdateSafety( float radius, float safetyDistance );
    bool CanMoveOnTerrain( const std::vector< MT_Vector2D >& points ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    PHY_RolePion_TerrainAnalysis( const PHY_RolePion_TerrainAnalysis& );            //!< Copy constructor
    PHY_RolePion_TerrainAnalysis& operator=( const PHY_RolePion_TerrainAnalysis& ); //!< Assignment operator
    //@}

    //! @name Types
    //@{
    typedef std::map< MT_Vector2D, boost::shared_ptr< MT_Vector2D > > T_Buffer;
    //@}

private:
    //! @name Member data
    //@{
    MIL_Agent_ABC& owner_;
    MT_Vector2D    lastPos_;
    T_Buffer       crossroadsBuffer_;
    T_Buffer       safetyBuffer_;
    MIL_Fuseau     fuseau_;
    float          cacheRadius_;
    float          cacheSafety_;
    bool           crossroadsCacheValid_;
    bool           safetyCacheValid_;
    //@}
};

#endif // __PHY_RolePion_TerrainAnalysis_h_
