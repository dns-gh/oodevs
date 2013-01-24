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
    template< typename T > class ExtensionVisitor_ABC;

// =============================================================================
/** @class  Entity_ABC
    @brief  Entity base class
*/
// Created: AGE 2006-02-07
// =============================================================================

class EntityBase_ABC : public tools::Extendable< Extension_ABC >
{
public:
             EntityBase_ABC() {}
    virtual ~EntityBase_ABC() {}

    typedef public tools::SortedInterfaceContainer< Extension_ABC > T_Interfaces;

    template< typename T >
    void Update( const T& updateMessage )
    {
        GetInterfaces().Apply( & Updatable_ABC< T >::DoUpdate, updateMessage );
    }
    template< typename T >
    void Attach( T& extension )
    {
        tools::Extendable< Extension_ABC >::Attach( extension );
        AddExtension( extension );
    }
    virtual void AddExtension( Extension_ABC& );

    virtual T_Interfaces& GetInterfaces() const
    {
        return const_cast< T_Interfaces& >( interfaces_ );
    }

private:
    T_Interfaces interfaces_;
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

    //! @name GUI methods
    //@{
    virtual void ContextMenu( ActionController& controller, const QPoint& where ) const = 0;
    virtual void Activate( ActionController& controller ) const = 0;
    virtual void OverFly( ActionController& controller ) const; // $$$$ ABR 2011-10-28: Not abstract cause not yet needed for all entities

    //! @name Extension methods
    //@{
    virtual void AddExtension( Extension_ABC& ext );
    virtual void Apply( ExtensionVisitor_ABC< Extension_ABC >& visitor ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    Entity_ABC( const Entity_ABC& );            //!< Copy constructor
    Entity_ABC& operator=( const Entity_ABC& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    std::vector< Extension_ABC* > extensions_;
    //@}
};

}

#endif // __Entity_ABC_h_
