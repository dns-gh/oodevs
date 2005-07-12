// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Units/Categories/PHY_MissionCapacity.h $
// $Author: Jvt $
// $Modtime: 9/12/04 15:09 $
// $Revision: 1 $
// $Workfile: PHY_MissionCapacity.h $
//
// *****************************************************************************

#ifndef __PHY_MissionCapacity_h_
#define __PHY_MissionCapacity_h_

#include "MIL.h"

#include "Network/NET_ASN_Types.h"

// =============================================================================
// @class  PHY_MissionCapacity
// Created: JVT 2004-08-03
// =============================================================================
class PHY_MissionCapacity
{
    MT_COPYNOTALLOWED( PHY_MissionCapacity )

public:
    //! @name Manager
    //@{
    static void Initialize();
    static void Terminate ();

    static const PHY_MissionCapacity* Find( const std::string& strName );
    //@}

    //! @name Accessors
    //@{
    const std::string&                  GetName () const;
          uint                          GetID   () const;
          ASN1T_EnumUnitCapaciteMission GetAsnID() const;
    //@}

private:
    //! @name Types
    //@{
    enum E_Type
    {
        eCivilianLawEnforcement,
        eCommandOperations,
        eCouvertureAerienne,
        eJointIntelligence,
        eMoraleWelfareRecreation,
        eReglageElectronique,
        eRemplacement,
        eServiceMortuaire,
        eSignalsIntelligence,
        eTheatreMissileDefence,
        eUtilitaire,
        eAéroporte,
        eAffairesPubliques,
        eAffairesPubliques_Communication,
        eAffairesPubliques_JointInformation,
        eAlpin,
        eAmphibie,
        eArctique,
        eArtillerieReconnaissance,
        eAttaque,
        eAttaqueAerienne,
        eC2,
        eCheminDeFer,
        eCiblage,
        eContreEspionnage,
        eEclairage,
        eEntretien,
        eEntretienMateriel,
        eEntretienMissile,
        eEntretienOptoElectronique,
        eEspionnage,
        eEspionnageMilitaireAerialExploitation,
        eEspionnageMilitaireOperation,
        eEspionnageMilitaireTacticalExploitation,
        eEvacuationSanitaire,
        eFinance,
        eGenieCombat,
        eGenieConstruction,
        eGenieConstructionNavale,
        eGuerreElectronique,
        eGuerreElectroniqueInterception,
        eGuerreElectroniqueBrouillage,
        eGuerreElectroniqueDétection,
        eInterrogation,
        eMainDOeuvre,
        eMaintienDeLaPaix,
        eMarine,
        eMineDeContreMesure,
        eNBCBiologique,
        eNBCChimique,
        eNBCChimiqueFumee,
        eNBCDecontamination,
        eNBCDecontaminationChimique,
        eNBCNucleaire,
        eNonRenseigne,
        eObservation,
        eObservationCapteur,
        eObservationLonguePortee,
        eObservationMeteo,
        eObservationModuleTerrestre,
        ePurificationDeLEau,
        eRavitaillement_ClasseI,
        eRavitaillement_ClasseII,
        eRavitaillement_ClasseIIIAviation,
        eRavitaillement_ClasseIII,
        eRavitaillement_ClasseIV,
        eRavitaillement_ClasseV,
        eRavitaillement_Eau,
        eRavitaillementBlanchisserieBain,
        eRechercheEtSauvetage,
        eReconnaissance,
        eReconnaissanceAerienneTactique,
        eRecuperation,
        eServiceDuPersonnel,
        eServiceJuridique,
        eSupportAerienRapproche,
        eTransmissions_ForwardCommunications,
        eTransmissions_NodeCentre,
        eTransmissions_NodeLargeExtension,
        eTransmissions_NodeSmallExtension,
        eTransmissions_RadioRelay,
        eTransmissions_RadioTacticalSatellite,
        eTransmissions_RadioTeletype,
        eTransmissions_Support,
        eTransmissions_Radio,
        eTransport_AllocationDesMouvements,
        eTransport_APOD_APOE,
        eTransport_SPOD_SPOE,
        eTransportMissile,
        eVerrouillageDeCible,
        eVerrouillageDeCible_Flash,
        eVerrouillageDeCible_Radar,
        eVerrouillageDeCible_Sonore,
    };

    typedef std::map< std::string, const PHY_MissionCapacity*, sCaseInsensitiveLess > T_MissionCapacityMap;
    typedef T_MissionCapacityMap::const_iterator                                      CIT_MissionCapacityMap;
    //@}

private:
     PHY_MissionCapacity( const std::string& strName, E_Type nType, ASN1T_EnumUnitCapaciteMission nAsnID );
    ~PHY_MissionCapacity();

private:
    const std::string                   strName_;
    const E_Type                        nType_;
    const ASN1T_EnumUnitCapaciteMission nAsnID_;

private:
    static T_MissionCapacityMap missionCapacity_;
};

#include "PHY_MissionCapacity.inl"

#endif // __PHY_MissionCapacity_h_
