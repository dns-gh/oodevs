// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __Formation_ABC_h_
#define __Formation_ABC_h_

#include "Entity_ABC.h"
#include "ENT/ENT_Enums_Gen.h"

namespace kernel
{
    class HierarchyLevel_ABC;
    class LogisticLevel;

// =============================================================================
/** @class  Formation_ABC
    @brief  Formation_ABC
*/
// Created: SBO 2006-09-19
// =============================================================================
class Formation_ABC : public Entity_ABC
{
public:
    //! @name Static
    //@{
    static const std::string typeName_;
    //@}

public:
    //! @name Constructors/Destructor
    //@{
             Formation_ABC();
    virtual ~Formation_ABC();
    //@}

    //! @name Accessors
    //@{
    virtual E_NatureLevel GetLevel() const = 0;
    virtual const LogisticLevel& GetLogisticLevel() const = 0;
    //@}

    //! @name Operations
    //@{
    virtual const std::string& GetTypeName() const;

    virtual void Select( ActionController& controller ) const;
    virtual void MultipleSelect( ActionController& controller, const std::vector< const kernel::Selectable_ABC* >& elements ) const;
    virtual void ContextMenu( ActionController& controller, const QPoint& where ) const;
    virtual void Activate( ActionController& controller ) const;
    virtual void NotifyChange();
    //@}
};

}

#endif // __Formation_ABC_h_
