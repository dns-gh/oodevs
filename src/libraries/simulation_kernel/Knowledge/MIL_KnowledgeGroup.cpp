// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Knowledge/MIL_KnowledgeGroup.cpp $
// $Author: Jvt $
// $Modtime: 14/04/05 18:07 $
// $Revision: 9 $
// $Workfile: MIL_KnowledgeGroup.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "MIL_KnowledgeGroup.h"
#include "KnowledgeGroupFactory_ABC.h"
#include "Network/NET_ASN_Messages.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Agents/Roles/Location/PHY_RoleInterface_Location.h"
#include "Entities/Automates/MIL_Automate.h"
#include "Entities/MIL_Army.h"
#include "DEC_Knowledge_Agent.h"
#include "DEC_KnowledgeBlackBoard_KnowledgeGroup.h"
#include <xeumeuleu/xml.h>

std::set< uint > MIL_KnowledgeGroup::ids_;

BOOST_CLASS_EXPORT_GUID( MIL_KnowledgeGroup, "MIL_KnowledgeGroup" )

// -----------------------------------------------------------------------------
// Name: MIL_KnowledgeGroup constructor
// Created: NLD 2004-08-11
// -----------------------------------------------------------------------------
MIL_KnowledgeGroup::MIL_KnowledgeGroup( const MIL_KnowledgeGroupType& type, uint nID, MIL_Army& army )
    : pType_               ( &type )
    , nID_                 ( nID )
    , pArmy_               ( &army )
    , pKnowledgeBlackBoard_( new DEC_KnowledgeBlackBoard_KnowledgeGroup( *this ) )
    , automates_           ()
{
    pArmy_->RegisterKnowledgeGroup( *this );
    if( !ids_.insert( nID_ ).second )
        throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, MT_FormatString( "KnowledgeGroup id %d is already used", nID_ ) );
}

// -----------------------------------------------------------------------------
// Name: MIL_KnowledgeGroup constructor
// Created: SLG 2009-11-11
// -----------------------------------------------------------------------------
MIL_KnowledgeGroup::MIL_KnowledgeGroup( xml::xistream& xis, MIL_Army& army, MIL_KnowledgeGroup* pParent, KnowledgeGroupFactory_ABC& knowledgeGroupFactory )
    : nID_                  ( 0 )
    , pArmy_                ( &army )
    , pParent_              ( pParent )
    , pKnowledgeBlackBoard_ ( new DEC_KnowledgeBlackBoard_KnowledgeGroup( *this ) )
    , automates_            ()
{
    std::string strType;
    xis >> xml::attribute( "id", nID_ )
        >> xml::attribute( "type", strType );
    pType_ = MIL_KnowledgeGroupType::FindType( strType );

    if( pParent_ )
        pParent_->RegisterKnowledgeGroup( *this );
    else
        pArmy_->RegisterKnowledgeGroup( *this );

    xis >> xml::list( "knowledge-group", *this, &MIL_KnowledgeGroup::InitializeKnowledgeGroup, knowledgeGroupFactory );
}

// -----------------------------------------------------------------------------
// Name: MIL_KnowledgeGroup constructor
// Created: JVT 2005-03-15
// -----------------------------------------------------------------------------
MIL_KnowledgeGroup::MIL_KnowledgeGroup()
    : pType_               ( 0 )
    , nID_                 ( 0 )
    , pArmy_               ( 0 )
    , pKnowledgeBlackBoard_( 0 )
    , automates_           ()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_KnowledgeGroup destructor
// Created: NLD 2004-08-11
// -----------------------------------------------------------------------------
MIL_KnowledgeGroup::~MIL_KnowledgeGroup()
{
    pArmy_->UnregisterKnowledgeGroup( *this );
    delete pKnowledgeBlackBoard_;
    ids_.erase( nID_ );
}

// -----------------------------------------------------------------------------
// Name: MIL_KnowledgeGroup::InitializeKnowledgeGroup
// Created: SLG 2009-11-11
// -----------------------------------------------------------------------------
void MIL_KnowledgeGroup::InitializeKnowledgeGroup( xml::xistream& xis, KnowledgeGroupFactory_ABC& knowledgeGroupFactory )
{
    knowledgeGroupFactory.Create( xis, *pArmy_, this );
}

// -----------------------------------------------------------------------------
// Name: MIL_KnowledgeGroup::load
// Created: JVT 2005-03-24
// -----------------------------------------------------------------------------
void MIL_KnowledgeGroup::load( MIL_CheckPointInArchive& file, const uint )
{
    uint nTypeID;
    file >> nTypeID;
    pType_ = MIL_KnowledgeGroupType::FindType( nTypeID );
    
    file >> const_cast< uint& >( nID_ )
         >> pArmy_
         >> pKnowledgeBlackBoard_
         >> automates_;
         
    ids_.insert( nID_ );
}

// -----------------------------------------------------------------------------
// Name: MIL_KnowledgeGroup::save
// Created: JVT 2005-03-24
// -----------------------------------------------------------------------------
void MIL_KnowledgeGroup::save( MIL_CheckPointOutArchive& file, const uint ) const
{
    assert( pType_ );
    unsigned type = pType_->GetID();
    file << type
         << nID_
         << pArmy_
         << pParent_
         << pKnowledgeBlackBoard_
         << automates_;
}

// -----------------------------------------------------------------------------
// Name: MIL_KnowledgeGroup::WriteODB
// Created: NLD 2006-05-29
// -----------------------------------------------------------------------------
void MIL_KnowledgeGroup::WriteODB( xml::xostream& xos ) const
{
    assert( pType_ );
    xos << xml::start( "knowledge-group" )
            << xml::attribute( "id", nID_ )
            << xml::attribute( "type", pType_->GetName() )
        << xml::end();

    tools::Resolver< MIL_KnowledgeGroup >::Apply( boost::bind( &MIL_KnowledgeGroup::WriteODB, _1, boost::ref(xos) ) );
}

// -----------------------------------------------------------------------------
// Name: MIL_KnowledgeGroup::UpdateKnowledges
// Created: NLD 2004-08-19
// -----------------------------------------------------------------------------
void MIL_KnowledgeGroup::UpdateKnowledges()
{
    for( CIT_AutomateVector it = automates_.begin(); it != automates_.end(); ++it )
        (**it).UpdateKnowledges();

    tools::Resolver< MIL_KnowledgeGroup >::Apply( boost::bind( &MIL_KnowledgeGroup::UpdateKnowledges, _1 ) );

    assert( pKnowledgeBlackBoard_ );
    pKnowledgeBlackBoard_->Update();
}

// -----------------------------------------------------------------------------
// Name: MIL_KnowledgeGroup::CleanKnowledges
// Created: NLD 2004-08-19
// -----------------------------------------------------------------------------
void MIL_KnowledgeGroup::CleanKnowledges()
{
    assert( pKnowledgeBlackBoard_ );
    pKnowledgeBlackBoard_->Clean();

    tools::Resolver< MIL_KnowledgeGroup >::Apply( boost::bind( &MIL_KnowledgeGroup::CleanKnowledges, _1 ) );

    for( CIT_AutomateVector it = automates_.begin(); it != automates_.end(); ++it )
        (**it).CleanKnowledges();
}

// -----------------------------------------------------------------------------
// Name: MIL_KnowledgeGroup::IsPerceived
// Created: NLD 2004-03-22
// -----------------------------------------------------------------------------
bool MIL_KnowledgeGroup::IsPerceived( const DEC_Knowledge_Agent& knowledge ) const
{
    // Hack : Pour éviter les destructions de connaissances dues à la posture du l'unité source
    //  => On triche en forcant la connaissance à ne pas être détruite si la connaissance et l'
    //     unité réelle se trouvent au même endroit
    if( knowledge.GetPosition() == knowledge.GetAgentKnown().GetRole< PHY_RoleInterface_Location >().GetPosition() )
        return false;

    for( CIT_AutomateVector it = automates_.begin(); it != automates_.end(); ++it )
        if( (*it)->IsPerceived( knowledge ) )
            return true;
    return false;
}

// -----------------------------------------------------------------------------
// Name: MIL_KnowledgeGroup::IsPerceived
// Created: NLD 2004-03-22
// -----------------------------------------------------------------------------
bool MIL_KnowledgeGroup::IsPerceived( const DEC_Knowledge_Object& knowledge ) const
{
    for( CIT_AutomateVector it = automates_.begin(); it != automates_.end(); ++it )
        if( (*it)->IsPerceived( knowledge ) )
            return true;
    return false;
}

// -----------------------------------------------------------------------------
// Name: MIL_KnowledgeGroup::SendCreation
// Created: NLD 2004-09-06
// -----------------------------------------------------------------------------
void MIL_KnowledgeGroup::SendCreation() const
{
    assert( pArmy_ );

    NET_ASN_MsgKnowledgeGroupCreation asn;   
    asn().oid      = nID_;
    asn().oid_camp = pArmy_->GetID();

    if( pParent_ )
    {
        asn().m.oid_knowledgegroup_parentePresent = 1;
        asn().oid_knowledgegroup_parente = pParent_->GetID();
    }
    asn.Send();
    tools::Resolver< MIL_KnowledgeGroup >::Apply( boost::bind( &MIL_KnowledgeGroup::SendCreation, _1 ) );//SLG : @TODO MGD Move to factory
}

// -----------------------------------------------------------------------------
// Name: MIL_KnowledgeGroup::SendFullState
// Created: SLG 2009-11-13
// -----------------------------------------------------------------------------
void MIL_KnowledgeGroup::SendFullState() const
{
    tools::Resolver< MIL_KnowledgeGroup >::Apply( boost::bind( &MIL_KnowledgeGroup::SendFullState, _1 ) );
}

// -----------------------------------------------------------------------------
// Name: MIL_KnowledgeGroup::SendKnowledge
// Created: NLD 2004-09-06
// -----------------------------------------------------------------------------
void MIL_KnowledgeGroup::SendKnowledge() const
{
    assert( pKnowledgeBlackBoard_ );
    pKnowledgeBlackBoard_->SendFullState();   

    for( CIT_AutomateVector it = automates_.begin(); it != automates_.end(); ++it )
        (**it).SendKnowledge();

    tools::Resolver< MIL_KnowledgeGroup >::Apply( boost::bind( &MIL_KnowledgeGroup::SendKnowledge, _1 ) );// SLG TEST : to diffuse in the tree
}

// -----------------------------------------------------------------------------
// Name: MIL_KnowledgeGroup::operator==
// Created: NLD 2004-08-30
// -----------------------------------------------------------------------------
bool MIL_KnowledgeGroup::operator==( const MIL_KnowledgeGroup& rhs ) const
{
    return nID_ == rhs.nID_;
}

// -----------------------------------------------------------------------------
// Name: MIL_KnowledgeGroup::operator!=
// Created: NLD 2004-09-10
// -----------------------------------------------------------------------------
bool MIL_KnowledgeGroup::operator!=( const MIL_KnowledgeGroup& rhs ) const
{
    return nID_ != rhs.nID_;
}

// -----------------------------------------------------------------------------
// Name: MIL_KnowledgeGroup::GetID
// Created: NLD 2004-08-31
// -----------------------------------------------------------------------------
uint MIL_KnowledgeGroup::GetID() const
{
    return nID_;
}

// -----------------------------------------------------------------------------
// Name: MIL_KnowledgeGroup::GetAutomates
// Created: NLD 2004-09-01
// -----------------------------------------------------------------------------
const MIL_KnowledgeGroup::T_AutomateVector& MIL_KnowledgeGroup::GetAutomates() const
{
    return automates_;
}

// -----------------------------------------------------------------------------
// Name: MIL_KnowledgeGroup::GetKnowledge
// Created: NLD 2004-11-08
// -----------------------------------------------------------------------------
const DEC_KnowledgeBlackBoard_KnowledgeGroup& MIL_KnowledgeGroup::GetKnowledge() const
{
    assert( pKnowledgeBlackBoard_ );
    return *pKnowledgeBlackBoard_;
}

// -----------------------------------------------------------------------------
// Name: MIL_KnowledgeGroup::GetArmy
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
MIL_Army& MIL_KnowledgeGroup::GetArmy() const
{
    assert( pArmy_ );
    return *pArmy_;
}

// -----------------------------------------------------------------------------
// Name: MIL_KnowledgeGroup::RegisterFormation
// Created: NLD 2006-10-13
// -----------------------------------------------------------------------------
void MIL_KnowledgeGroup::RegisterKnowledgeGroup( MIL_KnowledgeGroup& knowledgeGroup )
{
    tools::Resolver< MIL_KnowledgeGroup >::Register( knowledgeGroup.GetID(), knowledgeGroup );
}

// -----------------------------------------------------------------------------
// Name: MIL_KnowledgeGroup::RegisterAutomate
// Created: NLD 2004-09-01
// -----------------------------------------------------------------------------
void MIL_KnowledgeGroup::RegisterAutomate( MIL_Automate& automate )
{
    assert( std::find( automates_.begin(), automates_.end(), &automate ) == automates_.end() );
    automates_.push_back( &automate );
}

// -----------------------------------------------------------------------------
// Name: MIL_KnowledgeGroup::UnregisterAutomate
// Created: NLD 2004-09-01
// -----------------------------------------------------------------------------
void MIL_KnowledgeGroup::UnregisterAutomate( MIL_Automate& automate )
{
    IT_AutomateVector it = std::find( automates_.begin(), automates_.end(), &automate );
    assert( it != automates_.end() );
    automates_.erase( it );
}

// -----------------------------------------------------------------------------
// Name: MIL_KnowledgeGroup::GetType
// Created: JVT 2004-11-29
// -----------------------------------------------------------------------------
const MIL_KnowledgeGroupType& MIL_KnowledgeGroup::GetType() const
{
    assert( pType_ );
    return *pType_;
}

// -----------------------------------------------------------------------------
// Name: MIL_Army::FindKnowledgeGroup
// Created: SLG 2009-11-30
// -----------------------------------------------------------------------------
MIL_KnowledgeGroup* MIL_KnowledgeGroup::FindKnowledgeGroup( uint nID ) const
{
     MIL_KnowledgeGroup* knowledgeGroup = Resolver< MIL_KnowledgeGroup >::Find( nID );
     if( knowledgeGroup == 0 )
     {
         tools::Resolver< MIL_KnowledgeGroup >::Apply( boost::bind( &MIL_KnowledgeGroup::FindKnowledgeGroup, _1, nID ) );
     }
     return knowledgeGroup;
}

