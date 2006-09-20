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

#include "ASN_Types.h"
#include "clients_kernel/Updatable_ABC.h"
#include "clients_kernel/Extension_ABC.h"
#include "clients_kernel/Resolver.h"

namespace kernel
{
    class Controller;
    class KnowledgeGroup_ABC;
    class InstanciationComplete;
}

class PopulationKnowledge;
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
                           , public kernel::Updatable_ABC< ASN1T_MsgPopulationFluxKnowledgeCreation >
                           , public kernel::Updatable_ABC< ASN1T_MsgPopulationFluxKnowledgeUpdate >
                           , public kernel::Updatable_ABC< ASN1T_MsgPopulationFluxKnowledgeDestruction >
                           , public kernel::Updatable_ABC< kernel::InstanciationComplete >
                           , public kernel::Resolver< PopulationKnowledge >
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
    virtual void DoUpdate( const ASN1T_MsgPopulationFluxKnowledgeCreation&             message );
    virtual void DoUpdate( const ASN1T_MsgPopulationFluxKnowledgeUpdate&               message );
    virtual void DoUpdate( const ASN1T_MsgPopulationFluxKnowledgeDestruction&          message );
    virtual void DoUpdate( const kernel::InstanciationComplete& );

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
