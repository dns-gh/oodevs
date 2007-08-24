// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ModelVisitors_h_
#define __ModelVisitors_h_

#include "ModelDataVisitor_ABC.h"
#include "game_asn/Asn.h"

namespace dispatcher
{
    class ClientPublisher_ABC;

// =============================================================================
/** @class  SideModelVisitor
    @brief  SideModelVisitor
*/
// Created: JCR 2007-05-15
// =============================================================================
    class SideModelVisitor 
        : public ModelDataVisitor_ABC        
    {
    public:
        SideModelVisitor( ClientPublisher_ABC& publisher );

        virtual void Visit  ( Entity_ABC& entity );
        virtual bool Accept ( const ASN1T_MsgTeamCreation& asn );

        ASN1T_EnumDiplomacy GetSide() const;
    private:
        ClientPublisher_ABC* publisher_;
        ASN1T_EnumDiplomacy type_;
    };

    class FormationModelVisitor 
        : public ModelDataVisitor_ABC        
    {
    public:
        FormationModelVisitor( ClientPublisher_ABC& publisher );

        virtual void Visit  ( Entity_ABC& entity );
        virtual bool Accept ( const ASN1T_MsgFormationCreation& asn );

        ASN1T_EnumNatureLevel GetEchelon() const;
        ASN1T_OID GetOID() const;
    private:
        ClientPublisher_ABC* publisher_;
        ASN1T_EnumNatureLevel echelon_;
        ASN1T_OID oid_;
    };

    class UnitKnowledgeModelVisitor 
        : public ModelDataVisitor_ABC        
    {
    public:
        UnitKnowledgeModelVisitor( ClientPublisher_ABC& publisher );

        virtual void Visit  ( Entity_ABC& entity );
        virtual bool Accept ( const ASN1T_MsgUnitKnowledgeCreation& asn );

        ASN1T_UnitType GetType() const;
    private:
        ClientPublisher_ABC* publisher_;
        ASN1T_UnitType type_;
    };

}

#endif // __ModelVisitors_h_
