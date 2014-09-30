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
#include "clients_kernel/OptionsObserver_ABC.h"

namespace kernel
{
    class Controllers;
    class OptionsController;
}

namespace gui
{
    class CheckBox;
    class GlSelector;
    class Layer;

// =============================================================================
/** @class  LayersPanel
    @brief  LayersPanel
*/
// Created: AGE 2007-01-04
// =============================================================================
class LayersPanel : public PreferencePanel_ABC
                  , public tools::Observer_ABC
                  , public kernel::OptionsObserver_ABC
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             LayersPanel( QWidget* parent, kernel::Controllers& controllers, GlSelector& selector );
    virtual ~LayersPanel();
    //@}

    //! @name Operations
    //@{
    void AddLayer( const QString& name, Layer& layer, bool dynamic = false );
    void Update();
    virtual void Commit();
    virtual void Reset();
    //@}

private slots:
    //! @name Slots
    //@{
    void OnValueChanged();
    void OnSelectionChanged();
    void OnUp();
    void OnDown();
    void OnFogOfWarChanged( bool value );
    void OnInfraChanged( bool value );
    void OnRemoveDynamicLayer();
    //@}

private:
    //! @name Helpers
    //@{
    virtual void OptionChanged( const std::string& name, const kernel::OptionVariant& value );
    virtual void showEvent( QShowEvent* event );
    void UpdateLeastAndMostVisible();
    void ResetLayers();
    void RemoveDynamicLayer( QStandardItem& item );
    void MoveItem( int row, Layer* layer, int newPlace, int oldPlace, int step );
    Layer* GetCurrentLayer() const;
    int GetCurrentRow( Layer* layer ) const;
    //@}

    //! @name Types
    //@{
    typedef std::vector< Layer* >     T_Layers;
    typedef std::vector< float >          T_Alphas;
    typedef std::vector< QString >        T_Names;
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    kernel::OptionsController& options_;
    GlSelector& selector_;
    T_Layers layers_;
    T_Layers dynamicLayers_;
    T_Alphas current_;
    T_Alphas new_;
    T_Names  names_;
    CheckBox* fogOfWar_;
    CheckBox* infra_;
    QTreeView* layersList_;
    QStandardItemModel* layersModel_;
    QSlider* transparency_;
    QLabel* transparencyLabel_;
    int currentLayer_;
    T_Layers currentLayers_;
    T_Layers newLayers_;
    QWidget* removeButton_;
    //@}
};

}

#endif // __LayersPanel_h_
