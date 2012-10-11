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
#include <map>

DECLARE_HOOK( ModifyPh, double, ( const SWORD_Model* firer, const SWORD_Model* target, const char* dotation, double rPh ) )
DECLARE_HOOK( ModifyDangerosity, double, ( const SWORD_Model* compTarget, const char* dotation ) )
DECLARE_HOOK( HasDotation, bool, ( const SWORD_Model* entity, const char* dotation ) )
DECLARE_HOOK( CanFire, bool, ( const SWORD_Model* component, const char* dotation, const SWORD_Model* parameters ) )
DECLARE_HOOK( ReserveAmmunition, size_t, ( const SWORD_Model* firer, const char* dotation, size_t ammos ) )
DECLARE_HOOK( GetDotationValue, double, ( const SWORD_Model* entity, const char* dotation ) )

DEFINE_HOOK( InitializeDotations, void, ( const char* xml ) )
{
    try
    {
        // $$$$ MCO : TODO : maybe we need to store configuration data in a model somehow ?
        sword::fire::DotationCategory::Initialize( xml::xistringstream( xml ) );
    }
    catch( std::exception& e )
    {
        ::SWORD_Log( SWORD_LOG_LEVEL_ERROR, e.what() );
    }
    catch( ... )
    {
        ::SWORD_Log( SWORD_LOG_LEVEL_ERROR, "Unknown exception during weapon type initialization" );
    }
    if( GET_PREVIOUS_HOOK( InitializeDotations ) )
        GET_PREVIOUS_HOOK( InitializeDotations )( xml );
}

using namespace sword;
using namespace sword::fire;

namespace
{
    typedef std::map< std::string, boost::shared_ptr< DotationCategory > > T_Dotations;
    T_Dotations dotations;
}

//-----------------------------------------------------------------------------
// Name: DotationCategory::DotationCategory
// Created: NLD/JVT 2004-08-03
//-----------------------------------------------------------------------------
DotationCategory::DotationCategory( xml::xistream& xis )
    : strName_   ( xis.attribute< std::string >( "name" ) )
    , directFire_( false )
{
    xis >> xml::list( "attritions", *this, &DotationCategory::ReadAttritions )
        >> xml::list( "indirect-fires", *this, &DotationCategory::ReadIndirectFires );
}

//-----------------------------------------------------------------------------
// Name: DotationCategory::~DotationCategory
// Created: NLD/JVT 2004-08-03
//-----------------------------------------------------------------------------
DotationCategory::~DotationCategory()
{
    // NOTHING
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
    const std::string name = xis.attribute< std::string >( "name" );
    dotations[ name ].reset( new DotationCategory( xis ) );
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
    T_Dotations::const_iterator it = dotations.find( name );
    if( it == dotations.end() )
        return boost::shared_ptr< DotationCategory >();
    return it->second;
}

// -----------------------------------------------------------------------------
// Name: DotationCategory::ModifyPh
// Created: MCO 2012-06-21
// -----------------------------------------------------------------------------
double DotationCategory::ModifyPh( const wrapper::View& firer, const wrapper::View& target, double ph ) const
{
    return GET_HOOK( ::ModifyPh )( firer, target, strName_.c_str(), ph );
}

// -----------------------------------------------------------------------------
// Name: DotationCategory::ModifyDangerosity
// Created: MCO 2012-06-21
// -----------------------------------------------------------------------------
double DotationCategory::ModifyDangerosity( const wrapper::View& compTarget ) const
{
    return GET_HOOK( ::ModifyDangerosity )( compTarget, strName_.c_str() );
}

// -----------------------------------------------------------------------------
// Name: DotationCategory::GetName
// Created: MCO 2012-06-21
// -----------------------------------------------------------------------------
const std::string& DotationCategory::GetName() const
{
    return strName_;
}

// -----------------------------------------------------------------------------
// Name: DotationCategory::HasDotation
// Created: MCO 2012-06-21
// -----------------------------------------------------------------------------
bool DotationCategory::HasDotation( const wrapper::View& firer ) const
{
    return GET_HOOK( ::HasDotation )( firer, strName_.c_str() );
}

// -----------------------------------------------------------------------------
// Name: DotationCategory::CanFire
// Created: MCO 2012-06-21
// -----------------------------------------------------------------------------
bool DotationCategory::CanFire( const wrapper::View& component, const wrapper::View& parameters ) const
{
    return GET_HOOK( ::CanFire )( component, strName_.c_str(), parameters );
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
    return GET_HOOK( ::ReserveAmmunition )( firer, strName_.c_str(), ammos );
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
    return GET_HOOK( GetDotationValue )( entity, strName_.c_str() );
}
