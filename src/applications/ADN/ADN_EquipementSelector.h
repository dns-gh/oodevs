// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: APE 2005-01-05 $
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_EquipementSelector.h $
// $Author: Ape $
// $Modtime: 25/04/05 17:44 $
// $Revision: 3 $
// $Workfile: ADN_EquipementSelector.h $
//
// *****************************************************************************

#ifndef __ADN_EquipementSelector_h_
#define __ADN_EquipementSelector_h_

#include <qpushbutton.h>

#include "ADN_Gfx_ABC.h"
#include "ADN_Connector_Vector_ABC.h"


// =============================================================================
/** @class  ADN_EquipementSelector
    @brief  ADN_EquipementSelector
    @par    Using example
    @code
    ADN_EquipementSelector;
    @endcode
*/
// Created: APE 2005-01-05
// =============================================================================
class ADN_EquipementSelector
: public QPushButton
, public ADN_Gfx_ABC
{
    Q_OBJECT
    MT_COPYNOTALLOWED( ADN_EquipementSelector )

public:
    //! @name Constructors/Destructor
    //@{
     ADN_EquipementSelector( QWidget* pParent, const char* szName = 0 );
    ~ADN_EquipementSelector();
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

private slots:
    //! @name Helpers
    //@{
    void OnButtonPressed();

    void ItemRemoved( void* );
    void SetItem( void* );
    //@}

private:
    QObject* pConnected_;
    void* pData_;

private:
    //! @name Internal class
    //@{
    class ADN_EquipementSelector_Connector
    : public ADN_Connector_Vector_ABC
    {
        MT_COPYNOTALLOWED( ADN_EquipementSelector_Connector )
    public:
        ADN_EquipementSelector_Connector( ADN_EquipementSelector& gui );
        ~ADN_EquipementSelector_Connector();

        void NotifySelected( void* pData );
        void SetDataPrivate( void* pData );

    private:
        ADN_EquipementSelector& gui_;
    };

    friend class ADN_EquipementSelector_Connector;
    //@}
};

#endif // __ADN_EquipementSelector_h_
