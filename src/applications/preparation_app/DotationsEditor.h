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

#include "clients_gui/ModalDialog.h"
#include "tools/Resolver.h"
#include "clients_kernel/ValueEditor.h"
#include <boost/noncopyable.hpp>

namespace kernel
{
    class DotationType;
    class Entity_ABC;
}

class Dotation;
class DotationsItem;
class StaticModel;

// =============================================================================
/** @class  DotationsEditor
    @brief  DotationsEditor
*/
// Created: SBO 2006-11-10
// =============================================================================
class DotationsEditor : public ModalDialog
                      , public kernel::ValueEditor< DotationsItem* >
                      , private boost::noncopyable
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             DotationsEditor( QDialog*& self, QWidget* parent, const ::StaticModel& staticModel );
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
    void AddItem( const Dotation* dotation = 0 );
    void AddInfoItem( int row, E_InfosColumns currentCol, double currentValue, E_InfosColumns maxCol, double maxValue );
    void UpdateInfos();
    void FillMissingWithZero( T_StockCapacities& src, T_StockCapacities& dst );
    //@}

private:
    //! @name Member data
    //@{
    const StaticModel& staticModel_;
    const kernel::Entity_ABC* current_;
    DotationsItem** value_;
    Q3Table* table_;
    Q3Table* infosTable_;
    QStringList types_;
    QDialog*& self_;
    QLabel* infosLabel_;
    static QColor warningColor_;
    //@}
};

#endif // __DotationsEditor_h_
