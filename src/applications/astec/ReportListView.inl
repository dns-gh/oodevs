// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: APE 2004-03-10 $
// $Archive: /MVW_v10/Build/SDK/Light2/src/ReportListView.inl $
// $Author: Ape $
// $Modtime: 27/09/04 16:44 $
// $Revision: 2 $
// $Workfile: ReportListView.inl $
//
// *****************************************************************************


// -----------------------------------------------------------------------------
// Name: ReportListView::GetItemValue
// Created: APE 2004-09-27
// -----------------------------------------------------------------------------
inline
Report_ABC& ReportListView::GetItemValue( QListViewItem& item )
{
    if( item.rtti() == eRichItem )
        return *((T_RichReportItem&)item).GetValue();
    else
        return *((T_ReportItem&)item).GetValue();
}
