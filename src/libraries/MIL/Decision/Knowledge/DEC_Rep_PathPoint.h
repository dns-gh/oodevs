//*****************************************************************************
//
// $Created: JDY 03-05-05 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Decision/Knowledge/DEC_Rep_PathPoint.h $
// $Author: Age $
// $Modtime: 3/02/05 18:16 $
// $Revision: 4 $
// $Workfile: DEC_Rep_PathPoint.h $
//
//*****************************************************************************

#ifndef __DEC_Rep_PathPoint_h_
#define __DEC_Rep_PathPoint_h_

#include "MIL.h"
#include "Decision/Path/DEC_PathPoint.h"

class DEC_Path;
class DEC_RolePion_Decision;

//*****************************************************************************
// Created: JDY 03-05-05
//*****************************************************************************
class DEC_Rep_PathPoint :   public DEC_PathPoint
                          , public DIA_Representation
{
public:
    //-------------------------------------------------------------------------
    /** @name Types */
    //-------------------------------------------------------------------------
    //@{
    typedef enum E_TypePoint
    {
        eTypePointNormal        = 1,    // défaut
        eTypePointObservation   = 2,    // point d'observation
        eTypePointParticulier   = 4,    // point particulier
        eTypePointBond          = 8,    // point de limite de bond
        eTypePointLima          = 16,   // point sur une lima
        eTypePointSonde         = 32,   // point necessitant coup de sonde 
        eTypePointCCT           = 64    // point de changement de compartiment terrain
    };
    //@}

public:
    DEC_Rep_PathPoint( DEC_Path& path, const MT_Vector2D& vPos, E_TypePoint nTypePoint, const TerrainData& nTypeTerrain, const char* szDIARepType = "Rep_Point" );
    DEC_Rep_PathPoint( const MT_Vector2D& vPos, E_TypePoint nTypePoint, const TerrainData& nTypeTerrain, const char* szDIARepType = "Rep_Point" );
    virtual ~DEC_Rep_PathPoint();
    
    //-------------------------------------------------------------------------
    /** @name Init */
    //-------------------------------------------------------------------------
    //@{
    static void InitializeDIA();
    //@}

    //-------------------------------------------------------------------------
    /** @name Accessors */
    //-------------------------------------------------------------------------
    //@{
    E_TypePoint        GetTypePoint() const;
    const TerrainData& GetTypeTerrain() const;
    //@}

    //-------------------------------------------------------------------------
    /** @name DIA */
    //-------------------------------------------------------------------------
    //@{
    void SendToDIA    ( DEC_RolePion_Decision& agent );   
    void RemoveFromDIA( DEC_RolePion_Decision& agent );
    //@}

    //! @name Debug
    //@{
    virtual void Dump() const;
    //@}
   
protected:
    DEC_Rep_PathPoint( const DEC_Rep_PathPoint& );

private:
    const DEC_Rep_PathPoint& operator=( const DEC_Rep_PathPoint& );

private:
    E_TypePoint    nTypePoint_;
    DIA_Parameters diaParameters_;
    bool           bAlreadySended_;
    TerrainData    nTypeTerrain_;

public:
    static int nDIAPointIdx_;
    static int nDIAClsIdx_;
    static int nDIATypeIdx_;
    static int nDIAavtIdx_;
    static int nDIAItinerairedx_;
};

#include "DEC_Rep_PathPoint.inl"

#endif // __DEC_Rep_PathPoint_h_