// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __UrbanKnowledge_ABC_h_
#define __UrbanKnowledge_ABC_h_

#include "Knowledge_ABC.h"
//#include "Urban_ABC.h"
#include "Team_ABC.h"

namespace urban
{
    class TerrainObject_ABC;
}
namespace kernel
{
    class Displayer_ABC;

// =============================================================================
/** @class  UrbanKnowledge_ABC
    @brief  UrbanKnowledge_ABC
*/
// Created: MGD 2009-12-09
// =============================================================================
class UrbanKnowledge_ABC : public Knowledge_ABC
{
public:
    //! @name Static
    //@{
    static const QString typeName_;
    //@}

public:
    //! @name Constructors/Destructor
    //@{
             UrbanKnowledge_ABC();
    virtual ~UrbanKnowledge_ABC();
    //@}

    //! @name Operations
    //@{
    virtual const kernel::Entity_ABC* GetEntity() const = 0;
    virtual const kernel::Team_ABC&   GetOwner() const = 0;

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

#endif // __UrbanKnowledge_ABC_h_
