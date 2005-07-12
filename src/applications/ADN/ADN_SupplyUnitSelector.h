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
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_SupplyUnitSelector.h $
// $Author: Ape $
// $Modtime: 25/04/05 18:10 $
// $Revision: 1 $
// $Workfile: ADN_SupplyUnitSelector.h $
//
// *****************************************************************************

#ifndef __ADN_SupplyUnitSelector_h_
#define __ADN_SupplyUnitSelector_h_

#include <qpushbutton.h>

#include "ADN_Gfx_ABC.h"
#include "ADN_Connector_Vector_ABC.h"


// =============================================================================
/** @class  ADN_SupplyUnitSelector
    @brief  ADN_SupplyUnitSelector
    @par    Using example
    @code
    ADN_SupplyUnitSelector;
    @endcode
*/
// Created: APE 2005-01-05
// =============================================================================
class ADN_SupplyUnitSelector
: public QPushButton
, public ADN_Gfx_ABC
{
    Q_OBJECT
    MT_COPYNOTALLOWED( ADN_SupplyUnitSelector )

public:
    //! @name Constructors/Destructor
    //@{
     ADN_SupplyUnitSelector( QWidget* pParent, const char* szName = 0 );
    ~ADN_SupplyUnitSelector();
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
    void* pData_;

private:
    //! @name Internal class
    //@{
    class ADN_SupplyUnitSelector_Connector
    : public ADN_Connector_Vector_ABC
    {
        MT_COPYNOTALLOWED( ADN_SupplyUnitSelector_Connector )
    public:
        ADN_SupplyUnitSelector_Connector( ADN_SupplyUnitSelector& gui );
        ~ADN_SupplyUnitSelector_Connector();

        void NotifySelected( void* pData );
        void SetDataPrivate( void* pData );

    private:
        ADN_SupplyUnitSelector& gui_;
    };

    friend class ADN_SupplyUnitSelector_Connector;
    //@}
};

#endif // __ADN_SupplyUnitSelector_h_
