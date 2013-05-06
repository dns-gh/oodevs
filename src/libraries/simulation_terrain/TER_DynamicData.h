// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __TER_DynamicData_h_
#define __TER_DynamicData_h_

#include <spatialcontainer/TerrainData.h>
#include "MT_Tools/MT_Vector2DTypes.h"
#include <boost/noncopyable.hpp>

// =============================================================================
// Created: AGE 2005-02-01
// =============================================================================
class TER_DynamicData : private boost::noncopyable
{
public:
    explicit TER_DynamicData( const T_PointVector& points );
             TER_DynamicData( const T_PointVector& points, const std::string& type );
    virtual ~TER_DynamicData();

    const T_PointVector& GetPoints() const;
    const TerrainData& GetData() const;

private:
    const T_PointVector points_;
    const TerrainData terrainData_;
};

typedef boost::shared_ptr< TER_DynamicData > DynamicDataPtr;

// Create a new TER_DynamicData from the input geometry and type, and if the
// singletons are available, register it. If type is the empty string, use
// the default type defined in terrain analyzer.
DynamicDataPtr CreateAndRegisterDynamicData( const T_PointVector& points,
        const std::string& type );

#endif // __TER_DynamicData_h_
