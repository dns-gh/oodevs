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

class MIL_Agent_ABC;
// =============================================================================
/** @class  PHY_RolePion_TerrainAnalysis
    @brief  PHY_RolePion_TerrainAnalysis
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
    virtual void GetCrossroads( std::vector< boost::shared_ptr< MT_Vector2D > >& points ) const;
    virtual void NotifyHasMove( const MT_Vector2D& newPos );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    PHY_RolePion_TerrainAnalysis( const PHY_RolePion_TerrainAnalysis& );            //!< Copy constructor
    PHY_RolePion_TerrainAnalysis& operator=( const PHY_RolePion_TerrainAnalysis& ); //!< Assignment operator
    //@}


private:
    //! @name Member data
    //@{
    MIL_Agent_ABC& pion_;
    MT_Vector2D lastPos_;
    std::map< MT_Vector2D, boost::shared_ptr< MT_Vector2D > > crossroadsBuffer_;
    //@}
};

#endif // __PHY_RolePion_TerrainAnalysis_h_
