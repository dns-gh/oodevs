// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __PopulationKnowledges_h_
#define __PopulationKnowledges_h_

#include "game_asn/Asn.h"
#include "clients_kernel/Updatable_ABC.h"
#include "clients_kernel/Extension_ABC.h"
#include "clients_kernel/Resolver.h"
#include "clients_kernel/Creatable.h"

namespace kernel
{
    class Controller;
    class KnowledgeGroup_ABC;
}

class PopulationKnowledge_ABC;
class PopulationKnowledgeFactory_ABC;

// =============================================================================
/** @class  PopulationKnowledges
    @brief  PopulationKnowledges
*/
// Created: AGE 2006-02-15
// =============================================================================
class PopulationKnowledges : public kernel::Extension_ABC
                           , public kernel::Updatable_ABC< ASN1T_MsgPopulationKnowledgeCreation >
                           , public kernel::Updatable_ABC< ASN1T_MsgPopulationKnowledgeUpdate >
                           , public kernel::Updatable_ABC< ASN1T_MsgPopulationKnowledgeDestruction >
                           , public kernel::Updatable_ABC< ASN1T_MsgPopulationConcentrationKnowledgeCreation >
                           , public kernel::Updatable_ABC< ASN1T_MsgPopulationConcentrationKnowledgeUpdate >
                           , public kernel::Updatable_ABC< ASN1T_MsgPopulationConcentrationKnowledgeDestruction >
                           , public kernel::Updatable_ABC< ASN1T_MsgPopulationFlowKnowledgeCreation >
                           , public kernel::Updatable_ABC< ASN1T_MsgPopulationFlowKnowledgeUpdate >
                           , public kernel::Updatable_ABC< ASN1T_MsgPopulationFlowKnowledgeDestruction >
                           , public kernel::Resolver< PopulationKnowledge_ABC >
                           , public kernel::Creatable< PopulationKnowledges >
{

public:
    //! @name Constructors/Destructor
    //@{
             PopulationKnowledges( kernel::Controller& controller, const kernel::KnowledgeGroup_ABC& owner, PopulationKnowledgeFactory_ABC& factory );
    virtual ~PopulationKnowledges();
    //@}

    //! @name Operations
    //@{
    //@}

private:
    //! @name Copy/Assignement
    //@{
    PopulationKnowledges( const PopulationKnowledges& );            //!< Copy constructor
    PopulationKnowledges& operator=( const PopulationKnowledges& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    virtual void DoUpdate( const ASN1T_MsgPopulationKnowledgeCreation&                 message );

    virtual void DoUpdate( const ASN1T_MsgPopulationKnowledgeUpdate&                   message );
    virtual void DoUpdate( const ASN1T_MsgPopulationKnowledgeDestruction&              message );
    virtual void DoUpdate( const ASN1T_MsgPopulationConcentrationKnowledgeCreation&    message );
    virtual void DoUpdate( const ASN1T_MsgPopulationConcentrationKnowledgeUpdate&      message );
    virtual void DoUpdate( const ASN1T_MsgPopulationConcentrationKnowledgeDestruction& message );
    virtual void DoUpdate( const ASN1T_MsgPopulationFlowKnowledgeCreation&             message );
    virtual void DoUpdate( const ASN1T_MsgPopulationFlowKnowledgeUpdate&               message );
    virtual void DoUpdate( const ASN1T_MsgPopulationFlowKnowledgeDestruction&          message );

    template< typename T >
    void UpdatePopulation( const T& message );
    //@}

private:
    //! @name Member data
    //@{
    const kernel::KnowledgeGroup_ABC& owner_;
    PopulationKnowledgeFactory_ABC& factory_;
    kernel::Controller& controller_;
    //@}
};

#endif // __PopulationKnowledges_h_
