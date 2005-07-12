// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: AGE 2005-02-23 $
// $Archive: /MVW_v10/Build/SDK/TER/src/TER_PathFinderThread.h $
// $Author: Age $
// $Modtime: 25/03/05 15:29 $
// $Revision: 4 $
// $Workfile: TER_PathFinderThread.h $
//
// *****************************************************************************

#ifndef __TER_PathFinderThread_h_
#define __TER_PathFinderThread_h_

#include "tools/thread/RequestProcessor_ABC.h"
namespace boost
{
    class mutex;
}
class TerrainPathfinder;
class TerrainRetractationHandle;
class TER_NodeFunctor_ABC;
class TER_PathFindRequest_ABC;

// =============================================================================
/** @class  TER_PathFinderThread
    @brief  pathfinder thread
*/
// Created: AGE 2005-02-23
// =============================================================================
class TER_PathFinderThread : public tools::thread::RequestProcessor_ABC< TER_PathFindRequest_ABC* >
{

public:
    //! @name Constructors/Destructor
    //@{
             TER_PathFinderThread( const std::string& strGraphArchive, const std::string& strNodeArchive, const std::string& strLinkArchive, tools::thread::MessageQueue_ABC< TER_PathFindRequest_ABC* >& queue );
    virtual ~TER_PathFinderThread();
    //@}

    //! @name Dynamic data
    //@{
    TerrainRetractationHandle& CreateRetractationHandle();
    void CreateLineTree( const MT_Vector2D& from, const MT_Vector2D& to, TerrainRetractationHandle& handle, const TerrainData& terrainData );
    //@}

    //! @name Terrain analysis
    //@{
    TerrainData GetTerrainDataAt( const MT_Vector2D& pos ) const;
    void ApplyOnNodesWithinCircle( const MT_Vector2D& vCenter, MT_Float rRadius, TER_NodeFunctor_ABC& bestNodeFunction ) const;
    //@}

    //! @name Debug
    //@{
    void Dump( const std::string& strBaseArchiveName ) const;
    //@}

    //! @name Threading
    //@{
    boost::mutex& GetMutex();
    //@}

private:
    //! @name Copy/Assignement
    //@{
    TER_PathFinderThread( const TER_PathFinderThread& );            //!< Copy constructor
    TER_PathFinderThread& operator=( const TER_PathFinderThread& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    void Dump() const;
    virtual void Process( TER_PathFindRequest_ABC* const& pRequest );
    //@}

private:
    //! @name Member data
    //@{
    TerrainPathfinder* pPathfinder_;
    //@}
};

#endif // __TER_PathFinderThread_h_
