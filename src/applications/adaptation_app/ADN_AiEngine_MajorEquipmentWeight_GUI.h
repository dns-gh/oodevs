// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: AGN 2004-06-15 $
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_AiEngine_MajorEquipmentWeight_GUI.h $
// $Author: Ape $
// $Modtime: 19/01/05 18:29 $
// $Revision: 2 $
// $Workfile: ADN_AiEngine_MajorEquipmentWeight_GUI.h $
//
// *****************************************************************************

#ifndef __ADN_AiEngine_MajorEquipmentWeight_GUI_h_
#define __ADN_AiEngine_MajorEquipmentWeight_GUI_h_

#include "ADN_EditLine.h"

// =============================================================================
/** @class  ADN_AiEngine_MajorEquipmentWeight_GUI
    @brief  ADN_AiEngine_MajorEquipmentWeight_GUI
    @par    Using example
    @code
    ADN_AiEngine_MajorEquipmentWeight_GUI;
    @endcode
*/
// Created: AGN 2004-06-15
// =============================================================================
class ADN_AiEngine_MajorEquipmentWeight_GUI
: public ADN_EditLine_Double
{

public:
    //! @name Constructors/Destructor
    //@{
             ADN_AiEngine_MajorEquipmentWeight_GUI( QWidget* pParent, QLineEdit* pMinorEditLine );
    virtual ~ADN_AiEngine_MajorEquipmentWeight_GUI();
    //@}

    //! @name Operations
    //@{
    //@}

    //! @name Accessors
    //@{
    //@}

    //! @name Modifiers
    //@{
    //@}

    //! @name Operators
    //@{
    //@}
protected:
    virtual void TextChanged(const QString& );

private:
    //! @name Copy/Assignement
    //@{
    ADN_AiEngine_MajorEquipmentWeight_GUI( const ADN_AiEngine_MajorEquipmentWeight_GUI& );            //!< Copy constructor
    ADN_AiEngine_MajorEquipmentWeight_GUI& operator=( const ADN_AiEngine_MajorEquipmentWeight_GUI& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    //@}

private:
    //! @name Member data
    //@{
    QLineEdit* pMinorEditLine_;
    //@}
};


#endif // __ADN_AiEngine_MajorEquipmentWeight_GUI_h_
