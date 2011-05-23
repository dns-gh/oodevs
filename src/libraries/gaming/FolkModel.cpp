// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "FolkModel.h"
#include "protocol/SimulationSenders.h"
#include "clients_kernel/Controller.h"
#include <boost/bind.hpp>
#include <algorithm>

// -----------------------------------------------------------------------------
// Name: FolkModel constructor
// Created: AGE 2007-09-04
// -----------------------------------------------------------------------------
FolkModel::FolkModel( kernel::Controller& controller )
    : controller_      ( controller )
    , edgeCount_       ( 0 )
    , dirty_           ( false )
    , currentContainer_( -1 )
    , currentProfile_  ( -1 )
    , currentActivity_ ( -1 )
{
    containers_.push_back( "pavement" );
    containers_.push_back( "road" );
    containers_.push_back( "office" );
    containers_.push_back( "residential" );
    containers_.push_back( "shop" );
}

// -----------------------------------------------------------------------------
// Name: FolkModel destructor
// Created: AGE 2007-09-04
// -----------------------------------------------------------------------------
FolkModel::~FolkModel()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: FolkModel::Update
// Created: AGE 2007-09-04
// -----------------------------------------------------------------------------
void FolkModel::Update( const sword::FolkCreation& creation )
{
    activities_.reserve( creation.activities().elem_size() );
    for( int i = 0; i < creation.activities().elem_size(); ++i )
        activities_.push_back( creation.activities().elem( i ).data() );
    profiles_.reserve( creation.profiles().elem_size() );
    for( int i = 0; i < creation.profiles().elem_size(); ++i )
        profiles_.push_back( creation.profiles().elem( i ).data() );
    edgeCount_ = creation.edge_number();

    boost::array< T_Values::index, 4 > shape = { edgeCount_, containers_.size(), profiles_.size(), activities_.size() };
    values_.reset( new T_Values( shape ) );
    ratios_.resize( edgeCount_ );

    controller_.Update( *this );
}

// -----------------------------------------------------------------------------
// Name: FolkModel::Update
// Created: AGE 2007-09-04
// -----------------------------------------------------------------------------
void FolkModel::Update( const sword::FolkGraphUpdate& update )
{
    for( int i = 0; i < update.elem_size(); ++i )
        Update( update.elem( i ) );
}

// -----------------------------------------------------------------------------
// Name: FolkModel::Update
// Created: AGE 2007-09-04
// -----------------------------------------------------------------------------
void FolkModel::Update( const sword::FolkGraphEdgeUpdate& update )
{
    T_Values& values = *values_;
    for( int i = 0; i < update.crowd_occupation_size(); ++i )
        *values[ update.shape_id() ].origin() = update.crowd_occupation( i );
    dirty_ = true;
}

namespace
{
    int Find( const QString& entry, const std::vector< QString >& names )
    {
        std::vector< QString >::const_iterator it = std::find( names.begin(), names.end(), entry );
        if( it != names.end() )
            return int( it - names.begin() );
        return -1;
    }
}

// -----------------------------------------------------------------------------
// Name: FolkModel::SwitchTo
// Created: AGE 2007-09-04
// -----------------------------------------------------------------------------
void FolkModel::SwitchTo( const QString& container, const QString& profile, const QString& activity )
{
    int con = Find( container, containers_ ),
        pro = Find( profile, profiles_ ),
        act = Find( activity, activities_ );
    if( con != currentContainer_ || pro != currentProfile_ || act != currentActivity_ )
    {
        currentContainer_ = con; currentProfile_ = pro; currentActivity_ = act;
        dirty_ = true;
    }
}

namespace
{
    typedef std::vector< int > T_Config;

    template< unsigned Size >
    struct Adder
    {
        template< typename Array, typename Config  >
        float operator()( const Array& array, const Config& config )
        {
            Adder< Size - 1 > next;
            if( config[ Size - 1 ] >= 0 )
                return next( array[ config[ Size - 1 ] ], config );
            float result = 0.f;
            for( Array::const_iterator it = array.begin(); it != array.end(); ++it )
                result += next( *it, config );
            return result;
        }
    };
    template< >
    struct Adder< 0 >
    {
        template< typename Array, typename Config  >
        float operator()( const Array& array, const Config& )
        {
            return float( array );
        }
    };

    template< typename Array, typename Config >
    float Sum( const Array& array, const Config& config )
    {
        return Adder< 3 >()( array, config );
    }
}

// -----------------------------------------------------------------------------
// Name: FolkModel::ComputeRatios
// Created: AGE 2007-09-04
// -----------------------------------------------------------------------------
void FolkModel::ComputeRatios()
{
    boost::array< int, 3 > config = { currentActivity_, currentProfile_, currentContainer_ };
    T_Values& values = *values_;
    for( unsigned i = 0; i < edgeCount_; ++i )
        ratios_[ i ] = Sum( values[ i ], config );
    dirty_ = false;
}

// -----------------------------------------------------------------------------
// Name: FolkModel::GetCurrentOccupation
// Created: AGE 2007-09-04
// -----------------------------------------------------------------------------
const std::vector< float >& FolkModel::GetCurrentOccupation()
{
    if( dirty_ )
        ComputeRatios();
    return ratios_;
}

// -----------------------------------------------------------------------------
// Name: FolkModel::Containers
// Created: AGE 2007-09-05
// -----------------------------------------------------------------------------
const FolkModel::T_Names& FolkModel::Containers() const
{
    return containers_;
}

// -----------------------------------------------------------------------------
// Name: FolkModel::Profiles
// Created: AGE 2007-09-05
// -----------------------------------------------------------------------------
const FolkModel::T_Names& FolkModel::Profiles() const
{
    return profiles_;
}

// -----------------------------------------------------------------------------
// Name: FolkModel::Activities
// Created: AGE 2007-09-05
// -----------------------------------------------------------------------------
const FolkModel::T_Names& FolkModel::Activities() const
{
    return activities_;
}
