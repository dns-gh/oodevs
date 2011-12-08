// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ADN_Funeral_GUI_h_
#define __ADN_Funeral_GUI_h_

#include "ADN_GUI_ABC.h"

class ADN_Funeral_Data;
class ADN_FuneralPackagingResources_GUI;

// =============================================================================
/** @class  ADN_Funeral_GUI
    @brief  ADN_Funeral_GUI
*/
// Created: APE 2004-12-06
// =============================================================================
class ADN_Funeral_GUI : public ADN_GUI_ABC
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
    explicit ADN_Funeral_GUI( ADN_Funeral_Data& data );
    virtual ~ADN_Funeral_GUI();
    //@}

    //! @name Operations
    //@{
    void Build();
    void SwapResource( int offsetRow );
    //@}

    private slots:
        //! @name Helpers
        //@{
        void OnButtonUp() { SwapResource( -1 ); }
        void OnButtonDown() { SwapResource( 1 ); }
        //@}

private:
    //! @name Member data
    //@{
    ADN_Funeral_Data& data_;
    ADN_FuneralPackagingResources_GUI* resourceTable_;
    //@}
};


#endif // __ADN_Funeral_GUI_h_
