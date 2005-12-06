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
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MOS_ReportListView.inl $
// $Author: Ape $
// $Modtime: 27/09/04 16:44 $
// $Revision: 2 $
// $Workfile: MOS_ReportListView.inl $
//
// *****************************************************************************


// -----------------------------------------------------------------------------
// Name: MOS_ReportListView::GetItemValue
// Created: APE 2004-09-27
// -----------------------------------------------------------------------------
inline
MOS_Report_ABC& MOS_ReportListView::GetItemValue( QListViewItem& item )
{
    if( item.rtti() == eRichItem )
        return *((T_RichReportItem&)item).GetValue();
    else
        return *((T_ReportItem&)item).GetValue();
}
