// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Agents/Units/Dotations/PHY_DotationType.cpp $
// $Author: Nld $
// $Modtime: 17/03/05 17:27 $
// $Revision: 8 $
// $Workfile: PHY_DotationType.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "Entities/MIL_EntityManager.h"
#include "PHY_DotationType.h"
#include "PHY_DotationCategory.h"
#include "PHY_DotationLogisticType.h"
#include "MT_Tools/MT_Logger.h"
#include <boost/make_shared.hpp>

PHY_DotationType* PHY_DotationType::munition_ = 0;
PHY_DotationType* PHY_DotationType::carburant_ = 0;
PHY_DotationType* PHY_DotationType::explosif_ = 0;
PHY_DotationType* PHY_DotationType::mine_ = 0;
PHY_DotationType* PHY_DotationType::barbele_ = 0;
PHY_DotationType* PHY_DotationType::piece_ = 0;
PHY_DotationType* PHY_DotationType::ration_ = 0;
PHY_DotationType* PHY_DotationType::agentExtincteur_ = 0;
PHY_DotationType* PHY_DotationType::energie_ = 0;
PHY_DotationType* PHY_DotationType::funeraire_ = 0;

namespace
{
    std::map< std::string, PHY_DotationType* > types;
    std::map< std::string, boost::shared_ptr< const PHY_DotationCategory > > categoriesByName;
    std::map< unsigned int, boost::shared_ptr< const PHY_DotationCategory > > categoriesByIdentifier;
}

//-----------------------------------------------------------------------------
// Name: PHY_DotationType::Initialize
// Created: NLD/JVT 2004-08-03
//-----------------------------------------------------------------------------
void PHY_DotationType::Initialize( xml::xisubstream xis )
{
    MT_LOG_INFO_MSG( "Initializing dotation types" );
    PHY_DotationType::munition_        = new PHY_DotationType( "munition"         , sword::category_ammunition );
    PHY_DotationType::carburant_       = new PHY_DotationType( "carburant"        , sword::category_fuel );
    PHY_DotationType::explosif_        = new PHY_DotationType( "explosif"         , sword::category_explosive );
    PHY_DotationType::mine_            = new PHY_DotationType( "mine"             , sword::category_mine );
    PHY_DotationType::barbele_         = new PHY_DotationType( "barbele"          , sword::category_barbed_wire );
    PHY_DotationType::piece_           = new PHY_DotationType( "piece"            , sword::category_parts );
    PHY_DotationType::ration_          = new PHY_DotationType( "ration"           , sword::category_food_and_water );
    PHY_DotationType::agentExtincteur_ = new PHY_DotationType( "agent extincteur" , sword::category_extinguisher_agent );
    PHY_DotationType::energie_         = new PHY_DotationType( "energie"          , sword::category_energy );
    PHY_DotationType::funeraire_       = new PHY_DotationType( "funeraire"        , sword::category_funeral );
    types[ munition_       ->GetName() ] = munition_;
    types[ carburant_      ->GetName() ] = carburant_;
    types[ explosif_       ->GetName() ] = explosif_;
    types[ mine_           ->GetName() ] = mine_;
    types[ barbele_        ->GetName() ] = barbele_;
    types[ piece_          ->GetName() ] = piece_;
    types[ ration_         ->GetName() ] = ration_;
    types[ agentExtincteur_->GetName() ] = agentExtincteur_;
    types[ energie_        ->GetName() ] = energie_;
    types[ funeraire_      ->GetName() ] = funeraire_;
    xis >> xml::start( "resources" )
            >> xml::list( "resource", &PHY_DotationType::ReadDotation );
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationType::ReadDotation
// Created: ABL 2007-07-19
// -----------------------------------------------------------------------------
void PHY_DotationType::ReadDotation( xml::xistream& xis )
{
    auto it = types.find( xis.attribute< std::string >( "category" ) );
    if( it == types.end() )
        throw MASA_EXCEPTION( xis.context() + "Invalid dotation category name" );
    it->second->RegisterDotation( xis );
}

//-----------------------------------------------------------------------------
// Name: PHY_DotationType::Terminate
// Created: NLD/JVT 2004-08-03
//-----------------------------------------------------------------------------
void PHY_DotationType::Terminate()
{
    for( auto it = types.begin(); it != types.end(); ++it )
        delete it->second;
    types.clear();
    categoriesByName.clear();
    categoriesByIdentifier.clear();
}

//-----------------------------------------------------------------------------
// Name: PHY_DotationType::RegisterDotation
// Created: NLD/JVT 2004-08-03
//-----------------------------------------------------------------------------
void PHY_DotationType::RegisterDotation( xml::xistream& xis )
{
    const std::string name = xis.attribute< std::string >( "name" );
    boost::shared_ptr< const PHY_DotationCategory >& pCategory = categoriesByName[ name ];
    if( pCategory )
        throw MASA_EXCEPTION( xis.context() + "Dotation category '" + name + "' already registered" );
    pCategory = boost::make_shared< PHY_DotationCategory >( boost::cref( *this ), name, boost::ref( xis ) );
    const unsigned int id = xis.attribute< unsigned int >( "id" );
    boost::shared_ptr< const PHY_DotationCategory >& pCategoryID = categoriesByIdentifier[ id ];
    if( pCategoryID )
        throw MASA_EXCEPTION( xis.context() + "Dotation category ID already registered" );
    pCategoryID = pCategory;
}

//-----------------------------------------------------------------------------
// Name: PHY_DotationType constructor
// Created: NLD/JVT 2004-08-03
//-----------------------------------------------------------------------------
PHY_DotationType::PHY_DotationType( const std::string& name, sword::DotationCategory type )
    : name_( name )
    , type_( type )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationType::FindDotationType
// Created: NLD 2004-08-04
// -----------------------------------------------------------------------------
const PHY_DotationType* PHY_DotationType::FindDotationType( const std::string& name )
{
    auto it = types.find( name );
    if( it == types.end() )
        return 0;
    return it->second;
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationType::FindDotationType
// Created: NLD 2005-03-17
// -----------------------------------------------------------------------------
const PHY_DotationType* PHY_DotationType::FindDotationType( unsigned int id )
{
    for( auto it = types.begin(); it != types.end(); ++it )
        if( it->second->GetID() == id )
            return it->second;
    return 0;
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationType::FindDotationCategory
// Created: NLD 2005-02-03
// -----------------------------------------------------------------------------
const PHY_DotationCategory* PHY_DotationType::FindDotationCategory( unsigned int id )
{
    auto it = categoriesByIdentifier.find( id );
    if( it != categoriesByIdentifier.end() )
        return it->second.get();
    return 0;
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationType::FindDotationCategory
// Created: NLD 2006-10-19
// -----------------------------------------------------------------------------
const PHY_DotationCategory* PHY_DotationType::FindDotationCategory( const std::string& name )
{
    auto it = categoriesByName.find( name );
    if( it != categoriesByName.end() )
        return it->second.get();
    return 0;
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationType::GetName
// Created: NLD 2005-03-17
// -----------------------------------------------------------------------------
const std::string& PHY_DotationType::GetName() const
{
    return name_;
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationType::GetID
// Created: NLD 2005-03-17
// -----------------------------------------------------------------------------
unsigned int PHY_DotationType::GetID() const
{
    return type_;
}

// -----------------------------------------------------------------------------
// Name: PHY_DotationType::operator==
// Created: NLD 2005-02-28
// -----------------------------------------------------------------------------
bool PHY_DotationType::operator==( const PHY_DotationType& rhs ) const
{
    return rhs.type_ == type_;
}
