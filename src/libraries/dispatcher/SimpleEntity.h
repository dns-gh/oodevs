// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __SimpleEntity_h_
#define __SimpleEntity_h_

#include "clients_kernel/Entity_ABC.h"
#pragma warning( push, 0 )
#include <QtCore/qstring.h>
#include <QtCore/qstringlist.h>
#pragma warning( pop )

namespace dispatcher
{
// =============================================================================
/** @class  SimpleEntity
    @brief  SimpleEntity
*/
// Created: AGE 2008-06-20
// =============================================================================
template< typename B = kernel::Entity_ABC >
class SimpleEntity : public B
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit SimpleEntity( unsigned long id, const QString& name = "" )
        : id_  ( id )
        , name_( name )
    {
        // NOTHING
    }
    virtual ~SimpleEntity()
    {
        // NOTHING
    }
    //@}

    //! @name Operations
    //@{
    template< typename T >
    void ApplyUpdate( const T& message )
    {
        B::Update( message );
    }

    virtual QString GetName() const
    {
        return name_;
    }
    virtual void SetName( const QString& name )
    {
        name_ = name;
    }
    virtual unsigned long GetId() const
    {
        return id_;
    }

    virtual void Select( kernel::ActionController& /*controller*/ ) const
    {
        // NOTHING
    }
    virtual void MultipleSelect( kernel::ActionController& /*controller*/, const std::vector< const kernel::GraphicalEntity_ABC* >& /*elements*/ ) const
    {
        // NOTHING
    }
    virtual void ContextMenu( kernel::ActionController& /*controller*/, const QPoint& /*where*/ ) const
    {
        // NOTHING
    }
    virtual void Activate( kernel::ActionController& /*controller*/ ) const
    {
        // NOTHING
    }
    //@}

private:
    //! @name Copy/Assignment
    //@{
    SimpleEntity( const SimpleEntity& );            //!< Copy constructor
    SimpleEntity& operator=( const SimpleEntity& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    unsigned long id_;
    QString name_;
    //@}
};

}

#endif // __SimpleEntity_h_
