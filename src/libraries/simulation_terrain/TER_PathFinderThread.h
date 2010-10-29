// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __TER_PathFinderThread_h_
#define __TER_PathFinderThread_h_

#include "MT_Tools/MT_Vector2DTypes.h"
#include "tools/thread/RequestProcessor_ABC.h"
#pragma warning( push, 1 )
#pragma warning( disable : 4244 4275 )
#include <boost/thread/mutex.hpp>
#pragma warning( pop )

class TerrainPathfinder;
class TerrainRetractationHandle;
class TER_NodeFunctor_ABC;
class TER_PathFindRequest_ABC;
class TER_DynamicData;
class TerrainData;

// =============================================================================
// Created: AGE 2005-02-23
// =============================================================================
class TER_PathFinderThread : public tools::thread::RequestProcessor_ABC< boost::shared_ptr< TER_PathFindRequest_ABC > >
{
public:
    //! @name Constructors/Destructor
    //@{
             TER_PathFinderThread( const std::string& strGraphArchive, const std::string& strNodeArchive, const std::string& strLinkArchive, tools::thread::MessageQueue_ABC< boost::shared_ptr< TER_PathFindRequest_ABC > >& queue, bool bUseSameThread );
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
    void ApplyOnNodesWithinCircle( const MT_Vector2D& vCenter, double rRadius, TER_NodeFunctor_ABC& bestNodeFunction ) const;

    std::vector< boost::shared_ptr< MT_Vector2D > > FindCrossroadsWithinCircle( const MT_Vector2D& center, float radius );
    std::vector< boost::shared_ptr< MT_Vector2D > > FindSafetyPositionsWithinCircle( const MT_Vector2D& center, float radius, float safetyDistance );
    //@}

    //! @name Operations
    //@{
    void ProcessInSimulationThread( const boost::shared_ptr< TER_PathFindRequest_ABC >& pRequest );
    //@}

    //! @name Debug
    //@{
    void Dump( const std::string& strBaseArchiveName ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    TER_PathFinderThread( const TER_PathFinderThread& );            //!< Copy constructor
    TER_PathFinderThread& operator=( const TER_PathFinderThread& ); //!< Assignment operator
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
    bool                bUseSameThread_;
    //@}
};

#endif // __TER_PathFinderThread_h_
