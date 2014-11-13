// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __TER_PathFinderThread_h_
#define __TER_PathFinderThread_h_

#include "MT_Tools/MT_Vector2DTypes.h"
#pragma warning( push, 1 )
#pragma warning( disable : 4244 4275 )
#include <boost/thread/mutex.hpp>
#pragma warning( pop )
#include <boost/shared_ptr.hpp>
#include <boost/noncopyable.hpp>

namespace spatialcontainer
{
    class RetractationHandle;
}
typedef boost::shared_ptr< spatialcontainer::RetractationHandle > RetractationPtr;

class TerrainPathfinder;
class TER_DynamicData;
typedef boost::shared_ptr< TER_DynamicData > DynamicDataPtr;
class TER_StaticData;

// =============================================================================
// Created: AGE 2005-02-23
// =============================================================================
class TER_PathFinderThread : private boost::noncopyable
{
public:
             TER_PathFinderThread( const TER_StaticData& staticData,
                   unsigned int nMaxEndConnections, double rMinEndConnectionLength );
    virtual ~TER_PathFinderThread();

    void AddDynamicDataToRegister  ( const DynamicDataPtr& data );
    void AddDynamicDataToUnregister( const DynamicDataPtr& data );
    void ProcessDynamicData();
    boost::shared_ptr< TerrainPathfinder > GetPathfinder( bool dynamic );

private:
    boost::shared_ptr< TerrainPathfinder > pathfinder_;
    boost::shared_ptr< TerrainPathfinder > staticPathfinder_;
    std::map< DynamicDataPtr, RetractationPtr > handlers_;
    boost::mutex                       dynamicDataMutex_;
    std::vector< DynamicDataPtr >      dynamicDataToRegister_;
    std::vector< DynamicDataPtr >      dynamicDataToUnregister_;
};

#endif // __TER_PathFinderThread_h_
