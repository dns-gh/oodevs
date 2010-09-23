// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __Param_ABC_h_
#define __Param_ABC_h_

#include "tools/Observer_ABC.h"
#include "clients_kernel/Drawable_ABC.h"
#include "clients_kernel/OrderParameter.h"

namespace kernel
{
    class ActionController;
    class GlTools_ABC;
    class Viewport_ABC;
}

namespace actions
{
    class ParameterContainer_ABC;

    namespace gui
    {

// =============================================================================
/** @class  Param_ABC
    @brief  Param_ABC
*/
// Created: APE 2004-04-20
// =============================================================================
class Param_ABC : public tools::Observer_ABC
                , public kernel::Drawable_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit Param_ABC( const QString& name );
    virtual ~Param_ABC();
    //@}

    //! @name Operations
    //@{
    virtual void RemoveFromController();
    virtual void RegisterIn( kernel::ActionController& controller );

    virtual void Draw( const geometry::Point2f& point, const kernel::Viewport_ABC& viewport, const kernel::GlTools_ABC& tools ) const;
    virtual QWidget* BuildInterface( QWidget* parent ) = 0;
    virtual bool CheckValidity();
    virtual void CommitTo( actions::ParameterContainer_ABC& ) const = 0;
    //@}

    //! @name Accessors
    //@{
    virtual QString GetName() const;
    virtual bool IsOptional() const;
    //@}

    //! @name Helpers
    //@{
    void RegisterListener( Param_ABC& param );
    void NotifyChange();
    virtual void NotifyChanged( Param_ABC& param );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    Param_ABC( const Param_ABC& );
    Param_ABC& operator=( const Param_ABC& );
    //@}

private:
    //! @name Member data
    //@{
    QString name_;
    kernel::ActionController* controller_;
    Param_ABC* listener_;
    //@}
};

    }
}

#endif // __Param_ABC_h_
