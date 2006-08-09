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
#include "astec_kernel/Updatable_ABC.h"
#include "astec_kernel/Extension_ABC.h"
#include "astec_kernel/Resolver.h"

class PopulationKnowledge;
class Controller;
class PopulationKnowledgeFactory_ABC;
class KnowledgeGroup_ABC;

// =============================================================================
/** @class  PopulationKnowledges
    @brief  PopulationKnowledges
*/
// Created: AGE 2006-02-15
// =============================================================================
class PopulationKnowledges : public Extension_ABC
                           , public Updatable_ABC< ASN1T_MsgPopulationKnowledgeCreation >
                           , public Updatable_ABC< ASN1T_MsgPopulationKnowledgeUpdate >
                           , public Updatable_ABC< ASN1T_MsgPopulationKnowledgeDestruction >
                           , public Updatable_ABC< ASN1T_MsgPopulationConcentrationKnowledgeCreation >
                           , public Updatable_ABC< ASN1T_MsgPopulationConcentrationKnowledgeUpdate >
                           , public Updatable_ABC< ASN1T_MsgPopulationConcentrationKnowledgeDestruction >
                           , public Updatable_ABC< ASN1T_MsgPopulationFluxKnowledgeCreation >
                           , public Updatable_ABC< ASN1T_MsgPopulationFluxKnowledgeUpdate >
                           , public Updatable_ABC< ASN1T_MsgPopulationFluxKnowledgeDestruction >
                           , public Resolver< PopulationKnowledge >
{

public:
    //! @name Constructors/Destructor
    //@{
             PopulationKnowledges( Controller& controller, const KnowledgeGroup_ABC& owner, PopulationKnowledgeFactory_ABC& factory );
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

    template< typename T >
    void UpdatePopulation( const T& message );
    //@}

private:
    //! @name Member data
    //@{
    const KnowledgeGroup_ABC& owner_;
    PopulationKnowledgeFactory_ABC& factory_;
    Controller& controller_;
    //@}
};

#endif // __PopulationKnowledges_h_
