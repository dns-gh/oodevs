// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#ifndef __GradientWidget_h_
#define __GradientWidget_h_

#include "GradientItem.h"

namespace gui
{
    class Gradient;
    class GradientButton;

// =============================================================================
/** @class  GradientWidget
    @brief  GradientWidget
*/
// Created: ABR 2014-10-03
// =============================================================================
class GradientWidget : public QWidget
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             GradientWidget( const QString& gradientEditorName,
                             bool disableState,
                             const GradientItem::T_Drawer& itemDrawer,
                             QWidget* parent = 0 );
    virtual ~GradientWidget();
    //@}

    //! @name Operations
    //@{
    void LoadGradient( const std::shared_ptr< Gradient >& gradient );
    //@}

signals:
    //! @name Signals
    //@{
    void GradientChanged();
    void FitToViewPortChanged( int );
    //@}

private:
    //! @name Member data
    //@{
    GradientButton* gradientButton_;
    //@}
};

} //! namespace gui

#endif // __GradientWidget_h_
