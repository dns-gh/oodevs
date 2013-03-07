// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#ifndef kernel_PropertiesGroup_h
#define kernel_PropertiesGroup_h

#include "Property_ABC.h"

namespace kernel
{
    class Displayer_ABC;
    class Entity_ABC;

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
    explicit PropertiesGroup( Displayer_ABC& comparator, const Entity_ABC& owner );
    virtual ~PropertiesGroup();
    //@}

    //! @name Property_ABC operations
    //@{
    virtual QWidget* CreateEditor( QWidget* parent, EditorFactory_ABC& factory );
    virtual void SetValueFromEditor( QWidget* editor );
    virtual void Display( Displayer_ABC& displayer );
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
    Displayer_ABC& comparator_;
    const Entity_ABC& owner_;
    //@}
};

}

#endif // kernel_PropertiesGroup_h
