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

namespace kernel
{
    class TacticalLine_ABC;
    class CoordinateConverter_ABC;
}

class Lima;
class ActionParameter_ABC;

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
             LimaParameter( QObject* parent, const QString& name, const kernel::CoordinateConverter_ABC& converter, const Lima& lima );
    virtual ~LimaParameter();
    //@}

    //! @name Operations
    //@{
    virtual bool CheckValidity();
    virtual void BuildInterface( QWidget* parent );
    virtual void Draw( const geometry::Point2f& point, const kernel::Viewport_ABC& viewport, const kernel::GlTools_ABC& tools ) const;

    virtual void CommitTo( ActionParameterContainer_ABC& action ) const;
    //@}

public slots:
    //! @name Slots
    //@{
    void MenuItemValidated( int index );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    LimaParameter( const LimaParameter& );            //!< Copy constructor
    LimaParameter& operator=( const LimaParameter& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    virtual void NotifyContextMenu( const kernel::TacticalLine_ABC& entity, kernel::ContextMenu& menu );
    //@}

private:
    //! @name Member data
    //@{
    const kernel::CoordinateConverter_ABC& converter_;
    const Lima* lima_;
    QListBox* functions_;
    QLabel* entityLabel_;
    //@}
};

#endif // __LimaParameter_h_
