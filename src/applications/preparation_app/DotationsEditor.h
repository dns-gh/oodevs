// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __DotationsEditor_h_
#define __DotationsEditor_h_

#include "clients_gui/PropertyDialog.h"
#include "clients_kernel/Resolver2.h"
#include "clients_gui/ValueEditor.h"
#include "tools/Resolver.h"

namespace kernel
{
    class Entity_ABC;
    class DotationType;
    class EquipmentType;
    class ObjectTypes;
}

namespace gui
{
    template< typename T > class RichWidget;
}

class Dotation;
class DotationsItem;

// =============================================================================
/** @class  DotationsEditor
    @brief  Dotations editor
*/
// Created: SBO 2006-11-10
// =============================================================================
class DotationsEditor : public gui::PropertyDialog
                      , public gui::ValueEditor< DotationsItem* >
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             DotationsEditor( QWidget* parent, const kernel::ObjectTypes& objectTypes );
    virtual ~DotationsEditor();
    //@}

    //! @name Operations
    //@{
    void SetCurrentItem( DotationsItem*& dotations, const kernel::Entity_ABC& current );
    virtual DotationsItem* GetValue();
    //@}

private slots:
    //! @name Slots
    //@{
    void OnClear();
    void OnAccept();
    void OnReject();
    void OnValueChanged( int row, int col );
    void OnLinkActivated( const QString& link );
    //@}

private:
    //! @name Types
    //@{
    typedef std::map< std::string, std::pair< double, double > > T_StockCapacities;
    typedef T_StockCapacities::const_iterator                  CIT_StockCapacities;

    enum E_InfosColumns { eWeightCurrent = 0, eWeightMax = 1, eVolumeCurrent = 2, eVolumeMax = 3 };
    //@}

    //! @name Helpers
    //@{
    void AddItem( int row, const Dotation* dotation = 0 );
    void AddInfoItem( int row, E_InfosColumns currentCol, double currentValue, E_InfosColumns maxCol, double maxValue );
    void UpdateInfos();
    void FillMissingWithZero( T_StockCapacities& src, T_StockCapacities& dst );
    //@}

private:
    //! @name Member data
    //@{
    const kernel::Resolver2< kernel::EquipmentType >& equipments_;
    const kernel::Resolver2< kernel::DotationType >& dotations_;
    const kernel::Entity_ABC* current_;
    DotationsItem** value_;
    gui::RichWidget< QTableWidget >* table_;
    gui::RichWidget< QTableWidget >* infosTable_;
    QLabel* infosLabel_;
    //@}
};

#endif // __DotationsEditor_h_
