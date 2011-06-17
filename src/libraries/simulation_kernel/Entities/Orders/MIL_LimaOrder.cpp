// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "MIL_LimaOrder.h"
#include "MIL_LimaFunction.h"
#include "Network/NET_ASN_Tools.h"
#include "Network/NET_AsnException.h"
#include "simulation_terrain/TER_Localisation.h"
#include "MT_Tools/MT_Line.h"
#include "MIL.h"
#include "Checkpoints/SerializationTools.h"
#include "protocol/Protocol.h"

unsigned int MIL_LimaOrder::nNextID_ = 0;

BOOST_CLASS_EXPORT_IMPLEMENT( MIL_LimaOrder )

// -----------------------------------------------------------------------------
// Name: MIL_LimaOrder constructor
// Created: LGY 2011-06-07
// -----------------------------------------------------------------------------
MIL_LimaOrder::MIL_LimaOrder()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_LimaOrder constructor
// Created: NLD 2006-11-14
// -----------------------------------------------------------------------------
MIL_LimaOrder::MIL_LimaOrder( const sword::PhaseLineOrder& asn )
    : nID_          ( ++nNextID_ )
    , functions_    ()
    , bFlag_        ( false )
    , bScheduleFlag_( false )
{
    NET_ASN_Tools::ReadTick( asn.time(), nSchedule_);

    T_PointVector points;
    localisation_ = boost::shared_ptr< TER_Localisation >( new TER_Localisation( TER_Localisation::eLine, points ) );
    if( !NET_ASN_Tools::ReadLine( asn.line(), *localisation_ ) )
        throw NET_AsnException< sword::OrderAck_ErrorCode >( sword::OrderAck::error_invalid_lima );

    for( int i = 0; i < asn.fonctions_size(); ++i )
    {
        const MIL_LimaFunction* pFunction = MIL_LimaFunction::Find( asn.fonctions(i) );
        if( !pFunction )
            throw NET_AsnException< sword::OrderAck_ErrorCode >( sword::OrderAck::error_invalid_lima_function );
        functions_.insert( pFunction );
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_LimaOrder destructor
// Created: NLD 2006-11-14
// -----------------------------------------------------------------------------
MIL_LimaOrder::~MIL_LimaOrder()
{
    // NOTHING
}

//-----------------------------------------------------------------------------
// Name: MIL_LimaOrder::Intersect2D
// Created: JVT 02-12-05
// Modified: MGD 2010-10-28
//-----------------------------------------------------------------------------
bool MIL_LimaOrder::Intersect2D( const MT_Line& line, MT_Vector2D& vPos ) const      //$$$ devrait prendre un set en paramètre (plusieurs intersections possibles)
{
    TER_DistanceLess cmp( MT_Vector2D( 0,0 ) );
    T_PointSet collisions( cmp );
    if( localisation_->Intersect2D( line, collisions ) )
    {
        vPos = *collisions.begin();
        return true;
    }
    return false;
}

// -----------------------------------------------------------------------------
// Name: MIL_LimaOrder::Intersect2D
// Created: NLD 2003-08-22
// -----------------------------------------------------------------------------
bool MIL_LimaOrder::Intersect2D( const T_PointVector& polyline, T_PointSet& intersectionSet ) const
{
    intersectionSet.clear();

    MT_Vector2D vIntersect;

    CIT_PointVector itPoint = polyline.begin();
    const MT_Vector2D* pLastPoint = &*itPoint;
    for( ++itPoint; itPoint != polyline.end(); ++itPoint )
    {
        const MT_Vector2D* pCurPoint = &*itPoint;

        MT_Line lineTmp( *pLastPoint, *pCurPoint );

        if( Intersect2D( lineTmp, vIntersect ) )
            intersectionSet.insert( vIntersect );
        pLastPoint = pCurPoint;
    }
    return !intersectionSet.empty();
}

// -----------------------------------------------------------------------------
// Name: MIL_LimaOrder::Serialize
// Created: NLD 2006-11-14
// -----------------------------------------------------------------------------
void MIL_LimaOrder::Serialize( sword::PhaseLineOrder& asn ) const
{
    NET_ASN_Tools::WriteLine( *localisation_, *asn.mutable_line() );
    NET_ASN_Tools::WriteTick( nSchedule_, *asn.mutable_time() );

    for( CIT_LimaFunctions it = functions_.begin(); it != functions_.end(); ++it )
        asn.add_fonctions( (*it)->GetAsnID() );
}

// -----------------------------------------------------------------------------
// Name: MIL_LimaOrder::HasFunction
// Created: NLD 2006-11-14
// -----------------------------------------------------------------------------
bool MIL_LimaOrder::HasFunction( const MIL_LimaFunction& function ) const
{
    return functions_.find( &function ) != functions_.end();
}

// -----------------------------------------------------------------------------
// Name: MIL_LimaOrder::GetPoints
// Created: NLD 2006-11-14
// -----------------------------------------------------------------------------
const T_PointVector& MIL_LimaOrder::GetPoints() const
{
    return localisation_->GetPoints();
}

// -----------------------------------------------------------------------------
// Name: MIL_LimaOrder::GetLocalisation
// Created: MGD 2010-10-28
// -----------------------------------------------------------------------------
boost::shared_ptr< TER_Localisation > MIL_LimaOrder::GetLocalisation() const
{
    return localisation_;
}

// -----------------------------------------------------------------------------
// Name: MIL_LimaOrder::GetSchedule
// Created: NLD 2007-04-24
// -----------------------------------------------------------------------------
unsigned int MIL_LimaOrder::GetSchedule() const
{
    return nSchedule_;
}

// -----------------------------------------------------------------------------
// Name: MIL_LimaOrder::FlagSchedule
// Created: NLD 2007-04-24
// -----------------------------------------------------------------------------
void MIL_LimaOrder::FlagSchedule( bool bFlag )
{
    bScheduleFlag_ = bFlag;
}

// -----------------------------------------------------------------------------
// Name: MIL_LimaOrder::IsScheduleFlagged
// Created: NLD 2007-04-24
// -----------------------------------------------------------------------------
bool MIL_LimaOrder::IsScheduleFlagged() const
{
    return bScheduleFlag_;
}

// -----------------------------------------------------------------------------
// Name: MIL_LimaOrder::Flag
// Created: NLD 2006-11-16
// -----------------------------------------------------------------------------
void MIL_LimaOrder::Flag( bool bFlag )
{
    bFlag_ = bFlag;
}

// -----------------------------------------------------------------------------
// Name: MIL_LimaOrder::IsFlagged
// Created: NLD 2006-11-16
// -----------------------------------------------------------------------------
bool MIL_LimaOrder::IsFlagged() const
{
    return bFlag_;
}

// -----------------------------------------------------------------------------
// Name: MIL_LimaOrder::GetID
// Created: NLD 2006-11-16
// -----------------------------------------------------------------------------
unsigned int MIL_LimaOrder::GetID() const
{
    return nID_;
}

// -----------------------------------------------------------------------------
// Name: MIL_LimaOrder::GetFunctions
// Created: NLD 2006-11-16
// -----------------------------------------------------------------------------
const MIL_LimaOrder::T_LimaFunctions& MIL_LimaOrder::GetFunctions() const
{
    return functions_;
}

namespace boost
{
    namespace serialization
    {
        typedef std::set< const MIL_LimaFunction* > T_LimaFunctions;
        typedef T_LimaFunctions::const_iterator   CIT_LimaFunctions;

        // =============================================================================
        // T_LimaFunctions
        // =============================================================================
        template< typename Archive >
        inline
            void serialize( Archive& file, T_LimaFunctions& functions, const unsigned int nVersion )
        {
            split_free( file, functions, nVersion );
        }

        template< typename Archive >
        void save( Archive& file, const T_LimaFunctions& functions, const unsigned int )
        {
            std::size_t size = functions.size();
            file << size;
            for ( CIT_LimaFunctions it = functions.begin(); it != functions.end(); ++it )
            {
                unsigned id = (*it)->GetID();
                file << id;
            }
        }

        template< typename Archive >
        void load( Archive& file, T_LimaFunctions& functions, const unsigned int )
        {
            std::size_t nNbr;
            file >> nNbr;
            while ( nNbr-- )
            {
                unsigned int nID;
                file >> nID;
                functions.insert( MIL_LimaFunction::Find( nID ) );
            }
        }
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_LimaOrder::load
// Created: LGY 2011-06-07
// -----------------------------------------------------------------------------
void MIL_LimaOrder::load( MIL_CheckPointInArchive& file, const unsigned int )
{
    TER_Localisation* localisation = 0;
    file >> nID_
         >> localisation
         >> functions_
         >> bFlag_
         >> bScheduleFlag_
         >> nSchedule_
         >> nNextID_;
    localisation_.reset( new TER_Localisation( *localisation ) );
}

// -----------------------------------------------------------------------------
// Name: MIL_LimaOrder::save
// Created: LGY 2011-06-07
// -----------------------------------------------------------------------------
void MIL_LimaOrder::save( MIL_CheckPointOutArchive& file, const unsigned int ) const
{
    TER_Localisation* localisation = localisation_.get();
    file << nID_
         << localisation
         << functions_
         << bFlag_
         << bScheduleFlag_
         << nSchedule_
         << nNextID_;
}
