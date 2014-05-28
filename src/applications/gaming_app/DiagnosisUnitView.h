// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#ifndef GAMING_APP_DIAGNOSIS_UNIT_VIEW_H
#define GAMING_APP_DIAGNOSIS_UNIT_VIEW_H

#include "clients_gui/RichWidget.h"
#include "tools/Observer_ABC.h"
#include <vector>

namespace kernel
{
    class Agent_ABC;
    class Entity_ABC;
    class EquipmentType;
}

namespace gui
{
    class DisplayExtractor;
}

// =============================================================================
/** @class  DiagnosisUnitView
    @brief  Diagnosis unit view
*/
// Created: SLI 2014-03-17
// =============================================================================
class DiagnosisUnitView : public gui::RichWidget< QTreeView >
                        , public tools::Observer_ABC
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
              DiagnosisUnitView( QWidget* parent, gui::DisplayExtractor& extractor );
             ~DiagnosisUnitView();
    //@}

    //! @name Operations
    //@{
    void Fill( const std::vector< const kernel::Entity_ABC* >& destinations, const kernel::Agent_ABC& brokenAgent, const kernel::EquipmentType* carrier );
    void Purge();
    //@}

signals:
    //! @name Signals
    //@{
    void DestinationSelected( unsigned int destination );
    //@}

private slots:
    //! @name Slots
    //@{
    void OnLinkClicked( const QString& url, const QModelIndex& index );
    void OnSelectionChanged( const QModelIndex& current, const QModelIndex& );
    //@}

private:
    //! @name Data Members
    //@{
    gui::DisplayExtractor& extractor_;
    QStandardItemModel* model_;
    //@}
};

#endif
