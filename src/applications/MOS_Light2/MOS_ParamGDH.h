// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: APE 2004-03-18 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MOS_ParamGDH.h $
// $Author: Ape $
// $Modtime: 23/09/04 17:36 $
// $Revision: 5 $
// $Workfile: MOS_ParamGDH.h $
//
// *****************************************************************************

#ifndef __MOS_ParamGDH_h_
#define __MOS_ParamGDH_h_

#ifdef __GNUG__
#   pragma interface
#endif

#include "MOS_ASN_Types.h"
#include "MOS_Param_ABC.h"

class QDateTimeEdit;
class QCheckBox;


// =============================================================================
// Created: APE 2004-03-18
// =============================================================================
class MOS_ParamGDH : public QHBox, public MOS_Param_ABC 
{
    Q_OBJECT;
    MT_COPYNOTALLOWED( MOS_ParamGDH );

public:
    //! @name Constructors/Destructor
    //@{
    MOS_ParamGDH( ASN1T_GDH& asnGDH, const std::string& strName, QWidget* pParent );
    ~MOS_ParamGDH();
    //@}

    //! @name Operations
    //@{
    void FillRemotePopupMenu( QPopupMenu& popupMenu, const MOS_ActionContext& context );
    bool CheckValidity();
    void WriteMsg( std::stringstream& strMsg );
    //@}

private slots:
    void OnCheckboxToogled( bool b );

private:
    std::string strName_;
    ASN1T_GDH& asnGDH_;

    QDateTimeEdit* pDateTimeEdit_;
    QCheckBox*     pCheckbox_;
};

#endif // __MOS_ParamGDH_h_
