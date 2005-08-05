// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: AGE 2005-08-04 $
// $Archive: $
// $Author: $
// $Modtime: $
// $Revision: $
// $Workfile: $
//
// *****************************************************************************

#ifndef __DEC_PathClass_h_
#define __DEC_PathClass_h_

class TerrainRule_ABC;
class DEC_Path;

// =============================================================================
/** @class  DEC_PathClass
    @brief  DEC_PathClass
*/
// Created: AGE 2005-08-04
// =============================================================================
class DEC_PathClass
{

public:
    //! @name Constructors/Destructor
    //@{
             DEC_PathClass( MIL_InputArchive& archive, const DEC_PathClass* pCopyFrom = 0 );
    virtual ~DEC_PathClass();
    //@}

    //! @name Operations
    //@{
    TerrainRule_ABC& CreateRule( const DEC_Path& path, const MT_Vector2D& from, const MT_Vector2D& to ) const;
    //@}

private:
    //! @name Types
    //@{
    typedef std::vector< MT_Float > T_ObjectCosts;
    //@}

    //! @name Helpers
    //@{
    void Initialize( MIL_InputArchive& archive );
    void ReadFuseau( MIL_InputArchive& archive );
    void ReadDangerDirection( MIL_InputArchive& archive );
    void ReadEnemiesCost( MIL_InputArchive& archive );
    void ReadTerrains( MIL_InputArchive& archive, TerrainData& data );

    static MT_Float      GetObjectCost( uint nObjectTypeId );
    static T_ObjectCosts InitializeObjectCosts();
    //@}

private:
    //! @name Member data
    //@{
    bool        bShort_;
    MT_Float    rPreferCost_;
    TerrainData prefer_;
    MT_Float    rAvoidCost_;
    TerrainData avoid_;
    bool        bAvoidObjects_;
    MT_Float    rAltitudePreference_;

    MT_Float rMaximumFuseauDistance_;
    MT_Float rMaximumFuseauDistanceWithAutomata_;
    MT_Float rComfortFuseauDistance_;
    MT_Float rFuseauCostPerMeterOut_;
    MT_Float rFuseauCostPerMeterIn_;

    MT_Float rDangerDirectionBaseCost_;
    MT_Float rDangerDirectionLinearCost_;

    MT_Float rCostOnContact_;
    MT_Float rCostAtSecurityRange_;
    MT_Float rMaximumCost_;
    //@}
};

#endif // __DEC_PathClass_h_
