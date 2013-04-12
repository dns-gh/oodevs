// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#ifndef gui_PropertiesGroup_h
#define gui_PropertiesGroup_h

#include "Property_ABC.h"

namespace kernel
{
    class Displayer_ABC;
    class Entity_ABC;
}

namespace gui
{
// =============================================================================
/** @class  PropertiesGroup
    @brief  Properties group
*/
// Created: LGY 2013-01-15
// =============================================================================
class PropertiesGroup : public Property_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit PropertiesGroup( kernel::Displayer_ABC& comparator, const kernel::Entity_ABC& owner );
    virtual ~PropertiesGroup();
    //@}

    //! @name Property_ABC operations
    //@{
    virtual QWidget* CreateEditor( QWidget* parent, EditorFactory_ABC& factory );
    virtual void SetValueFromEditor( QWidget* editor );
    virtual void Display( kernel::Displayer_ABC& displayer );
    virtual const QString& GetName() const;
    virtual E_Category GetCategory() const;
    virtual const kernel::Entity_ABC& GetOwner() const;
    //@}

    //! @name Operations
    //@{
    virtual void AddSubProperty( Property_ABC* property );
    //@}

private:
    //! @name Helpers
    //@{
    bool HasSameProperty() const;
    //@}

private:
    //! @name Types
    //@{
    typedef std::vector< Property_ABC* > T_SubProperties;
    //@}

private:
    //! @name Member data
    //@{
    T_SubProperties subProperties_;
    kernel::Displayer_ABC& comparator_;
    const kernel::Entity_ABC& owner_;
    //@}
};

}

#endif // gui_PropertiesGroup_h
