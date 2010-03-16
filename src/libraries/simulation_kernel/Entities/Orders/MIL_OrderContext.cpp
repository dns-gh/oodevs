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
#include "protocol/protocol.h"
#include "meteo/ReadDirections.h"

// -----------------------------------------------------------------------------
// Name: MIL_OrderContext constructor
// Created: SBO 2008-03-03
// -----------------------------------------------------------------------------
MIL_OrderContext::MIL_OrderContext( bool present /*= false */ )
    : hasContext_   ( present )
    , limas_        ()
    , intelligences_()
    , fuseau_       ()
    , dirDanger_    ( 0., 1. )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_OrderContext constructor
// Created: SBO 2008-03-03
// -----------------------------------------------------------------------------
MIL_OrderContext::MIL_OrderContext( const Common::MsgMissionParameters& asn, const MT_Vector2D& orientationReference )
    : hasContext_( true )
{
    if( unsigned int( asn.elem_size() ) < Length() )
        throw NET_AsnException< MsgsSimToClient::OrderAck_ErrorCode >( MsgsSimToClient::OrderAck_ErrorCode_error_invalid_mission_parameters );
    ReadDirection( asn.elem(0) );
    ReadPhaseLines( asn.elem(1) );
    ReadLimits( asn.elem(2), asn.elem(3), orientationReference );
    ReadIntelligences( asn.elem(4) );
}

// -----------------------------------------------------------------------------
// Name: MIL_OrderContext constructor
// Created: NLD 2006-11-21
// -----------------------------------------------------------------------------
MIL_OrderContext::MIL_OrderContext( const MIL_OrderContext& rhs )
    : hasContext_   ( rhs.hasContext_ )
    , limas_        ( rhs.limas_  )
    , intelligences_()
    , fuseau_       ()
    , dirDanger_    ( rhs.dirDanger_ )
{
    fuseau_ = rhs.fuseau_; // $$$$ SBO 2008-03-04: ...
    for( CIT_IntelligenceOrders it = rhs.intelligences_.begin(); it != rhs.intelligences_.end(); ++it )
        intelligences_.push_back( new MIL_IntelligenceOrder( **it ) );
}

//-----------------------------------------------------------------------------
// Name: MIL_OrderContext destructor
// Created: NLD 2002-07-15
//-----------------------------------------------------------------------------
MIL_OrderContext::~MIL_OrderContext()
{
    for( CIT_IntelligenceOrders it = intelligences_.begin(); it != intelligences_.end(); ++it )
        delete *it;
}

// -----------------------------------------------------------------------------
// Name: MIL_OrderContext::Serialize
// Created: NLD 2006-11-14
// -----------------------------------------------------------------------------
void MIL_OrderContext::Serialize( Common::MsgMissionParameters& asn ) const
{
    if( hasContext_ )
    {
        if( unsigned int( asn.elem_size() ) < Length() )
            throw std::runtime_error( __FUNCTION__ );
        WriteDirection( *asn.mutable_elem(0) );
        WritePhaseLines( *asn.mutable_elem(1) );
        WriteLimits( *asn.mutable_elem(2), *asn.mutable_elem(3) );
        WriteIntelligences( *asn.mutable_elem(4) );
    }
}

namespace
{
    void CleanPhaseLines( Common::MsgMissionParameter& asn )
    {
        if( asn.value().has_limasorder() )
        {
            for( int i = 0; i < asn.value().limasorder().elem_size(); ++i )
            {
                asn.mutable_value()->mutable_limasorder()->mutable_elem( i )->clear_lima();
                if( asn.value().limasorder().elem( i ).fonctions_size() > 0 )
                    asn.mutable_value()->mutable_limasorder()->mutable_elem( i )->Clear();
            }
            asn.mutable_value()->mutable_limasorder()->mutable_elem()->Clear();
        }
        asn.mutable_value()->mutable_limasorder()->Clear();
    }

    void CleanLimit( Common::MsgMissionParameter& asn )
    {
        if( asn.value().has_line() )
            asn.mutable_value()->mutable_line()->mutable_location()->mutable_coordinates()->Clear();
        asn.mutable_value()->mutable_line()->Clear();
    }

    void CleanIntelligences( Common::MsgMissionParameter& asn )
    {
        if( asn.value().has_intelligencelist() )
            asn.mutable_value()->mutable_intelligencelist()->mutable_elem()->Clear();
        asn.mutable_value()->mutable_intelligencelist()->Clear();
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_OrderContext::Accept
// Created: SBO 2007-11-12
// -----------------------------------------------------------------------------
void MIL_OrderContext::Accept( MIL_IntelligenceOrdersVisitor_ABC& visitor ) const
{
    for( CIT_IntelligenceOrders it = intelligences_.begin(); it != intelligences_.end(); ++it )
        visitor.Visit( **it );
}

// -----------------------------------------------------------------------------
// Name: MIL_OrderContext::ReadLimits
// Created: SBO 2008-03-04
// -----------------------------------------------------------------------------
void MIL_OrderContext::ReadLimits( const Common::MsgMissionParameter& limit1, const Common::MsgMissionParameter& limit2, const MT_Vector2D& orientationReference )
{
    if( !limit1.value().has_line() || !limit2.value().has_line() )
        throw NET_AsnException< MsgsSimToClient::OrderAck_ErrorCode >( MsgsSimToClient::OrderAck_ErrorCode_error_invalid_mission_parameters );
    if( limit1.null_value() != limit2.null_value() )
        throw NET_AsnException< MsgsSimToClient::OrderAck_ErrorCode >( MsgsSimToClient::OrderAck_ErrorCode_error_invalid_limit );
    if( limit1.null_value() )
        return;
    
    T_PointVector limit1Data, limit2Data;
    if(    !NET_ASN_Tools::ReadLine( limit1.value().line(), limit1Data )
        || !NET_ASN_Tools::ReadLine( limit2.value().line(), limit2Data ) )
        throw NET_AsnException< MsgsSimToClient::OrderAck_ErrorCode >( MsgsSimToClient::OrderAck_ErrorCode_error_invalid_limit );

    const bool equal  = limit1Data == limit2Data;
    const bool empty1 = limit1Data.empty();
    const bool empty2 = limit1Data.empty();
    if( ( empty1 || empty2 ) && !equal )
        throw NET_AsnException< MsgsSimToClient::OrderAck_ErrorCode >( MsgsSimToClient::OrderAck_ErrorCode_error_invalid_limit );
    if( !empty1 && equal )
        throw NET_AsnException< MsgsSimToClient::OrderAck_ErrorCode >( MsgsSimToClient::OrderAck_ErrorCode_error_invalid_limit );
    if( !empty1 && !empty2 )
        fuseau_.Reset( orientationReference, limit1Data, limit2Data, FindLima( MIL_LimaFunction::LDM_ ), FindLima( MIL_LimaFunction::LFM_ ) );
}

// -----------------------------------------------------------------------------
// Name: MIL_OrderContext::ReadPhaseLines
// Created: SBO 2008-03-04
// -----------------------------------------------------------------------------
void MIL_OrderContext::ReadPhaseLines( const Common::MsgMissionParameter& asn )
{
    if( !asn.value().has_limasorder() )
        throw NET_AsnException< MsgsSimToClient::OrderAck_ErrorCode >( MsgsSimToClient::OrderAck_ErrorCode_error_invalid_mission_parameters );
    if( !asn.null_value() )
        for( int i = 0; i < asn.value().limasorder().elem_size(); ++i )
            limas_.push_back( MIL_LimaOrder( asn.value().limasorder().elem(i) ) );
}

// -----------------------------------------------------------------------------
// Name: MIL_OrderContext::ReadDirection
// Created: SBO 2008-03-04
// -----------------------------------------------------------------------------
void MIL_OrderContext::ReadDirection( const Common::MsgMissionParameter& asn )
{
    if( !asn.value().has_heading() )
        throw NET_AsnException< MsgsSimToClient::OrderAck_ErrorCode >( MsgsSimToClient::OrderAck_ErrorCode_error_invalid_mission_parameters );
    if( !asn.null_value() )
        ReadDirections::ReadDirection( asn.value().heading(), dirDanger_ );
}

// -----------------------------------------------------------------------------
// Name: MIL_OrderContext::ReadIntelligences
// Created: SBO 2008-03-04
// -----------------------------------------------------------------------------
void MIL_OrderContext::ReadIntelligences( const Common::MsgMissionParameter& asn )
{
    if( !asn.value().has_intelligencelist() )
        throw NET_AsnException< MsgsSimToClient::OrderAck_ErrorCode >( MsgsSimToClient::OrderAck_ErrorCode_error_invalid_mission_parameters );
    if( !asn.has_null_value() )
        for( int i = 0; i < asn.value().intelligencelist().elem_size(); ++i )
            intelligences_.push_back( new MIL_IntelligenceOrder( asn.value().intelligencelist().elem(i) ) );
}

// -----------------------------------------------------------------------------
// Name: MIL_OrderContext::WritePhaseLines
// Created: SBO 2008-03-04
// -----------------------------------------------------------------------------
void MIL_OrderContext::WritePhaseLines( Common::MsgMissionParameter& asn ) const
{
    asn.set_null_value( 0 );
    asn.mutable_value()->mutable_limasorder(); // force initialization of message parameter
    for( CIT_LimaVector it = limas_.begin(); it != limas_.end(); ++it )
        it->Serialize( *asn.mutable_value()->mutable_limasorder()->add_elem() );
}

// -----------------------------------------------------------------------------
// Name: MIL_OrderContext::WriteLimits
// Created: SBO 2008-03-04
// -----------------------------------------------------------------------------
void MIL_OrderContext::WriteLimits( Common::MsgMissionParameter& limit1, Common::MsgMissionParameter& limit2 ) const
{
    limit1.set_null_value( !fuseau_.GetLeftLimit() || !fuseau_.GetRightLimit() );
    limit2.set_null_value( !fuseau_.GetLeftLimit() || !fuseau_.GetRightLimit() );
    if( !limit1.null_value() )
    {
        NET_ASN_Tools::WriteLine( fuseau_.GetLeftLimit ()->GetPoints(), *limit1.mutable_value()->mutable_line() );
        NET_ASN_Tools::WriteLine( fuseau_.GetRightLimit()->GetPoints(), *limit2.mutable_value()->mutable_line() );
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_OrderContext::WriteDirection
// Created: SBO 2008-03-04
// -----------------------------------------------------------------------------
void MIL_OrderContext::WriteDirection( Common::MsgMissionParameter& asn ) const
{
    asn.set_null_value( 0 );    
    NET_ASN_Tools::WriteDirection( dirDanger_, *asn.mutable_value()->mutable_heading() );
}

// -----------------------------------------------------------------------------
// Name: MIL_OrderContext::WriteIntelligences
// Created: SBO 2008-03-04
// -----------------------------------------------------------------------------
void MIL_OrderContext::WriteIntelligences( Common::MsgMissionParameter& asn ) const
{
    asn.set_null_value( 0 );
    asn.mutable_value()->mutable_intelligencelist();   // force initialization of message parameter
    for( CIT_IntelligenceOrders it = intelligences_.begin(); it != intelligences_.end(); ++it )
        (*it)->Serialize( *asn.mutable_value()->mutable_intelligencelist()->add_elem() );
}

// -----------------------------------------------------------------------------
// Name: MIL_OrderContext::Length
// Created: SBO 2008-03-04
// -----------------------------------------------------------------------------
unsigned int MIL_OrderContext::Length() const
{
    return hasContext_ ? 5 : 0;
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
