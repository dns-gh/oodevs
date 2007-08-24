// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "dispatcher_pch.h"
#include "ModelVisitors.h"
#include "dispatcher/Entity_ABC.h"
#include "dispatcher/ClientPublisher_ABC.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: ModelVisitors::SideModelVisitor
// Created: JCR 2007-05-15
// -----------------------------------------------------------------------------
SideModelVisitor::SideModelVisitor( ClientPublisher_ABC& publisher )
    : publisher_ ( &publisher )
{
    // NOTHING
}

    
// -----------------------------------------------------------------------------
// Name: SideModelVisitor::Visit
// Created: JCR 2007-05-15
// -----------------------------------------------------------------------------
void SideModelVisitor::Visit( Entity_ABC& entity )
{
    entity.SendCreation( *publisher_ );
}
    
// -----------------------------------------------------------------------------
// Name: SideModelVisitor::Accept
// Created: JCR 2007-05-15
// -----------------------------------------------------------------------------
bool SideModelVisitor::Accept( const ASN1T_MsgTeamCreation& asn )
{
    type_ = asn.type;
    return true;
}

// -----------------------------------------------------------------------------
// Name: ModelVisitors::GetSide
// Created: JCR 2007-05-15
// -----------------------------------------------------------------------------
ASN1T_EnumDiplomacy SideModelVisitor::GetSide() const
{
    return type_;
}


// -----------------------------------------------------------------------------
// Name: ModelVisitors::FormationModelVisitor
// Created: JCR 2007-05-16
// -----------------------------------------------------------------------------
FormationModelVisitor::FormationModelVisitor( ClientPublisher_ABC& publisher )
    : publisher_ ( &publisher )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ModelVisitors::Visit
// Created: JCR 2007-05-16
// -----------------------------------------------------------------------------
void FormationModelVisitor::Visit( Entity_ABC& entity )
{
    entity.SendCreation( *publisher_ );
}
        
// -----------------------------------------------------------------------------
// Name: ModelVisitors::Accept
// Created: JCR 2007-05-16
// -----------------------------------------------------------------------------
bool FormationModelVisitor::Accept( const ASN1T_MsgFormationCreation& asn )
{
    oid_ = asn.oid; 
    echelon_ = asn.niveau;
    return true;
}

// -----------------------------------------------------------------------------
// Name: ModelVisitors::GetNatureLevel
// Created: JCR 2007-05-16
// -----------------------------------------------------------------------------
ASN1T_EnumNatureLevel FormationModelVisitor::GetEchelon() const
{
    return echelon_;
}

// -----------------------------------------------------------------------------
// Name: ModelVisitors::GetOID
// Created: JCR 2007-05-16
// -----------------------------------------------------------------------------
ASN1T_OID FormationModelVisitor::GetOID() const
{
    return oid_;
}

// -----------------------------------------------------------------------------
// Name: ModelVisitors::UnitKnowledgeModelVisitor
// Created: JCR 2007-05-24
// -----------------------------------------------------------------------------
UnitKnowledgeModelVisitor::UnitKnowledgeModelVisitor( ClientPublisher_ABC& publisher )
    : publisher_ ( &publisher )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ModelVisitors::Visit
// Created: JCR 2007-05-24
// -----------------------------------------------------------------------------
void UnitKnowledgeModelVisitor::Visit( Entity_ABC& entity )
{
    entity.SendCreation( *publisher_ );
}

// -----------------------------------------------------------------------------
// Name: ModelVisitors::Accept
// Created: JCR 2007-05-24
// -----------------------------------------------------------------------------
bool UnitKnowledgeModelVisitor::Accept( const ASN1T_MsgUnitKnowledgeCreation& asn )
{
    type_ = asn.type_unite;
    return true;
}

// -----------------------------------------------------------------------------
// Name: ModelVisitors::GetType
// Created: JCR 2007-05-24
// -----------------------------------------------------------------------------
ASN1T_UnitType UnitKnowledgeModelVisitor::GetType() const
{
    return type_;
}
