// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ModelWalker_h_
#define __ModelWalker_h_

#include "ClientPublisher_ABC.h"
#include "game_asn/Asn.h"

namespace dispatcher
{
    class Model;
    class ModelDataVisitor_ABC;
    class Entity_ABC;

// =============================================================================
/** @class  ModelWalker
    @brief  ModelWalker
*/
// Created: JCR 2007-05-15
// =============================================================================
class ModelWalker : public ClientPublisher_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit ModelWalker( const Model& model );
    virtual ~ModelWalker();
    //@}

    //! @name Accessors
    //@{
    template< typename ASN_MsgType >
    void WalkTo( ModelDataVisitor_ABC& visitor, const ASN_MsgType& asn );            
    void Visit( ModelDataVisitor_ABC& visitor, Entity_ABC& entity ); 
    //@}

private:
    //! @name Copy/Assignement
    //@{
    ModelWalker( const ModelWalker& );            //!< Copy constructor
    ModelWalker& operator=( const ModelWalker& ); //!< Assignement operator
    //@}

    //! @name Walking methods
    //@{
    void Send   ( const ASN1T_MsgsSimToClient& msg );    
    void WalkTo ( const ASN1T_MsgUnitCreation& asn );
    void WalkTo ( const ASN1T_MsgAutomatCreation& asn );
    void WalkTo ( const ASN1T_MsgUnitKnowledgeCreation& asn );
    void WalkTo ( const ASN1T_MsgUnitKnowledgeUpdate& asn );
    void WalkTo ( const ASN1T_MsgKnowledgeGroupCreation& asn );    
    void WalkTo ( const ASN1T_MsgTeamCreation& asn );    
    void WalkTo ( const ASN1T_MsgObjectCreation& asn );
    void WalkTo ( const ASN1T_TacticalLine& asn );
    void WalkTo ( const ASN1T_MsgFormationCreation& asn );
    //@}

private:
    //! @name Member data
    //@{
    const Model&            model_;
    ModelDataVisitor_ABC*   visitor_;
    //@}
};

// -----------------------------------------------------------------------------
// Name: ModelWalker::WalkTo
// Created: JCR 2007-05-16
// -----------------------------------------------------------------------------
template< typename ASN_MsgType >
void ModelWalker::WalkTo( ModelDataVisitor_ABC& visitor, const ASN_MsgType& asn )
{
    visitor_ = &visitor;
    WalkTo( asn );
}

}

#endif // __ModelWalker_h_
