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
#include "tools/thread/RequestProcessor_ABC.h"
#include <tools/Path.h>
#pragma warning( push, 1 )
#pragma warning( disable : 4244 4275 )
#include <boost/thread/mutex.hpp>
#pragma warning( pop )
#include <boost/shared_ptr.hpp>
#include <boost/noncopyable.hpp>
#include <set>

namespace spatialcontainer
{
    class RetractationHandle;
}
typedef boost::shared_ptr< spatialcontainer::RetractationHandle > RetractationPtr;

class TerrainPathfinder;
class TER_PathFindRequest_ABC;
class TER_DynamicData;
typedef boost::shared_ptr< TER_DynamicData > DynamicDataPtr;
class TER_StaticData;

// =============================================================================
// Created: AGE 2005-02-23
// =============================================================================
class TER_PathFinderThread : public tools::thread::RequestProcessor_ABC< boost::shared_ptr< TER_PathFindRequest_ABC > >, private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             TER_PathFinderThread( const TER_StaticData& staticData,
                                   tools::thread::MessageQueue_ABC< boost::shared_ptr< TER_PathFindRequest_ABC > >& queue,
                                   unsigned int nMaxEndConnections, double rMinEndConnectionLength, bool bUseSameThread,
                                   const tools::Path& dump,
                                   const std::string& filter );
    virtual ~TER_PathFinderThread();
    //@}

    //! @name Dynamic data
    //@{
    void AddDynamicDataToRegister  ( const DynamicDataPtr& data );
    void AddDynamicDataToUnregister( const DynamicDataPtr& data );
    //@}

    //! @name Operations
    //@{
    void ProcessInSimulationThread( const boost::shared_ptr< TER_PathFindRequest_ABC >& pRequest );
    //@}

private:
    //! @name Tools
    //@{
    virtual void Process           ( const boost::shared_ptr< TER_PathFindRequest_ABC >& pRequest );
            void ProcessDynamicData();
    //@}

private:
    //! @name Member data
    //@{
    bool                               bUseSameThread_;
    const tools::Path                  dump_; // empty if dump is disabled
    const std::set< size_t >           filter_; // empty if no id filters
    std::unique_ptr< TerrainPathfinder > pathfinder_;
    std::unique_ptr< TerrainPathfinder > staticPathfinder_;
    std::map< DynamicDataPtr, RetractationPtr > handlers_;
    boost::mutex                       dynamicDataMutex_;
    std::vector< DynamicDataPtr >      dynamicDataToRegister_;
    std::vector< DynamicDataPtr >      dynamicDataToUnregister_;
    //@}
};

#endif // __TER_PathFinderThread_h_
