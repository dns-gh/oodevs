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
#include "MIL_ParameterType_ABC.h"
#include "Network/NET_ASN_Messages.h"
#include "Network/NET_ASN_Tools.h"
#include "MIL_AgentServer.h"
#include "Entities/Effects/MIL_Effect_IndirectFire.h"
#include <xeumeuleu/xml.h>




// =============================================================================
// FACTORY
// =============================================================================

MIL_Report::T_ReportMap      MIL_Report::reports_;
MIL_Report::T_DiaEventVector MIL_Report::diaEvents_( MIL_Report::eNbrReport );
MT_IdentifierManager         MIL_Report::ids_;

struct MIL_Report::LoadingWrapper
{
    void ReadReport( xml::xistream& xis )
    {
        MIL_Report::ReadReport( xis );
    }
};

// -----------------------------------------------------------------------------
// Name: MIL_Report::Initialize
// Created: NLD 2006-12-06
// -----------------------------------------------------------------------------
void MIL_Report::Initialize( xml::xistream& xis )
{
    MT_LOG_INFO_MSG( "Initializing reports types" );

    LoadingWrapper loader;
    xis >> xml::start( "reports" )
            >> xml::list( "report", loader, &LoadingWrapper::ReadReport )
        >> xml::end();

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
}

// -----------------------------------------------------------------------------
// Name: MIL_Report::ReadReport
// Created: ABL 2007-07-18
// -----------------------------------------------------------------------------
void MIL_Report::ReadReport( xml::xistream& xis )
{
    uint id;
    xis >> xml::attribute( "id", id );

    const MIL_Report*& pReport = reports_[ id ];
    if( pReport )
        xis.error( "Report id already defined" );
    pReport = new MIL_Report( id, xis );
}

// =============================================================================
// INSTANCE
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_Report constructor
// Created: NLD 2006-12-06
// -----------------------------------------------------------------------------
MIL_Report::MIL_Report( uint id, xml::xistream& xis )
    : nID_       ( id )
    , strMessage_()
{
    xis >> xml::attribute( "message", strMessage_ );
    xis >> xml::list( "parameter", *this, &MIL_Report::ReadParameter );
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
// Name: MIL_Report destructor
// Created: NLD 2006-12-06
// -----------------------------------------------------------------------------
MIL_Report::~MIL_Report()
{
}

// =============================================================================
// OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_Report::DoSend
// Created: NLD 2006-12-06
// -----------------------------------------------------------------------------
bool MIL_Report::DoSend( uint nSenderID, E_Type nType, const DEC_KnowledgeResolver_ABC& knowledgeResolver, const DIA_Parameters& diaParameters ) const
{
    // DIA parameter 0 : Report ID
    if( ( diaParameters.GetParameters().size() - 1 ) != parameters_.size() )
    {
        MT_LOG_ERROR_MSG( "Report '" << strMessage_ << "' send failed (invalid DIA parameters)" );
        return false;
    }

    NET_ASN_MsgReport asn;
 
    asn().oid          = nSenderID;
    asn().cr           = nID_;
    asn().cr_oid       = std::numeric_limits<uint>::max() - ids_.GetFreeIdentifier(); // descending order
    asn().type         = (ASN1T_EnumReportType)nType;
    NET_ASN_Tools::WriteGDH( MIL_AgentServer::GetWorkspace().GetRealTime(), asn().time );
    asn().parametres.n = parameters_.size();
    if( !parameters_.empty() )
    {
        asn().parametres.elem = new ASN1T_MissionParameter[ parameters_.size() ];
    
        uint nDiaParameter = 1;
        for( CIT_ParameterVector it = parameters_.begin(); it != parameters_.end(); ++it, ++nDiaParameter )
        {
            const DIA_Variable_ABC& diaParameter = const_cast< DIA_Parameters& >( diaParameters ).GetParameter( nDiaParameter );
            if( !(**it).Copy( diaParameter, asn().parametres.elem[ nDiaParameter - 1 ], knowledgeResolver, false /*not optional*/ ) )
                return false; //$$$ Memory leak
        }
    }

    asn.Send();

    uint nTmp = 0;
    for( CIT_ParameterVector it = parameters_.begin(); it != parameters_.end(); ++it, ++nTmp )
        (**it).CleanAfterSerialization( asn().parametres.elem[ nTmp ] );
    if( asn().parametres.n > 0 )
        delete [] asn().parametres.elem;

    return true;
}
