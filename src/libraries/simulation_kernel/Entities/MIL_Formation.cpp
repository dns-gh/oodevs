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
#include "Entities/MIL_Army_ABC.h"
#include "Entities/Actions/PHY_ActionLogistic.h"
#include "Entities/Agents/Units/Categories/PHY_NatureLevel.h"
#include "Entities/Agents/Units/Logistic/PHY_LogisticLevel.h"
#include "Entities/Automates/MIL_Automate.h"
#include "Entities/Specialisations/LOG/MIL_AutomateLOG.h"
#include "Network/NET_Publisher_ABC.h"
#include "protocol/ClientSenders.h"
#include "simulation_kernel/FormationFactory_ABC.h"
#include "simulation_kernel/AutomateFactory_ABC.h"
#include "MT_Tools/MT_Logger.h"
#include "Tools/MIL_IDManager.h"
#include <xeumeuleu/xml.hpp>
#include <boost/serialization/map.hpp>
#include <boost/bind.hpp>
#include <boost/foreach.hpp>

class MIL_Intelligence;
namespace
{
    MIL_IDManager idManager_;
}

// -----------------------------------------------------------------------------
// Name: MIL_Formation constructor
// Created: NLD 2006-10-11
// -----------------------------------------------------------------------------
MIL_Formation::MIL_Formation( xml::xistream& xis, MIL_Army_ABC& army, MIL_Formation* pParent, FormationFactory_ABC& formationFactory, AutomateFactory_ABC& automateFactory )
    : MIL_Entity_ABC( xis )
    , nID_    ( xis.attribute< unsigned int >( "id" ) )
    , pArmy_  ( &army )
    , pParent_( pParent )
    , pLevel_ ( 0 )
{
    pLevel_ = PHY_NatureLevel::Find( xis.attribute< std::string >( "level" ) );
    if( !pLevel_ )
        xis.error( "Unknown level" );
    if( pParent_ )
        pParent_->RegisterFormation( *this );
    else
        pArmy_->RegisterFormation( *this );
    xis >> xml::list( "formation", *this, &MIL_Formation::InitializeFormation, formationFactory )
        >> xml::list( "automat", *this, &MIL_Formation::InitializeAutomate, automateFactory )
        >> xml::optional >> xml::start( "extensions" )
            >> xml::list( "entry", *this, &MIL_Formation::ReadExtension )
        >> xml::end;

    std::string logLevelStr(PHY_LogisticLevel::none_.GetName());
    xis >> xml::optional() >> xml::attribute("logistic-level", logLevelStr);
    const PHY_LogisticLevel* logLevel = PHY_LogisticLevel::Find(logLevelStr);
    if(logLevel==0 || PHY_LogisticLevel::tc2_==*logLevel)
        xis.error( "Wrong logistic level" );
    if(PHY_LogisticLevel::logistic_base_==*logLevel)
    {
        pBrainLogistic_.reset( new MIL_AutomateLOG( *this, *logLevel ) );
        pLogisticAction_.reset( new PHY_ActionLogistic<MIL_AutomateLOG>(*pBrainLogistic_.get() ) );
        this->RegisterAction( pLogisticAction_ );
    }
    idManager_.Lock( nID_ );
}

// -----------------------------------------------------------------------------
// Name: MIL_Formation constructor
// Created: LDC 2010-10-21
// -----------------------------------------------------------------------------
MIL_Formation::MIL_Formation( int level, const std::string& name, std::string logLevelStr, MIL_Army_ABC& army, MIL_Formation* parent )
    : MIL_Entity_ABC( name )
    , nID_    ( idManager_.GetFreeId() )
    , pArmy_  ( &army )
    , pParent_( parent )
    , pLevel_ ( 0 )
{
    pLevel_ = PHY_NatureLevel::Find( level );
    if( !pLevel_ )
        throw std::runtime_error( "Unknown level" );
    if( pParent_ )
        pParent_->RegisterFormation( *this );
    else
        pArmy_->RegisterFormation( *this );

    if( logLevelStr.empty() )
        logLevelStr = PHY_LogisticLevel::none_.GetName();
    const PHY_LogisticLevel* logLevel = PHY_LogisticLevel::Find( logLevelStr );
    if( logLevel==0 || PHY_LogisticLevel::tc2_==*logLevel )
        throw std::runtime_error( "Wrong logistic level" );
    if( PHY_LogisticLevel::logistic_base_==*logLevel )
    {
        pBrainLogistic_.reset( new MIL_AutomateLOG( *this, *logLevel ) );
        pLogisticAction_.reset( new PHY_ActionLogistic<MIL_AutomateLOG>(*pBrainLogistic_.get() ) );
        this->RegisterAction( pLogisticAction_ );
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_Formation constructor
// Created: NLD 2006-10-11
// -----------------------------------------------------------------------------
MIL_Formation::MIL_Formation( const std::string& name )
    : MIL_Entity_ABC( name )
    , nID_    ( 0 )
    , pArmy_  ( 0 )
    , pParent_( 0 )
    , pLevel_ ( 0 )
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
// Created: MGD 2009-10-22
// -----------------------------------------------------------------------------
void MIL_Formation::InitializeFormation( xml::xistream& xis, FormationFactory_ABC& formationFactory )
{
    formationFactory.Create( xis, *pArmy_, this );
}

// -----------------------------------------------------------------------------
// Name: MIL_Formation::InitializeSubordinates
// Created: MGD 2009-10-22
// -----------------------------------------------------------------------------
void MIL_Formation::InitializeAutomate( xml::xistream& xis, AutomateFactory_ABC& automateFactory )
{
    try
    {
        automateFactory.Create( xis, *this );
    }
    catch( std::exception& e )
    {
        MT_LOG_ERROR_MSG( e.what() );
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_Formation::ReadExtension
// Created: JSR 2010-10-11
// -----------------------------------------------------------------------------
void MIL_Formation::ReadExtension( xml::xistream& xis )
{
    extensions_[ xis.attribute< std::string >( "key" ) ] = xis.attribute< std::string >( "value" );
}

BOOST_CLASS_EXPORT_IMPLEMENT( MIL_Formation )

template< typename Archive >
void save_construct_data( Archive& archive, const MIL_Formation* formation, const unsigned int /*version*/ )
{
    archive << formation->GetName();
}

template< typename Archive >
void load_construct_data( Archive& archive, MIL_Formation* formation, const unsigned int /*version*/ )
{
    std::string name;
    archive >> name;
    ::new( formation )MIL_Formation( name );
}

namespace boost
{
    namespace serialization
    {
        template< typename Archive >
        inline
        void serialize( Archive& file, std::map< unsigned int, MIL_Intelligence* >& map, const unsigned int version )
        {
            split_free( file, map, version );
        }

        template< typename Archive >
        void save( Archive& file, const std::map< unsigned int, MIL_Intelligence* >& map, const unsigned int )
        {
            unsigned int size = map.size();
            file << size;
            for( std::map< unsigned int, MIL_Intelligence* >::const_iterator it = map.begin(); it != map.end(); ++it )
                file << it->first << it->second;
        }

        template< typename Archive >
        void load( Archive& file, std::map< unsigned int, MIL_Intelligence* >& map, const unsigned int )
        {
            unsigned int count;
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
void MIL_Formation::load( MIL_CheckPointInArchive& file, const unsigned int )
{
   file >> const_cast< unsigned int& >( nID_ )
        >> pArmy_
        >> pParent_;
   idManager_.Lock( nID_ );
   unsigned int nLevel;
   file >> nLevel;
   pLevel_ = PHY_NatureLevel::Find( nLevel );
   assert( pLevel_ );
   file >> tools::Resolver< MIL_Formation >::elements_
        >> tools::Resolver< MIL_Automate >::elements_;
}

// -----------------------------------------------------------------------------
// Name: MIL_Formation::save
// Created: NLD 2006-10-18
// -----------------------------------------------------------------------------
void MIL_Formation::save( MIL_CheckPointOutArchive& file, const unsigned int ) const
{
    assert( pLevel_ );
    unsigned int level = pLevel_->GetID();
    file << nID_
         << pArmy_
         << pParent_
         << level
         << tools::Resolver< MIL_Formation >::elements_
         << tools::Resolver< MIL_Automate >::elements_;
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
            << xml::attribute( "name", GetName() );
    tools::Resolver< MIL_Formation >::Apply( boost::bind( &MIL_Formation::WriteODB, _1, boost::ref( xos ) ) );
    tools::Resolver< MIL_Automate >::Apply( boost::bind( &MIL_Automate::WriteODB, _1, boost::ref( xos ) ) );
    if( !extensions_.empty() )
    {
        xos << xml::start( "extensions" );
        BOOST_FOREACH( const T_Extensions::value_type& extension, extensions_ )
        {
            xos << xml::start( "entry" )
                    << xml::attribute( "key", extension.first )
                    << xml::attribute( "value", extension.second )
                << xml::end;
        }
        xos << xml::end;
    }
    xos << xml::end; // formation
}

// -----------------------------------------------------------------------------
// Name: MIL_Formation::WriteLogisticLinksODB
// Created: NLD 2006-10-19
// -----------------------------------------------------------------------------
void MIL_Formation::WriteLogisticLinksODB( xml::xostream& xos ) const
{
    if( pBrainLogistic_.get() )
        pBrainLogistic_->WriteLogisticLinksODB( xos );
    tools::Resolver< MIL_Formation >::Apply( boost::bind( &MIL_Formation::WriteLogisticLinksODB, _1, boost::ref( xos ) ) );
    tools::Resolver< MIL_Automate >::Apply( boost::bind( &MIL_Automate::WriteLogisticLinksODB, _1, boost::ref( xos ) ) );
}

// -----------------------------------------------------------------------------
// Name: MIL_Formation::SendCreation
// Created: NLD 2006-10-13
// -----------------------------------------------------------------------------
void MIL_Formation::SendCreation() const
{
    assert( pLevel_ );
    assert( pArmy_ );
    client::FormationCreation message;
    message().mutable_formation()->set_id( nID_ );
    message().mutable_party()->set_id( pArmy_->GetID() );
    message().set_name( GetName() );
    message().set_level( pLevel_->GetAsnID() );
    message().set_app6symbol( "combat" );
    message().set_logistic_level( pBrainLogistic_.get() ?
        (sword::EnumLogisticLevel)pBrainLogistic_->GetLogisticLevel().GetID() : sword::none );
    if( pParent_ )
        message().mutable_parent()->set_id( pParent_->GetID() );
    for( std::map< std::string, std::string >::const_iterator it = extensions_.begin(); it != extensions_.end(); ++it )
    {
        sword::Extension_Entry* entry = message().mutable_extension()->add_entries();
        entry->set_name( it->first );
        entry->set_value( it->second );
    }
    message.Send( NET_Publisher_ABC::Publisher() );
    if( message().has_extension() )
        message().mutable_extension()->mutable_entries()->Clear();
    tools::Resolver< MIL_Formation >::Apply( boost::bind( &MIL_Formation::SendCreation, _1 ) );//@TODO MGD Move to factory
    tools::Resolver< MIL_Automate >::Apply( boost::bind( &MIL_Automate::SendCreation, _1, 0 ) );
}

// -----------------------------------------------------------------------------
// Name: MIL_Formation::SendFullState
// Created: NLD 2006-10-13
// -----------------------------------------------------------------------------
void MIL_Formation::SendFullState() const
{
    SendLogisticLinks();
    tools::Resolver< MIL_Formation >::Apply( boost::bind( &MIL_Formation::SendFullState, _1 ) );
    tools::Resolver< MIL_Automate >::Apply( boost::bind( &MIL_Automate::SendFullState, _1 ) );
}

// -----------------------------------------------------------------------------
// Name: MIL_Formation::GetArmy
// Created: NLD 2006-10-11
// -----------------------------------------------------------------------------
MIL_Army_ABC& MIL_Formation::GetArmy() const
{
    assert( pArmy_ );
    return *pArmy_;
}

// -----------------------------------------------------------------------------
// Name: MIL_Formation::GetID
// Created: NLD 2006-10-13
// -----------------------------------------------------------------------------
unsigned int MIL_Formation::GetID() const
{
    return nID_;
}
// -----------------------------------------------------------------------------
// Name: MIL_Formation::RegisterAutomate
// Created: NLD 2006-10-13
// -----------------------------------------------------------------------------
void MIL_Formation::RegisterAutomate( MIL_Automate& automate )
{
    tools::Resolver< MIL_Automate >::Register( automate.GetID(), automate );
}

// -----------------------------------------------------------------------------
// Name: MIL_Formation::UnregisterAutomate
// Created: NLD 2006-10-13
// -----------------------------------------------------------------------------
void MIL_Formation::UnregisterAutomate( MIL_Automate& automate )
{
    tools::Resolver< MIL_Automate >::Remove( automate.GetID() );
}

// -----------------------------------------------------------------------------
// Name: MIL_Formation::RegisterFormation
// Created: NLD 2006-10-13
// -----------------------------------------------------------------------------
void MIL_Formation::RegisterFormation( MIL_Formation& formation )
{
    tools::Resolver< MIL_Formation >::Register( formation.GetID(), formation );
}

// -----------------------------------------------------------------------------
// Name: MIL_Formation::UnregisterFormation
// Created: NLD 2006-10-13
// -----------------------------------------------------------------------------
void MIL_Formation::UnregisterFormation( MIL_Formation& formation )
{
   tools::Resolver< MIL_Formation >::Remove( formation.GetID() );
}

// -----------------------------------------------------------------------------
// Name: MIL_Formation::pBrainLogistic_
// Created: AHC 2010-09-27
// -----------------------------------------------------------------------------
MIL_AutomateLOG* MIL_Formation::GetBrainLogistic() const
{
    return pBrainLogistic_.get();
}

// -----------------------------------------------------------------------------
// Name: MIL_Formation::FindLogisticManager
// Created: AHC 2010-09-27
// -----------------------------------------------------------------------------
MIL_AutomateLOG* MIL_Formation::FindLogisticManager() const
{
   if( pBrainLogistic_.get() )
       return pBrainLogistic_.get();
   else if( pParent_ )
       return pParent_->FindLogisticManager();
   return 0;
}

namespace
{
    struct VisitorApplyer
    {
        VisitorApplyer( MIL_EntityVisitor_ABC< MIL_AgentPion >& visitor) : visitor_(visitor){}
        void operator()(const MIL_Formation& f) const
        {
            f.Apply( visitor_ );
        }
        void operator()(const MIL_Automate& f) const
        {
            f.Apply( visitor_ );
        }
        MIL_EntityVisitor_ABC< MIL_AgentPion >& visitor_;
    };
}

// -----------------------------------------------------------------------------
// Name: MIL_Formation::Apply
// Created: AHC 2010-09-28
// -----------------------------------------------------------------------------
void MIL_Formation::Apply( MIL_EntityVisitor_ABC< MIL_AgentPion >& visitor ) const
{
    VisitorApplyer applyer(visitor);
    tools::Resolver< MIL_Formation >::Apply(applyer);
    tools::Resolver< MIL_Automate >::Apply(applyer);
}

// -----------------------------------------------------------------------------
// Name: MIL_Formation::SendLogisticLinks
// Created: AHC 2010-10-13
// -----------------------------------------------------------------------------
void MIL_Formation::SendLogisticLinks() const
{
    if( pBrainLogistic_.get() )
        pBrainLogistic_->SendFullState();
}
