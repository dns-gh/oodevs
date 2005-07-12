//*****************************************************************************
//
// $Created: JVT 02-12-04 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Decision/Path/DEC_PathPoint.h $
// $Author: Age $
// $Modtime: 24/02/05 17:24 $
// $Revision: 4 $
// $Workfile: DEC_PathPoint.h $
//
//*****************************************************************************

#ifndef __DEC_PathPoint_h_
#define __DEC_PathPoint_h_

#include "MIL.h"

class DEC_Path;
class DEC_PathPoint;
class DIA_Representation;
class DEC_RolePion_Decision;

//*****************************************************************************
// Created: JVT 02-12-04
//*****************************************************************************
class DEC_PathPoint
{
public:
    //-------------------------------------------------------------------------
    /** @name Types */
    //-------------------------------------------------------------------------
    //@{
    enum E_Point
    {
        eAvantPoint,
        ePoint
    };

    enum E_Type
    {
        eTypePointPath,
        eTypePointSpecial
    };
    //@}

public:
    DEC_PathPoint( const MT_Vector2D& vPos, const TerrainData& nObjectTypes, const TerrainData& nObjectTypesToNextPoint );
    DEC_PathPoint( const DEC_PathPoint& pathPoint );
    virtual ~DEC_PathPoint();

    //-------------------------------------------------------------------------
    /** @name Accessors */
    //-------------------------------------------------------------------------
    //@{
    const MT_Vector2D& GetPos                   () const;
    E_Type             GetType                  () const;
    const TerrainData& GetObjectTypes           () const;
    const TerrainData& GetObjectTypesToNextPoint() const;
    //@}

    //-------------------------------------------------------------------------
    /** @name Main */
    //-------------------------------------------------------------------------
    //@{
    bool IsInObject    ( const TerrainData& data ) const;
    bool WillBeInObject( const TerrainData& data ) const;
    //@}

    //-------------------------------------------------------------------------
    /** @name DIA */
    //-------------------------------------------------------------------------
    //@{
    virtual void SendToDIA    ( DEC_RolePion_Decision& agent );
    virtual void RemoveFromDIA( DEC_RolePion_Decision& agent );
    //@}

protected:
    DEC_PathPoint( const MT_Vector2D& vPos );

protected:
    MT_Vector2D vPos_;
    E_Type      nType_;
    TerrainData nObjectTypes_;
    TerrainData nObjectTypesToNextPoint_;   
};

#include "DEC_PathPoint.inl"

#endif // __DEC_PathPoint_h_