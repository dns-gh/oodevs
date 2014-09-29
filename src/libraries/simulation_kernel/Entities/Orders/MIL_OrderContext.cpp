//*****************************************************************************
//
// $Created: NLD 2002-07-15 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Orders/Pion/MIL_OrderContext.cpp $
// $Author: Nld $
// $Modtime: 21/03/05 12:17 $
// $Revision: 8 $
// $Workfile: MIL_OrderContext.cpp $
//
//*****************************************************************************

#include "simulation_kernel_pch.h"
#include "MIL_OrderContext.h"
#include "MIL_LimaFunction.h"
#include "Tools/NET_AsnException.h"
#include "Network/NET_ASN_Tools.h"
#include "protocol/Protocol.h"
#include "meteo/ReadDirections.h"
#include "MT_Tools/MT_Logger.h"

BOOST_CLASS_EXPORT_IMPLEMENT( MIL_OrderContext )

#define ORDER_BADLIMIT(reason)                     \
    MASA_BADPARAM_ASN( sword::OrderAck::ErrorCode, \
        sword::OrderAck::error_invalid_limit,      \
        static_cast< std::stringstream& >( std::stringstream() << reason ).str().c_str() )

namespace
{

const MT_Vector2D defaultDirection( 0, 1 );

const sword::MissionParameter& GetParam( const sword::MissionParameters& params,
        int index, const char* expected )
{
    if( params.elem_size() < index + 1 )
        throw ORDER_BADPARAM( "parameters[" << index << "] is missing, "
                << expected << " expected" );
    return params.elem( index );
}

MT_Vector2D ReadDirections( const sword::MissionParameters& params, int index )
{
    const auto& param = GetParam( params, index, "heading" );
    if( param.null_value() )
        return defaultDirection;
    if( !param.value_size() || !param.value( 0 ).has_heading() )
        throw ORDER_BADPARAM( "parameters[" << index << "] is invalid, heading expected" );
    return weather::ReadDirection( param.value( 0 ).heading() );
}

void ReadPhaseLines( const sword::MissionParameters& params, int index,
                     T_LimaVector& phaselines, const DEC_KnowledgeResolver_ABC& resolver )
{
    const auto& param = GetParam( params, index, "phaseline" );
    if( param.null_value() )
        return;
    if( !param.value_size() || !param.value( 0 ).has_phaseline() )
        throw ORDER_BADPARAM( "parameters[" << index
                << "] is invalid, phaseline expected" );
    const int count = param.value().size();
    for( int i = 0; i < count; ++i )
        phaselines.push_back( MIL_LimaOrder( param.value( i ).phaseline().elem( 0 ), resolver));
}

void ReadPointVector( const sword::MissionParameters& params, int index,
        T_PointVector& points )
{
    const auto& limit = GetParam( params, index, "limit" );
    if( limit.null_value() )
        return;
    if( !limit.value( 0 ).has_limit() )
        throw ORDER_BADLIMIT( "parameters[" << index << "] must be a limit" );
    if( !NET_ASN_Tools::ReadLine( limit.value( 0 ).limit(), points ) )
        throw ORDER_BADLIMIT( "parameters[" << index << "] limit value is invalid" );
}

} // namespace

// -----------------------------------------------------------------------------
// Name: MIL_OrderContext constructor
// Created: SBO 2008-03-03
// -----------------------------------------------------------------------------
MIL_OrderContext::MIL_OrderContext( bool present /*= false */ )
    : hasContext_( present )
    , dirDanger_ ( new MT_Vector2D( defaultDirection ) )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_OrderContext constructor
// Created: SBO 2008-03-03
// -----------------------------------------------------------------------------
MIL_OrderContext::MIL_OrderContext( const sword::MissionParameters& params,
        const MT_Vector2D& orientationReference, const DEC_KnowledgeResolver_ABC& resolver )
    : hasContext_( true )
    , dirDanger_ ( new MT_Vector2D( defaultDirection ) )
{
    *dirDanger_ = ReadDirections( params, 0 );
    ReadPhaseLines( params, 1, limas_, resolver );

    T_PointVector limit1, limit2;
    ReadPointVector( params, 2, limit1 );
    ReadPointVector( params, 3, limit2 );
    if( limit1.empty() != limit2.empty() || !limit1.empty() && limit1 == limit2 )
        throw ORDER_BADLIMIT(
            "limits parameters 2 and 3 must be both null/empty or different" );
    if( !limit1.empty() )
        fuseau_.Reset( &orientationReference, limit1, limit2,
            FindLima( MIL_LimaFunction::LDM_ ),
            FindLima( MIL_LimaFunction::LFM_ ) );
}

// -----------------------------------------------------------------------------
// Name: MIL_OrderContext constructor
// Created: NLD 2006-11-21
// -----------------------------------------------------------------------------
MIL_OrderContext::MIL_OrderContext( const MIL_OrderContext& rhs )
    : hasContext_   ( rhs.hasContext_ )
    , limas_        ( rhs.limas_  )
    , fuseau_       ( rhs.fuseau_ )
    , dirDanger_    ( rhs.dirDanger_ )
{
    // NOTHING
}

//-----------------------------------------------------------------------------
// Name: MIL_OrderContext destructor
// Created: NLD 2002-07-15
//-----------------------------------------------------------------------------
MIL_OrderContext::~MIL_OrderContext()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_OrderContext::Serialize
// Created: NLD 2006-11-14
// -----------------------------------------------------------------------------
void MIL_OrderContext::Serialize( sword::MissionParameters& asn ) const
{
    if( hasContext_ )
    {
        if( unsigned int( asn.elem_size() ) < Length() )
            throw MASA_EXCEPTION( "Invalid mission parameters." );
        WriteDirection( *asn.mutable_elem(0) );
        WritePhaseLines( *asn.mutable_elem(1) );
        WriteLimits( *asn.mutable_elem(2), *asn.mutable_elem(3) );
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_OrderContext::WritePhaseLines
// Created: SBO 2008-03-04
// -----------------------------------------------------------------------------
void MIL_OrderContext::WritePhaseLines( sword::MissionParameter& asn ) const
{
    asn.set_null_value( limas_.empty() );
    for( auto it = limas_.begin(); it != limas_.end(); ++it )
        it->Serialize( *asn.add_value()->mutable_phaseline()->add_elem() );
}

// -----------------------------------------------------------------------------
// Name: MIL_OrderContext::WriteLimits
// Created: SBO 2008-03-04
// -----------------------------------------------------------------------------
void MIL_OrderContext::WriteLimits( sword::MissionParameter& limit1, sword::MissionParameter& limit2 ) const
{
    const bool isValid = !fuseau_.IsNull();
    limit1.set_null_value( !isValid );
    limit2.set_null_value( !isValid );
    if( !limit1.null_value() )
    {
        NET_ASN_Tools::WriteLine( fuseau_.GetLeftLimit (), *limit1.add_value()->mutable_limit() );
        NET_ASN_Tools::WriteLine( fuseau_.GetRightLimit(), *limit2.add_value()->mutable_limit() );
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_OrderContext::WriteDirection
// Created: SBO 2008-03-04
// -----------------------------------------------------------------------------
void MIL_OrderContext::WriteDirection( sword::MissionParameter& asn ) const
{
    asn.set_null_value( false );
    NET_ASN_Tools::WriteDirection( *dirDanger_, *asn.add_value()->mutable_heading() );
}

// -----------------------------------------------------------------------------
// Name: MIL_OrderContext::Length
// Created: SBO 2008-03-04
// -----------------------------------------------------------------------------
unsigned int MIL_OrderContext::Length() const
{
    return hasContext_ ? 4 : 0;
}

// -----------------------------------------------------------------------------
// Name: MIL_OrderContext::GetDirDanger
// Created: NLD 2006-11-14
// -----------------------------------------------------------------------------
boost::shared_ptr< MT_Vector2D > MIL_OrderContext::GetDirDanger() const
{
    return dirDanger_;
}

// -----------------------------------------------------------------------------
// Name: MIL_OrderContext::GetFuseau
// Created: NLD 2006-11-14
// -----------------------------------------------------------------------------
const MIL_Fuseau& MIL_OrderContext::GetFuseau() const
{
    return fuseau_;
}

// -----------------------------------------------------------------------------
// Name: MIL_OrderContext::AffectFuseau
// Created: NLD 2006-11-16
// -----------------------------------------------------------------------------
void MIL_OrderContext::AffectFuseau( const MIL_Fuseau& fuseau )
{
    fuseau_ = fuseau;
}

// -----------------------------------------------------------------------------
// Name: MIL_OrderContext::AffectDirection
// Created: SBO 2008-01-04
// -----------------------------------------------------------------------------
void MIL_OrderContext::AffectDirection( const MT_Vector2D& direction )
{
    *dirDanger_ = direction;
}

// -----------------------------------------------------------------------------
// Name: MIL_OrderContext::GetLimas
// Created: NLD 2006-11-16
// -----------------------------------------------------------------------------
const T_LimaVector& MIL_OrderContext::GetLimas() const
{
    return limas_;
}

// -----------------------------------------------------------------------------
// Name: MIL_OrderContext::FindLima
// Created: NLD 2006-11-16
// -----------------------------------------------------------------------------
MIL_LimaOrder* MIL_OrderContext::FindLima( unsigned int nID )
{
    for( auto it = limas_.begin(); it != limas_.end(); ++it )
        if( it->GetID() == nID )
            return &(*it);
    return 0;
}

// -----------------------------------------------------------------------------
// Name: MIL_OrderContext::FindLima
// Created: NLD 2006-11-14
// -----------------------------------------------------------------------------
MIL_LimaOrder* MIL_OrderContext::FindLima( const MIL_LimaFunction& func )
{
    for( auto it = limas_.begin(); it != limas_.end(); ++it )
        if( it->HasFunction( func ) )
            return &(*it);
    return 0;
}

// -----------------------------------------------------------------------------
// Name: MIL_OrderContext::FindAllLimas
// Created: NMI 2013-04-30
// -----------------------------------------------------------------------------
std::vector< MIL_LimaOrder* > MIL_OrderContext::FindAllLimas( const MIL_LimaFunction& func )
{
    std::vector< MIL_LimaOrder* > vec;
    for( auto it = limas_.begin(); it != limas_.end(); ++it )
        if( it->HasFunction( func ) )
            vec.push_back(&(*it));
    return vec;
}

// -----------------------------------------------------------------------------
// Name: MIL_OrderContext::FindNextScheduledLima
// Created: NLD 2007-04-25
// -----------------------------------------------------------------------------
MIL_LimaOrder* MIL_OrderContext::FindNextScheduledLima()
{
    MIL_LimaOrder* pNextLima = 0;
    for( auto it = limas_.begin(); it != limas_.end(); ++it )
    {
        MIL_LimaOrder& lima = *it;
        if( lima.GetSchedule() == 0 || lima.IsScheduleFlagged() )
            continue;

        if( ( lima.GetSchedule() != 0 ) && ( !pNextLima || lima.GetSchedule() < pNextLima->GetSchedule() ) )
            pNextLima = &lima;
    }
    return pNextLima;
}

// -----------------------------------------------------------------------------
// Name: MIL_OrderContext::serialize
// Created: LGY 2011-06-07
// -----------------------------------------------------------------------------
template< typename Archive >
void MIL_OrderContext::serialize( Archive& file, const unsigned int )
{
    file & hasContext_;
    file & limas_;
    file & fuseau_;
    file & dirDanger_;
}
