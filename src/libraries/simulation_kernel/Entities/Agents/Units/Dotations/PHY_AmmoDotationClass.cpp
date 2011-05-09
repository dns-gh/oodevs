// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Units/Dotations/PHY_AmmoDotationClass.cpp $
// $Author: Nld $
// $Modtime: 4/05/05 16:40 $
// $Revision: 2 $
// $Workfile: PHY_AmmoDotationClass.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "PHY_AmmoDotationClass.h"
#include "protocol/Protocol.h"
#include "MT_Tools/MT_Logger.h"
#include "MT_Tools/MT_String.h"

const PHY_AmmoDotationClass PHY_AmmoDotationClass::obus_      ( "Obus"      , 0 );
const PHY_AmmoDotationClass PHY_AmmoDotationClass::missileAir_( "MissileAir", 1 );
const PHY_AmmoDotationClass PHY_AmmoDotationClass::missileSol_( "MissileSol", 2 );
const PHY_AmmoDotationClass PHY_AmmoDotationClass::mitraille_ ( "Mitraille" , 3 );
const PHY_AmmoDotationClass PHY_AmmoDotationClass::alr_       ( "ALR" , 4 );

PHY_AmmoDotationClass::T_TypeMap PHY_AmmoDotationClass::types_;

// -----------------------------------------------------------------------------
// Name: PHY_AmmoDotationClass::Initialize
// Created: NLD 2004-10-08
// -----------------------------------------------------------------------------
void PHY_AmmoDotationClass::Initialize()
{
    MT_LOG_INFO_MSG( "Initializing ammo dotation classes" );
    types_[ obus_.GetID() ] = &obus_;
    types_[ missileAir_.GetID() ] = &missileAir_;
    types_[ missileSol_.GetID() ] = &missileSol_;
    types_[ mitraille_.GetID() ] = &mitraille_;
    types_[ alr_.GetID() ] = &alr_;
}

// -----------------------------------------------------------------------------
// Name: PHY_AmmoDotationClass::Terminate
// Created: NLD 2004-10-08
// -----------------------------------------------------------------------------
void PHY_AmmoDotationClass::Terminate()
{
    types_.clear();
}

// -----------------------------------------------------------------------------
// Name: PHY_AmmoDotationClass constructor
// Created: NLD 2004-10-08
// -----------------------------------------------------------------------------
PHY_AmmoDotationClass::PHY_AmmoDotationClass( const std::string& strName, int id )
    : strName_( strName )
    , id_     ( id )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_AmmoDotationClass destructor
// Created: NLD 2004-10-08
// -----------------------------------------------------------------------------
PHY_AmmoDotationClass::~PHY_AmmoDotationClass()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_AmmoDotationClass::Find
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
const PHY_AmmoDotationClass* PHY_AmmoDotationClass::Find( int nID )
{
    CIT_TypeMap it = types_.find( nID );
    if( it == types_.end() )
        return 0;
    return it->second;
}

// -----------------------------------------------------------------------------
// Name: PHY_AmmoDotationClass::Find
// Created: NLD 2004-10-08
// -----------------------------------------------------------------------------
const PHY_AmmoDotationClass* PHY_AmmoDotationClass::Find( const std::string& strName )
{
    for( CIT_TypeMap it = types_.begin(); it != types_.end(); ++it )
        if( sCaseInsensitiveEqual()( strName, it->second->GetName() ) )
            return it->second;
    return 0;
}

// -----------------------------------------------------------------------------
// Name: PHY_AmmoDotationClass::GetID
// Created: NLD 2004-10-08
// -----------------------------------------------------------------------------
int PHY_AmmoDotationClass::GetID() const
{
    return id_;
}

// -----------------------------------------------------------------------------
// Name: PHY_AmmoDotationClass::GetName
// Created: NLD 2004-10-08
// -----------------------------------------------------------------------------
const std::string& PHY_AmmoDotationClass::GetName() const
{
    return strName_;
}

// -----------------------------------------------------------------------------
// Name: PHY_AmmoDotationClass::operator==
// Created: NLD 2004-10-11
// -----------------------------------------------------------------------------
bool PHY_AmmoDotationClass::operator==( const PHY_AmmoDotationClass& rhs ) const
{
    return id_ == rhs.id_;
}

// -----------------------------------------------------------------------------
// Name: PHY_AmmoDotationClass::operator!=
// Created: NLD 2004-10-11
// -----------------------------------------------------------------------------
bool PHY_AmmoDotationClass::operator!=( const PHY_AmmoDotationClass& rhs ) const
{
    return id_ != rhs.id_;
}
