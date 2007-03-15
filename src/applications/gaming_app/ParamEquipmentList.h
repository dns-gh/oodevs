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

#include "Param_ABC.h"
#include "clients_kernel/Resolver.h"

namespace kernel
{
    class EquipmentType;
}

class QTable;

// =============================================================================
/** @class  ParamEquipmentList
    @brief  ParamEquipmentList
*/
// Created: SBO 2005-09-27
// =============================================================================
class ParamEquipmentList : public QObject, public Param_ABC
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             ParamEquipmentList( QObject* parent, const QString& name, const kernel::Resolver< kernel::EquipmentType >& resolver );
    virtual ~ParamEquipmentList();
    //@}

    //! @name Operations
    //@{
    virtual void BuildInterface( QWidget* parent );
    virtual void CommitTo( ASN1T_MissionParameter& asn ) const;
    virtual void Clean( ASN1T_MissionParameter& asn ) const;
    //@}

private slots:
    //! @name Slots
    //@{
    void OnEquipmentChanged( int row, int col );
    //@}

private:
    //! @name Types
    //@{
    typedef std::map< QString, const kernel::EquipmentType* > T_EquipmentTypes;
    typedef T_EquipmentTypes::const_iterator                CIT_EquipmentTypes;
    //@}

private:
    //! @name Member data
    //@{
    const kernel::Resolver< kernel::EquipmentType >& resolver_;
    QTable*                     table_;
    QStringList                 equipmentList_;
    T_EquipmentTypes            equipmentTypes_;
    //@}
};

#endif // __ParamEquipmentList_h_
