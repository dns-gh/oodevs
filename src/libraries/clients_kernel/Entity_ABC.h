// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Entity_ABC_h_
#define __Entity_ABC_h_

#include "tools/Extendable.h"
#include "tools/SortedInterfaceContainer.h"
#include "GraphicalEntity_ABC.h"
#include "Updatable_ABC.h"
#include "Extension_ABC.h"
#include "geometry/Types.h"
#include <boost/noncopyable.hpp>

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
                 , public GraphicalEntity_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             Entity_ABC();
    virtual ~Entity_ABC();
    //@}

    //! @name Accessors
    //@{
    virtual QString GetShortName() const;
    virtual unsigned long GetId() const = 0;
    virtual const std::string& GetTypeName() const;
    //@}

    //! @name GraphicalEntity_ABC implementation
    //@{
    virtual QString GetTooltip() const;
    virtual void OverFly( ActionController& controller ) const;
    //@}

    void Draw( const geometry::Point2f& where, const Viewport_ABC& viewport, const GlTools_ABC& tools ) const;

    tools::SortedInterfaceContainer< Extension_ABC >& Interface() const;
    //@}

protected:
    //! @name Helpers
    //@{
    void RegisterSelf( Extension_ABC& ext );
    //@}

public:

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
