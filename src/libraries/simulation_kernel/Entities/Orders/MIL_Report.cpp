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
#include "Entities/Effects/MIL_Effect_IndirectFire.h"
#include "xeumeuleu/xml.h"

using namespace xml;


// =============================================================================
// FACTORY
// =============================================================================

MIL_Report::T_ReportMap      MIL_Report::reports_;
MIL_Report::T_DiaEventVector MIL_Report::diaEvents_( MIL_Report::eNbrReport, "" );

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
    xis >> start( "reports" )
            >> list( "report", loader, &LoadingWrapper::ReadReport )
        >> end();

    diaEvents_[ eReport_ReAvailableAfterRepairation                 ] = "EVT_RC_ANouveauDisponibleApresReparation";                
    diaEvents_[ eReport_EquipementRepairedInPlace                   ] = "EVT_RC_MaterielRepareSurPlace";                           
    diaEvents_[ eReport_EquipementBackFromMaintenance               ] = "EVT_RC_MaterielRetourDeMaintenance";                      
    diaEvents_[ eReport_IndirectFireObservation                     ] = "EVT_RC_ObservationTirIndirect";                           
    diaEvents_[ eReport_StockSupplyRequest                          ] = "EVT_RC_DemandeRavitaillementStocks";                      
    diaEvents_[ eReport_DotationSupplyRequest                       ] = "EVT_RC_DemandeRavitaillementDotations";                   
    diaEvents_[ eReport_MedicalEvacuationRequest                    ] = "EVT_RC_DemandeEvacuationSanitaire";                       
    diaEvents_[ eReport_EquipementEvacuationRequest                 ] = "EVT_RC_DemandeEvacuationMateriel";                        
    diaEvents_[ eReport_DotationSupplyDone                          ] = "EVT_RC_RavitaillementDotationsEffectue";                  
    diaEvents_[ eReport_StockSupplyDone                             ] = "EVT_RC_RavitaillementStockEffectue";                      
    diaEvents_[ eReport_DotationSupplyCanceled                      ] = "EVT_RC_RavitaillementDotationsAnnule";                    
    diaEvents_[ eReport_StockSupplyCanceled                         ] = "EVT_RC_RavitaillementStockAnnule";                        
    diaEvents_[ eReport_LogisticStockThresholdExceeded              ] = "EVT_RC_SeuilLogistiqueStockDepasse";                      
    diaEvents_[ eReport_LogisticDotationThresholdExceeded           ] = "EVT_RC_SeuilLogistiqueDotationDepasse";                   
    diaEvents_[ eReport_HumanBackFromMedical                        ] = "EVT_RC_HumainRetourDeSante";                              
    diaEvents_[ eReport_WoundedManDeath                             ] = "EVT_RC_DecesBlesse";                                      
    diaEvents_[ eReport_WoundedManDeathDuringTransport              ] = "EVT_RC_DecesBlessePendantTransport";                      
    diaEvents_[ eReport_WoundedManDeathDuringHospitalization        ] = "EVT_RC_DecesBlessePendantHospitalisation";                
    diaEvents_[ eReport_FireInForbiddenArea                         ] = "EVT_RC_TirDansZoneInterdite";                             
    diaEvents_[ eReport_FratricideIndirectFire                      ] = "EVT_RC_TirIndirectFratricide";                            
    diaEvents_[ eReport_IndirectFireOnPopulation                    ] = "EVT_RC_TirIndirectSurPopulation";                         
    diaEvents_[ eReport_FireOnFriendSide                            ] = "EVT_RC_TirSurCampAmi";                                    
    diaEvents_[ eReport_FiredByFriendSide                           ] = "EVT_RC_TireParCampAmi";                                   
    diaEvents_[ eReport_FireOnNeutralSide                           ] = "EVT_RC_TirSurCampNeutre";                                 
    diaEvents_[ eReport_FiredByNeutralSide                          ] = "EVT_RC_TireParCampNeutre";                                
    diaEvents_[ eReport_FireOnCivilian                              ] = "EVT_RC_TirSurCivil";                                      
    diaEvents_[ eReport_FiredByCivilian                             ] = "EVT_RC_TireParCivil";                                     
    diaEvents_[ eReport_CommandPostDestruction                      ] = "EVT_RC_DestructionPC";                                    
    diaEvents_[ eReport_QuotaAlmostConsumed                         ] = "EVT_RC_AllocationConsentieBientotEpuisee";                
    diaEvents_[ eReport_StockCapacityExceeded                       ] = "EVT_RC_DepassementCapaciteStockage";                      
    diaEvents_[ eReport_MissionImpossible_                          ] = "EVT_RC_MissionImpossible";                                
    diaEvents_[ eReport_UnitDecontaminated                          ] = "EVT_RC_UniteDecontaminee";                                
    diaEvents_[ eReport_OutOfGas                                    ] = "EVT_RC_PlusDeCarburant";                                  
    diaEvents_[ eReport_DifficultTerrain                            ] = "EVT_RC_TerrainDifficile";                                 
    diaEvents_[ eReport_Surrendered                                 ] = "EVT_RC_Rendu";                                            
    diaEvents_[ eReport_CancelSurrender                             ] = "EVT_RC_RedditionAnnulee";    
    diaEvents_[ eReport_MaintenanceWorkRateExceeded                 ] = "EVT_RC_RegimeMaintenanceDelaiDepasse";                    
    diaEvents_[ eReport_EquipmentLoanInProgress                     ] = "EVT_RC_PretMaterielEnCours";                              
    diaEvents_[ eReport_EquipmentLoanCanceled                       ] = "EVT_RC_PretMaterielAnnule";                               
    diaEvents_[ eReport_EquipmentLoanDone                           ] = "EVT_RC_PretMaterielEffectue";                             
    diaEvents_[ eReport_EquipmentLoanPartiallyDone                  ] = "EVT_RC_PretMaterielPartiellementEffectue";                
    diaEvents_[ eReport_EquipmentLoanImpossible                     ] = "EVT_RC_PretMaterielImpossible";                           
    diaEvents_[ eReport_EquipmentLoanRetrievingDone                 ] = "EVT_RC_RecuperationMaterielPreteEffectuee";               
    diaEvents_[ eReport_EquipmentLoanRetrievingPartiallyDone        ] = "EVT_RC_RecuperationMaterielPretePartiellementEffectuee";  
    diaEvents_[ eReport_EquipmentLoanRetrievingImpossible           ] = "EVT_RC_RecuperationMaterielPreteImpossible";              
    diaEvents_[ eReport_EquipmentLent                               ] = "EVT_RC_MaterielPrete";                                    
    diaEvents_[ eReport_EquipmentLoanGivenBack                      ] = "EVT_RC_MaterielRendu";                                    
    diaEvents_[ eReport_InterventionAgainstPopulationStarted        ] = "EVT_RC_DebutInterventionFaceAPopulation";                 
    diaEvents_[ eReport_MineExplosionOnPopulation                   ] = "EVT_RC_PopulationVictimeExplosionMines";                  
    diaEvents_[ eReport_ConfrontationWithPopulation                 ] = "EVT_RC_PopulationVictimeAffrontements";                   
    diaEvents_[ eReport_Blocked                                     ] = "EVT_RC_Bloquee";                                          
    diaEvents_[ eReport_Filtered                                    ] = "EVT_RC_Filtree";                                          
    diaEvents_[ eReport_TerroristAttackAgainstPopulation            ] = "EVT_RC_AttentatTerroristeDansPopulation";                 
    diaEvents_[ eReport_EvacuationResourcesLevelReached             ] = "EVT_RC_AlerteDisponibiliteMoyensReleve";                  
    diaEvents_[ eReport_CollectionResourcesLevelReached             ] = "EVT_RC_AlerteDisponibiliteMoyensRamassage";               
    diaEvents_[ eReport_DoctorResourcesLevelReached                 ] = "EVT_RC_AlerteDisponibiliteMedecins";                      
    diaEvents_[ eReport_RepairerResourcesLevelReached               ] = "EVT_RC_AlerteDisponibiliteRemorqueurs";                   
    diaEvents_[ eReport_HaulerResourcesLevelReached                 ] = "EVT_RC_AlerteDisponibiliteReparateurs";                   
    diaEvents_[ eReport_ConvoyTransporterResourcesLevelReached      ] = "EVT_RC_AlerteDisponibiliteVecteurs";                      
}

// -----------------------------------------------------------------------------
// Name: MIL_Report::ReadReport
// Created: ABL 2007-07-18
// -----------------------------------------------------------------------------
void MIL_Report::ReadReport( xml::xistream& xis )
{
    uint id;
    xis >> attribute( "id", id );

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
    xis >> attribute( "message", strMessage_ );
    xis >> list( "parameter", *this, &MIL_Report::ReadParameter );
}

// -----------------------------------------------------------------------------
// Name: MIL_Report::ReadParameter
// Created: ABL 2007-07-18
// -----------------------------------------------------------------------------
void MIL_Report::ReadParameter( xml::xistream& xis )
{
    std::string strType;
    xis >> attribute( "type", strType );
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
// Name: MIL_Report::Send
// Created: NLD 2006-12-06
// -----------------------------------------------------------------------------
bool MIL_Report::Send( uint nSenderID, E_Type nType, const DEC_KnowledgeResolver_ABC& knowledgeResolver, const DIA_Parameters& diaParameters ) const
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
    asn().type         = (ASN1T_EnumReportType)nType;
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
