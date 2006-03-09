// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "astec_pch.h"
#include "WaitingForFragOrder.h"

// -----------------------------------------------------------------------------
// Name: WaitingForFragOrder constructor
// Created: AGE 2006-03-09
// -----------------------------------------------------------------------------
WaitingForFragOrder::WaitingForFragOrder( const Agent_ABC& agent, const Simulation& simulation, const ASN1T_MsgAttenteOrdreConduite& asnMsg )
    : Report_ABC( agent, simulation )
    , agent_( agent )
{
    Initialize( asnMsg );
}

// -----------------------------------------------------------------------------
// Name: WaitingForFragOrder destructor
// Created: AGE 2006-03-09
// -----------------------------------------------------------------------------
WaitingForFragOrder::~WaitingForFragOrder()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: WaitingForFragOrder::Initialize
// Created: AGE 2006-03-09
// -----------------------------------------------------------------------------
void WaitingForFragOrder::Initialize( const ASN1T_MsgAttenteOrdreConduite& asnMsg )
{
    strTitle_ = "Attente d'ordre";

    for( uint i = 0; i < asnMsg.ordres_conduite.n; ++i )
    {
        uint nOrderConduiteID;
        switch( asnMsg.ordres_conduite.elem[i].t  )
        {
            case T_MsgAttenteOrdreConduite_ordres_conduite_element_demande_order_conduite_attendre_se_poster     : nOrderConduiteID = eOrdreConduite_AttendreSePoster                            ; break;
            case T_MsgAttenteOrdreConduite_ordres_conduite_element_demande_order_conduite_poursuivre             : nOrderConduiteID = eOrdreConduite_Poursuivre                                  ; break;
            case T_MsgAttenteOrdreConduite_ordres_conduite_element_demande_order_conduite_decrocher              : nOrderConduiteID = eOrdreConduite_Decrocher                              ; break;
            case T_MsgAttenteOrdreConduite_ordres_conduite_element_demande_order_conduite_deboucher              : nOrderConduiteID = eOrdreConduite_Deboucher                              ; break;
            case T_MsgAttenteOrdreConduite_ordres_conduite_element_demande_order_conduite_pion_executer_tir      : nOrderConduiteID = eOrdreConduite_Pion_ExecuterTir                            ; break;
            case T_MsgAttenteOrdreConduite_ordres_conduite_element_demande_order_conduite_pion_contourner        : nOrderConduiteID = eOrdreConduite_Pion_Contourner                             ; break;
            case T_MsgAttenteOrdreConduite_ordres_conduite_element_demande_order_conduite_pion_activer_obstacle  : nOrderConduiteID = eOrdreConduite_Pion_ActiverObstacle                        ; break;
            default:
                assert( false );
        }
        fragOrders_.push_back( nOrderConduiteID );
    }
    eType_ = Report_ABC::eRC;
}
