// *****************************************************************************
//
// $Created: SBO 2005-05-13 $
// $Archive: /MVW_v10/Build/SDK/TIC/src/Entities/Natures/Pawn/NatureCapacity.h $
// $Author: Sbo $
// $Modtime: 20/06/05 9:51 $
// $Revision: 1 $
// $Workfile: NatureCapacity.h $
//
// *****************************************************************************

#ifndef __NatureCapacity_h_
#define __NatureCapacity_h_

namespace TIC
{

// =============================================================================
// @class  NatureCapacity
// Created: JVT 2004-08-03
// =============================================================================
class NatureCapacity
{
    MT_COPYNOTALLOWED( NatureCapacity )

public:
    //! @name Manager
    //@{
    static       void         Initialize();
    static       void         Terminate ();

    static const NatureCapacity* Find( const std::string&  strName );
    static const std::string*    Find( uint                asnId   );
    //@}

    //! @name Accessors
    //@{
    const std::string& GetName () const;
          uint         GetID   () const;
          uint         GetAsnID() const;
    const std::string& GetKey  () const;
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

    typedef std::map< std::string, const NatureCapacity*, sCaseInsensitiveLess > T_NatureCapacityMap;
    typedef T_NatureCapacityMap::const_iterator                                  CIT_NatureCapacityMap;
    //@}

private:
    //! @name Constructors/Destructor
    //@{
    NatureCapacity( const std::string& strName, E_Type nType, uint nAsnID, const std::string& strKey );
    ~NatureCapacity();
    //@}

private:
    //! @name Member data
    //@{
    const std::string           strName_;
    const E_Type                nType_;
    const uint                  nAsnID_;
    const std::string           strKey_;

    static T_NatureCapacityMap  natureCapacity_;
    //@}
};

} // end namespace TIC

#include "NatureCapacity.inl"

#endif // __NatureCapacity_h_
