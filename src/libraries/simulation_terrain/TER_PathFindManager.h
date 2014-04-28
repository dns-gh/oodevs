// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: AGE 2005-01-31 $
// $Archive: /MVW_v10/Build/SDK/ter/src/TER_PathFindManager.h $
// $Author: Age $
// $Modtime: 25/02/05 16:29 $
// $Revision: 1 $
// $Workfile: TER_PathFindManager.h $
//
// *****************************************************************************

#ifndef __TER_PathFindManager_h_
#define __TER_PathFindManager_h_

#include "TER_PathFinderThread.h"
#include <boost/noncopyable.hpp>

class TER_DynamicData;
class TER_StaticData;
class TER_PathFindRequest_ABC;

namespace tools
{
    namespace thread
    {
        template< typename T > class MessageQueue_ABC;
    }
}

// =============================================================================
// Created: AGE 2005-01-31
// =============================================================================
class TER_PathFindManager : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit TER_PathFindManager( const TER_StaticData& staticData );
    virtual ~TER_PathFindManager();
    //@}

    //! @name Operations
    //@{
    static TER_PathFindManager& GetPathFindManager();
    TER_PathFinderThread& CreatePathFinderThread( tools::thread::MessageQueue_ABC< boost::shared_ptr< TER_PathFindRequest_ABC > >& queue,
                                                  unsigned int nMaxEndConnections, double rMinEndConnectionLength, bool useSameThread,
                                                  const tools::Path& dump, const std::string& filter );

    void AddDynamicData   ( const DynamicDataPtr& data );
    void RemoveDynamicData( const DynamicDataPtr& data );
    //@}

private:
    //! @name Member data
    //@{
    const TER_StaticData& staticData_;
    std::vector< TER_PathFinderThread* > threads_;
    //@}
};

#endif // __TER_PathFindManager_h_
