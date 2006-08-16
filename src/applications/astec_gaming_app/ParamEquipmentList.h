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

#include "astec_gaming/ASN_Types.h"
#include "Param_ABC.h"
#include "astec_kernel/Resolver.h"

#include <qtable.h>

class EquipmentType;

// =============================================================================
/** @class  ParamEquipmentList
    @brief  ParamEquipmentList
*/
// Created: SBO 2005-09-27
// =============================================================================
class ParamEquipmentList : public QTable
                         , public Param_ABC
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             ParamEquipmentList( QWidget* pParent, ASN1T_MaintenancePriorites& asnListEquipment, const std::string& strLabel, const Resolver< EquipmentType >& resolver );
    virtual ~ParamEquipmentList();
    //@}

    //! @name Operations
    //@{
    virtual void Commit();
    //@}

private slots:
    //! @name Slots
    //@{
    void OnEquipmentChanged( int row, int col );
    //@}

private:
    //! @name Types
    //@{
    typedef std::map< QString, const EquipmentType* > T_EquipmentTypes;
    //@}

private:
    //! @name Member data
    //@{
    ASN1T_MaintenancePriorites* pAsnEquipmentList_;
    QStringList                 equipmentList_;
    T_EquipmentTypes            equipmentTypes_;
    //@}
};

#endif // __ParamEquipmentList_h_
