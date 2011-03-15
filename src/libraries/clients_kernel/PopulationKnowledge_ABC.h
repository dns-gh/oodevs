// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __PopulationKnowledge_ABC_h_
#define __PopulationKnowledge_ABC_h_

#include "Knowledge_ABC.h"
#include "Population_ABC.h"
#include "KnowledgeGroup_ABC.h"

namespace kernel
{
    class Displayer_ABC;
    class Team_ABC;

// =============================================================================
/** @class  PopulationKnowledge_ABC
    @brief  PopulationKnowledge_ABC
*/
// Created: AGE 2006-10-16
// =============================================================================
class PopulationKnowledge_ABC : public Knowledge_ABC
{
public:
    //! @name Static
    //@{
    static const QString typeName_;
    //@}

public:
    //! @name Constructors/Destructor
    //@{
             PopulationKnowledge_ABC();
    virtual ~PopulationKnowledge_ABC();
    //@}

    //! @name Operations
    //@{
    virtual const kernel::Population_ABC* GetEntity() const = 0;
    virtual const kernel::KnowledgeGroup_ABC& GetOwner() const = 0;

    virtual void Display( kernel::Displayer_ABC& displayer ) const = 0;
    virtual void DisplayInList( kernel::Displayer_ABC& displayer ) const = 0;
    //@}

    //! @name Operations
    //@{
    virtual QString GetTypeName() const;
    virtual void Select( kernel::ActionController& controller ) const;
    virtual void ContextMenu( kernel::ActionController& controller, const QPoint& where ) const;
    virtual void Activate( kernel::ActionController& controller ) const;
    //@}
};

}

#endif // __PopulationKnowledge_ABC_h_
