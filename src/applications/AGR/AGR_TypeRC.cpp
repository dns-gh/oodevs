// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: NLD 2005-09-07 $
// $Archive: $
// $Author: $
// $Modtime: $
// $Revision: $
// $Workfile: $
//
// *****************************************************************************

#include "AGR_pch.h"

#include "AGR_TypeRC.h"
#include "AGR_Workspace.h"
#include "AGR_Type_ABC.h"
#include "AGR_RC.h"
#include <boost/algorithm/string.hpp>

AGR_TypeRC::T_RCTypesMap AGR_TypeRC::rcTypes_;

// -----------------------------------------------------------------------------
// Name: AGR_TypeRC::Param
// Created: NLD 2005-09-07
// -----------------------------------------------------------------------------
AGR_TypeRC::Param::Param( MT_XXmlInputArchive& input, const AGR_Workspace& workspace )
    : strName_  ( )
    , pType_    ()
    , bOptional_( false )
{
    input.Section( "xsd:element" );
    std::string strParamType;
    input.ReadAttribute( "name", strName_ );
    input.ReadAttribute( "type", strParamType );

    pType_ = workspace.FindType( strParamType );
    if( !pType_ )
        throw std::exception( "Unable to determine type for RC" );

    uint nTmp;
    input.EnableExceptions( false );
    bool bOptional = input.ReadAttribute( "minOccurs", nTmp );
    if( !bOptional )
        input.ClearLastError();
    input.EnableExceptions( true );

    input.EndSection(); // xsd:element
}

// -----------------------------------------------------------------------------
// Name: AGR_TypeRC constructor
// Created: NLD 2005-09-07
// -----------------------------------------------------------------------------
AGR_TypeRC::AGR_TypeRC( const std::string& strName, MT_XXmlInputArchive& input, const AGR_Workspace& workspace )
    : strAsnName_    ( strName )
    , strName_       ()
    , strMILEnumName_( )
    , strHumanName_  ()
    , pType_         ( 0 )
{
    strName_        = strAsnName_.substr( 3 );
    strMILEnumName_ = "eRC_" + strName_;

    strHumanName_ = strName_;
    std::string::iterator prevIt = strHumanName_.begin();
    for( std::string::iterator it = strHumanName_.begin(); it != strHumanName_.end(); )
    {
        std::string::iterator nextIt = it;
        ++ nextIt;
        if( it != strHumanName_.begin() )
        {
            prevIt = it;
            --prevIt;
        }
        if(    ( it != strHumanName_.begin() && nextIt != strHumanName_.end() && boost::is_upper()( *it ) && boost::is_lower()( *nextIt ) )
            || ( boost::is_upper()( *it ) && boost::is_lower() ( *prevIt ) ) )
        {
            it = strHumanName_.insert( it, ' ' );
            ++it;
            ++it;
            
        }
        else
        {
            ++it;
        }
    }
    boost::to_lower( strHumanName_ );

    std::string strSubSection = input.GetCurrentElementName();
    if( strSubSection == "xsd:restriction" )
    {
        input.Section( "xsd:restriction" );
        std::string strType;
        input.ReadAttribute( "base", strType );
        pType_ = workspace.FindType( strType );
        if( !pType_ )
            throw std::exception( "Unable to determine type for RC"  );
        input.EndSection(); // "xsd:restriction"a
    }
    else if( strSubSection == "xsd:sequence" )
    {
        input.BeginList( "xsd:sequence" );
        while( input.NextListElement() )
        {            
            Param* pParam = new Param( input, workspace );
            params_.push_back( pParam );
        }
        input.EndList(); // xsd:sequence
    }
}

// -----------------------------------------------------------------------------
// Name: AGR_TypeRC destructor
// Created: NLD 2005-09-07
// -----------------------------------------------------------------------------
AGR_TypeRC::~AGR_TypeRC()
{

}

// =============================================================================
// GENERATORS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: AGR_TypeRC::GenerateEnum
// Created: NLD 2005-09-07
// -----------------------------------------------------------------------------
std::string AGR_TypeRC::GenerateEnum() const
{
    std::stringstream strTmp;

    strTmp << strMILEnumName_
           << ",";

    if( pType_ )
    {
        assert( params_.empty() );
        strTmp <<  " // Parametre : "
               << pType_->GetDIAType();
    }
    else if( !params_.empty() )
    {
        if( params_.size() > 1 )
            strTmp <<  " // Parametres : ";
        else
            strTmp <<  " // Parametre : ";
        for( CIT_ParamVector it = params_.begin(); it != params_.end(); ++it )
        {
            strTmp << (**it).strName_
                   << " (" << (**it).pType_->GetDIAType() << ")";
            if( it != params_.rbegin().base() )
                strTmp << ", ";
        }
    }
    strTmp << std::endl;
    return strTmp.str();
}

// -----------------------------------------------------------------------------
// Name: AGR_TypeRC::GenerateHALEnum
// Created: NLD 2005-09-07
// -----------------------------------------------------------------------------
std::string AGR_TypeRC::GenerateHALEnum( const AGR_RC& /*rcInstance*/ ) const
{
    return GenerateEnum();
}

// -----------------------------------------------------------------------------
// Name: AGR_TypeRC::GenerateMILRCClassHeader
// Created: NLD 2005-09-07
// -----------------------------------------------------------------------------
std::string AGR_TypeRC::GenerateMILRCClassHeader( const AGR_RC& /*rcInstance*/ ) const
{
    return GenerateEnum();
}

// -----------------------------------------------------------------------------
// Name: AGR_TypeRC::GenerateMILRCClassBodyInitialize
// Created: NLD 2005-09-07
// -----------------------------------------------------------------------------
std::string AGR_TypeRC::GenerateMILRCClassBodyInitialize( const AGR_RC& rcInstance ) const
{
    std::stringstream strTmp;

    strTmp << "rcs_[ " << strMILEnumName_ << " ] = new ";

    if( pType_ || !params_.empty() )
        strTmp << "MIL_RC_" << strName_;
    else
        strTmp << "MIL_RC";

    strTmp << "( " 
                << strMILEnumName_ 
                << ", "
                << "T_MsgCR_cr_" << rcInstance.GetAsnName()
           << " );";

    strTmp << std::endl;
    return strTmp.str();

}

// -----------------------------------------------------------------------------
// Name: AGR_TypeRC::GenerateMILRCClassBodyIncludes
// Created: NLD 2005-09-07
// -----------------------------------------------------------------------------
std::string AGR_TypeRC::GenerateMILRCClassBodyIncludes( const AGR_RC& rcInstance ) const
{
    std::stringstream strTmp;
    if( pType_ || !params_.empty() )
        strTmp << "#include \"MIL_RC_" << strName_ << ".h\"" << std::endl;

    return strTmp.str();
}

// -----------------------------------------------------------------------------
// Name: AGR_TypeRC::GenerateMOS2
// Created: NLD 2005-09-07
// -----------------------------------------------------------------------------
std::string AGR_TypeRC::GenerateMOS2( const AGR_RC& rcInstance ) const
{
    std::stringstream strTmp;

    strTmp << "case T_MsgCR_cr_" << rcInstance.GetAsnName() 
           << " : strMsg << \"" << strHumanName_ << "\"";


    if( pType_ )
    {
        assert( params_.empty() );
        strTmp << " << \" - " << pType_->GetHumanName() << " : \""
               << " << \" \" << "
               << pType_->MOS2RCCode( "asnMsg.cr.u." + rcInstance.GetAsnName() );

    }
    else if( !params_.empty() )
    {
        for( CIT_ParamVector it = params_.begin(); it != params_.end(); ++it )
        {
            strTmp << " << \" - "
                   << (**it).strName_
                   << " : "
                   << "\" << "
                   << (**it).pType_->MOS2RCCode( "asnMsg.cr.u." + rcInstance.GetAsnName() + "->" + (**it).strName_ );        
        }

    }

    strTmp << "; break;" << std::endl;
    return strTmp.str();
}

// -----------------------------------------------------------------------------
// Name: AGR_TypeRC::GenerateMOS
// Created: NLD 2005-09-07
// -----------------------------------------------------------------------------
std::string AGR_TypeRC::GenerateMOS( const AGR_RC& rcInstance ) const
{
    std::stringstream strTmp;

    strTmp << "case T_MsgCR_cr_" << rcInstance.GetAsnName() 
           << " : strMsg << \"" << strHumanName_ << "\"";


    if( pType_ )
    {
        assert( params_.empty() );
        strTmp << " << \" - " << pType_->GetHumanName() << " : \""
               << " << \" \" << "
               << pType_->MOSRCCode( "asnMsg.cr.u." + rcInstance.GetAsnName() );

    }
    else if( !params_.empty() )
    {
        for( CIT_ParamVector it = params_.begin(); it != params_.end(); ++it )
        {
            strTmp << " << \" - "
                   << (**it).strName_
                   << " : "
                   << "\" << "
                   << (**it).pType_->MOSRCCode( "asnMsg.cr.u." + rcInstance.GetAsnName() + "->" + (**it).strName_ );        
        }

    }

    strTmp << "; break;" << std::endl;
    return strTmp.str();
}

// -----------------------------------------------------------------------------
// Name: AGR_TypeRC::Initialize
// Created: SBO 2005-09-14
// -----------------------------------------------------------------------------
void AGR_TypeRC::Initialize()
{
	rcTypes_[ "eRC_RAS" ] = "eReportEnemy";
	rcTypes_[ "eRC_EnPosteFaceAObstacle" ] =  "eReportObstacle";
	rcTypes_[ "eRC_EnContournementObstacle" ] = "eReportObstacle";
	rcTypes_[ "eRC_MissionImpossible" ] = "eReportMission";
	rcTypes_[ "eRC_PretPourNouvelleMission" ] = "eReportMission";
	rcTypes_[ "eRC_RejointFuseau" ] = "eReportMission";
	rcTypes_[ "eRC_FuseauRejoint" ] = "eReportMission";
	rcTypes_[ "eRC_ProgressionSurAxe" ] = "eReportMission";
	rcTypes_[ "eRC_ProgressionVersPosition" ] = "eReportMission"; // ProgressionVersLocalisation
	rcTypes_[ "eRC_ArriveSurPosition" ] = "eReportMission"; // ArriveSurLocalisation
	rcTypes_[ "eRC_ReconnaissancePoint" ] = "eReportMission"; // ReconnaissanceLocalisation
	rcTypes_[ "eRC_EnPosteFaceADirectionDangereuse" ] = "eReportMission";
	rcTypes_[ "eRC_EnPosteSurBond" ] = "eReportMission";
	rcTypes_[ "eRC_EnJalonnement" ] = "eReportMission";
	rcTypes_[ "eRC_PreparationMission" ] = "eReportMission";
	rcTypes_[ "eRC_DebutMission" ] = "eReportMission";
	rcTypes_[ "eRC_FinMission" ] = "eReportMission";
	rcTypes_[ "eRC_DispositifInstalle" ] = "eReportMission";
	rcTypes_[ "eRC_EnPosition" ] = "eReportMission";
	rcTypes_[ "eRC_ObjectifAtteint" ] = "eReportMission";
	rcTypes_[ "eRC_Debouche" ] = "eReportMission";
	rcTypes_[ "eRC_SurPIA" ] = "eReportMission";
	rcTypes_[ "eRC_PassageSurPIA" ] = "eReportMission";
	rcTypes_[ "eRC_UniteRecueillie" ] = "eReportMission";
	rcTypes_[ "eRC_EnCoursDeFranchissement" ] = "eReportMission";
	rcTypes_[ "eRC_AttenteOrdrePoursuivre" ] = "eReportMission";
	rcTypes_[ "eRC_AttenteOrdreDeboucher" ] = "eReportMission";
	rcTypes_[ "eRC_DebutControleZone" ] = "eReportMission"; // EnControleZone
	rcTypes_[ "eRC_EnSurveillance" ] = "eReportMission";
	rcTypes_[ "eRC_EnInfiltration" ] = "eReportMission";
	rcTypes_[ "eRC_EnExfiltration" ] = "eReportMission";
	rcTypes_[ "eRC_AttenteConditionFavorablePourInfiltration" ] = "eReportMission";
	rcTypes_[ "eRC_DroneLance" ] = "eReportMission";
	rcTypes_[ "eRC_DroneEnExploitation" ] = "eReportMission";
	rcTypes_[ "eRC_DroneExploite" ] = "eReportMission";
	rcTypes_[ "eRC_DebutMiseEnOeuvreDrone" ] = "eReportMission";
	rcTypes_[ "eRC_FinMiseEnOeuvreDrone" ] = "eReportMission";
	rcTypes_[ "eRC_Brouille" ] = "eReportMission";
	rcTypes_[ "eRC_EnEcoute" ] = "eReportMission";
	rcTypes_[ "eRC_EnEcouteRadar" ] = "eReportMission";
	rcTypes_[ "eRC_PriseContact" ] = "eReportEnemy"; // AutomatePriseContact
	rcTypes_[ "eRC_RuptureContact" ] = "eReportEnemy"; // AutomateRuptureContact
	rcTypes_[ "eRC_PionPriseContact" ] = "eReportEnemy";
	rcTypes_[ "eRC_PionRuptureContact" ] = "eReportEnemy";
	rcTypes_[ "eRC_PrecisionContact" ] = "eReportEnemy";
	rcTypes_[ "eRC_EnnemiDetruit" ] = "eReportEnemy";
	rcTypes_[ "eRC_PerteDeContactVisuel" ] = "eReportEnemy";
	rcTypes_[ "eRC_PosteFaceEni" ] = "eReportEnemy";
	rcTypes_[ "eRC_PrisAPartieParTirDirect" ] = "eReportEnemy"; // PriseAPartie
	rcTypes_[ "eRC_PointTenuParEni" ] = "eReportEnemy";
	rcTypes_[ "eRC_ProgressionVersEni" ] = "eReportEnemy";
	rcTypes_[ "eRC_PresenceEniProbable" ] = "eReportEnemy";
	rcTypes_[ "eRC_Decroche" ] = "eReportEnemy";
	rcTypes_[ "eRC_EniSurObjectif" ] = "eReportEnemy";
	rcTypes_[ "eRC_Riposte" ] = "eReportEnemy";
	rcTypes_[ "eRC_Releve" ] = "eReportMission";
	rcTypes_[ "eRC_SuitUnite" ] = "eReportMission";
	rcTypes_[ "eRC_PasseEnAppui" ] = "eReportMission";
	rcTypes_[ "eRC_EnAppui" ] = "eReportMission";
	rcTypes_[ "eRC_DetruitTactiquement" ] = "eReportLogisticSituation";
	rcTypes_[ "eRC_AmbianceVitesse" ] = "eReportMission";
	rcTypes_[ "eRC_AmbianceSurete" ] = "eReportMission";
	rcTypes_[ "eRC_SurLima" ] = "eReportMission";
	rcTypes_[ "eRC_SurTypeTerrainParticulier" ] = "eReportMission";
	rcTypes_[ "eRC_ObstacleEnAttenteActivation" ] = "eReportObstacle";
	rcTypes_[ "eRC_FinTravaux" ] = "eReportObstacle"; // TravauxRealises
	rcTypes_[ "eRC_ObstacleDegage" ] = "eReportObstacle";
	rcTypes_[ "eRC_OrganisationChantier" ] = "eReportMission";
	rcTypes_[ "eRC_DesorganisationChantier" ] = "eReportMission";
	rcTypes_[ "eRC_ReconnaissancePositionObstacle" ] = "eReportMission";
	rcTypes_[ "eRC_ContournementRealise" ] = "eReportObstacle";
	rcTypes_[ "eRC_AttenteRenforcement" ] = "eReportMission";
	rcTypes_[ "eRC_DebutTravaux" ] = "eReportMission";
	rcTypes_[ "eRC_DebutFranchissement" ] = "eReportMission";
	rcTypes_[ "eRC_FinFranchissement" ] = "eReportMission";
	rcTypes_[ "eRC_ReussiteRenforcement" ] = "eReportMission";
	rcTypes_[ "eRC_EchecRenforcement" ] = "eReportMission";
	rcTypes_[ "eRC_ItineraireRetabli" ] = "eReportObstacle";
	rcTypes_[ "eRC_RenforcementBientotFini" ] = "eReportMission";
	rcTypes_[ "eRC_SituationLogFavorable" ] = "eReportLogisticSituation";
	rcTypes_[ "eRC_SituationLogDefavorable" ] = "eReportLogisticSituation";
    rcTypes_[ "eRC_RegimeMaintenanceDelaiDepasse" ] = "eReportLogisticSituation";
	rcTypes_[ "eRC_SectionDeployee" ] = "eReportMission";
	rcTypes_[ "eRC_MaterielRetourDeMaintenance" ] = "eReportLogisticSituation";
	rcTypes_[ "eRC_DemandeRavitaillementDotations" ] = "eReportLogisticSituation";
    rcTypes_[ "eRC_DemandeRavitaillementStock" ] = "eReportLogisticSituation";
    rcTypes_[ "eRC_SeuilLogistiqueDotationDepasse" ] = "eReportLogisticSituation";
    rcTypes_[ "eRC_SeuilLogistiqueStockDepasse" ] = "eReportLogisticSituation";
	rcTypes_[ "eRC_DemandeEvacuationSanitaire" ] = "eReportLogisticSituation";
	rcTypes_[ "eRC_DemandeEvacuationMateriel" ] = "eReportLogisticSituation";
	rcTypes_[ "eRC_AllocationConsentieBientotEpuisee" ] = "eReportLogisticSituation";
    rcTypes_[ "eRC_DepassementCapaciteStockage" ] = "eReportLogisticSituation";
	rcTypes_[ "eRC_EnCoursRavitaillement" ] = "eReportLogisticSituation";
	rcTypes_[ "eRC_RavitaillementTermine" ] = "eReportLogisticSituation";
	rcTypes_[ "eRC_HelicoptereEnObservation" ] = "eReportMission";
	rcTypes_[ "eRC_PerteInfoGuidage" ] = "eReportMission";
	rcTypes_[ "eRC_TransportUnitePasPrete" ] = "eReportMission";
	rcTypes_[ "eRC_TransportEnCours" ] = "eReportMission";
	rcTypes_[ "eRC_TransportEmbarquement" ] = "eReportMission";
	rcTypes_[ "eRC_TransportDebarquement" ] = "eReportMission";
	rcTypes_[ "eRC_VehiculeLaisseSurPosition" ] = "eReportMission";
	rcTypes_[ "eRC_RejointPositionEmbarquement" ] = "eReportMission";
	rcTypes_[ "eRC_EnAttenteTransporteurs" ] = "eReportMission";
	rcTypes_[ "eRC_ContamineNBC" ] = "eReportNbc";
	rcTypes_[ "eRC_DecontamineNBC" ] = "eReportNbc";
	rcTypes_[ "eRC_TenueProtectionNBCMise" ] = "eReportNbc";
	rcTypes_[ "eRC_TenueProtectionNBCEnlevee" ] = "eReportNbc";
	rcTypes_[ "eRC_AlerteNBCNiveau4" ] = "eReportNbc"; // AlerteNbc
    rcTypes_[ "eRC_FinAlerteNBCNiveau4" ] = "eReportNbc"; // AlerteNbc
	rcTypes_[ "eRC_SiteDecontaminationAnime" ] = "eReportMission";
	rcTypes_[ "eRC_SiteDecontaminationDesactive" ] = "eReportMission";
	rcTypes_[ "eRC_DemandeDecontamination" ] = "eReportNbc";
	rcTypes_[ "eRC_ZoneReconnue" ] = "eReportMission";
	rcTypes_[ "eRC_ZoneDecontaminee" ] = "eReportMission";
	rcTypes_[ "eRC_SiteROTAReconnu" ] = "eReportMission";
	rcTypes_[ "eRC_IncidentNBCTermine" ] = "eReportNbc";
	rcTypes_[ "eRC_PionDecontamine" ] = "eReportNbc";
	rcTypes_[ "eRC_ReconnaissanceTerminee" ] = "eReportMission";
	rcTypes_[ "eRC_ReconnaissanceInitialeTerminee" ] = "eReportMission";
	rcTypes_[ "eRC_RejointPlotRavitaillement" ] = "eReportMission";
	rcTypes_[ "eRC_PremierCoupParti" ] = "eReportArtillery";
	rcTypes_[ "eRC_TirExecute" ] = "eReportArtillery";
	rcTypes_[ "eRC_TirHorsDePortee" ] = "eReportArtillery";
	rcTypes_[ "eRC_TirAnnule" ] = "eReportArtillery";
	rcTypes_[ "eRC_ChangementDePosition" ] = "eReportMission";
	rcTypes_[ "eRC_DebutSurveillance" ] = "eReportMission";
	rcTypes_[ "eRC_DebutInstallation" ] = "eReportMission";
	rcTypes_[ "eRC_PretPourConduiteTir" ] = "eReportArtillery";
	rcTypes_[ "eRC_ObservationTirIndirect" ] = "eReportArtillery";
	rcTypes_[ "eRC_ZoneImplantationOccupee" ] = "eReportMission";
	rcTypes_[ "eRC_InstallationSurZoneImplantation" ] = "eReportMission";
	rcTypes_[ "eRC_ReceptionOrdreDeFeu" ] = "eReportArtillery";
	rcTypes_[ "eRC_PretAuTir" ] = "eReportArtillery";
	rcTypes_[ "eRC_PresencePopulation" ] = "eReportEnemy";
	rcTypes_[ "eRC_OrientationPopulationVersCampRefugies" ] = "eReportEnemy";
	rcTypes_[ "eRC_PasDeCampRefugiesDisponible" ] = "eReportEnemy";
	rcTypes_[ "eRC_PresenceEnnemiRendu" ] = "eReportEnemy";
	rcTypes_[ "eRC_OrientationEnnemiRenduVersCampPrisonniers" ] = "eReportEnemy";
	rcTypes_[ "eRC_PasDeCampPrisonniersDisponible" ] = "eReportEnemy";
	rcTypes_[ "eRC_TirDansZoneInterdite" ] = "eReportEnemy";
	rcTypes_[ "eRC_TirSurCampAmi" ] = "eReportEnemy";
	rcTypes_[ "eRC_TireParCampAmi" ] = "eReportEnemy";
	rcTypes_[ "eRC_TirSurCampNeutre" ] = "eReportEnemy";
	rcTypes_[ "eRC_TireParCampNeutre" ] = "eReportEnemy";
	rcTypes_[ "eRC_TirSurCivil" ] = "eReportEnemy";
	rcTypes_[ "eRC_TireParCivil" ] = "eReportEnemy";
	rcTypes_[ "eRC_DestructionPC" ] = "eReportEnemy";
	rcTypes_[ "eRC_DecesBlesse" ] = "eReportLogisticSituation";
	rcTypes_[ "eRC_DecesBlessePendantTransport" ] = "eReportLogisticSituation";
	rcTypes_[ "eRC_DecesBlessePendantHospitalisation" ] = "eReportLogisticSituation";
	rcTypes_[ "eRC_MissionImpossiblePionsSubordonnesNonOperationnels" ] = "eReportScipio";
	rcTypes_[ "eRC_MissionImpossibleLimaInvalide" ] = "eReportScipio";
	rcTypes_[ "eRC_MissionImpossibleZoneHorsFuseau" ] = "eReportScipio";
	rcTypes_[ "eRC_RapportDeForceDefavorable" ] = "eReportEnemy";
	rcTypes_[ "eRC_TerrainDifficile" ] = "eReportScipio";
	rcTypes_[ "eRC_PlusDeCarburant" ] = "eReportLogisticSituation";
	rcTypes_[ "eRC_DispositifDisperse" ] = "eReportScipio";
	rcTypes_[ "eRC_DebutNeutralisation" ] = "eReportEnemy"; // PionNeutralise
	rcTypes_[ "eRC_FinNeutralisation" ] = "eReportEnemy"; // PionNeutralise
	rcTypes_[ "eRC_DebutSilenceRadio" ] = "eReportEnemy"; // SilenceRadio
	rcTypes_[ "eRC_FinSilenceRadio" ] = "eReportEnemy"; // SilenceRadio
	rcTypes_[ "eRC_DebutSilenceRadar" ] = "eReportEnemy";
	rcTypes_[ "eRC_FinSilenceRadar" ] = "eReportEnemy";
	rcTypes_[ "eRC_TirRestreint" ] = "eReportEnemy";
	rcTypes_[ "eRC_TirInterdit" ] = "eReportEnemy";
	rcTypes_[ "eRC_TirLibre" ] = "eReportEnemy";
	rcTypes_[ "eRC_Fixe" ] = "eReportEnemy";
	rcTypes_[ "eRC_EnEsquive" ] = "eReportEnemy";
	rcTypes_[ "eRC_PrisSousTirArtillerie" ] = "eReportEnemy";
	rcTypes_[ "eRC_UniteDetectee" ] = "eReportEnemy";
	rcTypes_[ "eRC_UniteAmieReconnue" ] = "eReportEnemy";
	rcTypes_[ "eRC_UniteEnnemieReconnue" ] = "eReportEnemy";
	rcTypes_[ "eRC_UniteNeutreReconnue" ] = "eReportEnemy";
	rcTypes_[ "eRC_ObjetDetecte" ] = "eReportObstacle";
	rcTypes_[ "eRC_Trace" ] = "eReportScipio";
	rcTypes_[ "eRC_TraceID" ] = "eReportScipio";
	rcTypes_[ "eRC_ANouveauDisponibleApresReparation" ] = "eReportLogisticSituation";
	rcTypes_[ "eRC_MaterielRepareSurPlace" ] = "eReportLogisticSituation";
	rcTypes_[ "eRC_RavitaillementDotationsEffectue" ] = "eReportLogisticSituation";
	rcTypes_[ "eRC_RavitaillementStockEffectue" ] = "eReportLogisticSituation";
    rcTypes_[ "eRC_RavitaillementDotationsAnnule" ] = "eReportLogisticSituation";
    rcTypes_[ "eRC_RavitaillementStockAnnule" ] = "eReportLogisticSituation";
	rcTypes_[ "eRC_HumainRetourDeSante" ] = "eReportLogisticSituation";
	rcTypes_[ "eRC_NonOperationnel" ] = "eReportLogisticSituation";
	rcTypes_[ "eRC_Operationnel" ] = "eReportLogisticSituation";
	rcTypes_[ "eRC_RejointUnite" ] = "eReportMission";
	rcTypes_[ "eRC_EscorteUnite" ] = "eReportMission";
	rcTypes_[ "eRC_EnFreinage" ] = "eReportMission";
	rcTypes_[ "eRC_Assaut" ] = "eReportMission";
	rcTypes_[ "eRC_EnCouverture" ] = "eReportMission";
	rcTypes_[ "eRC_EnEclairage" ] = "eReportMission";
	rcTypes_[ "eRC_EnManoeuvreRavitaillement" ] = "eReportMission";
	rcTypes_[ "eRC_FinControleZone" ] = "eReportMission";
	rcTypes_[ "eRC_FinSurveillance" ] = "eReportMission";
	rcTypes_[ "eRC_DroneDisponible" ] = "eReportMission";
	rcTypes_[ "eRC_ActivationBrouillage" ] = "eReportMission";
	rcTypes_[ "eRC_DesactivationBrouillage" ] = "eReportMission";
	rcTypes_[ "eRC_ActivationSystemeEcoute" ] = "eReportMission";
	rcTypes_[ "eRC_DesactivationSystemeEcoute" ] = "eReportMission";
	rcTypes_[ "eRC_ActivationSystemeEcouteRadar" ] = "eReportMission";
	rcTypes_[ "eRC_DesactivationSystemeEcouteRadar" ] = "eReportMission";
	rcTypes_[ "eRC_DebutLiaison" ] = "eReportMission";
	rcTypes_[ "eRC_FinLiaison" ] = "eReportMission";
	rcTypes_[ "eRC_EtablissementLiaisonImpossible" ] = "eReportMission";
	rcTypes_[ "eRC_UniteTropDistante" ] = "eReportMission";
	rcTypes_[ "eRC_EnregistrementDonnes" ] = "eReportMission";
	rcTypes_[ "eRC_ExploitationDonnees" ] = "eReportMission";
	rcTypes_[ "eRC_ExtractionDonnes" ] = "eReportMission";
	rcTypes_[ "eRC_DebutExploitationSiteFranchissement" ] = "eReportMission";
	rcTypes_[ "eRC_FinExploitationSiteFranchissement" ] = "eReportMission";
	rcTypes_[ "eRC_DebutAmenagementBerges" ] = "eReportMission";
	rcTypes_[ "eRC_FinAmenagementBerges" ] = "eReportMission";
	rcTypes_[ "eRC_TransportUnitePrete" ] = "eReportMission";
	rcTypes_[ "eRC_SiteRotaReconnu" ] = "eReportMission";
	rcTypes_[ "eRC_UniteDecontaminee" ] = "eReportMission";
	rcTypes_[ "eRC_SiteSature" ] = "eReportMission";
	rcTypes_[ "eRC_DebutMesure" ] = "eReportMission";
	rcTypes_[ "eRC_FinMesure" ] = "eReportMission";
	rcTypes_[ "eRC_FinInstallation" ] = "eReportMission";
    rcTypes_[ "eRC_MissionImpossibleUnitesSubordonneesNonOperationnelles" ] = "eReportScipio";
    rcTypes_[ "eRC_PretMaterielEnCours" ] = "eReportLogisticSituation";
    rcTypes_[ "eRC_PretMaterielAnnule" ] = "eReportLogisticSituation";
    rcTypes_[ "eRC_PretMaterielEffectue" ] = "eReportLogisticSituation";
    rcTypes_[ "eRC_PretMaterielPartiellementEffectue" ] = "eReportLogisticSituation";
    rcTypes_[ "eRC_PretMaterielImpossible" ] = "eReportLogisticSituation";
    rcTypes_[ "eRC_RecuperationMaterielPreteEffectuee" ] = "eReportLogisticSituation";
    rcTypes_[ "eRC_RecuperationMaterielPretePartiellementEffectuee" ] = "eReportLogisticSituation";
    rcTypes_[ "eRC_RecuperationMaterielPreteImpossible" ] = "eReportLogisticSituation";
    rcTypes_[ "eRC_MaterielPrete" ] = "eReportLogisticSituation";
    rcTypes_[ "eRC_MaterielRendu" ] = "eReportLogisticSituation";
	rcTypes_[ "eRC_VSRAM_PretEnCours" ] = "eReportLogisticSituation";
	rcTypes_[ "eRC_VSRAM_PretAnnule" ] = "eReportLogisticSituation";    
	rcTypes_[ "eRC_VSRAM_PretEffectue" ] = "eReportLogisticSituation";
	rcTypes_[ "eRC_VSRAM_RecuperationEffectuee" ] = "eReportLogisticSituation";
	rcTypes_[ "eRC_Convoi_DeplacementVersPointChargement" ] = "eReportLogisticSituation";
	rcTypes_[ "eRC_Convoi_ChargementEnCours" ] = "eReportLogisticSituation";
	rcTypes_[ "eRC_Convoi_ChargementEffectue" ] = "eReportLogisticSituation";
	rcTypes_[ "eRC_Convoi_DeplacementVersPointDechargement" ] = "eReportLogisticSituation";
	rcTypes_[ "eRC_Convoi_DechargementEnCours" ] = "eReportLogisticSituation";
	rcTypes_[ "eRC_Convoi_DechargementEffectue" ] = "eReportLogisticSituation";
	rcTypes_[ "eRC_Convoi_Retour" ] = "eReportLogisticSituation";
	rcTypes_[ "eRC_TirImpossiblePlusDeMunitions" ] = "eReportArtillery";
	rcTypes_[ "eRC_Rendu" ] = "eReportMission";
	rcTypes_[ "eRC_UniteAmieIdentifiee" ] = "eReportEnemy";
	rcTypes_[ "eRC_UniteEnnemieIdentifiee" ] = "eReportEnemy";
	rcTypes_[ "eRC_UniteNeutreIdentifiee" ] = "eReportEnemy";

	rcTypes_[ "eRC_EmploiForceInterdit" ] = "eReportMission";
	rcTypes_[ "eRC_MaintienADistanceParMoyensNonLetaux" ] = "eReportMission";
	rcTypes_[ "eRC_DispersionParMoyensDeDefenseActifs" ] = "eReportMission";
	rcTypes_[ "eRC_ArmesLetalesAutorisees" ] = "eReportMission";
	rcTypes_[ "eRC_PopulationDetectee" ] = "eReportMission";
	rcTypes_[ "eRC_PriseAPartieParPopulation" ] = "eReportMission";
	rcTypes_[ "eRC_RiposteSurPopulation" ] = "eReportMission";
	rcTypes_[ "eRC_PopulationDangereuseAProximite" ] = "eReportMission";
	rcTypes_[ "eRC_EsquiveDeLaPopulation" ] = "eReportMission";
	rcTypes_[ "eRC_DecrochageFaceAPopulation" ] = "eReportMission";
	rcTypes_[ "eRC_MiseEnPlaceControleDeFoule" ] = "eReportMission";
	rcTypes_[ "eRC_MiseEnPlaceFiltrage" ] = "eReportMission";
	rcTypes_[ "eRC_AnimationBarrage" ] = "eReportMission";
	rcTypes_[ "eRC_AnimationCanalisation" ] = "eReportMission";
	rcTypes_[ "eRC_CollecteRenseignementsSurPopulations" ] = "eReportMission";
	rcTypes_[ "eRC_PopulationTotalementReconnue" ] = "eReportMission";
	rcTypes_[ "eRC_AttitudeCalme" ] = "eReportMission";
	rcTypes_[ "eRC_AttitudeAgitee" ] = "eReportMission";
	rcTypes_[ "eRC_AttitudeExcitee" ] = "eReportMission";
	rcTypes_[ "eRC_AttitudeAgressive" ] = "eReportMission";
	rcTypes_[ "eRC_AggravationAttitude" ] = "eReportMission";
	rcTypes_[ "eRC_AffrontementAvecForcesEnnemies" ] = "eReportMission";
	rcTypes_[ "eRC_PopulationTemporairementControlee" ] = "eReportMission";
	rcTypes_[ "eRC_PopulationTemporairementRepoussee" ] = "eReportMission";
	rcTypes_[ "eRC_RepriseDesAffrontements" ] = "eReportMission";
	rcTypes_[ "eRC_RecuperationProgressive" ] = "eReportMission";
	rcTypes_[ "eRC_AgressionDePopulation" ] = "eReportMission";
	rcTypes_[ "eRC_AgressionParPopulation" ] = "eReportMission";
	rcTypes_[ "eRC_RiposteContreAgressionParPopulation" ] = "eReportMission";
	rcTypes_[ "eRC_AttaqueInstallation" ] = "eReportMission";
	rcTypes_[ "eRC_EnStationnement" ] = "eReportMission";
	rcTypes_[ "eRC_ExecutionAttentat" ] = "eReportMission";
    rcTypes_[ "eRC_RiposteContrePopulation" ] = "eReportMission";
    rcTypes_[ "eRC_EsquiveFaceAPopulation" ] = "eReportMission";
    rcTypes_[ "eRC_PopulationReconnue" ] = "eReportMission";
}
    
// -----------------------------------------------------------------------------
// Name: AGR_TypeRC::Terminate
// Created: SBO 2005-09-14
// -----------------------------------------------------------------------------
void AGR_TypeRC::Terminate()
{
    rcTypes_.clear();
}

// -----------------------------------------------------------------------------
// Name: AGR_TypeRC::GenerateTIC
// Created: SBO 2005-09-14
// -----------------------------------------------------------------------------
std::string AGR_TypeRC::GenerateTIC( const AGR_RC& rcInstance ) const
{
    std::stringstream strTmp;

    strTmp << "messages_[ \"" << strName_ << "\" ] = new MessageReport"
           << "( \"" << strName_ << "\", "
           << strMILEnumName_
           << ", T_MsgCR_cr_" << rcInstance.GetAsnName()
           << ", CommunicationType::"
           << rcTypes_[ strMILEnumName_ ]
           << " );";

    strTmp << std::endl;
    return strTmp.str();
}
