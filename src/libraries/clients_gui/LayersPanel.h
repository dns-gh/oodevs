// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __LayersPanel_h_
#define __LayersPanel_h_

#include "PreferencePanel_ABC.h"

namespace kernel
{
    class Controllers;
    class OptionsController;
}

namespace gui
{
    class CheckBox;
    class GlProxy;
    class Layer_ABC;

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
                          kernel::Controllers& controllers,
                          GlProxy& view );
    virtual ~LayersPanel();
    //@}

private:
    //! @name PreferencePanel_ABC implementation
    //@{
    virtual void Load( const GlProxy& proxy );
    virtual void Reset();
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
    kernel::Controllers& controllers_;
    kernel::OptionsController& options_;

    GlProxy& proxy_;
    QCheckBox* fogOfWar_;
    QCheckBox* infra_;
    QListView* layersListView_;
    QStandardItemModel dataModel_;
    QSortFilterProxyModel proxyModel_;
    QSlider* transparency_;
    QLabel* transparencyLabel_;
    QWidget* removeButton_;
    QPushButton* upButton_;
    QPushButton* downButton_;
    //@}

//public:
//    //! @name Constructors/Destructor
//    //@{
//             LayersPanel( QWidget* parent, kernel::Controllers& controllers, GlProxy& proxy );
//    virtual ~LayersPanel();
//    //@}
//
//    //! @name Operations
//    //@{
//    void AddLayer( const QString& name, const std::shared_ptr< Layer_ABC >& layer, bool dynamic = false );
//    void Update();
//    virtual void Commit();
//    virtual void Reset();
//    //@}
//
//private slots:
//    //! @name Slots
//    //@{
//    void OnValueChanged();
//    void OnSelectionChanged();
//    void OnUp();
//    void OnDown();
//    void OnFogOfWarChanged( bool value );
//    void OnInfraChanged( bool value );
//    void OnRemoveDynamicLayer();
//    //@}
//
//private:
//    //! @name Helpers
//    //@{
//    virtual void OptionChanged( const std::string& name, const kernel::OptionVariant& value );
//    virtual void showEvent( QShowEvent* event );
//    void UpdateLeastAndMostVisible();
//    void ResetLayers();
//    void RemoveDynamicLayer( QStandardItem& item );
//    void MoveItem( int row, const std::shared_ptr< Layer_ABC >& layer, int newPlace, int oldPlace, int step );
//    std::shared_ptr< Layer_ABC > GetCurrentLayer() const;
//    int GetCurrentRow( const std::shared_ptr< Layer_ABC >& layer ) const;
//    //@}
//
//    //! @name Types
//    //@{
//    typedef std::vector< std::shared_ptr< Layer_ABC > > T_Layers;
//    typedef std::vector< float >          T_Alphas;
//    typedef std::vector< QString >        T_Names;
//    //@}
//
//private:
//    //! @name Member data
//    //@{
//    kernel::Controllers& controllers_;
//    kernel::OptionsController& options_;
//    GlProxy& proxy_;
//    T_Layers layers_;
//    T_Layers dynamicLayers_;
//    T_Alphas current_;
//    T_Alphas new_;
//    T_Names  names_;
//    CheckBox* fogOfWar_;
//    CheckBox* infra_;
//    QTreeView* layersList_;
//    QStandardItemModel* layersModel_;
//    QSlider* transparency_;
//    QLabel* transparencyLabel_;
//    int currentLayer_;
//    T_Layers currentLayers_;
//    T_Layers newLayers_;
//    QWidget* removeButton_;
//    //@}
};

}

#endif // __LayersPanel_h_
