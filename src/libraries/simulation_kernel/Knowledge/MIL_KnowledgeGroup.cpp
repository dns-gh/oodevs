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
#include "Network/NET_AsnException.h"
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
MIL_KnowledgeGroup::MIL_KnowledgeGroup( MIL_KnowledgeGroupType& type, uint nID, MIL_Army& army )
    : pType_               ( &type )
    , nID_                 ( nID )
    , pArmy_               ( &army )
    , pParent_             ( 0 )
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
    , timeToDiffuse_        ( 0 )
    , isActivated_          ( true )
{
    std::string strType;
    xis >> xml::attribute( "id", nID_ )
        >> xml::attribute( "type", strType );
    pType_ = const_cast< MIL_KnowledgeGroupType* >( MIL_KnowledgeGroupType::FindType( strType ) );
    if( pParent_ )
    {
        pParent_->RegisterKnowledgeGroup( *this );
        const MT_Float nCurrentTimeStep = MIL_AgentServer::GetWorkspace().GetCurrentTimeStep();
        timeToDiffuse_ = pParent_->GetType().GetKnowledgeCommunicationDelay() + nCurrentTimeStep;
    }
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
    , pParent_             ( 0 )
    , pKnowledgeBlackBoard_( 0 )
    , automates_           ()
    , isActivated_         ( true )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_KnowledgeGroup destructor
// Created: NLD 2004-08-11
// -----------------------------------------------------------------------------
MIL_KnowledgeGroup::~MIL_KnowledgeGroup()
{
    if( pArmy_ )
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
    pType_ = const_cast< MIL_KnowledgeGroupType* >( MIL_KnowledgeGroupType::FindType( nTypeID ) );
    
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

    for( CIT_KnowledgeGroupVector it = knowledgeGroups_.begin(); it != knowledgeGroups_.end(); ++it )
        (**it).WriteODB( boost::ref(xos) );    
    //tools::Resolver< MIL_KnowledgeGroup >::Apply( boost::bind( &MIL_KnowledgeGroup::WriteODB, _1, boost::ref(xos) ) );
}

// -----------------------------------------------------------------------------
// Name: MIL_KnowledgeGroup::UpdateKnowledges
// Created: NLD 2004-08-19
// -----------------------------------------------------------------------------
void MIL_KnowledgeGroup::UpdateKnowledges()
{
    for( CIT_AutomateVector it = automates_.begin(); it != automates_.end(); ++it )
        (**it).UpdateKnowledges();

    for( CIT_KnowledgeGroupVector it = knowledgeGroups_.begin(); it != knowledgeGroups_.end(); ++it )
        (**it).UpdateKnowledges();
    //tools::Resolver< MIL_KnowledgeGroup >::Apply( boost::bind( &MIL_KnowledgeGroup::UpdateKnowledges, _1 ) );

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

    for( CIT_KnowledgeGroupVector it = knowledgeGroups_.begin(); it != knowledgeGroups_.end(); ++it )
        (**it).CleanKnowledges();
    //tools::Resolver< MIL_KnowledgeGroup >::Apply( boost::bind( &MIL_KnowledgeGroup::CleanKnowledges, _1 ) );

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
    asn().oid       = nID_;
    asn().oid_camp  = pArmy_->GetID();
    asn().type      = GetType().GetName().c_str();
    if( pParent_ )
    {
        asn().m.oid_knowledgegroup_parentPresent = 1;
        asn().oid_knowledgegroup_parent = pParent_->GetID();
    }
    asn.Send();
    //SLG : @TODO MGD Move to factory
    for( CIT_KnowledgeGroupVector it = knowledgeGroups_.begin(); it != knowledgeGroups_.end(); ++it )
        (**it).SendCreation();
//    tools::Resolver< MIL_KnowledgeGroup >::Apply( boost::bind( &MIL_KnowledgeGroup::SendCreation, _1 ) );//SLG : @TODO MGD Move to factory
}

// -----------------------------------------------------------------------------
// Name: MIL_KnowledgeGroup::SendFullState
// Created: SLG 2009-11-13
// -----------------------------------------------------------------------------
void MIL_KnowledgeGroup::SendFullState() const
{
    for( CIT_KnowledgeGroupVector it = knowledgeGroups_.begin(); it != knowledgeGroups_.end(); ++it )
        (**it).SendFullState();    
}

// -----------------------------------------------------------------------------
// Name: MIL_KnowledgeGroup::UpdateKnowledgeGroup
// Created: SLG 2009-12-23
// -----------------------------------------------------------------------------
void MIL_KnowledgeGroup::UpdateKnowledgeGroup() const
{
    NET_ASN_MsgKnowledgeGroupUpdate asn;
    asn().oid = nID_;
    MIL_KnowledgeGroup *pParent = GetParent();
    if( pParent )
    {
        asn().m.oid_knowledgegroup_parentPresent = 1;
        asn().oid_knowledgegroup_parent = pParent->GetID();
    }
    else
    {
        // army is the parent
        asn().m.oid_knowledgegroup_parentPresent = 1;
        asn().oid_knowledgegroup_parent = 0;
    }
    asn().type = GetType().GetName().c_str();
    asn().enabled = IsEnabled();
    asn.Send();

    for( CIT_KnowledgeGroupVector it = knowledgeGroups_.begin(); it != knowledgeGroups_.end(); ++it )
        (**it).UpdateKnowledgeGroup(); 
}

// -----------------------------------------------------------------------------
// Name: MIL_KnowledgeGroup::UpdateKnowledgeGroup
// Created: SLG 2009-12-23
// -----------------------------------------------------------------------------
void MIL_KnowledgeGroup::DeleteKnowledgeGroup()
{
    MIL_KnowledgeGroup *pParent = GetParent();
    if( pParent )
    {
        T_AutomateVector automate_copied;
        for( IT_AutomateVector it(automates_.begin()); it != automates_.end(); ++it )
        {
            automate_copied.push_back( *it );
            MIL_Automate *automat( *it );
            pParent->RegisterAutomate( *automat );
        }
        for( IT_AutomateVector iter(automate_copied.begin()); iter != automate_copied.end(); ++iter)
            UnregisterAutomate( **iter );
        pParent->UnregisterKnowledgeGroup( *this );
        
        std::vector< MIL_KnowledgeGroup* > children;
        for( IT_KnowledgeGroupVector itKG( knowledgeGroups_.begin() ); itKG != knowledgeGroups_.end(); ++itKG )
        {
            MIL_KnowledgeGroup& child = **itKG;
            children.push_back( &child );
            pParent->RegisterKnowledgeGroup( child );            
        }
        for( std::vector< MIL_KnowledgeGroup* >::iterator it( children.begin() ); it != children.end(); ++it )
            UnregisterKnowledgeGroup( **it );

    }
    NET_ASN_MsgKnowledgeGroupDelete asn;
    asn().oid = nID_;
    asn.Send();
}

// -----------------------------------------------------------------------------
// Name: MIL_KnowledgeGroup::MoveKnowledgeGroup
// Created: NLD 2004-09-06
// -----------------------------------------------------------------------------
void MIL_KnowledgeGroup::MoveKnowledgeGroup( MIL_KnowledgeGroup *pNewParent )
{
    MIL_KnowledgeGroup *pParent = GetParent();
    if( pNewParent && pParent )
    {
        pParent->UnregisterKnowledgeGroup( *this );
        pNewParent->RegisterKnowledgeGroup( *this );
    }
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

    for( CIT_KnowledgeGroupVector it = knowledgeGroups_.begin(); it != knowledgeGroups_.end(); ++it )
        (**it).SendKnowledge(); 
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
// Name: MIL_KnowledgeGroup::GetAutomates
// Created: NLD 2004-09-01
// -----------------------------------------------------------------------------
const MIL_KnowledgeGroup::T_KnowledgeGroupVector& MIL_KnowledgeGroup::GetKnowledgeGroups() const
{
    return knowledgeGroups_;
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
// Name: MIL_KnowledgeGroup::GetParent
// Created: NLD 2004-09-07
// -----------------------------------------------------------------------------
MIL_KnowledgeGroup* MIL_KnowledgeGroup::GetParent() const
{
    return pParent_;
}

// -----------------------------------------------------------------------------
// Name: MIL_KnowledgeGroup::SetParent
// Created: FHD 2009-12-22
// -----------------------------------------------------------------------------
void MIL_KnowledgeGroup::SetParent( MIL_KnowledgeGroup* pParent )
{
    pParent_ = pParent;
}

// -----------------------------------------------------------------------------
// Name: MIL_KnowledgeGroup::RegisterKnowledgeGroup
// Created: NLD 2006-10-13
// -----------------------------------------------------------------------------
void MIL_KnowledgeGroup::RegisterKnowledgeGroup( MIL_KnowledgeGroup& knowledgeGroup )
{
    assert( std::find( knowledgeGroups_.begin(), knowledgeGroups_.end(), &knowledgeGroup ) == knowledgeGroups_.end() );
    knowledgeGroups_.push_back( &knowledgeGroup );    
}

// -----------------------------------------------------------------------------
// Name: MIL_KnowledgeGroup::UnregisterKnowledgeGroup
// Created: FHD 2009-12-16: 
// -----------------------------------------------------------------------------
void MIL_KnowledgeGroup::UnregisterKnowledgeGroup( const MIL_KnowledgeGroup& knowledgeGroup )
{
    IT_KnowledgeGroupVector it = std::find( knowledgeGroups_.begin(), knowledgeGroups_.end(), &knowledgeGroup );
    assert( it != knowledgeGroups_.end() );
    knowledgeGroups_.erase( it );
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
// Name: MIL_KnowledgeGroup::IsEnabled
// Created: SLG 2009-12-17
// -----------------------------------------------------------------------------
bool MIL_KnowledgeGroup::IsEnabled() const
{
    return isActivated_;
}

// -----------------------------------------------------------------------------
// Name: MIL_KnowledgeGroup::FindKnowledgeGroup
// Created: SLG 2009-11-30
// -----------------------------------------------------------------------------
MIL_KnowledgeGroup* MIL_KnowledgeGroup::FindKnowledgeGroup( uint nID ) const
{
    MIL_KnowledgeGroup* knowledgeGroup = 0;
    for( CIT_KnowledgeGroupVector itKG( GetKnowledgeGroups().begin() ); itKG != GetKnowledgeGroups().end(); ++itKG )
        if ( (*itKG)->GetID() == nID )
            knowledgeGroup = *itKG;
    if( knowledgeGroup == 0 )
    {
        //tools::Iterator< const MIL_KnowledgeGroup& > it = CreateIterator();
        for( MIL_KnowledgeGroup::CIT_KnowledgeGroupVector itKG( GetKnowledgeGroups().begin() ); itKG != GetKnowledgeGroups().end(); ++itKG )
//         while (it.HasMoreElements() )
         {
            knowledgeGroup = (*itKG)->FindKnowledgeGroup( nID );
            if ( knowledgeGroup )
                return knowledgeGroup;         
         }
         return 0;
     }
     return knowledgeGroup;
}

// -----------------------------------------------------------------------------
// Name: MIL_KnowledgeGroup::GetTimeToDiffuseToKnowledgeGroup
// Created: SLG 2009-11-30
// -----------------------------------------------------------------------------
MT_Float MIL_KnowledgeGroup::GetTimeToDiffuseToKnowledgeGroup() const
{
    return timeToDiffuse_;
}

// -----------------------------------------------------------------------------
// Name: MIL_KnowledgeGroup::RefreshTimeToDiffuseToKnowledgeGroup
// Created: SLG 2009-11-30
// -----------------------------------------------------------------------------
void MIL_KnowledgeGroup::RefreshTimeToDiffuseToKnowledgeGroup()
{
    if ( pParent_ )
        timeToDiffuse_ += pParent_->GetType().GetKnowledgeCommunicationDelay();
}

// -----------------------------------------------------------------------------
// Name: MIL_KnowledgeGroup::OnReceiveMsgKnowledgeGroupEnable
// Created: SLG 2009-12-17
// -----------------------------------------------------------------------------
void MIL_KnowledgeGroup::OnReceiveMsgKnowledgeGroupEnable( const ASN1T_MsgKnowledgeGroupEnable& asnMsg )
{
    isActivated_ = asnMsg.enabled;
    UpdateKnowledgeGroup();
}

// -----------------------------------------------------------------------------
// Name: MIL_KnowledgeGroup::OnReceiveMsgKnowledgeGroupChangeSuperior
// Created: FHD 2009-12-17: 
// -----------------------------------------------------------------------------
void MIL_KnowledgeGroup::OnReceiveMsgKnowledgeGroupChangeSuperior( const ASN1T_MsgKnowledgeGroupChangeSuperior& msg, const tools::Resolver< MIL_Army >& armies )
{
    MIL_Army* pTargetArmy = armies.Find( msg.oid_camp );
    if( !pTargetArmy || *pTargetArmy != GetArmy() )
        throw NET_AsnException< ASN1T_EnumKnowledgeGroupErrorCode >( EnumKnowledgeGroupErrorCode::error_invalid_camp );

    MIL_KnowledgeGroup* pNewKnowledgeGroup = pTargetArmy->FindKnowledgeGroup( msg.oid_knowledgegroup_parent );
    if( !pNewKnowledgeGroup && msg.oid_knowledgegroup_parent != 0 )
        throw NET_AsnException< ASN1T_EnumKnowledgeGroupErrorCode >( EnumKnowledgeGroupErrorCode::error_invalid_superior );
    else if( pNewKnowledgeGroup )        
    {
        MIL_KnowledgeGroup* pParent = GetParent();
        if( pParent != NULL && pParent != pNewKnowledgeGroup )
        {
            pParent->UnregisterKnowledgeGroup( *this );
            pNewKnowledgeGroup->RegisterKnowledgeGroup( *this );
            SetParent( pNewKnowledgeGroup );
            UpdateKnowledgeGroup();
        }
        else if( pParent == NULL )
        {
            GetArmy().UnregisterKnowledgeGroup( *this );
            pNewKnowledgeGroup->RegisterKnowledgeGroup( *this );
            SetParent( pNewKnowledgeGroup );
            UpdateKnowledgeGroup();
        }
    }
    else
    {
        // moving knowledge group under army node
        MIL_KnowledgeGroup* pParent = GetParent();
        if( pParent )
        {
            pParent->UnregisterKnowledgeGroup( *this );
            GetArmy().RegisterKnowledgeGroup( *this );
            SetParent( NULL );
            UpdateKnowledgeGroup();
        }
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_KnowledgeGroup::OnReceiveMsgKnowledgeGroupDelete
// Created: FHD 2009-12-17: 
// -----------------------------------------------------------------------------
void MIL_KnowledgeGroup::OnReceiveMsgKnowledgeGroupDelete( const ASN1T_MsgKnowledgeGroupDelete& /*msg*/ )
{
    // TO DO: FHD 2010-01-04: 
    //DeleteKnowledgeGroup();
}

// -----------------------------------------------------------------------------
// Name: MIL_KnowledgeGroup::OnReceiveMsgKnowledgeGroupSetType
// Created: FHD 2009-12-17: 
// -----------------------------------------------------------------------------
void MIL_KnowledgeGroup::OnReceiveMsgKnowledgeGroupSetType( const ASN1T_MsgKnowledgeGroupSetType& msg )
{
    const MIL_KnowledgeGroupType* pFoundType = MIL_KnowledgeGroupType::FindType( msg.type );
    if( pFoundType && pFoundType->GetID() != GetType().GetID() )
    {
        SetType( const_cast< MIL_KnowledgeGroupType* >( pFoundType ) );
        UpdateKnowledgeGroup();
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_KnowledgeGroup::OnReceiveMsgKnowledgeGroupCreation
// Created: FHD 2009-12-17: 
// -----------------------------------------------------------------------------
void MIL_KnowledgeGroup::OnReceiveMsgKnowledgeGroupCreation( const ASN1T_MsgKnowledgeGroupCreation& msg )
{

}
