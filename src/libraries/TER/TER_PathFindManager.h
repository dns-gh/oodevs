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

#include "TER.h"

class TER_NodeFunctor_ABC;
class TerrainData;
class TER_PathFinderThread;
class TER_DynamicData;
class TER_PathFindRequest_ABC;
namespace tools
{
    namespace thread
    {
        template< typename T > class MessageQueue_ABC;
    }
}

// =============================================================================
/** @class  TER_PathFindManager
    @brief  TER_PathFindManager
*/
// Created: AGE 2005-01-31
// =============================================================================
class TER_PathFindManager
{
public:
    //! @name Constructors/Destructor
    //@{
             TER_PathFindManager( const std::string& strGraphArchive, const std::string& strNodeArchive, const std::string& strLinkArchive );
    virtual ~TER_PathFindManager();
    //@}

    //! @name Operations
    //@{
    static TER_PathFindManager& GetPathFindManager();

    TerrainData GetTerrainDataAt( const MT_Vector2D& pos ) const;
    void ApplyOnNodesWithinCircle( const MT_Vector2D& vCenter, MT_Float rRadius, TER_NodeFunctor_ABC& bestNodeFunction ) const;

    TER_DynamicData& CreateDynamicData();
    void CreateLineTree( const MT_Vector2D& from, const MT_Vector2D& to, TER_DynamicData& data, const TerrainData& terrainData = DefaultTerrainData() );

    TER_PathFinderThread& CreatePathFinderThread( tools::thread::MessageQueue_ABC< TER_PathFindRequest_ABC* >& queue );
    //@}

private:
    //! @name Copy/Assignement
    //@{
    TER_PathFindManager( const TER_PathFindManager& );            //!< Copy constructor
    TER_PathFindManager& operator=( const TER_PathFindManager& ); //!< Assignement operator
    //@}

    //! @name Types
    //@{
    typedef std::vector< TER_PathFinderThread* > T_Threads;
    typedef T_Threads::const_iterator        CIT_Threads;
    //@}

    //! @name Helpers
    //@{
    static TerrainData& DefaultTerrainData();
    //@}

private:
    //! @name Member data
    //@{
    const std::string strGraphArchive_;
    const std::string strNodeArchive_;
    const std::string strLinkArchive_;
    T_Threads threads_;
    //@}
};

#endif // __TER_PathFindManager_h_
