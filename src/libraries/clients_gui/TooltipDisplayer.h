// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#ifndef __TooltipDisplayer_h_
#define __TooltipDisplayer_h_

#include "NoLinkDisplayer.h"
#include "Tooltip.h"
#include "clients_kernel/Styles.h"

namespace gui
{
    class Tooltip;

// =============================================================================
/** @class  TooltipDisplayer
    @brief  Tooltip displayer
*/
// Created: MCO 2014-10-23
// =============================================================================
class TooltipDisplayer : public NoLinkDisplayer
                       , public tools::Caller< QColor >
                       , public tools::Caller< QFont >
                       , public tools::Caller< kernel::Styles::Style >
                       , public tools::Caller< E_OperationalStatus >
                       , public tools::Caller< E_ForceRatioStatus >
                       , public tools::Caller< E_MeetingEngagementStatus >
{
public:
             TooltipDisplayer();
    virtual ~TooltipDisplayer();

private:
    virtual void Call( const QColor& value );
    virtual void Call( const QFont& font );
    virtual void Call( const kernel::Styles::Style& value );
    virtual void Call( const E_OperationalStatus& value );
    virtual void Call( const E_ForceRatioStatus& value );
    virtual void Call( const E_MeetingEngagementStatus& value );

    virtual Displayer_ABC& SubItem( const QString& name );
    virtual void StartDisplay();
    virtual void DisplayFormatted( const QString& formatted );
    virtual void EndDisplay();

    void Collect( Tooltip::T_Messages& messages ) const;

    virtual void DirtyImage() = 0;

protected:
    Tooltip GetTooltip();

private:
    QString currentItem_;
    QColor color_;
    QFont font_;
    QString message_;
    Tooltip::T_Messages messages_;
};

}

#endif // __TooltipDisplayer_h_
