// *****************************************************************************
//
// $Created: NLD 2003-08-14 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Decision/Path/DEC_PathFind_Manager.cpp $
// $Author: Age $
// $Modtime: 10/05/05 16:50 $
// $Revision: 15 $
// $Workfile: DEC_PathFind_Manager.cpp $
//
// *****************************************************************************

#include "MIL_pch.h"

#include "DEC_PathFind_Manager.h"
#include "DEC_Path_ABC.h"

#include "Entities/Objects/MIL_RealObjectType.h"
#include "Agent/DEC_PathType.h"
#include "Agent/DEC_PathClass.h"

#include "TER/TER_PathfinderThread.h"
#include "TER/TER_PathFindManager.h"
#include "TER/TER_World.h"

// -----------------------------------------------------------------------------
// Name: DEC_PathFind_Manager constructor
// Created: NLD 2003-08-14
// -----------------------------------------------------------------------------
DEC_PathFind_Manager::DEC_PathFind_Manager( MIL_InputArchive& archive  )
    : nMaxComputationDuration_( std::numeric_limits< uint >::max() )
    , rDistanceThreshold_     ( 0. )
{
    archive.Section( "Pathfind" );
    int nPathfindThreads = 1;
    archive.ReadField    ( "PathfindNumber"   , nPathfindThreads, CheckValueGreaterOrEqual( 1 ) );
    archive.ReadField    ( "DistanceThreshold", rDistanceThreshold_ );
    archive.ReadTimeField( "TempsMaxCalcul"   , nMaxComputationDuration_, CheckValueGreater( 0 ), MIL_InputArchive::eThrow, MIL_InputArchive::eNothing );

    std::string strRulesArchive;
    archive.ReadField( "Rules", strRulesArchive );
    archive.EndSection(); // Pathfind

    MIL_InputArchive rulesArchive;
    rulesArchive.AddWarningStream( std::cout );
    rulesArchive.Open( strRulesArchive );

    DEC_PathType::Initialize();
    DEC_PathClass::Initialize( rulesArchive );

    MT_LOG_INFO_MSG( MT_FormatString( "Starting %d pathfind thread(s)", nPathfindThreads ) );

    for( int i = 0; i < nPathfindThreads; ++i )
        pathFindThreads_.push_back( & TER_World::GetWorld().GetPathFindManager().CreatePathFinderThread( *this ) );
}   

// -----------------------------------------------------------------------------
// Name: DEC_PathFind_Manager destructor
// Created: NLD 2003-08-14
// -----------------------------------------------------------------------------
DEC_PathFind_Manager::~DEC_PathFind_Manager()
{
    DEC_PathClass::Terminate();
    DEC_PathType ::Terminate();    
}

// =============================================================================
// MAIN
// =============================================================================

// -----------------------------------------------------------------------------
// Name: DEC_PathFind_Manager::StartCompute
// Created: NLD 2003-08-14
// -----------------------------------------------------------------------------
void DEC_PathFind_Manager::StartCompute( DEC_Path_ABC& path )
{
#ifdef _DEBUG
    MT_LOG_MESSAGE_MSG( MT_FormatString( "DEC_PathFind_Manager: New job pending : path 0x%p", &path ).c_str() );
#endif
    AddPendingJob( path );
}

// -----------------------------------------------------------------------------
// Name: DEC_PathFind_Manager::GetNbrShortRequests
// Created: NLD 2005-04-01
// -----------------------------------------------------------------------------
uint DEC_PathFind_Manager::GetNbrShortRequests() const
{
    boost::mutex::scoped_lock locker( mutex_ );
    return shortRequests_.size();
}
    
// -----------------------------------------------------------------------------
// Name: DEC_PathFind_Manager::GetNbrLongRequests
// Created: NLD 2005-04-01
// -----------------------------------------------------------------------------
uint DEC_PathFind_Manager::GetNbrLongRequests() const
{
    boost::mutex::scoped_lock locker( mutex_ );
    return longRequests_.size();
}

// -----------------------------------------------------------------------------
// Name: DEC_PathFind_Manager::GetNbrRequests
// Created: AGE 2005-05-10
// -----------------------------------------------------------------------------
uint DEC_PathFind_Manager::GetNbrRequests() const
{
    boost::mutex::scoped_lock locker( mutex_ );
    return longRequests_.size() + shortRequests_.size();
}

// -----------------------------------------------------------------------------
// Name: DEC_PathFind_Manager::AddPendingJob
// Created: NLD 2003-08-14
// -----------------------------------------------------------------------------
void DEC_PathFind_Manager::AddPendingJob( DEC_Path_ABC& path )
{
    boost::mutex::scoped_lock locker( mutex_ );
    path.IncRef();
    if( path.GetLength() > rDistanceThreshold_ )
        longRequests_.push_back( &path );
    else
        shortRequests_.push_back( &path );
    condition_.notify_all();
}

// -----------------------------------------------------------------------------
// Name: DEC_PathFind_Manager::GetMessage
// Created: AGE 2005-02-25
// -----------------------------------------------------------------------------
TER_PathFindRequest_ABC* DEC_PathFind_Manager::GetMessage()
{
    unsigned int nIndex = 0;
    for( ; nIndex < pathFindThreads_.size(); ++nIndex )
        if( pathFindThreads_[ nIndex ]->IsCurrent() )
            break;

    return GetMessage( nIndex );
}

namespace
{
    template< typename T >
    struct IsNotEmpty_
    {
        IsNotEmpty_( const T& cont ) : cont_( cont ) {};
        bool operator()() { return ! cont_.empty(); };
    private:
        const T& cont_;
        IsNotEmpty_& operator=( const IsNotEmpty_& );
    };
    template< typename T >
    IsNotEmpty_< T > IsNotEmpty( const T& cont ) { return IsNotEmpty_< T >( cont ); };

    template< typename T >
    struct AreNotEmpty_
    {
        AreNotEmpty_( const T& cont, const T& cont2 ) : cont_( cont ), cont2_( cont2 ) {};
        bool operator()() { return ! cont_.empty() || ! cont2_.empty(); };
    private:
        const T& cont_;
        const T& cont2_;
        AreNotEmpty_& operator=( const AreNotEmpty_& );
    };
    template< typename T >
    AreNotEmpty_< T > AreNotEmpty( const T& cont, const T& cont2 ) { return AreNotEmpty_< T >( cont, cont2 ); };
}

// -----------------------------------------------------------------------------
// Name: DEC_PathFind_Manager::GetMessage
// Created: AGE 2005-02-25
// -----------------------------------------------------------------------------
TER_PathFindRequest_ABC* DEC_PathFind_Manager::GetMessage( unsigned int nThread )
{
    TER_PathFindRequest_ABC* pRequest = 0;
    boost::mutex::scoped_lock locker( mutex_ );
    if( ( nThread % 2 ) )
    {
        condition_.wait( locker, IsNotEmpty( shortRequests_ ) );
        pRequest = shortRequests_.front();
        shortRequests_.pop_front();
    }
    else
    {
        condition_.wait( locker, AreNotEmpty( shortRequests_, longRequests_ ) );
        if( ! shortRequests_.empty() )
        {
            pRequest = shortRequests_.front();
            shortRequests_.pop_front();
        }
        else
        {
            pRequest = longRequests_.front();
            longRequests_.pop_front();
        }
    }
    return pRequest;
}

// -----------------------------------------------------------------------------
// Name: DEC_PathFind_Manager::GetCurrentThread
// Created: AGE 2005-02-28
// -----------------------------------------------------------------------------
int DEC_PathFind_Manager::GetCurrentThread() const
{
    unsigned int nIndex = 0;
    for( ; nIndex < pathFindThreads_.size(); ++nIndex )
        if( pathFindThreads_[ nIndex ]->IsCurrent() )
            return nIndex;
    return -1;
}

// -----------------------------------------------------------------------------
// Name: DEC_PathFind_Manager::DeletePath
// Created: AGE 2005-09-20
// -----------------------------------------------------------------------------
void DEC_PathFind_Manager::DeletePath( DEC_Path_ABC& path )
{
    boost::mutex::scoped_lock locker( cleanAndDestroyMutex_ );
    destroyedRequests_.push_back( &path );
}

// -----------------------------------------------------------------------------
// Name: DEC_PathFind_Manager::CleanPathAfterComputation
// Created: NLD 2006-01-23
// -----------------------------------------------------------------------------
void DEC_PathFind_Manager::CleanPathAfterComputation( DEC_Path_ABC& path )
{
    boost::mutex::scoped_lock locker( cleanAndDestroyMutex_ );
    requestsToCleanAfterComputation_.push_back( &path );
}

// -----------------------------------------------------------------------------
// Name: DEC_PathFind_Manager::Update
// Created: NLD 2005-09-20
// -----------------------------------------------------------------------------
void DEC_PathFind_Manager::Update()
{
    boost::mutex::scoped_lock locker( cleanAndDestroyMutex_ );

    while( ! requestsToCleanAfterComputation_.empty() )
    {
        TER_PathFindRequest_ABC* pRequest = requestsToCleanAfterComputation_.back();

        DEC_Path_ABC& path = static_cast< DEC_Path_ABC& >( *pRequest );
        path.CleanAfterComputation();
        requestsToCleanAfterComputation_.pop_back();
    }

#ifdef _DEBUG
    if( !destroyedRequests_.empty() )
        MT_LOG_DEBUG_MSG( "Flushing " << destroyedRequests_.size() << " path requests" );
#endif

    while( ! destroyedRequests_.empty() )
    {
        delete destroyedRequests_.back();
        destroyedRequests_.pop_back();
    }
}

