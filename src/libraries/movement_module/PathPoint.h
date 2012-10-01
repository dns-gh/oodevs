// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2002 MASA Group
//
// *****************************************************************************

#ifndef movement_module_PathPoint_h_
#define movement_module_PathPoint_h_

#include "MT_Tools/MT_Vector2D.h"
#include <spatialcontainer/TerrainData.h>
#include <boost/shared_ptr.hpp>
#include <boost/noncopyable.hpp>

namespace sword
{
namespace wrapper
{
    class View;
}
namespace movement
{
//*****************************************************************************
// Created: JVT 02-12-04
//*****************************************************************************
class PathPoint : private boost::noncopyable
{
public:
    //-------------------------------------------------------------------------
    /** @name Types */
    //-------------------------------------------------------------------------
    //@{
    enum E_Type
    {
        eTypePointPath,
        eTypePointFront,
        eTypePointSpecial
    };
    enum E_TypePoint
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
    //! @name Constructor/Destructor
    //@{
             PathPoint( const MT_Vector2D& vPos, const TerrainData& nObjectTypes, const TerrainData& nObjectTypesToNextPoint );
    virtual ~PathPoint();
protected:
             PathPoint( const MT_Vector2D& vPos, E_Type type, E_TypePoint nPointType, const char* szDIARepType);
    //@}

public:
    //! @name Accessors
    //@{
    const MT_Vector2D& GetPos() const;
    E_Type GetType() const;
    E_TypePoint GetTypePoint() const;
    const TerrainData& GetObjectTypes() const;
    const TerrainData& GetObjectTypesToNextPoint() const;
    virtual const boost::shared_ptr< PathPoint >& GetDestPoint() const;
    virtual int GetTypeLima();
    virtual unsigned int GetLimaID();
    virtual const TerrainData& GetTypeTerrain() const;
    //@}

    //! @name Main
    //@{
    bool IsInObject( const TerrainData& data ) const;
    bool WillBeInObject( const TerrainData& data ) const;
    //@}

    //! @name DIA
    //@{
    virtual void SendToDIA( unsigned int entity, boost::shared_ptr< PathPoint > point ) const;
    virtual void RemoveFromDIA( boost::shared_ptr< PathPoint > self );
    const std::string& GetDIAType() const;
    //@}

protected:
    //! @name Member data
    //@{
    MT_Vector2D vPos_;
    E_Type nType_;
    E_TypePoint nPointType_;
    TerrainData nObjectTypes_;
    TerrainData nObjectTypesToNextPoint_;
    std::string diaType_;
    //@}
};

#include "PathPoint.inl"

}
}

#endif // movement_module_PathPoint_h_
