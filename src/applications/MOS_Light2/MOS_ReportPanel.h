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
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MOS_ReportPanel.h $
// $Author: Age $
// $Modtime: 21/04/05 16:06 $
// $Revision: 1 $
// $Workfile: MOS_ReportPanel.h $
//
// *****************************************************************************

#ifndef __MOS_ReportPanel_h_
#define __MOS_ReportPanel_h_

#include "MOS_InfoPanel_ABC.h"

class MOS_ReportListView;
class MOS_FireResultListView;

// =============================================================================
/** @class  MOS_ReportPanel
    @brief  MOS_ReportPanel
*/
// Created: AGE 2005-04-21
// =============================================================================
class MOS_ReportPanel : public MOS_InfoPanel_ABC
{
    friend class MOS_GLTool; // $$$$ AGE 2005-04-21: whatever
public:
    //! @name Constructors/Destructor
    //@{
             MOS_ReportPanel( QWidget* pParent );
    virtual ~MOS_ReportPanel();
    //@}

private:
    virtual void OnUpdate();
    void OnAgentUpdated( MOS_Agent& agent );

    //! @name Copy/Assignement
    //@{
    MOS_ReportPanel( const MOS_ReportPanel& );            //!< Copy constructor
    MOS_ReportPanel& operator=( const MOS_ReportPanel& ); //!< Assignement operator
    //@}

private:
    //! @name Member data
    //@{
    MOS_ReportListView*     pReportListView_;
    MOS_FireResultListView* pFireResultListView_;
    //@}
};

#endif // __MOS_ReportPanel_h_
