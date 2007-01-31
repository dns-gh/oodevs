// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "dispatcher_pch.h"

#include "Profile.h"

#include "Dispatcher.h"
#include "Model.h"
#include "Network_Def.h"
#include "Automat.h"
#include "Side.h"
#include "Formation.h"
#include "Population.h"
#include "ClientsNetworker.h"
#include "xeumeuleu/xml.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: Profile constructor
// Created: NLD 2006-10-06
// -----------------------------------------------------------------------------
Profile::Profile( Dispatcher& dispatcher, const std::string& strLogin, xml::xistream& xis )
    : dispatcher_           ( dispatcher )
    , strLogin_             ( strLogin )
    , strPassword_          () 
    , readOnlyAutomats_     ()
    , readOnlySides_        ()
    , readOnlyFormations_   ()
    , readOnlyPopulations_  ()
    , readWriteAutomats_    ()
    , readWriteSides_       ()
    , readWriteFormations_  ()
    , readWritePopulations_ ()
    , bSupervision_         ( false )
{
    xis >> xml::attribute( "password", strPassword_ )
        >> xml::start( "rights" )
            >> xml::start( "readonly" )
                >> xml::list( "automat"   , *this, &Profile::ReadAutomatRights   , readOnlyAutomats_    )
                >> xml::list( "side"      , *this, &Profile::ReadSideRights      , readOnlySides_       )
                >> xml::list( "formation" , *this, &Profile::ReadFormationRights , readOnlyFormations_  )
                >> xml::list( "population", *this, &Profile::ReadPopulationRights, readOnlyPopulations_ )
            >> xml::end()
            >> xml::start( "readwrite" )
                >> xml::list( "automat"   , *this, &Profile::ReadAutomatRights   , readWriteAutomats_    )
                >> xml::list( "side"      , *this, &Profile::ReadSideRights      , readWriteSides_       )
                >> xml::list( "formation" , *this, &Profile::ReadFormationRights , readWriteFormations_  )
                >> xml::list( "population", *this, &Profile::ReadPopulationRights, readWritePopulations_ )
            >> xml::end()
            >> xml::optional() >> xml::content( "supervision", bSupervision_ )
        >> xml::end();
}

// -----------------------------------------------------------------------------
// Name: Profile constructor
// Created: SBO 2007-01-22
// -----------------------------------------------------------------------------
Profile::Profile( Dispatcher& dispatcher, const ASN1T_MsgProfileCreationRequest& message )
    : dispatcher_           ( dispatcher )
    , strLogin_             ( message.login )
    , strPassword_          ( message.m.passwordPresent ? message.password : "" )
    , readOnlyAutomats_     ()
    , readOnlySides_        ()
    , readOnlyFormations_   ()
    , readOnlyPopulations_  ()
    , readWriteAutomats_    ()
    , readWriteSides_       ()
    , readWriteFormations_  ()
    , readWritePopulations_ ()
    , bSupervision_         ( message.superviseur )
{
    ReadRights( message );

    SendCreation( dispatcher_.GetClientsNetworker() );
}

// -----------------------------------------------------------------------------
// Name: Profile destructor
// Created: NLD 2006-10-06
// -----------------------------------------------------------------------------
Profile::~Profile()
{
    AsnMsgInClientProfileDestruction asn;
    asn() = strLogin_.c_str();
    asn.Send( dispatcher_.GetClientsNetworker() );
}

// =============================================================================
// INITIALIZATION
// =============================================================================

// -----------------------------------------------------------------------------
// Name: Profile::ReadAutomatRights
// Created: NLD 2006-10-06
// -----------------------------------------------------------------------------
void Profile::ReadAutomatRights( xml::xistream& xis, T_AutomatSet& container )
{
    int nID;
    xis >> xml::attribute( "id", nID );

    Automat* pAutomat = dispatcher_.GetModel().GetAutomats().Find( nID );
    if( pAutomat )
        container.insert( pAutomat );
    else
        MT_LOG_ERROR_MSG( "Invalid automat id ('" << nID << "') while reading profiles" );
}

// -----------------------------------------------------------------------------
// Name: Profile::ReadSideRights
// Created: NLD 2006-10-06
// -----------------------------------------------------------------------------
void Profile::ReadSideRights( xml::xistream& xis, T_SideSet& container )
{
    int nID;
    xis >> xml::attribute( "id", nID );

    Side* pSide = dispatcher_.GetModel().GetSides().Find( nID );
    if( pSide )
        container.insert( pSide );
    else
        MT_LOG_ERROR_MSG( "Invalid side id ('" << nID << "') while reading profiles" );
}

// -----------------------------------------------------------------------------
// Name: Profile::ReadFormationRights
// Created: NLD 2006-10-19
// -----------------------------------------------------------------------------
void Profile::ReadFormationRights( xml::xistream& xis, T_FormationSet&  container )
{
    int nID;
    xis >> xml::attribute( "id", nID );

    Formation* pFormation = dispatcher_.GetModel().GetFormations().Find( nID );
    if( pFormation )
        container.insert( pFormation );
    else
        MT_LOG_ERROR_MSG( "Invalid formation id ('" << nID << "') while reading profiles" );
}

// -----------------------------------------------------------------------------
// Name: Profile::ReadPopulationRights
// Created: NLD 2006-10-10
// -----------------------------------------------------------------------------
void Profile::ReadPopulationRights( xml::xistream& xis, T_PopulationSet& container )
{
    int nID;
    xis >> xml::attribute( "id", nID );

    Population* pPopulation = dispatcher_.GetModel().GetPopulations().Find( nID );
    if( pPopulation )
        container.insert( pPopulation );
    else
        MT_LOG_ERROR_MSG( "Invalid population id ('" << nID << "') while reading profiles" );

}

namespace
{
    template< typename T >
    void SetRights( const ASN1T_ListOID& asn, std::set< T* >& list, const ModelsContainer< T >& model )
    {
        list.clear();
        for( unsigned int i = 0; i < asn.n; ++i )
            if( T* entity = model.Find( asn.elem[i] ) )
                list.insert( entity );
    }
}
// $$$$ NLD 2007-01-30: Factoriser ASN & xml

// -----------------------------------------------------------------------------
// Name: Profile::ReadRights
// Created: SBO 2007-01-22
// -----------------------------------------------------------------------------
void Profile::ReadRights( const ASN1T_Profile& message )
{
    Model& model = dispatcher_.GetModel();
    if( message.m.read_only_automatesPresent )
        SetRights( (const ASN1T_ListOID&)message.read_only_automates, readOnlyAutomats_, model.GetAutomats() );
    if( message.m.read_only_campsPresent )
        SetRights( (const ASN1T_ListOID&)message.read_only_camps, readOnlySides_, model.GetSides() );
    if( message.m.read_only_formationsPresent )
        SetRights( (const ASN1T_ListOID&)message.read_only_formations, readOnlyFormations_, model.GetFormations() );
    if( message.m.read_only_populationsPresent )
        SetRights( (const ASN1T_ListOID&)message.read_only_populations, readOnlyPopulations_, model.GetPopulations() );
    if( message.m.read_write_automatesPresent )
        SetRights( (const ASN1T_ListOID&)message.read_write_automates, readWriteAutomats_, model.GetAutomats() );
    if( message.m.read_write_campsPresent )
        SetRights( (const ASN1T_ListOID&)message.read_write_camps, readWriteSides_, model.GetSides() );
    if( message.m.read_write_formationsPresent )
        SetRights( (const ASN1T_ListOID&)message.read_write_formations, readWriteFormations_, model.GetFormations() );
    if( message.m.read_write_populationsPresent )
        SetRights( (const ASN1T_ListOID&)message.read_write_populations, readWritePopulations_, model.GetPopulations() );
}

// =============================================================================
// MAIN
// =============================================================================

// -----------------------------------------------------------------------------
// Name: Profile::CheckPassword
// Created: NLD 2006-10-06
// -----------------------------------------------------------------------------
bool Profile::CheckPassword( const std::string& strPassword ) const
{
    return strPassword == strPassword_;
}

// -----------------------------------------------------------------------------
// Name: Profile::CheckRights
// Created: NLD 2006-10-06
// -----------------------------------------------------------------------------
bool Profile::CheckRights( const ASN1T_MsgsOutClient& msg ) const
{
    switch( msg.msg.t )
    {
        case T_MsgsOutClient_msg_msg_ctrl_stop                          : return bSupervision_;
        case T_MsgsOutClient_msg_msg_ctrl_pause                         : return bSupervision_;
        case T_MsgsOutClient_msg_msg_ctrl_resume                        : return bSupervision_; 
        case T_MsgsOutClient_msg_msg_ctrl_change_time_factor            : return bSupervision_;
        case T_MsgsOutClient_msg_msg_ctrl_meteo_globale                 : return bSupervision_;
        case T_MsgsOutClient_msg_msg_ctrl_meteo_locale                  : return bSupervision_;
        case T_MsgsOutClient_msg_msg_ctrl_checkpoint_save_now           : return bSupervision_;
        case T_MsgsOutClient_msg_msg_ctrl_checkpoint_set_frequency      : return bSupervision_;
        case T_MsgsOutClient_msg_msg_auth_login                         : return true;
        case T_MsgsOutClient_msg_msg_profile_creation_request           : return bSupervision_; //$$$ Administration
        case T_MsgsOutClient_msg_msg_profile_update_request             : return bSupervision_; //$$$ Administration
        case T_MsgsOutClient_msg_msg_profile_destruction_request        : return bSupervision_; //$$$ Administration
        case T_MsgsOutClient_msg_msg_limit_creation_request             : return true;
        case T_MsgsOutClient_msg_msg_limit_destruction_request          : return true;
        case T_MsgsOutClient_msg_msg_limit_update_request               : return true;
        case T_MsgsOutClient_msg_msg_lima_creation_request              : return true;
        case T_MsgsOutClient_msg_msg_lima_destruction_request           : return true;
        case T_MsgsOutClient_msg_msg_lima_update_request                : return true;
        case T_MsgsOutClient_msg_msg_pion_order                         : return true;
        case T_MsgsOutClient_msg_msg_automate_order                     : return true;
        case T_MsgsOutClient_msg_msg_population_order                   : return true;
        case T_MsgsOutClient_msg_msg_frag_order                         : return true;
        case T_MsgsOutClient_msg_msg_set_automate_mode                  : return true;
        case T_MsgsOutClient_msg_msg_unit_magic_action                  : return true;
        case T_MsgsOutClient_msg_msg_object_magic_action                : return true;
        case T_MsgsOutClient_msg_msg_population_magic_action            : return true;
        case T_MsgsOutClient_msg_msg_change_diplomatie                  : return true;
        case T_MsgsOutClient_msg_msg_change_groupe_connaissance         : return true;
        case T_MsgsOutClient_msg_msg_change_liens_logistiques           : return true;
        case T_MsgsOutClient_msg_msg_change_automate                    : return true;
        case T_MsgsOutClient_msg_msg_log_ravitaillement_pousser_flux    : return true;
        case T_MsgsOutClient_msg_msg_log_ravitaillement_change_quotas   : return true;
        default:
            return false;
    }
}

// =============================================================================
// NETWORK
// =============================================================================

// -----------------------------------------------------------------------------
// Name: Profile::Send
// Created: NLD 2006-10-09
// -----------------------------------------------------------------------------
void Profile::Send( ASN1T_Profile& asn ) const
{
    asn.m.read_only_automatesPresent    = 1;  
    asn.m.read_only_campsPresent        = 1;
    asn.m.read_only_formationsPresent   = 1;
    asn.m.read_only_populationsPresent  = 1;  
    asn.m.read_write_automatesPresent   = 1;
    asn.m.read_write_campsPresent       = 1;
    asn.m.read_write_formationsPresent  = 1;
    asn.m.read_write_populationsPresent = 1;

    asn.login = strLogin_.c_str();

    asn.superviseur = bSupervision_;

    SendContainerRefs< ASN1T_ListAutomate, ASN1T_Automate, T_AutomatSet >( readOnlyAutomats_ , asn.read_only_automates  );
    SendContainerRefs< ASN1T_ListAutomate, ASN1T_Automate, T_AutomatSet >( readWriteAutomats_, asn.read_write_automates );

    SendContainerRefs< ASN1T_ListCamp, ASN1T_Camp, T_SideSet >( readOnlySides_ , asn.read_only_camps  );
    SendContainerRefs< ASN1T_ListCamp, ASN1T_Camp, T_SideSet >( readWriteSides_, asn.read_write_camps );

    SendContainerRefs< ASN1T_ListFormation, ASN1T_Formation, T_FormationSet >( readOnlyFormations_ , asn.read_only_formations  );
    SendContainerRefs< ASN1T_ListFormation, ASN1T_Formation, T_FormationSet >( readWriteFormations_, asn.read_write_formations );
    
    SendContainerRefs< ASN1T_ListPopulation, ASN1T_Population, T_PopulationSet >( readOnlyPopulations_ , asn.read_only_populations  );
    SendContainerRefs< ASN1T_ListPopulation, ASN1T_Population, T_PopulationSet >( readWritePopulations_, asn.read_write_populations );
}

// -----------------------------------------------------------------------------
// Name: Profile::AsnDelete
// Created: NLD 2006-10-10
// -----------------------------------------------------------------------------
void Profile::AsnDelete( ASN1T_Profile& asn )
{
    if( asn.m.read_only_automatesPresent && asn.read_only_automates.n > 0 )
        delete [] asn.read_only_automates.elem;
    if( asn.m.read_write_automatesPresent && asn.read_write_automates.n > 0 )
        delete [] asn.read_write_automates.elem;

    if( asn.m.read_only_campsPresent && asn.read_only_camps.n > 0 )
        delete [] asn.read_only_camps.elem;
    if( asn.m.read_write_campsPresent && asn.read_write_camps.n > 0 )
        delete [] asn.read_write_camps.elem;

    if( asn.m.read_only_formationsPresent && asn.read_only_formations.n > 0 )
        delete [] asn.read_only_formations.elem;
    if( asn.m.read_write_formationsPresent && asn.read_write_formations.n > 0 )
        delete [] asn.read_write_formations.elem;

    if( asn.m.read_only_populationsPresent && asn.read_only_populations.n > 0 )
        delete [] asn.read_only_populations.elem;
    if( asn.m.read_write_populationsPresent && asn.read_write_populations.n > 0 )
        delete [] asn.read_write_populations.elem;
}

// -----------------------------------------------------------------------------
// Name: Profile::SendCreation
// Created: SBO 2007-01-22
// -----------------------------------------------------------------------------
void Profile::SendCreation( Publisher_ABC& publisher ) const
{
    AsnMsgInClientProfileCreation asn;
    Send( asn() );
    asn().m.passwordPresent = 1;
    asn().password = strPassword_.c_str();
    asn.Send( publisher );
    Profile::AsnDelete( asn() );
}

// -----------------------------------------------------------------------------
// Name: Profile::Update
// Created: SBO 2007-01-22
// -----------------------------------------------------------------------------
void Profile::Update( const ASN1T_MsgProfileUpdateRequest& message )
{
    strLogin_ = message.profile.login;
    if( message.profile.m.passwordPresent )
        strPassword_ = message.profile.password;
    bSupervision_ = message.profile.superviseur;
    ReadRights( message.profile );

    AsnMsgInClientProfileUpdate asn;
    asn().login = message.login;
    Send( asn().profile );
    asn.Send( dispatcher_.GetClientsNetworker() );
    Profile::AsnDelete( asn().profile );
}