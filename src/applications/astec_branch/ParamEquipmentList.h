// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ParamEquipmentList_h_
#define __ParamEquipmentList_h_

#ifdef __GNUG__
#   pragma interface
#endif

#include "ASN_Types.h"
#include "Param_ABC.h"

#include <qtable.h>

// =============================================================================
/** @class  ParamEquipmentList
    @brief  ParamEquipmentList
    @par    Using example
    @code
    ParamEquipmentList;
    @endcode
*/
// Created: SBO 2005-09-27
// =============================================================================
class ParamEquipmentList : public QTable, public Param_ABC
{
    Q_OBJECT
    MT_COPYNOTALLOWED( ParamEquipmentList );

public:
    //! @name Constructors/Destructor
    //@{
    explicit ParamEquipmentList( ASN1T_MaintenancePriorites& asnListEquipment, const std::string& strLabel, QWidget* pParent, bool bOptional );
    virtual ~ParamEquipmentList();
    //@}

    //! @name Operations
    //@{
    virtual void WriteMsg     ( std::stringstream& strMsg );
    virtual bool CheckValidity();
    //@}

private slots:
    //! @name Slots
    //@{
    void OnEquipmentChanged( int nRow, int nCol );
    //@}

private:
    //! @name Member data
    //@{
    ASN1T_MaintenancePriorites* pAsnEquipmentList_;
    QStringList*                pEquipmentsStringList_;
    //@}
};

#endif // __ParamEquipmentList_h_
