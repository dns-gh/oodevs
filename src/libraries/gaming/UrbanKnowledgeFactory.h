// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __UrbanKnowledgeFactory_h_
#define __UrbanKnowledgeFactory_h_

namespace kernel
{
    class Controller;
    class Team_ABC;
    class UrbanKnowledge_ABC;
}

namespace sword
{
    class UrbanKnowledgeCreation;
}

class Model;

// =============================================================================
/** @class  UrbanKnowledgeFactory
    @brief  UrbanKnowledgeFactory
*/
// Created: MGD 2009-12-09
// =============================================================================
class UrbanKnowledgeFactory
{
public:
    //! @name Constructors/Destructor
    //@{
             UrbanKnowledgeFactory( kernel::Controller& controller, Model& model );
    virtual ~UrbanKnowledgeFactory();
    //@}

    //! @name Operations
    //@{
    virtual kernel::UrbanKnowledge_ABC* Create( const kernel::Team_ABC& owner, const sword::UrbanKnowledgeCreation& message );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    UrbanKnowledgeFactory( const UrbanKnowledgeFactory& );            //!< Copy constructor
    UrbanKnowledgeFactory& operator=( const UrbanKnowledgeFactory& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controller& controller_;
    Model& model_;
    //@}
};

#endif // __UrbanKnowledgeFactory_h_
