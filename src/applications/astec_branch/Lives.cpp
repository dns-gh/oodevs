// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "astec_pch.h"
#include "Lives.h"
#include "GlTools_ABC.h"
#include "Agent.h"

// -----------------------------------------------------------------------------
// Name: Lives constructor
// Created: AGE 2006-04-10
// -----------------------------------------------------------------------------
Lives::Lives( const Agent& agent )
    : agent_( agent ) 
    , life_( 1.f )
    , bEmbraye_( false )
    , aggregated_( false )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Lives destructor
// Created: AGE 2006-04-10
// -----------------------------------------------------------------------------
Lives::~Lives()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Lives::GetLife
// Created: AGE 2006-04-11
// -----------------------------------------------------------------------------
float Lives::GetLife() const
{
    if( ! aggregated_ )
        return life_;
    float result = life_;
    unsigned count = 1;
    Iterator< const Agent& > children = agent_.CreateIterator();
    while( children.HasMoreElements() )
    {
        result += children.NextElement().Get< Lives >().GetLife();
        ++count;
    }
    return result / count;
}

// -----------------------------------------------------------------------------
// Name: Lives::Draw
// Created: AGE 2006-04-10
// -----------------------------------------------------------------------------
void Lives::Draw( const geometry::Point2f& where, const GlTools_ABC& tools ) const
{
    glPushAttrib( GL_CURRENT_BIT );
    if( agent_.GetAutomatDecisionalModel() && bEmbraye_ )
        glColor4f( 1, 1, 0, 0.4f );
    else
        glColor4f( 1, 1, 1, 0.4f );
    tools.DrawRectangle( where, GetLife(), aggregated_ ? 2.f : 1.f );
    glPopAttrib();
}

// -----------------------------------------------------------------------------
// Name: Lives::DoUpdate
// Created: AGE 2006-04-10
// -----------------------------------------------------------------------------
void Lives::DoUpdate( const ASN1T_MsgUnitAttributes& message )
{
    if( message.m.etat_operationnel_brutPresent )
        life_ = message.etat_operationnel_brut * 0.01f;
    if( message.m.etat_automatePresent )
        bEmbraye_ = ( message.etat_automate == EnumAutomateState::embraye );
}

// -----------------------------------------------------------------------------
// Name: Lives::Aggregate
// Created: AGE 2006-04-11
// -----------------------------------------------------------------------------
void Lives::Aggregate( const bool& bms )
{
    aggregated_ = bms;
}
