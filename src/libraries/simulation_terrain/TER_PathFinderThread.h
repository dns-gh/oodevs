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
#pragma warning( disable : 4275 )
#include "boost/thread/mutex.hpp"

class TerrainPathfinder;
class TerrainRetractationHandle;
class TER_NodeFunctor_ABC;
class TER_PathFindRequest_ABC;
class TER_DynamicData;

// =============================================================================
// Created: AGE 2005-02-23
// =============================================================================
class TER_PathFinderThread : public tools::thread::RequestProcessor_ABC< boost::shared_ptr< TER_PathFindRequest_ABC > >
{

public:
    //! @name Constructors/Destructor
    //@{
             TER_PathFinderThread( const std::string& strGraphArchive, const std::string& strNodeArchive, const std::string& strLinkArchive, tools::thread::MessageQueue_ABC< boost::shared_ptr< TER_PathFindRequest_ABC > >& queue );
    virtual ~TER_PathFinderThread();
    //@}

    //! @name Dynamic data
    //@{
    void AddDynamicDataToRegister  ( TER_DynamicData& data );
    void AddDynamicDataToUnregister( TER_DynamicData& data );

    TerrainRetractationHandle& CreateLineTree( const T_PointVector& points, const TerrainData& terrainData ); 
    //@}

    //! @name Terrain analysis
    //@{
    void ApplyOnNodesWithinCircle( const MT_Vector2D& vCenter, MT_Float rRadius, TER_NodeFunctor_ABC& bestNodeFunction ) const;

    std::vector< boost::shared_ptr< MT_Vector2D > > FindNodesWithinCircle( const MT_Vector2D& center, float rRadius );
    //@}

    //! @name Debug
    //@{
    void Dump( const std::string& strBaseArchiveName ) const;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    TER_PathFinderThread( const TER_PathFinderThread& );            //!< Copy constructor
    TER_PathFinderThread& operator=( const TER_PathFinderThread& ); //!< Assignement operator
    //@}

    //! @name Tools
    //@{
    virtual void Process           ( const boost::shared_ptr< TER_PathFindRequest_ABC >& pRequest );
            void ProcessDynamicData();
            void Dump              () const;
    //@}

private:
    //! @name Types
    //@{
    typedef std::vector< TER_DynamicData* >     T_DynamicDataVector;
    typedef T_DynamicDataVector::const_iterator CIT_DynamicDataVector;
    //@}

private:
    //! @name Member data
    //@{
    TerrainPathfinder*  pPathfinder_;
    T_DynamicDataVector dynamicDataToRegister_;
    T_DynamicDataVector dynamicDataToUnregister_;
    boost::mutex        dynamicDataMutex_;
    //@}
};

#endif // __TER_PathFinderThread_h_
