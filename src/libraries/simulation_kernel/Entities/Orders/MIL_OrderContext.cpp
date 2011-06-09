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
#include "TER_LimitData.h"
#include "MIL_AgentServer.h"
#include "MIL_IntelligenceOrder.h"
#include "MIL_IntelligenceOrdersVisitor_ABC.h"
#include "Network/NET_AsnException.h"
#include "Network/NET_ASN_Tools.h"
#include "protocol/Protocol.h"
#include "meteo/ReadDirections.h"

BOOST_CLASS_EXPORT_IMPLEMENT( MIL_OrderContext )

// -----------------------------------------------------------------------------
// Name: MIL_OrderContext constructor
// Created: SBO 2008-03-03
// -----------------------------------------------------------------------------
MIL_OrderContext::MIL_OrderContext( bool present /*= false */ )
    : hasContext_( present )
    , dirDanger_ ( 0., 1. )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_OrderContext constructor
// Created: SBO 2008-03-03
// -----------------------------------------------------------------------------
MIL_OrderContext::MIL_OrderContext( const sword::MissionParameters& asn, const MT_Vector2D& orientationReference )
    : hasContext_( true )
{
    if( unsigned int( asn.elem_size() ) < Length() )
        throw NET_AsnException< sword::OrderAck_ErrorCode >( sword::OrderAck::error_invalid_parameter );
    ReadDirection( asn.elem(0) );
    ReadPhaseLines( asn.elem(1) );
    ReadLimits( asn.elem(2), asn.elem(3), orientationReference );
}

// -----------------------------------------------------------------------------
// Name: MIL_OrderContext constructor
// Created: NLD 2006-11-21
// -----------------------------------------------------------------------------
MIL_OrderContext::MIL_OrderContext( const MIL_OrderContext& rhs )
    : hasContext_   ( rhs.hasContext_ )
    , limas_        ( rhs.limas_  )
    , fuseau_       ()
    , dirDanger_    ( rhs.dirDanger_ )
{
    fuseau_ = rhs.fuseau_; // $$$$ SBO 2008-03-04: ...
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
            throw std::runtime_error( __FUNCTION__ );
        WriteDirection( *asn.mutable_elem(0) );
        WritePhaseLines( *asn.mutable_elem(1) );
        WriteLimits( *asn.mutable_elem(2), *asn.mutable_elem(3) );
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_OrderContext::ReadLimits
// Created: SBO 2008-03-04
// -----------------------------------------------------------------------------
void MIL_OrderContext::ReadLimits( const sword::MissionParameter& limit1, const sword::MissionParameter& limit2, const MT_Vector2D& orientationReference )
{
    if( limit1.null_value() != limit2.null_value() )
        throw NET_AsnException< sword::OrderAck_ErrorCode >( sword::OrderAck::error_invalid_limit );
    if( limit1.null_value() )
        return;
    if( !limit1.value().Get( 0 ).has_limit() || !limit2.value().Get( 0 ).has_limit() )
        throw NET_AsnException< sword::OrderAck_ErrorCode >( sword::OrderAck::error_invalid_parameter );

    T_PointVector limit1Data, limit2Data;
    if( !NET_ASN_Tools::ReadLine( limit1.value().Get( 0 ).limit(), limit1Data )
        || !NET_ASN_Tools::ReadLine( limit2.value().Get( 0 ).limit(), limit2Data ) )
        throw NET_AsnException< sword::OrderAck_ErrorCode >( sword::OrderAck::error_invalid_limit );

    const bool equal  = limit1Data == limit2Data;
    const bool empty1 = limit1Data.empty();
    const bool empty2 = limit1Data.empty();
    if( ( empty1 || empty2 ) && !equal )
        throw NET_AsnException< sword::OrderAck_ErrorCode >( sword::OrderAck::error_invalid_limit );
    if( !empty1 && equal )
        throw NET_AsnException< sword::OrderAck_ErrorCode >( sword::OrderAck::error_invalid_limit );
    if( !empty1 && !empty2 )
        fuseau_.Reset( orientationReference, limit1Data, limit2Data, FindLima( MIL_LimaFunction::LDM_ ), FindLima( MIL_LimaFunction::LFM_ ) );
}

// -----------------------------------------------------------------------------
// Name: MIL_OrderContext::ReadPhaseLines
// Created: SBO 2008-03-04
// -----------------------------------------------------------------------------
void MIL_OrderContext::ReadPhaseLines( const sword::MissionParameter& asn )
{
    if( !asn.null_value() )
    {
        if( !asn.value_size() || !asn.value().Get( 0 ).has_phaseline() )
            throw NET_AsnException< sword::OrderAck_ErrorCode >( sword::OrderAck::error_invalid_parameter );
        for( int i = 0; i < asn.value().Get( 0 ).phaseline().elem_size(); ++i )
            limas_.push_back( MIL_LimaOrder( asn.value().Get( 0 ).phaseline().elem(i) ) );
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_OrderContext::ReadDirection
// Created: SBO 2008-03-04
// -----------------------------------------------------------------------------
void MIL_OrderContext::ReadDirection( const sword::MissionParameter& asn )
{
    if( !asn.null_value() )
    {
        if( !asn.value_size() || !asn.value().Get( 0 ).has_heading() )
            throw NET_AsnException< sword::OrderAck_ErrorCode >( sword::OrderAck::error_invalid_parameter );
        dirDanger_ = weather::ReadDirection( asn.value().Get( 0 ).heading() );
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_OrderContext::WritePhaseLines
// Created: SBO 2008-03-04
// -----------------------------------------------------------------------------
void MIL_OrderContext::WritePhaseLines( sword::MissionParameter& asn ) const
{
    asn.set_null_value( limas_.empty() );
    for( CIT_LimaVector it = limas_.begin(); it != limas_.end(); ++it )
        it->Serialize( *asn.mutable_value()->Add()->mutable_phaseline()->add_elem() );
}

// -----------------------------------------------------------------------------
// Name: MIL_OrderContext::WriteLimits
// Created: SBO 2008-03-04
// -----------------------------------------------------------------------------
void MIL_OrderContext::WriteLimits( sword::MissionParameter& limit1, sword::MissionParameter& limit2 ) const
{
    const bool isValid = fuseau_.GetLeftLimit() && fuseau_.GetRightLimit();
    limit1.set_null_value( !isValid );
    limit2.set_null_value( !isValid );
    if( !limit1.null_value() )
    {
        NET_ASN_Tools::WriteLine( fuseau_.GetLeftLimit ()->GetPoints(), *limit1.mutable_value()->Add()->mutable_limit() );
        NET_ASN_Tools::WriteLine( fuseau_.GetRightLimit()->GetPoints(), *limit2.mutable_value()->Add()->mutable_limit() );
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_OrderContext::WriteDirection
// Created: SBO 2008-03-04
// -----------------------------------------------------------------------------
void MIL_OrderContext::WriteDirection( sword::MissionParameter& asn ) const
{
    asn.set_null_value( false );
    NET_ASN_Tools::WriteDirection( dirDanger_, *asn.mutable_value()->Add()->mutable_heading() );
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
const MT_Vector2D& MIL_OrderContext::GetDirDanger() const
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
    dirDanger_ = direction;
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
    for( IT_LimaVector it = limas_.begin(); it != limas_.end(); ++it )
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
    for( IT_LimaVector it = limas_.begin(); it != limas_.end(); ++it )
        if( it->HasFunction( func ) )
            return &(*it);
    return 0;
}

// -----------------------------------------------------------------------------
// Name: MIL_OrderContext::FindNextScheduledLima
// Created: NLD 2007-04-25
// -----------------------------------------------------------------------------
MIL_LimaOrder* MIL_OrderContext::FindNextScheduledLima()
{
    MIL_LimaOrder* pNextLima = 0;
    for( IT_LimaVector it = limas_.begin(); it != limas_.end(); ++it )
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
    file & hasContext_
         & limas_
         & fuseau_
         & dirDanger_;
}
