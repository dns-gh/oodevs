// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ParamUrbanBlock_h_
#define __ParamUrbanBlock_h_

#include "Param_ABC.h"
#include "clients_kernel/OrderParameter.h"

namespace gui
{
    class ParametersLayer;
    class RichLabel;
}

namespace actions
{
    class Parameter_ABC;

namespace gui
{

// =============================================================================
/** @class  ParamUrbanBlock
    @brief  ParamUrbanBlock
*/
// Created: MGD 2009-11-03
// =============================================================================
class ParamUrbanBlock : public Param_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
    ParamUrbanBlock( const kernel::OrderParameter& parameter, ::gui::ParametersLayer& layer );
    virtual ~ParamUrbanBlock();
    //@}

    //! @name Operations
    //@{
    virtual void RemoveFromController();
    virtual void RegisterIn( kernel::ActionController& controller );
    virtual void Draw( const geometry::Point2f& point, const kernel::Viewport_ABC& extent, const kernel::GlTools_ABC& tools ) const;
    virtual void BuildInterface( QWidget* parent );
    virtual bool CheckValidity();

    virtual void CommitTo( actions::ParameterContainer_ABC& action ) const;
    //@} 

private:
    //! @name Copy/Assignment
    //@{
    ParamUrbanBlock( const ParamUrbanBlock& );
    ParamUrbanBlock& operator=( const ParamUrbanBlock& );
    //@}

    //! @name Helpers
    //@{
    virtual bool IsOptional() const;
    //@}

private:
    //! @name Member data
    //@{
    kernel::OrderParameter parameter_;
    ::gui::ParametersLayer& layer_;
    ::gui::RichLabel* pLabel_;
    QLabel* pShapeLabel_;
    kernel::ActionController* controller_; // $$$$ AGE 2006-04-03: sucks
    //@}
};

} //namespace gui

} //namespace action

#endif // __ParamUrbanBlock_h_
