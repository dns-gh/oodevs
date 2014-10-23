// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#ifndef __gui_InformationToolTip_h_
#define __gui_InformationToolTip_h_

#include "TooltipDisplayer.h"

namespace gui
{
// =============================================================================
/** @class  InformationToolTip
    @brief  InformationToolTip
*/
// Created: NPT 2013-02-11
// =============================================================================
class InformationToolTip : public QDialog
                         , public TooltipDisplayer
{
    Q_OBJECT

public:
             InformationToolTip();
    virtual ~InformationToolTip();

    void Draw();
    void Hide();

private:
    virtual void paintEvent( QPaintEvent * event );
    virtual void DirtyImage();

    QPoint ComputePosition() const;

private:
    QImage image_;
};

}
#endif // __gui_InformationToolTip_h_
