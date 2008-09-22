// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __KnowledgeManipulator_h_
#define __KnowledgeManipulator_h_

#include "dispatcher/Registrable_ABC.h"
#include "dispatcher/Position.h"
#include "clients_kernel/Extension_ABC.h"

namespace kernel
{
    class CoordinateConverter_ABC;
}

namespace dispatcher
{
    class AgentKnowledge;
}

namespace script
{

// =============================================================================
/** @class  KnowledgeManipulator
    @brief  KnowledgeManipulator
*/
// Created: AGE 2008-06-17
// =============================================================================
class KnowledgeManipulator : public kernel::Extension_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             KnowledgeManipulator( const kernel::CoordinateConverter_ABC& converter, const dispatcher::AgentKnowledge& knowledge );
    virtual ~KnowledgeManipulator();
    //@}

    //! @name Operations
    //@{
    struct Registrar : public dispatcher::Registrable_ABC
    {
        virtual void RegisterIn( directia::Brain& brain );
    };
    void UsedByDIA    () {};
    void ReleasedByDIA() {};
    //@}

private:
    //! @name Copy/Assignment
    //@{
    KnowledgeManipulator( const KnowledgeManipulator& );            //!< Copy constructor
    KnowledgeManipulator& operator=( const KnowledgeManipulator& ); //!< Assignment operator
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
    const dispatcher::AgentKnowledge& knowledge_;
    //@}
};

}

#endif // __KnowledgeManipulator_h_
