// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __gui_UnitStateTableEquipment_h_
#define __gui_UnitStateTableEquipment_h_

#include "UnitStateTable_ABC.h"

namespace kernel
{
    class Entity_ABC;
}

namespace gui
{
    class LinkItemDelegate;
    class DisplayExtractor;
// =============================================================================
/** @class  UnitStateTableEquipment
    @brief  UnitStateTableEquipment
*/
// Created: ABR 2011-07-05
// =============================================================================
class UnitStateTableEquipment : public UnitStateTable_ABC
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             UnitStateTableEquipment( QWidget* parent, DisplayExtractor& extractor );
    virtual ~UnitStateTableEquipment();
    //@}

public:
    //! @name Operations
    //@{
    virtual bool IsReadOnlyForType( const std::string& typeName ) const;
    //@}

protected:
    //! @name Helpers
    //@{
    virtual void Purge();
    void AddLines( const QString& name, const kernel::Entity_ABC& entity, int size, E_EquipmentState state, const QStringList& breakdowns,
                   const std::vector< unsigned int > currentBreakdowns = std::vector< unsigned int >() );
    QString GetDisplayName( const kernel::Entity_ABC& entity ) const;
    //@}

protected:
    //! @name Types
    //@{
    enum E_Column { eName = 0, eUnit = 1, eState = 2, eBreakdown = 3 };
    //@}

protected slots:
    //! @name Slots
    //@{
    void OnItemChanged( QStandardItem* item );
    void OnLinkClicked( const QString& url, const QModelIndex& index );
    //@}

private:
    //! @name Member data
    //@{
    LinkItemDelegate* linkItemDelegate_;
    DisplayExtractor& extractor_;
    //@}
};
}

#endif // __gui_UnitStateTableEquipment_h_
