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
#include "clients_kernel/ContextMenuObserver_ABC.h"
#include <Qt3Support/q3listbox.h>

namespace kernel
{
    class TacticalLine_ABC;
    class CoordinateConverter_ABC;
}

namespace actions
{
    namespace gui
    {
        class ParamDateTime;

// =============================================================================
/** @class  LimaParameter
    @brief  LimaParameter
*/
// Created: SBO 2007-05-02
// =============================================================================
class LimaParameter : public QObject
                    , public Param_ABC
                    , public kernel::ContextMenuObserver_ABC< kernel::TacticalLine_ABC >
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             LimaParameter( QObject* parent, const QString& name, const kernel::CoordinateConverter_ABC& converter, const QDateTime& currentDate, const kernel::TacticalLine_ABC& line );
             LimaParameter( QObject* parent, const kernel::OrderParameter& parameter, const kernel::CoordinateConverter_ABC& converter, const QDateTime& currentDate );
    virtual ~LimaParameter();
    //@}

    //! @name Operations
    //@{
    virtual bool CheckValidity();
    virtual QWidget* BuildInterface( QWidget* parent );
    virtual void Draw( const geometry::Point2f& point, const kernel::Viewport_ABC& viewport, const kernel::GlTools_ABC& tools ) const;

    virtual void CommitTo( actions::ParameterContainer_ABC& action ) const;
    //@}

public slots:
    //! @name Slots
    //@{
    void MenuItemValidated( int index );
    //@}

private:
    //! @name Helpers
    //@{
    virtual void NotifyContextMenu( const kernel::TacticalLine_ABC& entity, kernel::ContextMenu& menu );
    //@}

private:
    //! @name Member data
    //@{
    const kernel::CoordinateConverter_ABC& converter_;
    const QDateTime currentDate_;
    const kernel::TacticalLine_ABC* line_;
    Q3ListBox* functions_;
    QLabel* entityLabel_;
    ParamDateTime* schedule_;
    //@}
};
}
}

#endif // __LimaParameter_h_
