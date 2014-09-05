// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __LimaParameter_h_
#define __LimaParameter_h_

#include "Param_ABC.h"
#include "ListParameter.h"
#include "clients_kernel/ContextMenuObserver_ABC.h"
#include "clients_kernel/Types.h"
#include <tools/ElementObserver_ABC.h>

#pragma warning( push, 0 )
#include <QtGui/qlistwidget.h>
#pragma warning( pop )

namespace kernel
{
    class TacticalLine_ABC;
    class CoordinateConverter_ABC;
    class Controller;
}

namespace actions
{
    namespace gui
    {
        class ParamDateTime;
        class ParamObjectKnowledge;
        class InterfaceBuilder_ABC;

// =============================================================================
/** @class  LimaParameter
    @brief  LimaParameter
*/
// Created: SBO 2007-05-02
// =============================================================================
class LimaParameter : public Param_ABC
                    , public kernel::ContextMenuObserver_ABC< kernel::TacticalLine_ABC >
                    , public tools::ElementObserver_ABC< kernel::TacticalLine_ABC >
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             LimaParameter( const InterfaceBuilder_ABC& builder, const kernel::OrderParameter& parameter );
    virtual ~LimaParameter();
    //@}

    //! @name Operations
    //@{
    virtual QWidget* BuildInterface( const QString& objectName, QWidget* parent );
    virtual void Draw( const geometry::Point2f& point, const ::gui::Viewport_ABC& viewport, ::gui::GlTools_ABC& tools ) const;
    virtual void CommitTo( actions::ParameterContainer_ABC& action ) const;
    virtual void Visit( const actions::parameters::Lima& param );
    virtual void Visit( const actions::parameters::Location& param );
    virtual void Visit( const actions::parameters::DateTime& param );
    virtual void Visit( const actions::Parameter_ABC& param );
    virtual void SetEntity( const kernel::Entity_ABC* entity );
    virtual bool HasParameter( const Param_ABC& parameter ) const;
    //@}

public slots:
    //! @name Slots
    //@{
    void OnMenuClick( QAction* action );
    //@}

private:
    //! @name Helpers
    //@{
    bool HasTacticalLine() const;
    bool HasNewLima() const;
    virtual bool InternalCheckValidity() const;
    virtual void NotifyContextMenu( const kernel::TacticalLine_ABC& entity, kernel::ContextMenu& menu );
    virtual void NotifyUpdated( const kernel::TacticalLine_ABC& entity );
    virtual void NotifyDeleted( const kernel::TacticalLine_ABC& entity );
    virtual void CreateInternalMenu( kernel::ContextMenu& menu );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controller& controller_;
    const kernel::CoordinateConverter_ABC& converter_;
    tools::Resolver< kernel::TacticalLine_ABC >* resolver_;
    const QDateTime currentDate_;
    const kernel::TacticalLine_ABC* clickedLine_;
    const kernel::TacticalLine_ABC* selectedLine_;
    QListWidget* functions_;
    QLabel* entityLabel_;
    ParamDateTime* schedule_;
    ListParameter< ParamObjectKnowledge >* objects_;
    T_Actions actions_;
    T_PointVector newPoints_;
    const InterfaceBuilder_ABC& builder_;
    //@}
};
}
}

#endif // __LimaParameter_h_
