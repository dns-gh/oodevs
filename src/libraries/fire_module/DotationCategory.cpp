// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "DotationCategory.h"
#include <wrapper/Hook.h>
#include <wrapper/View.h>
#include <module_api/Log.h>
#include <xeumeuleu/xml.hpp>
#include <boost/lambda/lambda.hpp>
#include <boost/make_shared.hpp>
#include <map>

DECLARE_HOOK( ModifyPh, double, ( const SWORD_Model* firer, const SWORD_Model* target, int dotation, double rPh ) )
DECLARE_HOOK( ModifyDangerosity, double, ( const SWORD_Model* compTarget, int dotation ) )
DECLARE_HOOK( HasDotation, bool, ( const SWORD_Model* entity, int dotation ) )
DECLARE_HOOK( CanFire, bool, ( const SWORD_Model* component, int dotation, const SWORD_Model* parameters ) )
DECLARE_HOOK( ReserveAmmunition, size_t, ( const SWORD_Model* firer, int dotation, size_t ammos ) )
DECLARE_HOOK( GetDotationValue, double, ( const SWORD_Model* entity, int dotation ) )

DEFINE_HOOK( InitializeDotations, 1, void, ( const char* xml ) )
{
    // $$$$ MCO : TODO : maybe we need to store configuration data in a model somehow ?
    sword::fire::DotationCategory::Initialize( xml::xistringstream( xml ) );
    if( GET_PREVIOUS_HOOK( InitializeDotations ) )
        GET_PREVIOUS_HOOK( InitializeDotations )( xml );
}

using namespace sword;
using namespace sword::fire;

namespace
{
    std::map< std::string, boost::shared_ptr< DotationCategory > > dotationsByName;
    std::map< int, boost::shared_ptr< DotationCategory > > dotationsByIdentifier;
}

//-----------------------------------------------------------------------------
// Name: DotationCategory::DotationCategory
// Created: NLD/JVT 2004-08-03
//-----------------------------------------------------------------------------
DotationCategory::DotationCategory( xml::xistream& xis )
    : id_        ( xis.attribute< int >( "id" ) )
    , directFire_( false )
{
    xis >> xml::list( "attritions", *this, &DotationCategory::ReadAttritions )
        >> xml::list( "indirect-fires", *this, &DotationCategory::ReadIndirectFires );
}

// -----------------------------------------------------------------------------
// Name: DotationCategory::Initialize
// Created: MCO 2012-06-21
// -----------------------------------------------------------------------------
void DotationCategory::Initialize( xml::xisubstream xis )
{
    ::SWORD_Log( SWORD_LOG_LEVEL_INFO, "Initializing dotations" );
    xis >> xml::start( "resources" )
            >> xml::list( "resource", &DotationCategory::ReadResource );
}

// -----------------------------------------------------------------------------
// Name: DotationCategory::ReadResource
// Created: MCO 2012-06-21
// -----------------------------------------------------------------------------
void DotationCategory::ReadResource( xml::xistream& xis )
{
    boost::shared_ptr< DotationCategory > category = boost::make_shared< DotationCategory >( boost::ref( xis ) );
    dotationsByIdentifier[ category->id_ ] = category;
    dotationsByName[ xis.attribute< std::string >( "name" ) ] = category;
}

// -----------------------------------------------------------------------------
// Name: DotationCategory::ReadAttritions
// Created: ABL 2007-07-19
// -----------------------------------------------------------------------------
void DotationCategory::ReadAttritions( xml::xistream& xis )
{
    xis >> xml::list( "attrition", boost::lambda::var( directFire_ ) = true );
}

// -----------------------------------------------------------------------------
// Name: DotationCategory::ReadIndirectFires
// Created: MCO 2012-06-22
// -----------------------------------------------------------------------------
void DotationCategory::ReadIndirectFires( xml::xistream& xis )
{
    xis >> xml::list( "indirect-fire", *this, &DotationCategory::ReadIndirectFire );
}

// -----------------------------------------------------------------------------
// Name: DotationCategory::ReadIndirectFire
// Created: MCO 2012-06-28
// -----------------------------------------------------------------------------
void DotationCategory::ReadIndirectFire( xml::xistream& xis )
{
    indirectTypes_.insert( xis.attribute< std::string >( "type" ) );
}

// -----------------------------------------------------------------------------
// Name: DotationCategory::FindDotationCategory
// Created: MCO 2012-06-21
// -----------------------------------------------------------------------------
boost::shared_ptr< DotationCategory > DotationCategory::FindDotationCategory( const std::string& name )
{
    auto it = dotationsByName.find( name );
    if( it == dotationsByName.end() )
        return boost::shared_ptr< DotationCategory >();
    return it->second;
}

// -----------------------------------------------------------------------------
// Name: boost::shared_ptr< DotationCategory > DotationCategory::FindDotationCategory
// Created: MCO 2012-12-18
// -----------------------------------------------------------------------------
boost::shared_ptr< DotationCategory > DotationCategory::FindDotationCategory( int identifier )
{
    auto it = dotationsByIdentifier.find( identifier );
    if( it == dotationsByIdentifier.end() )
        return boost::shared_ptr< DotationCategory >();
    return it->second;
}

// -----------------------------------------------------------------------------
// Name: DotationCategory::ModifyPh
// Created: MCO 2012-06-21
// -----------------------------------------------------------------------------
double DotationCategory::ModifyPh( const wrapper::View& firer, const wrapper::View& target, double ph ) const
{
    return GET_HOOK( ::ModifyPh )( firer, target, id_, ph );
}

// -----------------------------------------------------------------------------
// Name: DotationCategory::ModifyDangerosity
// Created: MCO 2012-06-21
// -----------------------------------------------------------------------------
double DotationCategory::ModifyDangerosity( const wrapper::View& compTarget ) const
{
    return GET_HOOK( ::ModifyDangerosity )( compTarget, id_ );
}

// -----------------------------------------------------------------------------
// Name: DotationCategory::GetIdentifier
// Created: MCO 2012-12-18
// -----------------------------------------------------------------------------
const int& DotationCategory::GetIdentifier() const
{
    return id_;
}

// -----------------------------------------------------------------------------
// Name: DotationCategory::HasDotation
// Created: MCO 2012-06-21
// -----------------------------------------------------------------------------
bool DotationCategory::HasDotation( const wrapper::View& firer ) const
{
    return GET_HOOK( ::HasDotation )( firer, id_ );
}

// -----------------------------------------------------------------------------
// Name: DotationCategory::CanFire
// Created: MCO 2012-06-21
// -----------------------------------------------------------------------------
bool DotationCategory::CanFire( const wrapper::View& component, const wrapper::View& parameters ) const
{
    return GET_HOOK( ::CanFire )( component, id_, parameters );
}

DECLARE_HOOK( CanComponentFire, bool, ( const SWORD_Model* component ) )

// -----------------------------------------------------------------------------
// Name: DotationCategory::CanFire
// Created: MCO 2012-06-27
// -----------------------------------------------------------------------------
bool DotationCategory::CanFire( const wrapper::View& component, const std::string& type ) const
{
    if( indirectTypes_.find( type ) == indirectTypes_.end() )
        return false;
    return GET_HOOK( CanComponentFire )( component );
}

// -----------------------------------------------------------------------------
// Name: DotationCategory::ReserveAmmunition
// Created: MCO 2012-06-21
// -----------------------------------------------------------------------------
std::size_t DotationCategory::ReserveAmmunition( const wrapper::View& firer, std::size_t ammos )
{
    return GET_HOOK( ::ReserveAmmunition )( firer, id_, ammos );
}

// -----------------------------------------------------------------------------
// Name: DotationCategory::CanBeUsedForDirectFire
// Created: MCO 2012-06-21
// -----------------------------------------------------------------------------
bool DotationCategory::CanBeUsedForDirectFire() const
{
    return directFire_;
}

// -----------------------------------------------------------------------------
// Name: DotationCategory::CanBeUsedForIndirectFire
// Created: MCO 2012-06-22
// -----------------------------------------------------------------------------
bool DotationCategory::CanBeUsedForIndirectFire() const
{
    return ! indirectTypes_.empty();
}

// -----------------------------------------------------------------------------
// Name: DotationCategory::GetValue
// Created: MCO 2012-06-28
// -----------------------------------------------------------------------------
double DotationCategory::GetValue( const wrapper::View& entity ) const
{
    return GET_HOOK( GetDotationValue )( entity, id_ );
}
