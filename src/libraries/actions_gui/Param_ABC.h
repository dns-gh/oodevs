// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Param_ABC_h_
#define __Param_ABC_h_

#include "tools/Observer_ABC.h"
#include "clients_kernel/Drawable_ABC.h"
#include "clients_kernel/OrderParameter.h"
#include "clients_gui/RichGroupBox.h"
#include <boost/noncopyable.hpp>

namespace kernel
{
    class ActionController;
    class GlTools_ABC;
    class Viewport_ABC;
}

namespace gui
{
    class RichGroupBox;
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
                , private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit Param_ABC( const QString& name, bool optional = false );
    virtual ~Param_ABC();
    //@}

    //! @name Operations
    //@{
    virtual void RemoveFromController();
    virtual void RegisterIn( kernel::ActionController& controller );

    virtual void Draw( const geometry::Point2f& point, const kernel::Viewport_ABC& viewport, const kernel::GlTools_ABC& tools ) const;
    virtual QWidget* BuildInterface( QWidget* parent );
    virtual bool CheckValidity();
    virtual bool InternalCheckValidity() const;
    virtual void CommitTo( actions::ParameterContainer_ABC& ) const = 0;
    //@}

    //! @name Accessors
    //@{
    virtual QString GetName() const;
    virtual bool IsChecked() const;
    virtual bool IsOptional() const;
    void SetOptional( bool optional );
    //@}

    //! @name Helpers
    //@{
    void RegisterListener( Param_ABC& param );
    void NotifyChange();
    virtual void NotifyChanged( Param_ABC& param );
    //@}

protected:
    //! @name Member data
    //@{
    QString name_;
    ::gui::RichGroupBox* group_;
    bool optional_;
    //@}

private:
    //! @name Member data
    //@{
    kernel::ActionController* controller_;
    Param_ABC* listener_;
    //@}
};

    }
}

#endif // __Param_ABC_h_
