// *****************************************************************************
//
// $Created: SBO 2005-05-13 $
// $Archive: /MVW_v10/Build/SDK/TIC/src/Communications/MessageReport.h $
// $Author: Sbo $
// $Modtime: 20/07/05 18:10 $
// $Revision: 1 $
// $Workfile: MessageReport.h $
//
// *****************************************************************************

#ifndef __MessageReport_h_
#define __MessageReport_h_

#include "Types.h"
#include "CommunicationType.h"

namespace TIC
{

// =============================================================================
// @class  MessageReport
// Created: SBO 2005-06-30
// =============================================================================
class MessageReport
{
    MT_COPYNOTALLOWED( MessageReport )

public:
    //! @name constant
    //@{
    static const MessageReport notDefined;
    //@}

public:
    //! @name Manager
    //@{
    static void Initialize();
    static void Terminate ();

    static const MessageReport& Find( const std::string& strName );
    static const MessageReport& Find( uint               asnId   );
    //@}

    //! @name Accessors
    //@{
    const std::string&                GetName   () const;
          uint                        GetID     () const;
          uint                        GetAsnID  () const;
    const CommunicationType::ESubType GetSubType() const;
    //@}

private:
    //! @name Types
    //@{
    enum E_Type
    {
        eCr_ras                                             ,
        eCr_en_poste_face_a_obstacle                        ,
        eCr_en_contournement_obstacle                       ,
        eCr_mission_impossible                              ,
        eCr_pret_pour_nouvelle_mission                      ,
        eCr_rejoint_fuseau                                  ,
        eCr_fuseau_rejoint                                  ,
        eCr_progression_sur_axe                             ,
        eCr_progression_vers_localisation                   ,
        eCr_arrive_sur_localisation                         ,
        eCr_reconnaissance_localisation                     ,
        eCr_en_poste_face_a_direction_dangereuse            ,
        eCr_en_poste_sur_bond                               ,
        eCr_en_jalonnement                                  ,
        eCr_preparation_mission                             ,
        eCr_debut_mission                                   ,
        eCr_fin_mission                                     ,
        eCr_dispositif_installe                             ,
        eCr_en_position                                     ,
        eCr_objectif_atteint                                ,
        eCr_debouche                                        ,
        eCr_sur_pia                                         ,
        eCr_passage_sur_pia                                 ,
        eCr_unite_recueillie                                ,
        eCr_en_cours_de_franchissement                      ,
        eCr_attente_ordre_poursuivre                        ,
        eCr_attente_ordre_deboucher                         ,
        eCr_en_controle_zone                                ,
        eCr_en_surveillance                                 ,
        eCr_en_infiltration                                 ,
        eCr_en_exfiltration                                 ,
        eCr_attente_condition_favorable_pour_infiltration   ,
        eCr_drone_lance                                     ,
        eCr_drone_en_exploitation                           ,
        eCr_drone_exploite                                  ,
        eCr_debut_mise_en_oeuvre_drone                      ,
        eCr_fin_mise_en_oeuvre_drone                        ,
        eCr_brouille                                        ,
        eCr_en_ecoute                                       ,
        eCr_en_ecoute_radar                                 ,
        eCr_automate_prise_contact                          ,
        eCr_automate_rupture_contact                        ,
        eCr_pion_prise_contact                              ,
        eCr_pion_rupture_contact                            ,
        eCr_precision_contact                               ,
        eCr_ennemi_detruit                                  ,
        eCr_perte_de_contact_visuel                         ,
        eCr_poste_face_eni                                  ,
        eCr_prise_a_partie                                  ,
        eCr_point_tenu_par_eni                              ,
        eCr_progression_vers_eni                            ,
        eCr_presence_eni_probable                           ,
        eCr_decroche                                        ,
        eCr_eni_sur_objectif                                ,
        eCr_riposte                                         ,
        eCr_releve                                          ,
        eCr_suit_unite                                      ,
        eCr_passe_en_appui                                  ,
        eCr_en_appui                                        ,
        eCr_detruit_tactiquement                            ,
        eCr_ambiance_vitesse                                ,
        eCr_ambiance_surete                                 ,
        eCr_sur_lima                                        ,
        eCr_sur_type_terrain_particulier                    ,
        eCr_obstacle_en_attente_activation                  ,
        eCr_travaux_realises                                ,
        eCr_obstacle_degage                                 ,
        eCr_organisation_chantier                           ,
        eCr_desorganisation_chantier                        ,
        eCr_reconnaissance_position_obstacle                ,
        eCr_contournement_realise                           ,
        eCr_attente_renforcement                            ,
        eCr_debut_travaux                                   ,
        eCr_debut_franchissement                            ,
        eCr_fin_franchissement                              ,
        eCr_reussite_renforcement                           ,
        eCr_echec_renforcement                              ,
        eCr_itineraire_retabli                              ,
        eCr_renforcement_bientot_fini                       ,
        eCr_situation_log_favorable                         ,
        eCr_situation_log_defavorable                       ,
        eCr_section_deployee                                ,
        eCr_materiel_retour_de_maintenance                  ,
        eCr_demande_ravitaillement                          ,
        eCr_demande_evacuation_sanitaire                    ,
        eCr_demande_evacuation_materiel                     ,
        eCr_allocation_consentie_bientot_epuisee            ,
        eCr_en_cours_ravitaillement                         ,
        eCr_ravitaillement_termine                          ,
        eCr_helicoptere_en_observation                      ,
        eCr_perte_info_guidage                              ,
        eCr_transport_unite_pas_prete                       ,
        eCr_transport_en_cours                              ,
        eCr_transport_embarquement                          ,
        eCr_transport_debarquement                          ,
        eCr_vehicule_laisse_sur_position                    ,
        eCr_rejoint_position_embarquement                   ,
        eCr_en_attente_transporteurs                        ,
        eCr_contamine_nbc                                   ,
        eCr_decontamine_nbc                                 ,
        eCr_tenue_protection_nbc_mise                       ,
        eCr_tenue_protection_nbc_enlevee                    ,
        eCr_alerte_nbc                                      ,
        eCr_site_decontamination_anime                      ,
        eCr_site_decontamination_desactive                  ,
        eCr_demande_decontamination                         ,
        eCr_zone_reconnue                                   ,
        eCr_zone_decontaminee                               ,
        eCr_site_rota_reconnu                               ,
        eCr_incident_nbc_termine                            ,
        eCr_pion_decontamine                                ,
        eCr_reconnaissance_terminee                         ,
        eCr_reconnaissance_initiale_terminee                ,
        eCr_rejoint_plot_ravitaillement                     ,
        eCr_premier_coup_parti                              ,
        eCr_tir_execute                                     ,
        eCr_tir_hors_de_portee                              ,
        eCr_tir_annule                                      ,
        eCr_changement_de_position                          ,
        eCr_debut_surveillance                              ,
        eCr_debut_installation                              ,
        eCr_pret_pour_conduite_tir                          ,
        eCr_observation_tir_indirect                        ,
        eCr_zone_implantation_occupee                       ,
        eCr_installation_sur_zone_implantation              ,
        eCr_reception_ordre_de_feu                          ,
        eCr_pret_au_tir                                     ,
        eCr_presence_population                             ,
        eCr_orientation_population_vers_camp_refugies       ,
        eCr_pas_de_camp_refugies_disponible                 ,
        eCr_presence_ennemi_rendu                           ,
        eCr_orientation_ennemi_rendu_vers_camp_prisonniers  ,
        eCr_pas_de_camp_prisonniers_disponible              ,
        eCr_tir_dans_zone_interdite                         ,
        eCr_tir_sur_camp_ami                                ,
        eCr_tire_par_camp_ami                               ,
        eCr_tir_sur_camp_neutre                             ,
        eCr_tire_par_camp_neutre                            ,
        eCr_tir_sur_civil                                   ,
        eCr_tire_par_civil                                  ,
        eCr_destruction_pc                                  ,
        eCr_deces_blesse                                    ,
        eCr_deces_blesse_pendant_transport                  ,
        eCr_deces_blesse_pendant_hospitalisation            ,
        eCr_mission_impossible_pions_subordonnes_non_operationnels,
        eCr_mission_impossible_lima_invalide                ,
        eCr_mission_impossible_zone_hors_fuseau             ,
        eCr_rapport_de_force_defavorable                    ,
        eCr_terrain_difficile                               ,
        eCr_plus_de_carburant                               ,
        eCr_dispositif_disperse                             ,
        eCr_pion_neutralise                                 ,
        eCr_silence_radio                                   ,
        eCr_tir_restreint                                   ,
        eCr_tir_interdit                                    ,
        eCr_tir_libre                                       ,
        eCr_trace                                           ,
        eCr_trace_id                                        ,

        eNone
    };

    typedef std::map< std::string, const MessageReport*, sCaseInsensitiveLess > T_MessageReportMap;
    typedef T_MessageReportMap::const_iterator                                  CIT_MessageReportMap;
    //@}

private:
    //! @name Constructors/Destructor
    //@{
     MessageReport( const std::string& strName, E_Type nType, uint nAsnID, CommunicationType::ESubType eSubType );
    ~MessageReport();
    //@}

private:
    //! @name Member data
    //@{
    const std::string                 strName_;
    const E_Type                      nType_;
    const uint                        nAsnID_;
    const CommunicationType::ESubType eSubType_;
    //@}

    //! @name Static member
    //@{
    static T_MessageReportMap messages_;
    //@}
};

} // end namespace TIC

#include "MessageReport.inl"

#endif // __MessageReport_h_
