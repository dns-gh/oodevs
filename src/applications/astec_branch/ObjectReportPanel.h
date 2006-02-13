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
// $Archive: /MVW_v10/Build/SDK/Light2/src/ObjectReportPanel.h $
// $Author: Sbo $
// $Modtime: 5/04/05 17:11 $
// $Revision: 3 $
// $Workfile: ObjectReportPanel.h $
//
// *****************************************************************************

#ifndef __ObjectReportPanel_h_
#define __ObjectReportPanel_h_

#ifdef __GNUG__
#   pragma interface
#endif

#include "InfoPanel_ABC.h"

class Object_ABC;
class FireResultListView;

// =============================================================================
/** @class  ObjectReportPanel
    @brief  ObjectReportPanel
*/
// Created: SBO 2005-09-08
// =============================================================================
class ObjectReportPanel : public InfoPanel_ABC
{
    Q_OBJECT;
    MT_COPYNOTALLOWED( ObjectReportPanel );

public:
    //! @name Constructors/Destructor
    //@{
    explicit ObjectReportPanel( QWidget* pParent );
    virtual ~ObjectReportPanel();
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
    virtual void OnObjectUpdated( Object_ABC& object );
    //@}
private:
    //! @name Member data
    //@{
    FireResultListView* pFireResultListView_;
    //@}
};

#endif // __ObjectReportPanel_h_
