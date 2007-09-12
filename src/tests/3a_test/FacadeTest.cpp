// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "3a_test_pch.h"
#include "3a/DispatchedFunctionHelper.h"
#include "3a/Attributes.h"
#include "3a/IdentifierValue.h"
#include "3a/FunctionFactory.h"
#include "3a/Task.h"
#include "dispatcher/ClientPublisher_ABC.h"
#include "xeumeuleu/xml.h"
#include "game_asn/asn.h"

using namespace mockpp;

namespace
{
    ASN1T_MsgsSimToClient MakeMessage( unsigned opstate, unsigned long id )
    {
        static ASN1T_MsgUnitAttributes attributes;
        attributes.m.etat_operationnel_brutPresent = 1;
        attributes.etat_operationnel_brut = opstate;
        attributes.oid = id;
        ASN1T_MsgsSimToClient result;
        result.msg.t = T_MsgsSimToClient_msg_msg_unit_attributes;
        result.msg.u.msg_unit_attributes = &attributes;
        return result;
    }
    ASN1T_MsgsSimToClient BeginTick()
    {
        ASN1T_MsgsSimToClient result;
        result.msg.t = T_MsgsSimToClient_msg_msg_control_begin_tick;
        return result;
    }
    ASN1T_MsgsSimToClient EndTick()
    {
        ASN1T_MsgsSimToClient result;
        result.msg.t = T_MsgsSimToClient_msg_msg_control_end_tick;
        return result;
    }

    class MockPublisher : public dispatcher::ClientPublisher_ABC, public mockpp::ChainableMockObject
    {
    public:
        MockPublisher() 
            : mockpp::ChainableMockObject( "MockPublisher", 0 )
            , Send_mocker( "Send", this ) {}

        virtual void Send( const ASN1T_MsgsSimToClient& ) { }
        virtual void Send( const ASN1T_MsgsAuthenticationToClient& ) { }
        virtual void Send( const ASN1T_MsgsReplayToClient& ) { }
        virtual void Send( const ASN1T_MsgsAfterActionReviewToClient& msg )
        {
            const ASN1T_MsgIndicatorResult& result = *msg.msg.u.msg_indicator_result;
            std::vector< double > v;
            for( unsigned i = 0; i < result.values.n; ++i )
                v.push_back( result.values.elem[i] );
            Send_mocker.forward( v );
        };

        mockpp::ChainableMockMethod< void, std::vector< double > > Send_mocker;
    };

    template< std::size_t N >
    std::vector< double > MakeResult( double (&data)[N] )
    {
        std::vector< double > result;
        std::copy( data, data + N, std::back_inserter( result ) );
        return result;
    }
}
// -----------------------------------------------------------------------------
// Name: Facade_TestConcept
// Created: AGE 2004-12-15
// -----------------------------------------------------------------------------
BOOST_AUTO_TEST_CASE( Facade_TestOperationalState )
{
    const std::string input =
    "<indicator>"
        "<extract value='operational-state' name='opstate'/>"
        "<reduce type='float' function='select' input='opstate' id='2' name='myopstate'/>"
        "<plot input='myopstate' type='float'/>"
    "</indicator>";
    xml::xistringstream xis( input );

    MockPublisher publisher;
    FunctionFactory facade( publisher );
    std::auto_ptr< Task > task( facade.CreateTask( xis ) );

    task->Receive( BeginTick() );
    task->Receive( MakeMessage( 50, 1 ) );
    task->Receive( MakeMessage( 25, 2 ) );
    task->Receive( MakeMessage( 75, 3 ) );
    task->Receive( EndTick() );

    task->Receive( BeginTick() );
    task->Receive( MakeMessage( 75, 1 ) );
    task->Receive( MakeMessage( 85, 3 ) );
    task->Receive( EndTick() );

    task->Receive( BeginTick() );
    task->Receive( MakeMessage( 75, 2 ) );
    task->Receive( EndTick() );

    task->Receive( BeginTick() );
    task->Receive( EndTick() );

    double expectedResult[] = { 0.25, 0.25, 0.75, 0.75 };
    publisher.Send_mocker.expects( once() ).with( eq( MakeResult( expectedResult ) ) );
    task->Commit();
    publisher.verify();
}

//CREATE PROCEDURE dbo.[AAAT_GENERAL_DISTANCE_ENTRE_DEUX_UNITES_A_LA_DATE_T]
//(
//  @TDebut DATETIME,       -- Date du calcul
//  @Unite1  unit_id,       -- Premiere d'unites
//  @Unite2  unit_id,       -- Seconde d'unites
//  @Resultat tablename OUTPUT  -- Distance entre les deux unites
//)

//CREATE PROCEDURE DBO.[AAAT_LOGISTIQUE_MATERIELS_AU_NTI1_POUR_UNE_OU_PLUSIEURS_UNITES_ENTRE_T1_ET_T2_(Pourcentages)]
//(
//  @TDebut DATETIME,               -- Date
//  @TFin DATETIME,                 -- Date
//  @Unites unit_id_tablename,          -- Unites concernees
//  @Materiels equipment_id_tablename,      -- Materiels a prendre en compte
//  @Resultat tablename OUT             -- Tableau de resultat
//)

//CREATE PROCEDURE DBO.[AAAT_LOGISTIQUE_MATERIELS_AU_NTI2_POUR_UNE_OU_PLUSIEURS_UNITES_ENTRE_T1_ET_T2_(Pourcentages)]
//(
//  @TDebut DATETIME,               -- Date
//  @TFin DATETIME,                 -- Date
//  @Unites unit_id_tablename,          -- Unites concernees
//  @Materiels equipment_id_tablename,      -- Materiels a prendre en compte
//  @Resultat tablename OUT             -- Tableau de resultat
//)

//CREATE PROCEDURE DBO.[AAAT_LOGISTIQUE_MATERIELS_AU_NTI3_POUR_UNE_OU_PLUSIEURS_UNITES_ENTRE_T1_ET_T2_(Quantites)]
//(
//  @TDebut DATETIME,           -- Date
//  @TFin DATETIME,             -- Date
//  @Unites unit_id_tablename,      -- Unites concernees
//  @Materiels equipment_id_tablename,  -- Materiels a prendre en compte
//  @Resultat tablename OUT         -- Tableau de resultat
//)

//CREATE PROCEDURE DBO.[AAAT_LOGISTIQUE_MATERIELS_DISPONIBLES_POUR_UNE_OU_PLUSIEURS_UNITES_DANS_ZONE_ENTRE_T1_ET_T2(Pourcentages)]
//(
//  @TDebut DATETIME,                   -- Date
//  @Unites unit_id_tablename,              -- Unites concernees
//  @Materiels equipment_id_tablename,          -- Materiels a prendre en compte
//  @Zone geometry,                 -- Zone
//  @Resultat tablename OUT             -- Tableau de resultat
//)

// $$$$ AGE 2007-09-10: decedé au combat
//CREATE PROCEDURE dbo.[AAAT_LOGISTIQUE_PERSONNEL_DECEDE_AU_COMBAT_POUR_UNE_OU_PLUSIEURS_UNITES_ENTRE_T1_ET_T2_(Pourcentages)]
//(
//  @TDebut DATETIME,               -- Date
//  @TFin DATETIME,                 -- Date
//  @Unites unit_id_tablename,          -- Unites concernees
//  @Resultat tablename OUT             -- Tableau de resultat
//)

// $$$$ AGE 2007-09-10: decedé faute de soins
//CREATE PROCEDURE dbo.[AAAT_LOGISTIQUE_PERSONNEL_DECEDE_FAUTE_DE_SOIN_POUR_UNE_OU_PLUSIEURS_UNITES_ENTRE_T1_ET_T2_(Quantite)]
//(
//  @TDebut DATETIME,                   -- Date
//  @TFin DATETIME,                     -- Date
//  @Unites unit_id_tablename,              -- Unites concernees
//  @Resultat tablename OUT                 -- Tableau de resultat
//)

//CREATE PROCEDURE DBO.[AAAT_LOGISTIQUE_PERSONNEL_DISPONIBLE_POUR_UNE_OU_PLUSIEURS_UNITES_DANS_ZONE_ENTRE_T1_ET_T2(Pourcentage)]
//(
//  @TDebut DATETIME,               -- Date
//  @Unites unit_id_tablename,          -- Unites concernees
//  @Zone geometry,                 -- Zone
//  @Resultat tablename OUT             -- Tableau de resultat
//)

//CREATE PROCEDURE dbo.[AAAT_LOGISTIQUE_PERSONNEL_TOTAL_BLESSES_POUR_UNE_OU_PLUSIEURS_POPULATIONS_ENTRE_T1_ET_T2_(Quantite)]
//(
//  @TDebut DATETIME,                   -- Date
//  @Populations population_id_tablename,               -- Populations concernees
//  @Resultat tablename OUT                 -- Tableau de resultat
//)

//CREATE PROCEDURE dbo.[AAAT_LOGISTIQUE_PERTES_MATERIELS_POUR_UNE_OU_PLUSIEURS_UNITES_ENTRE_T1_ET_T2_(Pourcentages)]
//(
//  @TDebut DATETIME,                   -- Date
//  @Unites unit_id_tablename,              -- Unites concernees
//  @Materiels equipment_id_tablename,          -- Materiels a prendre en compte
//  @Resultat tablename OUT             -- Tableau de resultat
//)

// $$$$ AGE 2007-09-10: ressources consommées => variation <0 
//CREATE PROCEDURE dbo.[AAAT_LOGISTIQUE_RESSOURCES_CONSOMMEES_POUR_UNE_OU_PLUSIEURS_UNITES_ENTRE_T1_ET_T2_(Quantites)]
//(
//  @TDebut DATETIME,           -- Date Debut
//  @TFin DATETIME,             -- Date Fin
//  @Unites unit_id_tablename,      -- Unites concernees
//  @Ressources ressource_id_tablename, -- Ressource … comptabiliser
//  @Resultat tablename OUTPUT      -- Table de resultats
//)

//CREATE PROCEDURE dbo.[AAAT_LOGISTIQUE_RESSOURCES_DISPONIBLES_POUR_UNE_OU_PLUSIEURS_UNITES_ENTRE_T1_ET_T2_(Pourcentages)]
//(
//	@TDebut DATETIME,			-- Date Debut
//	@Unites unit_id_tablename,		-- Unites concernees
//	@Ressources ressource_id_tablename,	-- Ressource à comptabiliser
//	@Resultat tablename OUTPUT		-- Table de resultats
//)

// $$$$ AGE 2007-09-10: UE-UF-JV => consommation normalisée
//CREATE PROCEDURE dbo.[AAAT_LOGISTIQUE_STOCKS_DISPONIBLES_POUR_UNE_OU_PLUSIEURS_UNITES_ENTRE_T1_ET_T2_(UE-UF-JV)]
//(
//	@TDebut DATETIME,			-- Date Debut
//	@Unites unit_id_tablename,		-- Unites concernees
//	@Ressources ressource_id_tablename,	-- Ressource à comptabiliser
//	@Resultat tablename OUTPUT		-- Table de resultats
//)

//CREATE PROCEDURE dbo.[AAAT_MELEE-APPUI_NOMBRE_DE_COUPS_DIRECTS_TIRES_PAR_UNE_OU_PLUSIEURS_UNITES_ENTRE_T1_ET_T2]
//(
//	@TDebut DATETIME,			-- Date de debut de l'intervalle 
//	@TFin DATETIME,			-- Date de fin de l'intervalle
//	@Unites  unit_id_tablename,		-- Groupe d'unites
//	@Resultat tablename OUTPUT		-- Table contenant le résultat
//)

//CREATE PROCEDURE DBO.[AAAT_MELEE-APPUI_PERTES_EN_MATERIEL_INFLIGEES_PAR_UNE_UNITE_PAR_TIR_DIRECT_ENTRE_T1_ET_T2]
//(
//	@TDebut DATETIME,					-- Date debut
//	@TFin DATETIME,					-- Date Fin
//	@Unites unit_id_tablename,				-- Groupe d unites
//	@Resultat tablename OUT				-- Table de resultats
//)

// $$$$ AGE 2007-09-10: pourcentage efficacité => nombre de tirs avec degat / nombre de tirs
//CREATE PROCEDURE dbo.[AAAT_MELEE-APPUI_POURCENTAGE_EFFICACITE_DES_TIRS_INDIRECTS_D_UNE_OU_PLUSIEURS_UNITES_ENTRE_T1_ET_T2]
//(
//	@TDebut DATETIME,			-- DATE DEBUT 
//	@TFin DATETIME,			-- DATE FIN
//	@Unites  unit_id_tablename,		-- UNITES CONCERNEES
//	@Resultat tablename OUTPUT		-- TABLE DES RESULTATS
//)