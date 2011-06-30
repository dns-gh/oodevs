//*****************************************************************************
//
// $Created: NLD 2002-07-15 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Orders/Pion/MIL_Report.cpp $
// $Author: Nld $
// $Modtime: 21/03/05 12:17 $
// $Revision: 8 $
// $Workfile: MIL_Report.cpp $
//
//*****************************************************************************

#include "simulation_kernel_pch.h"
#include "MIL_Report.h"
#include "Entities/MIL_Formation.h"
#include "MIL_MissionParameter_ABC.h"
#include "MIL_ParameterType_ABC.h"
#include "Network/NET_ASN_Tools.h"
#include "Network/NET_Publisher_ABC.h"
#include "protocol/ClientSenders.h"
#include <xeumeuleu/xml.hpp>

MIL_Report::T_ReportMap      MIL_Report::reports_;
MIL_Report::T_DiaEventVector MIL_Report::diaEvents_( MIL_Report::eNbrReport );
MIL_IDManager                MIL_Report::ids_;

// -----------------------------------------------------------------------------
// Name: MIL_Report::Initialize
// Created: NLD 2006-12-06
// -----------------------------------------------------------------------------
void MIL_Report::Initialize( xml::xistream& xis )
{
    MT_LOG_INFO_MSG( "Initializing reports types" );
    xis >> xml::start( "reports" )
            >> xml::list( "report", &MIL_Report::ReadReport )
        >> xml::end;

    diaEvents_[ eReport_ReAvailableAfterRepairation                 ] = eRC_ANouveauDisponibleApresReparation;
    diaEvents_[ eReport_EquipementRepairedInPlace                   ] = eRC_MaterielRepareSurPlace;
    diaEvents_[ eReport_EquipementBackFromMaintenance               ] = eRC_MaterielRetourDeMaintenance;
    diaEvents_[ eReport_IndirectFireObservation                     ] = eRC_ObservationTirIndirect;
    diaEvents_[ eReport_StockSupplyRequest                          ] = eRC_DemandeRavitaillementStocks;
    diaEvents_[ eReport_DotationSupplyRequest                       ] = eRC_DemandeRavitaillementDotations;
    diaEvents_[ eReport_MedicalEvacuationRequest                    ] = eRC_DemandeEvacuationSanitaire;
    diaEvents_[ eReport_EquipementEvacuationRequest                 ] = eRC_DemandeEvacuationMateriel;
    diaEvents_[ eReport_DotationSupplyDone                          ] = eRC_RavitaillementDotationsEffectue;
    diaEvents_[ eReport_StockSupplyDone                             ] = eRC_RavitaillementStockEffectue;
    diaEvents_[ eReport_DotationSupplyCanceled                      ] = eRC_RavitaillementDotationsAnnule;
    diaEvents_[ eReport_StockSupplyCanceled                         ] = eRC_RavitaillementStockAnnule;
    diaEvents_[ eReport_LogisticStockThresholdExceeded              ] = eRC_SeuilLogistiqueStockDepasse;
    diaEvents_[ eReport_LogisticDotationThresholdExceeded           ] = eRC_SeuilLogistiqueDotationDepasse;
    diaEvents_[ eReport_HumanBackFromMedical                        ] = eRC_HumainRetourDeSante;
    diaEvents_[ eReport_WoundedManDeath                             ] = eRC_DecesBlesse;
    diaEvents_[ eReport_WoundedManDeathDuringTransport              ] = eRC_DecesBlessePendantTransport;
    diaEvents_[ eReport_WoundedManDeathDuringHospitalization        ] = eRC_DecesBlessePendantHospitalisation;
    diaEvents_[ eReport_FireInForbiddenArea                         ] = eRC_TirDansZoneInterdite;
    diaEvents_[ eReport_FratricideIndirectFire                      ] = eRC_TirIndirectFratricide;
    diaEvents_[ eReport_IndirectFireOnPopulation                    ] = eRC_TirIndirectSurPopulation;
    diaEvents_[ eReport_FireOnFriendSide                            ] = eRC_TirSurCampAmi;
    diaEvents_[ eReport_FiredByFriendSide                           ] = eRC_TireParCampAmi;
    diaEvents_[ eReport_FireOnNeutralSide                           ] = eRC_TirSurCampNeutre;
    diaEvents_[ eReport_FiredByNeutralSide                          ] = eRC_TireParCampNeutre;
    diaEvents_[ eReport_FireOnCivilian                              ] = eRC_TirSurCivil;
    diaEvents_[ eReport_FiredByCivilian                             ] = eRC_TireParCivil;
    diaEvents_[ eReport_CommandPostDestruction                      ] = eRC_DestructionPC;
    diaEvents_[ eReport_QuotaAlmostConsumed                         ] = eRC_AllocationConsentieBientotEpuisee;
    diaEvents_[ eReport_StockCapacityExceeded                       ] = eRC_DepassementCapaciteStockage;
    diaEvents_[ eReport_MissionImpossible_                          ] = eRC_MissionImpossible;
    diaEvents_[ eReport_UnitDecontaminated                          ] = eRC_UniteDecontaminee;
    diaEvents_[ eReport_OutOfGas                                    ] = eRC_PlusDeCarburant;
    diaEvents_[ eReport_DifficultTerrain                            ] = eRC_TerrainDifficile;
    diaEvents_[ eReport_Surrendered                                 ] = eRC_Rendu;
    diaEvents_[ eReport_CancelSurrender                             ] = eRC_RedditionAnnulee;
    diaEvents_[ eReport_MaintenanceWorkRateExceeded                 ] = eRC_RegimeMaintenanceDelaiDepasse;
    diaEvents_[ eReport_EquipmentLoanInProgress                     ] = eRC_PretMaterielEnCours;
    diaEvents_[ eReport_EquipmentLoanCanceled                       ] = eRC_PretMaterielAnnule;
    diaEvents_[ eReport_EquipmentLoanDone                           ] = eRC_PretMaterielEffectue;
    diaEvents_[ eReport_EquipmentLoanPartiallyDone                  ] = eRC_PretMaterielPartiellementEffectue;
    diaEvents_[ eReport_EquipmentLoanImpossible                     ] = eRC_PretMaterielImpossible;
    diaEvents_[ eReport_EquipmentLoanRetrievingDone                 ] = eRC_RecuperationMaterielPreteEffectuee;
    diaEvents_[ eReport_EquipmentLoanRetrievingPartiallyDone        ] = eRC_RecuperationMaterielPretePartiellementEffectuee;
    diaEvents_[ eReport_EquipmentLoanRetrievingImpossible           ] = eRC_RecuperationMaterielPreteImpossible;
    diaEvents_[ eReport_EquipmentLent                               ] = eRC_MaterielPrete;
    diaEvents_[ eReport_EquipmentLoanGivenBack                      ] = eRC_MaterielRendu;
    diaEvents_[ eReport_InterventionAgainstPopulationStarted        ] = eRC_DebutInterventionFaceAPopulation;
    diaEvents_[ eReport_MineExplosionOnPopulation                   ] = eRC_PopulationVictimeExplosionMines;
    diaEvents_[ eReport_ConfrontationWithPopulation                 ] = eRC_PopulationVictimeAffrontements;
    diaEvents_[ eReport_Blocked                                     ] = eRC_Bloquee;
    diaEvents_[ eReport_Filtered                                    ] = eRC_Filtree;
    diaEvents_[ eReport_TerroristAttackAgainstPopulation            ] = eRC_AttentatTerroristeDansPopulation;
    diaEvents_[ eReport_EvacuationResourcesLevelReached             ] = eRC_AlerteDisponibiliteMoyensReleve;
    diaEvents_[ eReport_CollectionResourcesLevelReached             ] = eRC_AlerteDisponibiliteMoyensRamassage;
    diaEvents_[ eReport_DoctorResourcesLevelReached                 ] = eRC_AlerteDisponibiliteMedecins;
    diaEvents_[ eReport_RepairerResourcesLevelReached               ] = eRC_AlerteDisponibiliteRemorqueurs;
    diaEvents_[ eReport_HaulerResourcesLevelReached                 ] = eRC_AlerteDisponibiliteReparateurs;
    diaEvents_[ eReport_ConvoyTransporterResourcesLevelReached      ] = eRC_AlerteDisponibiliteVecteurs;

    diaEvents_[ eReport_IndirectFireOnTarget] = eRC_TirIndirectSurCible;
    diaEvents_[ eReport_MunitionInterdite] = eRC_MunitionInterdite;
    diaEvents_[ eReport_MunitionAutorise] = eRC_MunitionAutorise;

    diaEvents_[ eReport_EmbarkmentStarted ]        = eRC_EmbarkmentStarted;
    diaEvents_[ eReport_EmbarkmentInterrupted ]    = eRC_EmbarkmentInterrupted;
    diaEvents_[ eReport_EmbarkmentResumed ]        = eRC_EmbarkmentResumed;
    diaEvents_[ eReport_EmbarkmentFinished ]       = eRC_EmbarkmentFinished;
    diaEvents_[ eReport_DisembarkmentStarted ]     = eRC_DisembarkmentStarted;
    diaEvents_[ eReport_DisembarkmentInterrupted ] = eRC_DisembarkmentInterrupted;
    diaEvents_[ eReport_DisembarkmentResumed ]     = eRC_DisembarkmentResumed;
    diaEvents_[ eReport_DisembarkmentFinished ]    = eRC_DisembarkmentFinished;
    diaEvents_[ eReport_ActiveProtectionCounter ]  = eRC_ActiveProtectionCounter;
    diaEvents_[ eReport_ActiveProtectionHardKill ] = eRC_ActiveProtectionHardKill;
    diaEvents_[ eReport_Poisoned ] = eRC_Poisoned;
    diaEvents_[ eReport_Contaminated ] = eRC_Contaminated;
    diaEvents_[ eReport_FireOnEnemySide] = eRC_TirSurCampEnnemi;
    diaEvents_[ eReport_FiredByEnemySide] = eRC_TireParCampEnnemi;

}

// -----------------------------------------------------------------------------
// Name: MIL_Report::ReadReport
// Created: ABL 2007-07-18
// -----------------------------------------------------------------------------
void MIL_Report::ReadReport( xml::xistream& xis )
{
    unsigned int id;
    xis >> xml::attribute( "id", id );
    const MIL_Report*& pReport = reports_[ id ];
    if( pReport )
        xis.error( "Report id already defined" );
    pReport = new MIL_Report( id, xis );
}

// -----------------------------------------------------------------------------
// Name: MIL_Report constructor
// Created: NLD 2006-12-06
// -----------------------------------------------------------------------------
MIL_Report::MIL_Report( unsigned int id, xml::xistream& xis )
    : nID_( id )
{
    xis >> xml::attribute( "message", strMessage_ );
    xis >> xml::list( "parameter", *this, &MIL_Report::ReadParameter );
    ids_.Lock( id );
}

// -----------------------------------------------------------------------------
// Name: MIL_Report destructor
// Created: NLD 2006-12-06
// -----------------------------------------------------------------------------
MIL_Report::~MIL_Report()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_Report::ReadParameter
// Created: ABL 2007-07-18
// -----------------------------------------------------------------------------
void MIL_Report::ReadParameter( xml::xistream& xis )
{
    std::string strType;
    xis >> xml::attribute( "type", strType );
    const MIL_ParameterType_ABC* pParameter = MIL_ParameterType_ABC::Find( strType );
    if( !pParameter )
        xis.error( "Unknown parameter type" );
    parameters_.push_back( pParameter );
}


// -----------------------------------------------------------------------------
// Name: MIL_Report::DoSend
// Created: LDC 2009-06-16
// -----------------------------------------------------------------------------
bool MIL_Report::DoSend( client::Report& message, E_Type nType, std::vector< boost::shared_ptr<MIL_MissionParameter_ABC> >& params ) const
{
    if( params.size() != parameters_.size() )
    {
        MT_LOG_ERROR_MSG( "Report '" << strMessage_ << "' send failed (invalid DIA parameters)" );
        return false;
    }
    message().mutable_report()->set_id( std::numeric_limits< unsigned int >::max() - ids_.GetFreeId() ); // descending order
    message().mutable_type()->set_id( nID_ );
    message().set_category( sword::Report::EnumReportType( nType ) );
    NET_ASN_Tools::WriteGDH( MIL_AgentServer::GetWorkspace().GetRealTime(), *message().mutable_time() );
    for( unsigned int i = 0; i < parameters_.size(); ++i )
    {
        if( !params[ i ]->IsOfType( parameters_[i]->GetType() ) )
            return false;
        sword::MissionParameter& paramProtobuff = *message().mutable_parameters()->add_elem();
        if( !params[ i ]->ToElement( *paramProtobuff.mutable_value()->Add() ) )
            return false;
    }
    return true;
}


// -----------------------------------------------------------------------------
// Name: MIL_Report::Send
// Created: NLD 2006-12-06
// -----------------------------------------------------------------------------

template<>
void MIL_Report::Send< MIL_Automate >( const MIL_Automate& sender, E_Type nType, std::vector< boost::shared_ptr< MIL_MissionParameter_ABC > >& diaParameters ) const
{
    client::Report message;
    if( DoSend( message, nType, diaParameters ) )
    {
        message().mutable_source()->mutable_automat()->set_id( sender.GetID() );
        message.Send( NET_Publisher_ABC::Publisher() );
    }
}

template<>
void MIL_Report::Send< MIL_Population >( const MIL_Population& sender, E_Type nType, std::vector< boost::shared_ptr< MIL_MissionParameter_ABC > >& diaParameters ) const
{
    client::Report message;
    if( DoSend( message, nType, diaParameters ) )
    {
        message().mutable_source()->mutable_crowd()->set_id( sender.GetID() );
        message.Send( NET_Publisher_ABC::Publisher() );
    }
}

template<>
void MIL_Report::Send<  MIL_Agent_ABC >( const MIL_Agent_ABC& sender, E_Type nType, std::vector< boost::shared_ptr< MIL_MissionParameter_ABC > >& diaParameters ) const
{
    client::Report message;
    if( DoSend( message, nType, diaParameters ) )
    {
        message().mutable_source()->mutable_unit()->set_id( sender.GetID() );
        message.Send( NET_Publisher_ABC::Publisher() );
    }
}

template<>
void MIL_Report::Send<  MIL_AgentPion >( const MIL_AgentPion& sender, E_Type nType, std::vector< boost::shared_ptr< MIL_MissionParameter_ABC > >& diaParameters ) const
{
    MIL_Report::Send< MIL_Agent_ABC >( sender, nType, diaParameters );
}

template<>
void MIL_Report::Send< MIL_Formation >( const MIL_Formation& sender, E_Type nType, std::vector< boost::shared_ptr< MIL_MissionParameter_ABC > >& diaParameters ) const
{
    client::Report message;
    if( DoSend( message, nType, diaParameters ) )
    {
        message().mutable_source()->mutable_formation()->set_id( sender.GetID() );
        message.Send( NET_Publisher_ABC::Publisher() );
    }
}

template<>
void MIL_Report::Send< DEC_Decision_ABC >( const DEC_Decision_ABC& sender, E_Type nType, std::vector< boost::shared_ptr< MIL_MissionParameter_ABC > >& diaParameters ) const
{
    client::Report message;
    if( DoSend( message, nType, diaParameters ) )
    {        
        MIL_AgentServer::GetWorkspace().GetEntityManager().SetToTasker( *message().mutable_source(), sender.GetID() );
        message.Send( NET_Publisher_ABC::Publisher() );
    }
}