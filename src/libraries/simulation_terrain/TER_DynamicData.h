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
#pragma warning( push, 1 )
#pragma warning( disable : 4244 4275 )
#include <boost/thread/mutex.hpp>
#pragma warning( pop )
#include <boost/noncopyable.hpp>

class TER_PathFinderThread;
class TerrainRetractationHandle;

// =============================================================================
// Created: AGE 2005-02-01
// =============================================================================
class TER_DynamicData : private boost::noncopyable
{
public:
    explicit TER_DynamicData( const T_PointVector& points );
             TER_DynamicData( const T_PointVector& points, const std::string& type );

    const T_PointVector& GetPoints() const;
    const TerrainData& GetData() const;

    //! @name Operations
    //@{
    void AddForRegistration   ( TER_PathFinderThread& thread );
    void AddForUnregistration ( TER_PathFinderThread& thread );

    void RegisterDynamicData  ( TER_PathFinderThread& thread, TerrainRetractationHandle& h );
    void UnregisterDynamicData( TER_PathFinderThread& thread );
    //@}

private:
    ~TER_DynamicData();

private:
    //! @name Types
    //@{
    typedef std::map< TER_PathFinderThread*, TerrainRetractationHandle* > T_HandleMap;
    typedef T_HandleMap::iterator                                        IT_HandleMap;
    typedef T_HandleMap::const_iterator                                 CIT_HandleMap;
    //@}

private:
    T_HandleMap handles_;
    unsigned int nNbrRefs_;
    const T_PointVector points_;
    const TerrainData terrainData_;
    boost::mutex mutex_;
};

// Create a new TER_DynamicData from the input geometry and type, and if the
// singletons are available, register it. If type is the empty string, use
// the default type defined in terrain analyzer.
boost::shared_ptr< TER_DynamicData > CreateAndRegisterDynamicData(
        const T_PointVector& points, const std::string& type );

#endif // __TER_DynamicData_h_
