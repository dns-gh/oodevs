// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __EntityImplementation_h_
#define __EntityImplementation_h_

#pragma warning( push, 0 )
#include <QtCore/qstring.h>
#pragma warning( pop )

namespace kernel
{
    class Controller;

// =============================================================================
/** @class  EntityImplementation
    @brief  Entity implementation
*/
// Created: AGE 2006-10-12
// =============================================================================
template< typename I >
class EntityImplementation : public I
{
public:
    //! @name Constructors/Destructor
    //@{
             EntityImplementation( Controller& controller, unsigned long id, const QString& name );
    virtual ~EntityImplementation();
    //@}

    //! @name Operations
    //@{
    virtual QString GetName() const;
    virtual unsigned long GetId() const;

    void Polish();
    //@}

protected:
    //! @name Modifiers
    //@{
    void Touch();
    void Destroy();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    EntityImplementation( const EntityImplementation& );            //!< Copy constructor
    EntityImplementation& operator=( const EntityImplementation& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    I& This();
    //@}

    //! @name Member data
    //@{
    Controller& controller_;
protected:
    unsigned long id_;
    QString name_;
    //@}
};

}

#include "EntityImplementation.inl"

#endif // __EntityImplementation_h_
