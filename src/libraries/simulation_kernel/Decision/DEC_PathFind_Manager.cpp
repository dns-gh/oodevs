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

#include "simulation_kernel_pch.h"
#include "DEC_PathFind_Manager.h"
#include "DEC_Path_ABC.h"
#include "DEC_PathType.h"
#include "DEC_PathFactory.h"
#include "Tools/MIL_Tools.h"
#include "simulation_terrain/TER_PathFindManager.h"
#include "simulation_terrain/TER_World.h"
#include "Entities/Objects/MIL_ObjectFactory.h"
#include "Entities/Objects/MIL_ObjectType_ABC.h"
#include "Entities/Objects/MIL_ObjectFilter.h"
#include "Entities/Objects/AttritionCapacity.h"
#include "Entities/Objects/ContaminationCapacity.h"
#include "Entities/Objects/InterferenceCapacity.h"
#include "Entities/Objects/AvoidanceCapacity.h"
#include "Tools/MIL_Config.h"
#include "tools/Loader_ABC.h"
#include "tools/xmlcodecs.h"
#include "MT_Tools/MT_FormatString.h"
#include "MT_Tools/MT_ScipioException.h"
#include "MT_Tools/MT_Logger.h"
#include <xeumeuleu/xml.hpp>

// -----------------------------------------------------------------------------
// Name: DEC_PathFind_Manager constructor
// Created: NLD 2003-08-14
// -----------------------------------------------------------------------------
DEC_PathFind_Manager::DEC_PathFind_Manager( MIL_Config& config )
    : nMaxComputationDuration_( std::numeric_limits< unsigned int >::max() )
    , rDistanceThreshold_     ( 0. )
    , treatedRequests_        ( 0 )
{
    std::vector< unsigned int > dangerousObjects;
    MIL_DangerousObjectFilter filter;
    MIL_ObjectFactory::FindDangerousIDs( dangerousObjects, filter );

    const std::string fileLoaded = config.GetLoader().LoadPhysicalFile( "pathfinder", boost::bind( &DEC_PathFind_Manager::ReadPathfind, this, _1, dangerousObjects ) );
    config.AddFileToCRC( fileLoaded );

    bUseInSameThread_ = config.GetPathFinderThreads() == 0;
    MT_LOG_INFO_MSG( MT_FormatString( "Starting %d pathfind thread(s)", config.GetPathFinderThreads() ) );
    if( bUseInSameThread_ ) // juste one "thread" that will never start
        pathFindThreads_.push_back( & TER_World::GetWorld().GetPathFindManager().CreatePathFinderThread( *this, true ) );
    else
        for( unsigned int i = 0; i < config.GetPathFinderThreads(); ++i )
            pathFindThreads_.push_back( & TER_World::GetWorld().GetPathFindManager().CreatePathFinderThread( *this ) );
}

// -----------------------------------------------------------------------------
// Name: DEC_PathFind_Manager::ReadPathfind
// Created: LDC 2010-11-30
// -----------------------------------------------------------------------------
void DEC_PathFind_Manager::ReadPathfind( xml::xistream& xis, const std::vector< unsigned int >& dangerousObjects )
{
    xis >> xml::start( "pathfind" )
            >> xml::start( "configuration" )
                >> xml::attribute( "distance-threshold", rDistanceThreshold_ );
    if( tools::ReadTimeAttribute( xis, "max-calculation-time", nMaxComputationDuration_ ) && nMaxComputationDuration_ <= 0 )
        throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "Pathfind configuration : max-calculation-time w<= 0" );

    xis     >> xml::end;

    DEC_PathType   ::Initialize();
    DEC_PathFactory::Initialize( xis, dangerousObjects );

    xis >> xml::end;
}

// -----------------------------------------------------------------------------
// Name: DEC_PathFind_Manager destructor
// Created: NLD 2003-08-14
// -----------------------------------------------------------------------------
DEC_PathFind_Manager::~DEC_PathFind_Manager()
{
    DEC_PathFactory::Terminate();
    DEC_PathType::Terminate();
}

// -----------------------------------------------------------------------------
// Name: DEC_PathFind_Manager::StartCompute
// Created: NLD 2003-08-14
// -----------------------------------------------------------------------------
void DEC_PathFind_Manager::StartCompute( boost::shared_ptr< DEC_Path_ABC > path )
{
#ifdef _DEBUG
    MT_LOG_MESSAGE_MSG( MT_FormatString( "DEC_PathFind_Manager: New job pending : path 0x%p", path.get() ).c_str() );
#endif
    AddPendingJob( path );
}

// -----------------------------------------------------------------------------
// Name: DEC_PathFind_Manager::GetNbrShortRequests
// Created: NLD 2005-04-01
// -----------------------------------------------------------------------------
unsigned int DEC_PathFind_Manager::GetNbrShortRequests() const
{
    boost::mutex::scoped_lock locker( mutex_ );
    return shortRequests_.size();
}

// -----------------------------------------------------------------------------
// Name: DEC_PathFind_Manager::GetNbrLongRequests
// Created: NLD 2005-04-01
// -----------------------------------------------------------------------------
unsigned int DEC_PathFind_Manager::GetNbrLongRequests() const
{
    boost::mutex::scoped_lock locker( mutex_ );
    return longRequests_.size();
}

// -----------------------------------------------------------------------------
// Name: DEC_PathFind_Manager::GetNbrTreatedRequests
// Created: AGE 2006-04-20
// -----------------------------------------------------------------------------
unsigned int DEC_PathFind_Manager::GetNbrTreatedRequests() const
{
    return treatedRequests_;
}

// -----------------------------------------------------------------------------
// Name: DEC_PathFind_Manager::GetNbrRequests
// Created: AGE 2005-05-10
// -----------------------------------------------------------------------------
unsigned int DEC_PathFind_Manager::GetNbrRequests() const
{
    boost::mutex::scoped_lock locker( mutex_ );
    return longRequests_.size() + shortRequests_.size();
}

// -----------------------------------------------------------------------------
// Name: DEC_PathFind_Manager::AddPendingJob
// Created: NLD 2003-08-14
// -----------------------------------------------------------------------------
void DEC_PathFind_Manager::AddPendingJob( boost::shared_ptr< DEC_Path_ABC > pPath )
{
    boost::mutex::scoped_lock locker( mutex_ );
    if( pPath->GetLength() > rDistanceThreshold_ )
        longRequests_.push_back( pPath );
    else
        shortRequests_.push_back( pPath );
    condition_.notify_all();
}

// -----------------------------------------------------------------------------
// Name: DEC_PathFind_Manager::GetMessage
// Created: AGE 2005-02-25
// -----------------------------------------------------------------------------
boost::shared_ptr< TER_PathFindRequest_ABC > DEC_PathFind_Manager::GetMessage()
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

    static const unsigned maximumShortRequest = 5;
}

// -----------------------------------------------------------------------------
// Name: DEC_PathFind_Manager::GetRequests
// Created: AGE 2006-05-11
// -----------------------------------------------------------------------------
DEC_PathFind_Manager::T_Requests& DEC_PathFind_Manager::GetRequests()
{
    const bool shortHavePriority =
           ( ! shortRequests_.empty() )
           && ( pathFindThreads_.size() == 1 || shortRequests_.size() > maximumShortRequest );
    if( shortHavePriority || longRequests_.empty() )
        return shortRequests_;
    return longRequests_;
}

// -----------------------------------------------------------------------------
// Name: DEC_PathFind_Manager::GetMessage
// Created: AGE 2005-02-25
// -----------------------------------------------------------------------------
boost::shared_ptr< TER_PathFindRequest_ABC > DEC_PathFind_Manager::GetMessage( unsigned int nThread )
{
    boost::shared_ptr< TER_PathFindRequest_ABC > pRequest;
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
        T_Requests& requests = GetRequests();
        pRequest = requests.front();
        requests.pop_front();
    }
    ++treatedRequests_;
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
// Name: DEC_PathFind_Manager::CleanPathAfterComputation
// Created: NLD 2006-01-23
// -----------------------------------------------------------------------------
void DEC_PathFind_Manager::CleanPathAfterComputation( const boost::shared_ptr< TER_PathFindRequest_ABC >& pPath )
{
    boost::mutex::scoped_lock locker( cleanAndDestroyMutex_ );
    requestsToCleanAfterComputation_.push_back( pPath );
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
        boost::shared_ptr< TER_PathFindRequest_ABC > pRequest = requestsToCleanAfterComputation_.back();
        pRequest->CleanAfterComputation();
        requestsToCleanAfterComputation_.pop_back();
    }
}

// -----------------------------------------------------------------------------
// Name: DEC_PathFind_Manager::UpdateInSimulationThread
// Created: JSR 2010-06-16
// -----------------------------------------------------------------------------
void DEC_PathFind_Manager::UpdateInSimulationThread()
{
    if( bUseInSameThread_ ) // Pathfind in same thread than simulation
        while( ! shortRequests_.empty() || ! longRequests_.empty() )
        {
            T_Requests& requests = GetRequests();
            boost::shared_ptr< TER_PathFindRequest_ABC > pRequest = requests.front();
            requests.pop_front();
            pathFindThreads_[ 0 ]->ProcessInSimulationThread( pRequest );
            ++treatedRequests_;
        }
}
