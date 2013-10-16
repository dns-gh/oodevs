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

#include "clients_kernel/Updatable_ABC.h"
#include "clients_kernel/Extension_ABC.h"
#include "clients_kernel/Creatable.h"
#include <tools/Resolver.h>

namespace kernel
{
    class Controller;
    class KnowledgeGroup_ABC;
    class PopulationKnowledge_ABC;
}

namespace sword
{
    class CrowdKnowledgeCreation;
    class CrowdKnowledgeUpdate;
    class CrowdKnowledgeDestruction;
    class CrowdConcentrationKnowledgeCreation;
    class CrowdConcentrationKnowledgeUpdate;
    class CrowdConcentrationKnowledgeDestruction;
    class CrowdFlowKnowledgeCreation;
    class CrowdFlowKnowledgeUpdate;
    class CrowdFlowKnowledgeDestruction;
}

class PopulationKnowledgeFactory_ABC;

// =============================================================================
/** @class  PopulationKnowledges
    @brief  PopulationKnowledges
*/
// Created: AGE 2006-02-15
// =============================================================================
class PopulationKnowledges : public kernel::Extension_ABC
                           , public kernel::Updatable_ABC< sword::CrowdKnowledgeCreation >
                           , public kernel::Updatable_ABC< sword::CrowdKnowledgeUpdate >
                           , public kernel::Updatable_ABC< sword::CrowdKnowledgeDestruction >
                           , public kernel::Updatable_ABC< sword::CrowdConcentrationKnowledgeCreation >
                           , public kernel::Updatable_ABC< sword::CrowdConcentrationKnowledgeUpdate >
                           , public kernel::Updatable_ABC< sword::CrowdConcentrationKnowledgeDestruction >
                           , public kernel::Updatable_ABC< sword::CrowdFlowKnowledgeCreation >
                           , public kernel::Updatable_ABC< sword::CrowdFlowKnowledgeUpdate >
                           , public kernel::Updatable_ABC< sword::CrowdFlowKnowledgeDestruction >
                           , public tools::Resolver< kernel::PopulationKnowledge_ABC >
                           , public kernel::Creatable< PopulationKnowledges >
{
public:
    //! @name Constructors/Destructor
    //@{
             PopulationKnowledges( kernel::Controller& controller, const kernel::KnowledgeGroup_ABC& owner, PopulationKnowledgeFactory_ABC& factory );
    virtual ~PopulationKnowledges();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    PopulationKnowledges( const PopulationKnowledges& );            //!< Copy constructor
    PopulationKnowledges& operator=( const PopulationKnowledges& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    virtual void DoUpdate( const sword::CrowdKnowledgeCreation&                 message );

    virtual void DoUpdate( const sword::CrowdKnowledgeUpdate&                   message );
    virtual void DoUpdate( const sword::CrowdKnowledgeDestruction&              message );
    virtual void DoUpdate( const sword::CrowdConcentrationKnowledgeCreation&    message );
    virtual void DoUpdate( const sword::CrowdConcentrationKnowledgeUpdate&      message );
    virtual void DoUpdate( const sword::CrowdConcentrationKnowledgeDestruction& message );
    virtual void DoUpdate( const sword::CrowdFlowKnowledgeCreation&             message );
    virtual void DoUpdate( const sword::CrowdFlowKnowledgeUpdate&               message );
    virtual void DoUpdate( const sword::CrowdFlowKnowledgeDestruction&          message );

    template< typename T >
    void UpdatePopulation( const T& message );
    template< typename T >
    void UpdatePopulationPart( const T& message );
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
