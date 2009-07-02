// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Agents/Units/Categories/PHY_RoePopulation.cpp $
// $Author: Nld $
// $Modtime: 18/03/05 12:13 $
// $Revision: 3 $
// $Workfile: PHY_RoePopulation.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "PHY_RoePopulation.h"

PHY_RoePopulation::T_RoePopulationMap PHY_RoePopulation::roePopulations_;
const PHY_RoePopulation               PHY_RoePopulation::none_( "None", PHY_RoePopulation::eNone, EnumPopulationRoe::none );

// -----------------------------------------------------------------------------
// Name: PHY_RoePopulation::Initialize
// Created: JVT 2004-12-09
// -----------------------------------------------------------------------------
void PHY_RoePopulation::Initialize()
{
    roePopulations_[ none_.GetName()  ] = &none_;
    roePopulations_[ "emploi force interdit"       ] = new PHY_RoePopulation( "Emploi force interdit"      , eEmploiForceInterdit      , EnumPopulationRoe::emploi_force_interdit );
    roePopulations_[ "maintien a distance par moyens non letaux"      ] = new PHY_RoePopulation( "Maintien � distance par moyens non l�taux"     , eMaintienADistanceParMoyensNonLetaux     , EnumPopulationRoe::maintien_a_distance_par_moyens_non_letaux ); 
    roePopulations_[ "dispersion par moyens de defense actifs"     ] = new PHY_RoePopulation( "Dispersion par moyens de d�fense actifs"    , eDispersionParMoyensDeDefenseActifs    , EnumPopulationRoe::dispersion_par_moyens_de_defense_actifs );
    roePopulations_[ "armes letales autorisees" ] = new PHY_RoePopulation( "Armes l�tales autoris�es", eArmesLetalesAutorisees, EnumPopulationRoe::armes_letales_autorisees );
}

// -----------------------------------------------------------------------------
// Name: PHY_RoePopulation::Terminate
// Created: JVT 2004-12-09
// -----------------------------------------------------------------------------
void PHY_RoePopulation::Terminate()
{
    roePopulations_.erase( none_.GetName() );
    for( CIT_RoePopulationMap it = roePopulations_.begin(); it != roePopulations_.end(); ++it )
        delete it->second;
    roePopulations_.clear();
}

// -----------------------------------------------------------------------------
// Name: PHY_RoePopulation constructor
// Created: JVT 2004-12-09
// -----------------------------------------------------------------------------
PHY_RoePopulation::PHY_RoePopulation( const std::string& strName, E_Type nType, ASN1T_EnumPopulationRoe nAsnID )
    : strName_ ( strName )
    , nType_   ( nType   )
    , nAsnID_  ( nAsnID  )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_RoePopulation destructor
// Created: JVT 2004-12-09
// -----------------------------------------------------------------------------
PHY_RoePopulation::~PHY_RoePopulation()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_RoePopulation::GetRoePopulations
// Created: NLD 2005-11-15
// -----------------------------------------------------------------------------
const PHY_RoePopulation::T_RoePopulationMap& PHY_RoePopulation::GetRoePopulations()
{
    return roePopulations_;
}

// -----------------------------------------------------------------------------
// Name: PHY_RoePopulation::Find
// Created: JVT 2004-12-09
// -----------------------------------------------------------------------------
const PHY_RoePopulation* PHY_RoePopulation::Find( const std::string& strName )
{
    CIT_RoePopulationMap it = roePopulations_.find( strName );
    return it == roePopulations_.end() ? 0 : it->second;
}

// -----------------------------------------------------------------------------
// Name: PHY_RoePopulation::Find
// Created: NLD 2005-11-15
// -----------------------------------------------------------------------------
const PHY_RoePopulation* PHY_RoePopulation::Find( uint nID )
{
    for( CIT_RoePopulationMap it = roePopulations_.begin(); it != roePopulations_.end(); ++it )
    {
        if( it->second->GetID() == nID )
            return it->second;
    }
    return 0;
}

// -----------------------------------------------------------------------------
// Name: PHY_RoePopulation::GetName
// Created: JVT 2004-12-09
// -----------------------------------------------------------------------------
const std::string& PHY_RoePopulation::GetName() const
{
    return strName_;
}

// -----------------------------------------------------------------------------
// Name: PHY_RoePopulation::GetID
// Created: JVT 2004-12-09
// -----------------------------------------------------------------------------
uint PHY_RoePopulation::GetID() const
{
    return nType_;
}

// -----------------------------------------------------------------------------
// Name: PHY_RoePopulation::GetAsnID
// Created: NLD 2005-11-15
// -----------------------------------------------------------------------------
ASN1T_EnumPopulationRoe PHY_RoePopulation::GetAsnID() const
{
    return nAsnID_;
}

// -----------------------------------------------------------------------------
// Name: PHY_RoePopulation::operator==
// Created: NLD 2005-11-15
// -----------------------------------------------------------------------------
bool PHY_RoePopulation::operator==( const PHY_RoePopulation& rhs ) const
{
    return nType_ == rhs.nType_;
}

// -----------------------------------------------------------------------------
// Name: PHY_RoePopulation::operator!=
// Created: NLD 2005-11-15
// -----------------------------------------------------------------------------
bool PHY_RoePopulation::operator!=( const PHY_RoePopulation& rhs ) const
{
    return nType_ != rhs.nType_;
}
