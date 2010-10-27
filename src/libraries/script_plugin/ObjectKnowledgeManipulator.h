// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ObjectKnowledgeManipulator_h_
#define __ObjectKnowledgeManipulator_h_

#include "dispatcher/Registrable_ABC.h"
#include "dispatcher/Position.h"
#include "clients_kernel/Extension_ABC.h"

namespace kernel
{
    class CoordinateConverter_ABC;
}

namespace dispatcher
{
    class ObjectKnowledge;
}

namespace plugins
{
namespace script
{

// =============================================================================
/** @class  ObjectObjectKnowledgeManipulator
    @brief  ObjectObjectKnowledgeManipulator
*/
// Created: AGE 2008-06-17
// =============================================================================
class ObjectKnowledgeManipulator : public kernel::Extension_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             ObjectKnowledgeManipulator( const kernel::CoordinateConverter_ABC& converter, const dispatcher::ObjectKnowledge& knowledge );
    virtual ~ObjectKnowledgeManipulator();
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
    ObjectKnowledgeManipulator( const ObjectKnowledgeManipulator& );            //!< Copy constructor
    ObjectKnowledgeManipulator& operator=( const ObjectKnowledgeManipulator& ); //!< Assignment operator
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
    const dispatcher::ObjectKnowledge& knowledge_;
    //@}
};

}
}

#endif // __ObjectKnowledgeManipulator_h_
