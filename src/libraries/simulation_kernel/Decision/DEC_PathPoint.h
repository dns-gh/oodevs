// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2002 MASA Group
//
// *****************************************************************************

#ifndef __DEC_PathPoint_h_
#define __DEC_PathPoint_h_

#include "MIL.h"
#include <spatialcontainer/TerrainData.h>
#include <boost/shared_ptr.hpp>
#include <boost/optional.hpp>

//*****************************************************************************
// Created: JVT 02-12-04
//*****************************************************************************
class DEC_PathPoint : private boost::noncopyable
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
        eTypePointNormal        = 1,    // d�faut
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
             DEC_PathPoint( const MT_Vector2D& vPos, const TerrainData& nObjectTypes, const TerrainData& nObjectTypesToNextPoint, bool waypoint );
    virtual ~DEC_PathPoint();
protected:
             DEC_PathPoint( const MT_Vector2D& vPos, E_Type type, E_TypePoint nPointType, const char* szDIARepType);
    //@}

public:
    //! @name Accessors
    //@{
    const MT_Vector2D& GetPos() const;
    E_Type GetType() const;
    E_TypePoint GetTypePoint() const;
    const TerrainData& GetObjectTypes() const;
    const TerrainData& GetObjectTypesToNextPoint() const;
    bool IsSlopeValid() const;
    bool IsWaypoint() const;
    bool IsPartial() const;
    double GetSlope() const;
    //@}

    //! @name Main
    //@{
    bool IsInObject( const TerrainData& data ) const;
    bool WillBeInObject( const TerrainData& data ) const;
    void SetSlope( double slope );
    void NotifyPartial();
    void NotifyWaypoint();
    //@}

    //! @name DIA
    //@{
    const std::string& GetDIAType() const;
    //@}

protected:
    MT_Vector2D vPos_;
    E_Type nType_;
    E_TypePoint nPointType_;
    TerrainData nObjectTypes_;
    TerrainData nObjectTypesToNextPoint_;
    std::string diaType_;
    boost::optional< double > slope_;
    bool waypoint_;
    bool partial_;
};

#endif // __DEC_PathPoint_h_
