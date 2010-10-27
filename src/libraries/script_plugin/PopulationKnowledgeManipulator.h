// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __PopulationKnowledgeManipulator_h_
#define __PopulationKnowledgeManipulator_h_

#include "dispatcher/Registrable_ABC.h"
#include "dispatcher/Position.h"
#include "clients_kernel/Extension_ABC.h"

namespace kernel
{
    class CoordinateConverter_ABC;
}

namespace dispatcher
{
    class PopulationKnowledge;
}

namespace plugins
{
namespace script
{

// =============================================================================
/** @class  PopulationPopulationKnowledgeManipulator
    @brief  PopulationPopulationKnowledgeManipulator
*/
// Created: DSO 2010-09-16
// =============================================================================
class PopulationKnowledgeManipulator : public kernel::Extension_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             PopulationKnowledgeManipulator( const kernel::CoordinateConverter_ABC& converter, const dispatcher::PopulationKnowledge& knowledge );
    virtual ~PopulationKnowledgeManipulator();
    //@}

    //! @name Operations
    //@{
    struct Registrar : public dispatcher::Registrable_ABC
    {
        virtual void RegisterIn( directia::brain::Brain& brain );
    };
    void UsedByDIA    () {}
    void ReleasedByDIA() {}
    //@}

private:
    //! @name Copy/Assignment
    //@{
    PopulationKnowledgeManipulator( const PopulationKnowledgeManipulator& );            //!< Copy constructor
    PopulationKnowledgeManipulator& operator=( const PopulationKnowledgeManipulator& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    unsigned int GetIdentifier() const;
    dispatcher::Position GetPosition() const;
    std::string GetOwnerTeam() const;
    //@}

private:
    //! @name Member data
    //@{
    const kernel::CoordinateConverter_ABC& converter_;
    const dispatcher::PopulationKnowledge& knowledge_;
    //@}
};

}
}

#endif // __PopulationKnowledgeManipulator_h_
