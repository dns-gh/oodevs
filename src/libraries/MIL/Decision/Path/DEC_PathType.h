//*****************************************************************************
//
// $Created: JDY 03-04-10 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Decision/Path/DEC_PathType.h $
// $Author: Nld $
// $Modtime: 21/07/05 16:33 $
// $Revision: 5 $
// $Workfile: DEC_PathType.h $
//
//*****************************************************************************

#ifndef __DEC_PathType_h_
#define __DEC_PathType_h_

#include "MIL.h"

class TerrainRule_ABC;
class DEC_Path;

//*****************************************************************************
// Created: JDY 03-04-10
//*****************************************************************************
class DEC_PathType
{
public:

    enum E_PathType
    {
        eInfoMovement = 0,
        eInfoRecon,
        eInfoInfiltration,
        eInfoAssault,
        eInfoRetreat,
        eInfoBackup,
        eInfoMineClearance,
        eNbrPathType
    };

public:
     DEC_PathType( E_PathType nType );
    ~DEC_PathType();

    //! @name Operations
    //@{
    TerrainRule_ABC& CreateRule( const DEC_Path& path, const MT_Vector2D& from, const MT_Vector2D& to );

    std::string ConvertPathTypeToString() const;
    //@}

private:
    //! @name Types
    //@{
    typedef std::pair< TerrainData, MT_Float > T_TerrainCost;
    //@}

    //! @name Tools
    //@{
    static T_TerrainCost PreferedTerrain( E_PathType type, bool bFly );
    static T_TerrainCost AvoidedTerrain( E_PathType type, bool bFly );
    static void          GetObjectCosts( uint nObjectTypeId, MT_Float& rCostIn, MT_Float& rCostOut );
    
    static std::vector< std::pair< MT_Float, MT_Float > > InitializeObjectCosts();
    //@}

private:
    E_PathType nPathType_;
};

#   include "DEC_PathType.inl"

#endif // __DEC_PathType_h_
