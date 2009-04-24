// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_kernel_pch.h"

#include "MIL_Formation.h"

#include "Entities/Agents/Units/Categories/PHY_NatureLevel.h"
#include "Entities/Automates/MIL_Automate.h"
#include "Entities/MIL_Army.h"
#include "Entities/MIL_EntityManager.h"
#include "Network/NET_ASN_Messages.h"
#include <xeumeuleu/xml.h>



// -----------------------------------------------------------------------------
// Name: MIL_Formation constructor
// Created: NLD 2006-10-11
// -----------------------------------------------------------------------------
MIL_Formation::MIL_Formation( MIL_EntityManager& manager, uint nID, MIL_Army& army, xml::xistream& xis, MIL_Formation* pParent )
    : nID_       ( nID )
    , pArmy_     ( &army )
    , pParent_   ( pParent )
    , pLevel_    ( 0 )
    , strName_   ()
    , formations_()
    , automates_ ()
{
    std::string strLevel;
    xis >> xml::attribute( "name", strName_ )
        >> xml::attribute( "level", strLevel );

    pLevel_ = PHY_NatureLevel::Find( strLevel );
    if( !pLevel_ )
        xis.error( "Unknown level" );

    if( pParent )
        pParent_->RegisterFormation( *this );
    else
        pArmy_->RegisterFormation( *this );

    InitializeSubordinates( manager, xis );
}

// -----------------------------------------------------------------------------
// Name: MIL_Formation constructor
// Created: NLD 2006-10-11
// -----------------------------------------------------------------------------
MIL_Formation::MIL_Formation()
    : nID_       ( 0 )
    , pArmy_     ( 0 )
    , pParent_   ( 0 )
    , pLevel_    ( 0 )
    , strName_   ()
    , formations_()
    , automates_ ()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_Formation destructor
// Created: NLD 2006-10-11
// -----------------------------------------------------------------------------
MIL_Formation::~MIL_Formation()
{
    assert( pArmy_ );
    if( pParent_ )
        pParent_->UnregisterFormation( *this );
    else
        pArmy_->UnregisterFormation( *this );
}

// -----------------------------------------------------------------------------
// Name: MIL_Formation::InitializeSubordinates
// Created: AGE 2007-08-22
// -----------------------------------------------------------------------------
void MIL_Formation::InitializeSubordinates( MIL_EntityManager& manager, xml::xistream& xis )
{
    assert( pArmy_ );
    xis >> xml::list( "formation"   , manager, &MIL_EntityManager::CreateFormation, *pArmy_, this )
        >> xml::list( "automat"     , *this,   &MIL_Formation::CreateAutomat, manager, *this ); 
}

// -----------------------------------------------------------------------------
// Name: MIL_Formation::CreateAutomat
// Created: NLD 2007-09-07
// -----------------------------------------------------------------------------
void MIL_Formation::CreateAutomat( xml::xistream& xis, MIL_EntityManager& manager, MIL_Formation& formation )
{
    manager.CreateAutomat( xis, formation );
}

// =============================================================================
// CHECKPOINTS
// =============================================================================

namespace boost
{
    namespace serialization
    {
        template< typename Archive >
        inline
        void serialize( Archive& file, std::map< unsigned int, MIL_Intelligence* >& map, const uint version )
        {
            split_free( file, map, version );
        }
        
        template< typename Archive >
        void save( Archive& file, const std::map< unsigned int, MIL_Intelligence* >& map, const uint )
        {
            unsigned int size = map.size();
            file << size;
            for( std::map< unsigned int, MIL_Intelligence* >::const_iterator it = map.begin(); it != map.end(); ++it )
                file << it->first << it->second;
        }
        
        template< typename Archive >
        void load( Archive& file, std::map< unsigned int, MIL_Intelligence* >& map, const uint )
        {
            uint count;
            file >> count;
            while( count-- )
            {
                unsigned int element;
                file >> element;
                file >> map[ element ];
            }
        }
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_Formation::load
// Created: NLD 2006-10-18
// -----------------------------------------------------------------------------
void MIL_Formation::load( MIL_CheckPointInArchive& file, const uint )
{
   file >> const_cast< uint& >( nID_ )
        >> pArmy_
        >> pParent_;

   uint nLevel;
   file >> nLevel;
   pLevel_ = PHY_NatureLevel::Find( nLevel );
   assert( pLevel_ );

   file >> strName_
        >> formations_
        >> automates_ ;
}

// -----------------------------------------------------------------------------
// Name: MIL_Formation::save
// Created: NLD 2006-10-18
// -----------------------------------------------------------------------------
void MIL_Formation::save( MIL_CheckPointOutArchive& file, const uint ) const
{
    assert( pLevel_ );
    unsigned level = pLevel_->GetID();
    file << nID_
         << pArmy_
         << pParent_
         << level
         << strName_
         << formations_
         << automates_ ;
}

// -----------------------------------------------------------------------------
// Name: MIL_Formation::WriteODB
// Created: NLD 2006-10-18
// -----------------------------------------------------------------------------
void MIL_Formation::WriteODB( xml::xostream& xos ) const
{
    assert( pLevel_ );

    xos << xml::start( "formation" )
            << xml::attribute( "id", nID_ )
            << xml::attribute( "level", pLevel_->GetName() )
            << xml::attribute( "name", strName_ );

    for( CIT_FormationSet it = formations_.begin(); it != formations_.end(); ++it )
        (**it).WriteODB( xos );

    for( CIT_AutomateSet it = automates_.begin(); it != automates_.end(); ++it )
        (**it).WriteODB( xos );

    xos << xml::end(); // formation
}

// -----------------------------------------------------------------------------
// Name: MIL_Formation::WriteLogisticLinksODB
// Created: NLD 2006-10-19
// -----------------------------------------------------------------------------
void MIL_Formation::WriteLogisticLinksODB( xml::xostream& xos ) const
{
    for( CIT_FormationSet it = formations_.begin(); it != formations_.end(); ++it )
        (**it).WriteLogisticLinksODB( xos );

    for( CIT_AutomateSet it = automates_.begin(); it != automates_.end(); ++it )
        (**it).WriteLogisticLinksODB( xos );
}

// =============================================================================
// NETWORK
// ============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_Formation::SendCreation
// Created: NLD 2006-10-13
// -----------------------------------------------------------------------------
void MIL_Formation::SendCreation() const
{
    assert( pLevel_ );
    assert( pArmy_ );

    NET_ASN_MsgFormationCreation asn;
    asn().oid      = nID_;
    asn().oid_camp = pArmy_->GetID();
    asn().nom      = strName_.c_str();
    asn().niveau   = pLevel_->GetAsnID();
    if( pParent_ )
    {
        asn().m.oid_formation_parentePresent = 1;
        asn().oid_formation_parente = pParent_->GetID();
    }
    asn.Send();

    for( CIT_FormationSet it = formations_.begin(); it != formations_.end(); ++it )
        (**it).SendCreation();

    for( CIT_AutomateSet it = automates_.begin(); it != automates_.end(); ++it )
        (**it).SendCreation();
}

// -----------------------------------------------------------------------------
// Name: MIL_Formation::SendFullState
// Created: NLD 2006-10-13
// -----------------------------------------------------------------------------
void MIL_Formation::SendFullState() const
{
    for( CIT_FormationSet it = formations_.begin(); it != formations_.end(); ++it )
        (**it).SendFullState();

    for( CIT_AutomateSet it = automates_.begin(); it != automates_.end(); ++it )
        (**it).SendFullState();

}

// -----------------------------------------------------------------------------
// Name: MIL_Formation::GetArmy
// Created: NLD 2006-10-11
// -----------------------------------------------------------------------------
MIL_Army& MIL_Formation::GetArmy() const
{
    assert( pArmy_ );
    return *pArmy_;
}

// -----------------------------------------------------------------------------
// Name: MIL_Formation::GetID
// Created: NLD 2006-10-13
// -----------------------------------------------------------------------------
uint MIL_Formation::GetID() const
{
    return nID_;
}

// -----------------------------------------------------------------------------
// Name: MIL_Formation::RegisterAutomate
// Created: NLD 2006-10-13
// -----------------------------------------------------------------------------
void MIL_Formation::RegisterAutomate( MIL_Automate& automate )
{
    assert( automates_.find( &automate ) == automates_.end() );
    automates_.insert( &automate );
}

// -----------------------------------------------------------------------------
// Name: MIL_Formation::UnregisterAutomate
// Created: NLD 2006-10-13
// -----------------------------------------------------------------------------
void MIL_Formation::UnregisterAutomate( MIL_Automate& automate )
{
    assert( automates_.find( &automate ) != automates_.end() );
    automates_.erase( &automate );
}

// -----------------------------------------------------------------------------
// Name: MIL_Formation::RegisterFormation
// Created: NLD 2006-10-13
// -----------------------------------------------------------------------------
void MIL_Formation::RegisterFormation( MIL_Formation& formation )
{
    assert( formations_.find( &formation ) == formations_.end() );
    formations_.insert( &formation );
}

// -----------------------------------------------------------------------------
// Name: MIL_Formation::UnregisterFormation
// Created: NLD 2006-10-13
// -----------------------------------------------------------------------------
void MIL_Formation::UnregisterFormation( MIL_Formation& formation )
{
    assert( formations_.find( &formation ) != formations_.end() );
    formations_.erase( &formation );
}
