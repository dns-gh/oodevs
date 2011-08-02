// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __ParamLocationComposite_h_
#define __ParamLocationComposite_h_

#include "Param_ABC.h"
#include "clients_kernel/OrderParameter.h"

class Q3WidgetStack;

namespace actions
{
    class Parameter_ABC;

    namespace gui
    {
        class InterfaceBuilder_ABC;
// =============================================================================
/** @class  ParamLocationComposite
    @brief  ParamLocationComposite
*/
// Created: LDC 2010-08-18
// =============================================================================
class ParamLocationComposite: public Param_ABC
{
    typedef actions::gui::Param_ABC* (Functor)( const kernel::OrderParameter& parameter );

public:
    //! @name Constructors/Destructor
    //@{
    ParamLocationComposite( const kernel::OrderParameter& parameter, const InterfaceBuilder_ABC& builder );
    virtual ~ParamLocationComposite();
    //@}

    //! @name Operations
    //@{
    virtual void RemoveFromController();
    virtual void RegisterIn( kernel::ActionController& controller );

    virtual void Draw( const geometry::Point2f& point, const kernel::Viewport_ABC& viewport, const kernel::GlTools_ABC& tools ) const;
    virtual QWidget* BuildInterface( QWidget* parent );
    virtual bool CheckValidity();
    virtual void CommitTo( actions::ParameterContainer_ABC& container ) const;
    //@}

protected:
    //! @name Helpers
    //@{
    virtual void NotifyChanged( Param_ABC& param );
    //@}

private:
    //! @name Types
    //@{
    typedef std::vector<Param_ABC*>  T_Params;
    typedef T_Params::const_iterator CIT_Params;
	virtual bool IsOptional() const;
    //@}

private:
    //! @name Member data
    //@{
    const kernel::OrderParameter& parameter_;
    std::vector<Param_ABC*> params_;
    std::vector<QWidget*> widgets_;
    Q3WidgetStack* stack_;
    Param_ABC* selectedParam_;
    //@}
};

    }
}

#endif // __ParamLocationComposite_h_
