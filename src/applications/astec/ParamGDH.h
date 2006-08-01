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
// $Archive: /MVW_v10/Build/SDK/Light2/src/ParamGDH.h $
// $Author: Ape $
// $Modtime: 23/09/04 17:36 $
// $Revision: 5 $
// $Workfile: ParamGDH.h $
//
// *****************************************************************************

#ifndef __ParamGDH_h_
#define __ParamGDH_h_

#ifdef __GNUG__
#   pragma interface
#endif

#include "ASN_Types.h"
#include "Param_ABC.h"

class QDateTimeEdit;
class QCheckBox;


// =============================================================================
// Created: APE 2004-03-18
// =============================================================================
class ParamGDH : public QHBox, public Param_ABC 
{
    Q_OBJECT;
    MT_COPYNOTALLOWED( ParamGDH );

public:
    //! @name Constructors/Destructor
    //@{
    ParamGDH( ASN1T_GDH& asnGDH, const std::string& strName, QWidget* pParent, OptionalParamFunctor_ABC* pOptional );
    ~ParamGDH();
    //@}

    //! @name Operations
    //@{
    void FillRemotePopupMenu( QPopupMenu& popupMenu, const ActionContext& context );
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

#endif // __ParamGDH_h_
