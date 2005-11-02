// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __MOS_LogisticToolbar_h_
#define __MOS_LogisticToolbar_h_

#ifdef __GNUG__
#   pragma interface
#endif

#include "MOS_Options.h"

// =============================================================================
/** @class  MOS_LogisticToolbar
    @brief  MOS_LogisticToolbar
    @par    Using example
    @code
    MOS_LogisticToolbar;
    @endcode
*/
// Created: HME 2005-11-02
// =============================================================================
class MOS_LogisticToolbar : public QToolBar
{
    Q_OBJECT;
    MT_COPYNOTALLOWED( MOS_LogisticToolbar );

public:
    //! @name Constructors/Destructor
    //@{
    MOS_LogisticToolbar( QMainWindow* pParent );
    ~MOS_LogisticToolbar();
    //@}

private slots:
    void OnAutoSetLogisticLinks();
    void OnLogisticLinks( float rValue );
    void OnMissingLogisticLinks( float rValue  );

private:
    void FillButton( MT_ToolListButton& button, MOS_Options::E_State nState );


private:

    QToolButton* pAutoSetLogisticLinksButton_;

};



#include "MOS_LogisticToolbar.inl"

#endif // __MOS_LogisticToolbar_h_
