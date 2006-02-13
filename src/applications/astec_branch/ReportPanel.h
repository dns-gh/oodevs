// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: AGE 2005-04-21 $
// $Archive: /MVW_v10/Build/SDK/Light2/src/ReportPanel.h $
// $Author: Age $
// $Modtime: 21/04/05 16:06 $
// $Revision: 1 $
// $Workfile: ReportPanel.h $
//
// *****************************************************************************

#ifndef __ReportPanel_h_
#define __ReportPanel_h_

#include "InfoPanel_ABC.h"

class ReportListView;
class FireResultListView;
class ReportFilterOptions;

// =============================================================================
/** @class  ReportPanel
    @brief  ReportPanel
*/
// Created: AGE 2005-04-21
// =============================================================================
class ReportPanel : public InfoPanel_ABC
{
    MT_COPYNOTALLOWED( ReportPanel );
    friend class GLTool; // $$$$ AGE 2005-04-21: whatever

public:
    //! @name Constructors/Destructor
    //@{
    explicit ReportPanel( QWidget* pParent );
    virtual ~ReportPanel();
    //@}

private:
    //! @name Operations
    //@{
    virtual void OnUpdate        ();
    virtual void OnClearSelection();
    virtual void OnAgentUpdated  ( Agent& agent );
    virtual void OnAgentUpdated  ( Population& population );
    //@}

private:
    //! @name Member data
    //@{
    ReportFilterOptions* pFilterOptions_;
    ReportListView*      pReportListView_;
    FireResultListView*  pFireResultListView_;
    //@}
};

#endif // __ReportPanel_h_
