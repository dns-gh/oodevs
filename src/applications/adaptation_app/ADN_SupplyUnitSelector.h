// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ADN_SupplyUnitSelector_h_
#define __ADN_SupplyUnitSelector_h_

#include <QtGui/qpushbutton.h>
#include "ADN_Gfx_ABC.h"
#include "ADN_Connector_Vector_ABC.h"
#include <boost/noncopyable.hpp>

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
class ADN_SupplyUnitSelector : public QPushButton
                             , public ADN_Gfx_ABC
                             , private boost::noncopyable
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
    explicit ADN_SupplyUnitSelector( QWidget* pParent, const char* szName = 0 );
    virtual ~ADN_SupplyUnitSelector();
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
    class ADN_SupplyUnitSelector_Connector : public ADN_Connector_Vector_ABC
                                           , private boost::noncopyable
    {
    public:
        ADN_SupplyUnitSelector_Connector( ADN_SupplyUnitSelector& gui );
        virtual ~ADN_SupplyUnitSelector_Connector();

        void NotifySelected( void* pData );
        void SetDataPrivate( void* pData );

    private:
        ADN_SupplyUnitSelector& gui_;
    };

    friend class ADN_SupplyUnitSelector_Connector;
    //@}
};

#endif // __ADN_SupplyUnitSelector_h_
