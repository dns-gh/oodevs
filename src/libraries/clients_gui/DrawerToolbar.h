// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __DrawerToolbar_h_
#define __DrawerToolbar_h_

namespace kernel
{
    class GlTools_ABC;
}

namespace gui
{
    class ExclusiveEventStrategy;
    class DrawerLayer;

// =============================================================================
/** @class  DrawerToolbar
    @brief  DrawerToolbar. Entry point of the drawer system
*/
// Created: AGE 2006-09-04
// =============================================================================
class DrawerToolbar : public QToolBar
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             DrawerToolbar( QMainWindow* mainWindow, ExclusiveEventStrategy& strategy, DrawerLayer& layer, kernel::GlTools_ABC& tools );
    virtual ~DrawerToolbar();
    //@}

private slots:
    //! @name Slots
    //@{
    void OnShow( bool );
    void OnDraw( bool );
    //@}

private:
    //! @name Copy/Assignement
    //@{
    DrawerToolbar( const DrawerToolbar& );            //!< Copy constructor
    DrawerToolbar& operator=( const DrawerToolbar& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    //@}

private:
    //! @name Member data
    //@{
    ExclusiveEventStrategy& strategy_;
    DrawerLayer& layer_;
    QDockWindow* dock_;
    QToolButton* show_;
    QToolButton* draw_;
    //@}
};

}

#endif // __DrawerToolbar_h_
