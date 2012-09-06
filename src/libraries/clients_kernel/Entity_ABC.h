// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __Entity_ABC_h_
#define __Entity_ABC_h_

#include "tools/Extendable.h"
#include "tools/SortedInterfaceContainer.h"
#include "Selectable_ABC.h"
#include "Updatable_ABC.h"
#include "Extension_ABC.h"
#include <geometry/types.h>

class QPoint;
class QString;

namespace xml
{
    class xostream;
}

namespace kernel
{
    class Controller;
    class ActionController;
    class GlTools_ABC;
    class Drawer;
    class Viewport_ABC;

// =============================================================================
/** @class  Entity_ABC
    @brief  Entity base class
*/
// Created: AGE 2006-02-07
// =============================================================================
    
class EntityBase_ABC : public tools::Extendable< Extension_ABC >
                 , public tools::SortedInterfaceContainer< Extension_ABC >
{
public:
             EntityBase_ABC() {};
    virtual ~EntityBase_ABC() {};

    template< typename T >
    void Update( const T& updateMessage )
    {
        Apply( & Updatable_ABC< T >::DoUpdate, updateMessage );
    }
    template< typename T >
    void Attach( T& extension )
    {
        tools::Extendable< Extension_ABC >::Attach( extension );
        AddExtension( extension );
    }
    virtual void AddExtension( Extension_ABC& );

};

class Entity_ABC : public EntityBase_ABC
                 , public Selectable_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             Entity_ABC();
    virtual ~Entity_ABC();
    //@}

    //! @name Accessors
    //@{
    virtual QString GetName() const = 0;
    virtual unsigned long GetId() const = 0;
    virtual const std::string& GetTypeName() const;
    virtual QString GetTooltip() const;
    //@}

    //! @name Operations
    //@{
    virtual void ContextMenu( ActionController& controller, const QPoint& where ) const = 0;
    virtual void Activate( ActionController& controller ) const = 0;
    virtual void OverFly( ActionController& controller ) const; // $$$$ ABR 2011-10-28: Not abstract cause not yet needed for all entities

    void Draw( const geometry::Point2f& where, const Viewport_ABC& viewport, const GlTools_ABC& tools ) const;

    tools::SortedInterfaceContainer< Extension_ABC >& Interface() const;
    //@}

protected:
    //! @name Helpers
    //@{
    void RegisterSelf( Extension_ABC& ext );
    //@}

public:
    //! @name Copy/Assignment
    //@{
    Entity_ABC( const Entity_ABC& );            //!< Copy constructor
    Entity_ABC& operator=( const Entity_ABC& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    virtual void AddExtension( Extension_ABC& ext );
    //@}

private:
    //! @name Member data
    //@{
    std::auto_ptr< Drawer > drawer_;
    //@}
};

}

#endif // __Entity_ABC_h_
