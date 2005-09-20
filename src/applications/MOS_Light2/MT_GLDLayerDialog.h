// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: APE 2004-06-23 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MT_GLDLayerDialog.h $
// $Author: Ape $
// $Modtime: 29/06/04 11:21 $
// $Revision: 1 $
// $Workfile: MT_GLDLayerDialog.h $
//
// *****************************************************************************

#ifndef __MT_GLDLayerDialog_h_
#define __MT_GLDLayerDialog_h_

#ifdef __GNUG__
#   pragma interface
#endif

#include <qdialog.h>

class MT_GLDLayer;
class MT_Slider;


// =============================================================================
/** @class  MT_GLDLayerDialog
    @brief  MT_GLDLayerDialog
    @par    Using example
    @code
    MT_GLDLayerDialog;
    @endcode
*/
// Created: APE 2004-06-23
// =============================================================================
class MT_GLDLayerDialog : public QDialog
{
    Q_OBJECT;
    MT_COPYNOTALLOWED( MT_GLDLayerDialog );

public:
    //! @name Constructors/Destructor
    //@{
     MT_GLDLayerDialog( QWidget* pParent );
    ~MT_GLDLayerDialog();
    //@}

    //! @name Operations
    //@{
    void SetLayer( MT_GLDLayer* pLayer );
    //@}

private slots:
    //! @name Helpers
    //@{
    void OnApply();
    //@}

signals:
    void LayerEdited( MT_GLDLayer& layer );

private:
    //! @name Member data
    //@{
    MT_GLDLayer* pLayer_;

    QLineEdit* pNameEdit_;
    MT_Slider* pOpacitySlider_;

    QPushButton* pOkButton_;
    QPushButton* pApplyButton_;
    QPushButton* pCancelButton_;
    //@}
};

#   include "MT_GLDLayerDialog.inl"

#endif // __MT_GLDLayerDialog_h_
