// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "LauncherType.h"
#include <xeumeuleu/xml.hpp>
#include <wrapper/Hook.h>
#include <wrapper/View.h>
#include <module_api/Log.h>

DECLARE_HOOK( GetPhModificator, double, ( const SWORD_Model* firer, const SWORD_Model* target, const char* launcher ) )
DECLARE_HOOK( GetPhModificator2, double, ( const char* launcher ) )

DEFINE_HOOK( InitializeLaunchers, 1, void, ( const char* xml ) )
{
    // $$$$ MCO : TODO : maybe we need to store configuration data in a model somehow ?
    sword::fire::LauncherType::Initialize( xml::xistringstream( xml ) );
    if( GET_PREVIOUS_HOOK( InitializeLaunchers ) )
        GET_PREVIOUS_HOOK( InitializeLaunchers )( xml );
}

using namespace sword;
using namespace sword::fire;

LauncherType::T_LauncherTypeMap LauncherType::launcherTypes_;

// -----------------------------------------------------------------------------
// Name: LauncherType::Initialize
// Created: NLD 2004-08-04
// -----------------------------------------------------------------------------
void LauncherType::Initialize( xml::xisubstream xis )
{
    ::SWORD_Log( SWORD_LOG_LEVEL_INFO, "Initializing launchers" );
    xis >> xml::start( "launchers" )
            >> xml::list( "launcher", &LauncherType::ReadLauncher );
}

// -----------------------------------------------------------------------------
// Name: LauncherType::ReadLauncher
// Created: ABL 2007-07-20
// -----------------------------------------------------------------------------
void LauncherType::ReadLauncher( xml::xistream& xis )
{
    std::string name;
    xis >> xml::attribute( "name", name );
    boost::shared_ptr< LauncherType >& pType = launcherTypes_[ name ];
    if( pType )
        throw xml::exception( xis.context() + "Launcher type already registered" );
    pType.reset( new LauncherType( name, xis ) );
}

// -----------------------------------------------------------------------------
// Name: LauncherType constructor
// Created: NLD 2004-08-04
// -----------------------------------------------------------------------------
LauncherType::LauncherType( const std::string& strName, xml::xistream& xis )
    : strName_       ( strName )
    , bDirectFire_   ( false )
    , bIndirectFire_ ( false )
{
    InitializeForDirectFire( xis );
    InitializeForIndirectFire( xis );
}

// -----------------------------------------------------------------------------
// Name: LauncherType::InitializeForDirectFire
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
void LauncherType::InitializeForDirectFire( xml::xistream& xis )
{
    xis >> xml::list( "ph-modifiers", *this, &LauncherType::ReadDirect );
}

// -----------------------------------------------------------------------------
// Name: LauncherType::InitializeForIndirectFire
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
void LauncherType::InitializeForIndirectFire( xml::xistream& xis )
{
    xis >> xml::optional
        >> xml::attribute( "indirect-fire", bIndirectFire_ );
}

// -----------------------------------------------------------------------------
// Name: LauncherType::ReadDirect
// Created: ABL 2007-07-20
// -----------------------------------------------------------------------------
void LauncherType::ReadDirect( xml::xistream& /*xis*/ )
{
    bDirectFire_ = true;
}

// -----------------------------------------------------------------------------
// Name: LauncherType::GetPHModificator
// Created: NLD 2004-10-05
// -----------------------------------------------------------------------------
double LauncherType::GetPHModificator( const wrapper::View& firer, const wrapper::View& target ) const
{
    return GET_HOOK( GetPhModificator )( firer, target, strName_.c_str() );
}

// -----------------------------------------------------------------------------
// Name: LauncherType::GetPHModificator
// Created: NLD 2004-10-15
// -----------------------------------------------------------------------------
double LauncherType::GetPHModificator() const
{
    return GET_HOOK( GetPhModificator2 )( strName_.c_str() );
}

// -----------------------------------------------------------------------------
// Name: LauncherType::FindLauncherType
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
boost::shared_ptr< LauncherType > LauncherType::FindLauncherType( const std::string& strName )
{
    CIT_LauncherTypeMap it = launcherTypes_.find( strName );
    if( it == launcherTypes_.end() )
        return boost::shared_ptr< LauncherType >();
    return it->second;
}

// -----------------------------------------------------------------------------
// Name: LauncherType::CanDirectFire
// Created: NLD 2004-10-08
// -----------------------------------------------------------------------------
bool LauncherType::CanDirectFire() const
{
    return bDirectFire_;
}

// -----------------------------------------------------------------------------
// Name: LauncherType::CanIndirectFire
// Created: NLD 2004-10-08
// -----------------------------------------------------------------------------
bool LauncherType::CanIndirectFire() const
{
    return bIndirectFire_;
}
