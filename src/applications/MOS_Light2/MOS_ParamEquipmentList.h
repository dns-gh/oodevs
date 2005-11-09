// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __MOS_ParamEquipmentList_h_
#define __MOS_ParamEquipmentList_h_

#ifdef __GNUG__
#   pragma interface
#endif

#include "MOS_ASN_Types.h"
#include "MOS_Param_ABC.h"

#include <qtable.h>

// =============================================================================
/** @class  MOS_ParamEquipmentList
    @brief  MOS_ParamEquipmentList
    @par    Using example
    @code
    MOS_ParamEquipmentList;
    @endcode
*/
// Created: SBO 2005-09-27
// =============================================================================
class MOS_ParamEquipmentList : public QTable, public MOS_Param_ABC
{
    Q_OBJECT
    MT_COPYNOTALLOWED( MOS_ParamEquipmentList );

public:
    //! @name Constructors/Destructor
    //@{
    explicit MOS_ParamEquipmentList( ASN1T_MaintenancePriorites& asnListEquipment, const std::string& strLabel, QWidget* pParent, bool bOptional );
    virtual ~MOS_ParamEquipmentList();
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

#endif // __MOS_ParamEquipmentList_h_
