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
    class Options;
}

namespace gui
{
    class Layer_ABC;

// =============================================================================
/** @class  LayersPanel
    @brief  LayersPanel
*/
// Created: AGE 2007-01-04
// =============================================================================
class LayersPanel : public PreferencePanel_ABC
                  , public kernel::Observer_ABC
                  , public kernel::OptionsObserver_ABC
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             LayersPanel( QWidget* parent, kernel::Controllers& controllers );
    virtual ~LayersPanel();
    //@}

    //! @name Operations
    //@{
    void AddLayer( const QString& name, Layer_ABC& layer );

    virtual void Commit();
    virtual void Reset();
    //@}

private slots:
    //! @name Slots
    //@{
    void OnValueChanged();
    void OnSelectionChanged( QListViewItem* );
    void OnUp();
    void OnDown();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    LayersPanel( const LayersPanel& );            //!< Copy constructor
    LayersPanel& operator=( const LayersPanel& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    virtual void OptionChanged( const std::string& name, const kernel::OptionVariant& value );
    //@}

    //! @name Types
    //@{
    typedef std::vector< Layer_ABC* >         T_Layers;
    typedef std::vector< float >              T_Alphas;
    typedef std::vector< std::string >        T_Names;
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    kernel::Options& options_;
    T_Layers layers_;
    T_Alphas current_;
    T_Alphas new_;
    T_Names  names_;
    QListView* layersList_;
    QSlider* transparency_;
    int currentLayer_;
    //@}
};

}

#endif // __LayersPanel_h_
