// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ActionParameter_ABC_h_
#define __ActionParameter_ABC_h_

#include "clients_kernel/Resolver.h"

namespace kernel
{
    class Displayer_ABC;
}

namespace xml
{
    class xostream;
}

// =============================================================================
/** @class  ActionParameter_ABC
    @brief  ActionParameter_ABC
*/
// Created: SBO 2007-03-12
// =============================================================================
class ActionParameter_ABC : public kernel::Resolver< ActionParameter_ABC >
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit ActionParameter_ABC( const QString& name );
    virtual ~ActionParameter_ABC();
    //@}

    //! @name Operations
    //@{
    virtual unsigned long GetId() const;
    virtual QString GetName() const;
    virtual void Display( kernel::Displayer_ABC& displayer ) const = 0;
    virtual void Draw( const geometry::Point2f& where, const kernel::Viewport_ABC& viewport, const kernel::GlTools_ABC& tools ) const;
    virtual void Serialize( xml::xostream& xos ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ActionParameter_ABC( const ActionParameter_ABC& );            //!< Copy constructor
    ActionParameter_ABC& operator=( const ActionParameter_ABC& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    //@}

private:
    //! @name Member data
    //@{
    static unsigned long idManager_;
    unsigned long id_;
    QString name_;
    //@}
};

#endif // __ActionParameter_ABC_h_
