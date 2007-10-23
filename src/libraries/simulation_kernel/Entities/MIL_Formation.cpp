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
#include "Entities/Automates/MIL_AutomateType.h"
#include "Entities/Automates/MIL_Automate.h"
#include "Entities/Orders/MIL_TacticalLineManager.h"
#include "Entities/MIL_Army.h"
#include "Entities/MIL_EntityManager.h"
#include "MIL_Intelligence.h"
#include "Network/NET_ASN_Messages.h"
#include "xeumeuleu/xml.h"

using namespace xml;

// -----------------------------------------------------------------------------
// Name: MIL_Formation constructor
// Created: NLD 2006-10-11
// -----------------------------------------------------------------------------
MIL_Formation::MIL_Formation( MIL_EntityManager& manager, MIL_TacticalLineManager& tacticalLines, uint nID, MIL_Army& army, xml::xistream& xis, MIL_Formation* pParent )
    : nID_       ( nID )
    , pArmy_     ( &army )
    , pParent_   ( pParent )
    , pLevel_    ( 0 )
    , strName_   ()
    , formations_()
    , automates_ ()
{
    std::string strLevel;
    xis >> attribute( "name", strName_ )
        >> attribute( "level", strLevel );

    pLevel_ = PHY_NatureLevel::Find( strLevel );
    if( !pLevel_ )
        xis.error( "Unknown level" );

    if( pParent )
        pParent_->RegisterFormation( *this );
    else
        pArmy_->RegisterFormation( *this );

    InitializeSubordinates( manager, tacticalLines, xis );
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
    for( CIT_Intelligences it = intelligences_.begin(); it != intelligences_.end(); ++it )
        delete it->second;
}

// -----------------------------------------------------------------------------
// Name: MIL_Formation::InitializeSubordinates
// Created: AGE 2007-08-22
// -----------------------------------------------------------------------------
void MIL_Formation::InitializeSubordinates( MIL_EntityManager& manager, MIL_TacticalLineManager& tacticalLines, xml::xistream& xis )
{
    assert( pArmy_ );
    xis >> list( "formation"   , manager, &MIL_EntityManager::CreateFormation, *pArmy_, this )
        >> list( "automat"     , *this,   &MIL_Formation::CreateAutomat, manager, *this )
        >> list( "limit"       , *this,   &MIL_Formation::CreateLimit, tacticalLines )
        >> list( "lima"        , *this,   &MIL_Formation::CreateLima,  tacticalLines )
        >> list( "intelligence", *this,   &MIL_Formation::CreateIntelligence );
}

// -----------------------------------------------------------------------------
// Name: MIL_Formation::CreateAutomat
// Created: NLD 2007-09-07
// -----------------------------------------------------------------------------
void MIL_Formation::CreateAutomat( xml::xistream& xis, MIL_EntityManager& manager, MIL_Formation& formation )
{
    manager.CreateAutomat( xis, formation );
}

// -----------------------------------------------------------------------------
// Name: MIL_Formation::CreateLimit
// Created: AGE 2007-08-23
// -----------------------------------------------------------------------------
void MIL_Formation::CreateLimit( xml::xistream& xis, MIL_TacticalLineManager& tacticalLines )
{
    tacticalLines.CreateLimit( xis, *this );
}

// -----------------------------------------------------------------------------
// Name: MIL_Formation::CreateLima
// Created: AGE 2007-08-23
// -----------------------------------------------------------------------------
void MIL_Formation::CreateLima( xml::xistream& xis, MIL_TacticalLineManager& tacticalLines )
{
    tacticalLines.CreateLima( xis, *this );
}

// -----------------------------------------------------------------------------
// Name: MIL_Formation::CreateIntelligence
// Created: SBO 2007-10-22
// -----------------------------------------------------------------------------
void MIL_Formation::CreateIntelligence( xml::xistream& xis )
{
    std::auto_ptr< MIL_Intelligence > intelligence( new MIL_Intelligence( xis, *this ) );
    intelligences_[intelligence->GetId()] = intelligence.get();
    intelligence.release();
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
        >> automates_
        >> intelligences_;
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
         << automates_
         << intelligences_;
}

// -----------------------------------------------------------------------------
// Name: MIL_Formation::WriteODB
// Created: NLD 2006-10-18
// -----------------------------------------------------------------------------
void MIL_Formation::WriteODB( xml::xostream& xos ) const
{
    assert( pLevel_ );

    xos << start( "formation" )
            << attribute( "id", nID_ )
            << attribute( "level", pLevel_->GetName() )
            << attribute( "name", strName_ );

    for( CIT_FormationSet it = formations_.begin(); it != formations_.end(); ++it )
        (**it).WriteODB( xos );

    for( CIT_AutomateSet it = automates_.begin(); it != automates_.end(); ++it )
        (**it).WriteODB( xos );

    for( CIT_Intelligences it = intelligences_.begin(); it != intelligences_.end(); ++it )
        it->second->WriteODB( xos );

    xos << end(); // formation
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

    for( CIT_Intelligences it = intelligences_.begin(); it != intelligences_.end(); ++it )
        it->second->SendCreation();
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

    for( CIT_Intelligences it = intelligences_.begin(); it != intelligences_.end(); ++it )
        it->second->SendFullState();
}

// -----------------------------------------------------------------------------
// Name: MIL_Formation::Update
// Created: SBO 2007-10-22
// -----------------------------------------------------------------------------
void MIL_Formation::Update( const ASN1T_MsgIntelligenceCreationRequest& message )
{
    std::auto_ptr< MIL_Intelligence > intelligence( new MIL_Intelligence( message, *this ) );
    intelligences_[intelligence->GetId()] = intelligence.get();
    intelligence->SendCreation();
    intelligence.release();

    // $$$$ SBO 2007-10-22: add more error handling...
    NET_ASN_MsgIntelligenceCreationRequestAck ack;
    ack().error_code = EnumIntelligenceErrorCode::no_error;
    ack.Send();
}

// -----------------------------------------------------------------------------
// Name: MIL_Formation::Update
// Created: SBO 2007-10-22
// -----------------------------------------------------------------------------
void MIL_Formation::Update( const ASN1T_MsgIntelligenceDestructionRequest& message )
{
    NET_ASN_MsgIntelligenceDestructionRequestAck ack;
    T_Intelligences::iterator it = intelligences_.find( message.oid );
    if( it != intelligences_.end() )
    {
        it->second->SendDestruction();
        delete it->second;
        intelligences_.erase( it );
        ack().error_code = EnumIntelligenceErrorCode::no_error;
    }
    else
        ack().error_code = EnumIntelligenceErrorCode::error_invalid_oid;
    ack.Send();
}
