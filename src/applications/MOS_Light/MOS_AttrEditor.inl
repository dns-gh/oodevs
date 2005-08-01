//*****************************************************************************
//
// $Created: FBD 02-12-17 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_AttrEditor.inl $
// $Author: Nld $
// $Modtime: 10/03/05 16:30 $
// $Revision: 37 $
// $Workfile: MOS_AttrEditor.inl $
//
//*****************************************************************************

#include <qlistview.h>

//-----------------------------------------------------------------------------
// Name: MOS_AttrEditor::SetWidgetStack
// Created: FBD 03-01-17
//-----------------------------------------------------------------------------
inline
void MOS_AttrEditor::SetWidgetStack( QWidgetStack* pWidgetStack )
{
    pWidgetStack_ = pWidgetStack;
}


//-----------------------------------------------------------------------------
// Name: MOS_AttrEditor::GetCoord
// Created: FBD 03-01-23
//-----------------------------------------------------------------------------
inline
MOS_MagicPos* MOS_AttrEditor::GetCoord() const
{
    return pMagicPos_;
}


//-----------------------------------------------------------------------------
// Name: MOS_AttrEditor::Cancel
// Created: FBD 03-01-15
//-----------------------------------------------------------------------------
inline
void MOS_AttrEditor::Cancel()
{
    SetAgent( pAgent_ );
}

//-----------------------------------------------------------------------------
// Name: MOS_AttrEditor::SetSpeed
// Created: NLD 2003-08-05
//-----------------------------------------------------------------------------
inline
void MOS_AttrEditor::SetSpeed( uint nSpeed )
{
    pSpeedItem_->setText( 1, MT_FormatString( "%d", nSpeed ).c_str() );
}

// -----------------------------------------------------------------------------
// Name: MOS_AttrEditor::ConvertPostureToString
// Created: NLD 2003-10-24
// -----------------------------------------------------------------------------
// static
inline
std::string MOS_AttrEditor::ConvertPostureToString( E_PostureType nPosture )
{
    switch( nPosture )
    {
        case ePostureMouvement        : return "Mouvement";
        case ePostureMouvementDiscret : return "Mouvement discret";
        case ePostureArret            : return "Arret";
        case ePosturePosteReflexe     : return "Poste reflexe";
        case ePosturePoste            : return "Poste";
        case ePosturePosteAmenage     : return "Poste aménagé";
        case ePosturePostePrepareGenie: return "Poste preparé génie";
        case ePostureTransporte       : return "Transporté";
        default:
            assert( false );
            return "Unknown";
    }
}


// -----------------------------------------------------------------------------
// Name: MOS_AttrEditor::SetPosture
// Created: NLD 2003-10-24
// -----------------------------------------------------------------------------
inline
void MOS_AttrEditor::SetPosture( E_PostureType nOldPosture, E_PostureType nCurrentPosture, uint nPostureCompletionPourcentage )
{
    pOldPostureItem_                  ->setText( 1, ConvertPostureToString( nOldPosture     ).c_str() );
    pCurrentPostureItem_              ->setText( 1, ConvertPostureToString( nCurrentPosture ).c_str() );
    pPostureCompletionPourcentageItem_->setText( 1, MT_FormatString( "%d%%", nPostureCompletionPourcentage).c_str() ); 
}

// -----------------------------------------------------------------------------
// Name: MOS_AttrEditor::SetRawOpState
// Created: NLD 2004-02-16
// -----------------------------------------------------------------------------
inline
void MOS_AttrEditor::SetRawOpState( uint nValue )
{
    pRawOpStateItem_->setText( 1, MT_FormatString( "%d%%", nValue ).c_str() );
}

// -----------------------------------------------------------------------------
// Name: MOS_AttrEditor::ResetPosture
// Created: NLD 2003-10-24
// -----------------------------------------------------------------------------
inline
void MOS_AttrEditor::ResetPosture()
{
    pOldPostureItem_                  ->setText( 1, "" );
    pCurrentPostureItem_              ->setText( 1, "" );
    pPostureCompletionPourcentageItem_->setText( 1, "" ); 
}


// -----------------------------------------------------------------------------
// Name: MOS_AttrEditor::SetAutomateMode
// Created: NLD 2003-12-01
// -----------------------------------------------------------------------------
inline
void MOS_AttrEditor::SetAutomateMode( bool bEmbraye )
{
    if( !pAutomateModeItem_ )
        pAutomateModeItem_ = new QListViewItem( pStateListView_, pPostureCompletionPourcentageItem_, "Automate", "" );

    if( bEmbraye )
    {
        pButtonChangeAutomateMode_->setText( "Debraye automate" );
        pAutomateModeItem_->setText( 1, "Embraye" );
    }
    else
    {
        pButtonChangeAutomateMode_->setText( "Embraye automate" );
        pAutomateModeItem_->setText( 1, "Debraye" );
    }
}

// -----------------------------------------------------------------------------
// Name: MOS_AttrEditor::SetFightRateState
// Created: NLD 2003-12-02
// -----------------------------------------------------------------------------
inline
void MOS_AttrEditor::SetFightRateState( E_ForceRatioState nState )
{
    switch( nState )
    {
        case eForceRatioStateNone:       pFightRateStateItem_->setText( 1, "None" ); break;
        case eForceRatioStateNeutral:     pFightRateStateItem_->setText( 1, "Neutre" ); break;
        case eForceRatioStateFavorable:   pFightRateStateItem_->setText( 1, "Favorable" ); break;
        case eForceRatioStateUnfavorable: pFightRateStateItem_->setText( 1, "Defavorable" ); break;
        default:
            assert( false );
    }
}

// -----------------------------------------------------------------------------
// Name: MOS_AttrEditor::SetRulesOfEngagementState
// Created: NLD 2003-12-02
// -----------------------------------------------------------------------------
inline
void MOS_AttrEditor::SetRulesOfEngagementState( E_RulesOfEngagementState nState )
{
    switch( nState )
    {
        case eRoeStateNone:         pRulesOfEngagementStateItem_->setText( 1, "None" ); break;
        case eRoeStateFreeFire:         pRulesOfEngagementStateItem_->setText( 1, "Tir libre" ); break;
        case eRoeStateRestrictedFire:   pRulesOfEngagementStateItem_->setText( 1, "Tir restreint" ); break;
        case eRoeStateFireByOrder:      pRulesOfEngagementStateItem_->setText( 1, "Tir sur ordre" ); break;
        default:
            assert( false );
    }
}

// -----------------------------------------------------------------------------
// Name: MOS_AttrEditor::SetOpState
// Created: NLD 2003-12-02
// -----------------------------------------------------------------------------
inline
void MOS_AttrEditor::SetOpState( E_OperationalState nState )
{
    switch( nState )
    {
        case eOpStateFullyDestroyed:        pOpStateItem_->setText( 1, "Détruit totalement" ); break;
        case eOpStateTacticallyDestroyed:   pOpStateItem_->setText( 1, "Détruit tactiquement" ); break;
        case eOpStateOperational:           pOpStateItem_->setText( 1, "Opérationnel" ); break;
        default:
            assert( false );
    }
}

// -----------------------------------------------------------------------------
// Name: MOS_AttrEditor::SetCloseCombatState
// Created: NLD 2003-12-02
// -----------------------------------------------------------------------------
inline
void MOS_AttrEditor::SetCloseCombatState( E_CloseCombatState nState )
{
    switch( nState )
    {
        case eCloseCombatStateNone:          pCloseCombatStateItem_->setText( 1, "None" ); break;
        case eCloseCombatStateDodge:          pCloseCombatStateItem_->setText( 1, "Esquive" ); break;
        case eCloseCombatStateFixed:          pCloseCombatStateItem_->setText( 1, "Fixé" ); break;
        case eCloseCombatStateProceedMission: pCloseCombatStateItem_->setText( 1, "Poursuite mission" ); break;
        default:
            assert( false );
    }
}

// -----------------------------------------------------------------------------
// Name: MOS_AttrEditor::SetLoadingState
// Created: NLD 2004-01-12
// -----------------------------------------------------------------------------
inline
void MOS_AttrEditor::SetLoadingState( bool bLoadingState )
{
    assert( pLoadingStateItem_ );    
    if( bLoadingState )
        pLoadingStateItem_->setText( 1, "Embarqué" ); 
    else
        pLoadingStateItem_->setText( 1, "Debarqué" ); 
}

// -----------------------------------------------------------------------------
// Name: MOS_AttrEditor::SetStealthModeEnabled
// Created: NLD 2004-10-13
// -----------------------------------------------------------------------------
inline
void MOS_AttrEditor::SetStealthModeEnabled( bool bStealthMode )
{
    assert( pStealthModeItem_ );    
    if( bStealthMode )
        pStealthModeItem_->setText( 1, "Actif" ); 
    else
        pStealthModeItem_->setText( 1, "Inactif" ); 
}


// -----------------------------------------------------------------------------
// Name: MOS_AttrEditor::SetNbcProtectionSuitWorn
// Created: NLD 2004-05-03
// -----------------------------------------------------------------------------
inline
void MOS_AttrEditor::SetNbcProtectionSuitWorn( bool bNbcProtectionSuitWorn )
{
    if( bNbcProtectionSuitWorn )
        pNbcProtectionSuitWornItem_->setText( 1, "Mise" );
    else
        pNbcProtectionSuitWornItem_->setText( 1, "Non mise" );
}

// -----------------------------------------------------------------------------
// Name: MOS_AttrEditor::SetNbcAgentsContaminating
// Created: NLD 2004-05-03
// -----------------------------------------------------------------------------
inline
void MOS_AttrEditor::SetNbcAgentsContaminating( const MOS_Agent::T_NbcAgentVector& nbcAgentContaminating )
{
    if( nbcAgentContaminating.empty() )
    {
        pNbcAgentsContaminatingItem_->setText( 1, "-" );
        return;
    }
    std::stringstream strOutputMsg;
    for( MOS_Agent::CIT_NbcAgentVector itNbcAgent = nbcAgentContaminating.begin(); itNbcAgent != nbcAgentContaminating.end(); ++itNbcAgent )
        strOutputMsg << *itNbcAgent << " ";
    pNbcAgentsContaminatingItem_->setText( 1, strOutputMsg.str().c_str() );
}

// -----------------------------------------------------------------------------
// Name: MOS_AttrEditor::SetContaminationState
// Created: NLD 2004-11-03
// -----------------------------------------------------------------------------
inline
void MOS_AttrEditor::SetContaminationState( uint nContaminationState )
{
    pContaminationStateItem_->setText( 1, MT_FormatString( "%d%%", nContaminationState ).c_str() );
}

// -----------------------------------------------------------------------------
// Name: MOS_AttrEditor::SetRenforts
// Created: NLD 2004-05-03
// -----------------------------------------------------------------------------
inline
void MOS_AttrEditor::SetRenforts( const MOS_Agent::T_RenfortVector& renforts )
{
    if( renforts.empty() )
    {
        pRenfortsItem_->setText( 1, "-" );
        return;
    }

    std::stringstream strOutputMsg;
    for( MOS_Agent::CIT_RenfortVector itRenfort = renforts.begin(); itRenfort != renforts.end(); ++itRenfort )
        strOutputMsg << *itRenfort << " ";
    pRenfortsItem_->setText( 1, strOutputMsg.str().c_str() );
}

// -----------------------------------------------------------------------------
// Name: MOS_AttrEditor::SetPionRenforce
// Created: NLD 2004-11-22
// -----------------------------------------------------------------------------
inline
void MOS_AttrEditor::SetPionRenforce( uint nID )
{
    if( nID ) 
        pPionRenforceItem_->setText( 1, MT_FormatString( "%d", nID ).c_str() );    
    else
        pPionRenforceItem_->setText( 1, "-" );
}

// -----------------------------------------------------------------------------
// Name: MOS_AttrEditor::SetPionTransportes
// Created: NLD 2004-11-22
// -----------------------------------------------------------------------------
inline
void MOS_AttrEditor::SetPionTransportes( const MOS_Agent::T_TransportVector& transports )
{
    if( transports.empty() )
    {
        pTransportsItem_->setText( 1, "-" );
        return;
    }

    std::stringstream strOutputMsg;
    for( MOS_Agent::CIT_TransportVector itTransport = transports.begin(); itTransport != transports.end(); ++itTransport )
        strOutputMsg << *itTransport << " ";
    pTransportsItem_->setText( 1, strOutputMsg.str().c_str() );
}

// -----------------------------------------------------------------------------
// Name: MOS_AttrEditor::SetTransporteur
// Created: NLD 2004-11-22
// -----------------------------------------------------------------------------
inline
void MOS_AttrEditor::SetTransporteur( uint nID )
{
    if( nID ) 
        pTransporteurItem_->setText( 1, MT_FormatString( "%d", nID ).c_str() );    
    else
        pTransporteurItem_->setText( 1, "-" );
}

// -----------------------------------------------------------------------------
// Name: MOS_AttrEditor::SetAltitude
// Created: NLD 2004-06-21
// -----------------------------------------------------------------------------
inline
void MOS_AttrEditor::SetAltitude( int nAltitude )
{
    pAltitudeItem_->setText( 1, MT_FormatString( "%d", nAltitude ).c_str() );    
}

// -----------------------------------------------------------------------------
// Name: MOS_AttrEditor::SetDirection
// Created: NLD 2004-06-21
// -----------------------------------------------------------------------------
inline
void MOS_AttrEditor::SetDirection( uint nDirection )
{
    pDirectionItem_->setText( 1, MT_FormatString( "%d°", nDirection ).c_str() );        
}

// -----------------------------------------------------------------------------
// Name: MOS_AttrEditor::SetJammedState
// Created: NLD 2004-11-04
// -----------------------------------------------------------------------------
inline
void MOS_AttrEditor::SetJammedState( bool bJammed )
{
    if( bJammed ) 
        pJammedStateItem_->setText( 1, "Brouillé" );
    else
        pJammedStateItem_->setText( 1, "Non brouillé" );
}

// -----------------------------------------------------------------------------
// Name: MOS_AttrEditor::SetBlackOutState
// Created: NLD 2004-11-05
// -----------------------------------------------------------------------------
inline
void MOS_AttrEditor::SetBlackOutState( bool bBlackOut )
{
    if( bBlackOut ) 
        pBlackOutStateItem_->setText( 1, "Actif" );
    else
        pBlackOutStateItem_->setText( 1, "Inactif" );
}

// -----------------------------------------------------------------------------
// Name: MOS_AttrEditor::SetRadarEnabled
// Created: NLD 2005-02-23
// -----------------------------------------------------------------------------
inline
void MOS_AttrEditor::SetRadarEnabled( bool bRadarEnabled )
{
    if( bRadarEnabled ) 
        pRadarStateItem_->setText( 1, "Actif" );
    else
        pRadarStateItem_->setText( 1, "Inactif" );
}

// -----------------------------------------------------------------------------
// Name: MOS_AttrEditor::SetPrisonerState
// Created: NLD 2005-03-04
// -----------------------------------------------------------------------------
inline
void MOS_AttrEditor::SetPrisonerState( bool bPrisoner   )
{
    if( bPrisoner ) 
        pPrisonerStateItem_->setText( 1, "Oui" );
    else
        pPrisonerStateItem_->setText( 1, "Non" );
}

// -----------------------------------------------------------------------------
// Name: MOS_AttrEditor::SetSurrenderedState
// Created: NLD 2005-03-04
// -----------------------------------------------------------------------------
inline
void MOS_AttrEditor::SetSurrenderedState( bool bSurrendered )
{
    if( bSurrendered ) 
        pSurrenderedStateItem_->setText( 1, "Oui" );
    else
        pSurrenderedStateItem_->setText( 1, "Non" );
}

// -----------------------------------------------------------------------------
// Name: MOS_AttrEditor::SetRefugeesManagedState
// Created: NLD 2005-03-10
// -----------------------------------------------------------------------------
inline
void MOS_AttrEditor::SetRefugeesManagedState( bool bRefugeesManaged )
{
    if( bRefugeesManaged )
        pRefugeesManagedStateItem_->setText( 1, "Oui" );
    else
        pRefugeesManagedStateItem_->setText( 1, "Non" );
}

// -----------------------------------------------------------------------------
// Name: MOS_AttrEditor::SetMorale
// Created: NLD 2004-11-30
// -----------------------------------------------------------------------------
inline
void MOS_AttrEditor::SetMorale( const MOS_Morale& morale )
{
    pMoraleItem_->setText( 1, morale.GetName().c_str() );
}

   
// -----------------------------------------------------------------------------
// Name: MOS_AttrEditor::SetExperience
// Created: NLD 2004-11-30
// -----------------------------------------------------------------------------
inline
void MOS_AttrEditor::SetExperience( const MOS_Experience& experience )
{
    pExperienceItem_->setText( 1, experience.GetName().c_str() );
}
    
// -----------------------------------------------------------------------------
// Name: MOS_AttrEditor::SetTiredness
// Created: NLD 2004-11-30
// -----------------------------------------------------------------------------
inline
void MOS_AttrEditor::SetTiredness( const MOS_Tiredness& tiredness )
{
    pTirednessItem_->setText( 1, tiredness.GetName().c_str() );
}

// -----------------------------------------------------------------------------
// Name: MOS_AttrEditor::SetTC2
// Created: NLD 2005-07-08
// -----------------------------------------------------------------------------
inline
void MOS_AttrEditor::SetTC2( uint nTC2ID )
{
    if( nTC2ID == 0 )
        pTC2Item_->setText( 1, "-" );
    else
        pTC2Item_->setText( 1, MT_FormatString( "%d", nTC2ID ).c_str() );
}

// -----------------------------------------------------------------------------
// Name: MOS_AttrEditor::SetLogMaintenanceSuperior
// Created: NLD 2005-07-08
// -----------------------------------------------------------------------------
inline
void MOS_AttrEditor::SetLogMaintenanceSuperior( uint nLogMaintenanceSuperior )
{
    if( nLogMaintenanceSuperior == 0 )
        pLogMaintenanceSuperior_->setText( 1, "-" );
    else
        pLogMaintenanceSuperior_->setText( 1, MT_FormatString( "%d", nLogMaintenanceSuperior ).c_str() );
}

// -----------------------------------------------------------------------------
// Name: MOS_AttrEditor::SetLogMedicalSuperior
// Created: NLD 2005-07-08
// -----------------------------------------------------------------------------
inline
void MOS_AttrEditor::SetLogMedicalSuperior( uint nLogMedicalSuperior )
{
    if( nLogMedicalSuperior == 0 )
        pLogMedicalSuperior_->setText( 1, "-" );
    else
        pLogMedicalSuperior_->setText( 1, MT_FormatString( "%d", nLogMedicalSuperior ).c_str() );
}

// -----------------------------------------------------------------------------
// Name: MOS_AttrEditor::SetLogSupplySuperior
// Created: NLD 2005-07-08
// -----------------------------------------------------------------------------
inline
void MOS_AttrEditor::SetLogSupplySuperior( uint nLogSupplySuperior )
{
    if( nLogSupplySuperior == 0 )
        pLogSupplySuperior_->setText( 1, "-" );
    else
        pLogSupplySuperior_->setText( 1, MT_FormatString( "%d", nLogSupplySuperior ).c_str() );
}

// -----------------------------------------------------------------------------
// Name: MOS_AttrEditor::SetHumanTransportersReady
// Created: NLD 2005-07-25
// -----------------------------------------------------------------------------
inline
void MOS_AttrEditor::SetHumanTransportersReady( bool bReady )
{
    pHumanTransportersReadyItem_->setText( 1, bReady ? "Ready" : "Not ready" );
}


// -----------------------------------------------------------------------------
// Name: MOS_AttrEditor::SetDead
// Created: NLD 2005-07-25
// -----------------------------------------------------------------------------
inline
void MOS_AttrEditor::SetDead( bool bDead )
{
    pDeadItem_->setText( 1, bDead ? "Oui" : "Non" );
}

// -----------------------------------------------------------------------------
// Name: MOS_AttrEditor::SetNeutralized
// Created: NLD 2005-07-25
// -----------------------------------------------------------------------------
inline
void MOS_AttrEditor::SetNeutralized( bool bNeutralized )
{
    pNeutralizedItem_->setText( 1, bNeutralized ? "Oui" : "Non" );
}
