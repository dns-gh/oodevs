// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __LayersPanel_h_
#define __LayersPanel_h_

#include "PreferencePanel_ABC.h"

namespace kernel
{
    class OptionsController;
}

namespace gui
{

// =============================================================================
/** @class  LayersPanel
    @brief  LayersPanel
*/
// Created: AGE 2007-01-04
// =============================================================================
class LayersPanel : public PreferencePanel_ABC
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             LayersPanel( QWidget* parent,
                          kernel::OptionsController& options,
                          GLView_ABC& view );
    virtual ~LayersPanel();
    //@}

private:
    //! @name PreferencePanel_ABC implementation
    //@{
    virtual void Load( const GLView_ABC& proxy );
    //@}

private slots:
    //! @name Slots
    //@{
    void OnAlphaChanged();
    void OnSelectionChanged( const QItemSelection& selected, const QItemSelection& deselected );
    void OnUp();
    void OnDown();
    void OnRemoveDynamicLayer();
    //@}

private:
    //! @name Helpers
    //@{
    void UpdateButtonsStates();
    void UpdateLeastAndMostVisible();
    void SwapSelection( int direction );
    QStandardItem* GetSelectedItem() const;
    //@}

private:
    //! @name Member data
    //@{
    kernel::OptionsController& options_;
    GLView_ABC& view_;
    QListView* layersListView_;
    QStandardItemModel dataModel_;
    QSortFilterProxyModel proxyModel_;
    QSlider* transparency_;
    QLabel* transparencyLabel_;
    QWidget* removeButton_;
    QPushButton* upButton_;
    QPushButton* downButton_;
};

} //! namespace gui

#endif // __LayersPanel_h_
