// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: SBO 2005-09-08 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MOS_ObjectReportPanel.h $
// $Author: Sbo $
// $Modtime: 5/04/05 17:11 $
// $Revision: 3 $
// $Workfile: MOS_ObjectReportPanel.h $
//
// *****************************************************************************

#ifndef __MOS_ObjectReportPanel_h_
#define __MOS_ObjectReportPanel_h_

#ifdef __GNUG__
#   pragma interface
#endif

#include "MOS_InfoPanel_ABC.h"

class MOS_Object_ABC;
class MOS_FireResultListView;

// =============================================================================
/** @class  MOS_ObjectReportPanel
    @brief  MOS_ObjectReportPanel
*/
// Created: SBO 2005-09-08
// =============================================================================
class MOS_ObjectReportPanel : public MOS_InfoPanel_ABC
{
    Q_OBJECT;
    MT_COPYNOTALLOWED( MOS_ObjectReportPanel );

public:
    //! @name Constructors/Destructor
    //@{
    explicit MOS_ObjectReportPanel( QWidget* pParent );
    virtual ~MOS_ObjectReportPanel();
    //@}

private:
    //! @name Operations
    //@{
    virtual void OnClearSelection();
    //@}

private slots:
    //! @name Helpers
    //@{
    virtual void OnUpdate();
    virtual void OnObjectUpdated( MOS_Object_ABC& object );
    //@}
private:
    //! @name Member data
    //@{
    MOS_FireResultListView* pFireResultListView_;
    //@}
};

#endif // __MOS_ObjectReportPanel_h_
