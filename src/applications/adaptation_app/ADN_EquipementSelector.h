// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ADN_EquipementSelector_h_
#define __ADN_EquipementSelector_h_

#include "ADN_Gfx_ABC.h"
#include "ADN_Connector_Vector_ABC.h"
#include <boost/noncopyable.hpp>

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
class ADN_EquipementSelector : public QPushButton
                             , public ADN_Gfx_ABC
                             , private boost::noncopyable
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
    explicit ADN_EquipementSelector( QWidget* pParent, const char* szName = 0 );
    virtual ~ADN_EquipementSelector();
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
    class ADN_EquipementSelector_Connector : public ADN_Connector_Vector_ABC
                                           , private boost::noncopyable
    {
    public:
        ADN_EquipementSelector_Connector( ADN_EquipementSelector& gui );
        virtual ~ADN_EquipementSelector_Connector();

        void NotifySelected( void* pData );
        void SetDataPrivate( void* pData );

    private:
        ADN_EquipementSelector& gui_;
    };

    friend class ADN_EquipementSelector_Connector;
    //@}
};

#endif // __ADN_EquipementSelector_h_
