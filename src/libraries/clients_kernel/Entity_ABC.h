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

#include "GraphicalEntity_ABC.h"
#include "Updatable_ABC.h"
#include "Extension_ABC.h"
#include <geometry/Types.h>
#include <tools/Extendable.h>
#include <tools/SortedInterfaceContainer.h>
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
    class ExtensionVisitor_ABC;

// =============================================================================
/** @class  Entity_ABC
    @brief  Entity base class
*/
// Created: AGE 2006-02-07
// =============================================================================

class EntityBase_ABC : public tools::Extendable< Extension_ABC >
                     , private boost::noncopyable
{
public:
             EntityBase_ABC() {}
    virtual ~EntityBase_ABC() {}
    
    virtual unsigned long GetId() const = 0;

    typedef tools::SortedInterfaceContainer< Extension_ABC > T_Interfaces;

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
    virtual const std::string& GetTypeName() const;
    //@}

    //! @name GraphicalEntity_ABC implementation
    //@{
    virtual QString GetTooltip() const;
    virtual void OverFly( ActionController& controller ) const;
    //@}

    //! @name Extension methods
    //@{
    virtual void AddExtension( Extension_ABC& ext );
    virtual void Apply( ExtensionVisitor_ABC& visitor ) const;
    //@}

    //! @name Aggregation
    //@{
    void Aggregate( bool aggregate );
    bool IsAggregated() const;
    //@}

private:
    //! @name Member data
    //@{
    std::vector< Extension_ABC* > extensions_;
    bool aggregated_;
    //@}
};

}

#endif // __Entity_ABC_h_
