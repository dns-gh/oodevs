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
#include "clients_kernel/ContextMenuObserver_ABC.h"

namespace gui
{
    class ParametersLayer;
    class RichLabel;
    class TerrainObjectProxy;
}

namespace kernel
{
    class ActionController;
}

namespace actions
{
    class Parameter_ABC;

namespace gui
{
    class ParametersLayer;
    class RichLabel;
// =============================================================================
/** @class  ParamUrbanBlock
    @brief  ParamUrbanBlock
*/
// Created: MGD 2009-11-03
// =============================================================================
class ParamUrbanBlock : public QObject
                      , public Param_ABC
                      , public kernel::ContextMenuObserver_ABC< ::gui::TerrainObjectProxy >
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
    ParamUrbanBlock( QObject* parent, const kernel::OrderParameter& parameter, ::gui::ParametersLayer& layer );
    virtual ~ParamUrbanBlock();
    //@}

    //! @name Operations
    //@{
    virtual void Draw( const geometry::Point2f& point, const kernel::Viewport_ABC& extent, const kernel::GlTools_ABC& tools ) const;
    virtual void BuildInterface( QWidget* parent );
    virtual bool CheckValidity();

    virtual void CommitTo( actions::ParameterContainer_ABC& action ) const;

    virtual void NotifyContextMenu( const ::gui::TerrainObjectProxy& object, kernel::ContextMenu& menu );
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

    private slots:
    //! @name Slots
    //@{
    virtual void MenuItemValidated();
    //@}

private:
    //! @name Member data
    //@{
    kernel::OrderParameter parameter_;
    ::gui::ParametersLayer& layer_;
    ::gui::RichLabel* pLabel_;
    QLabel* pBlockLabel_;
    const ::gui::TerrainObjectProxy* potential_;
    const ::gui::TerrainObjectProxy* selected_;
    //@}
};

} //namespace gui

} //namespace action

#endif // __ParamUrbanBlock_h_
