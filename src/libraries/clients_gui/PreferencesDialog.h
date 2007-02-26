// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __PreferencesDialog_h_
#define __PreferencesDialog_h_

#include <qdialog.h>

namespace kernel
{
    class Controllers;
}

namespace gui
{
    class LightingProxy;
    class GraphicPreferences;
    class GraphicsPanel;
    class PreferencesList;
    class PreferencePanel_ABC;
    class Layer2d_ABC;
    class LayersPanel;
    class Elevation2dLayer;

// =============================================================================
/** @class  PreferencesDialog
    @brief  PreferencesDialog
*/
// Created: SBO 2006-05-03
// =============================================================================
class PreferencesDialog : public QDialog
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             PreferencesDialog( QWidget* parent, kernel::Controllers& controllers, LightingProxy& lighting );
    virtual ~PreferencesDialog();
    //@}

    //! @name Operations
    //@{
    virtual QSize sizeHint () const;
    GraphicPreferences& GetPreferences() const; // $$$$ SBO 2006-05-04: 

    void AddLayer( const QString& name, gui::Layer2d_ABC& layer );
    void AddLayer( const QString& name, gui::Elevation2dLayer& layer ); // $$$$ AGE 2007-01-17: 
    //@}

private slots:
    //! @name Slots
    //@{
    void OnOk();
    void OnApply();
    void OnCancel();
    //@}

private:
    //! @name Copy/Assignement
    //@{
    PreferencesDialog( const PreferencesDialog& );            //!< Copy constructor
    PreferencesDialog& operator=( const PreferencesDialog& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    void AddPage( const QString& name, PreferencePanel_ABC& page );
    //@}

    //! @name Types
    //@{
    typedef std::vector< PreferencePanel_ABC* > T_Pages;
    typedef T_Pages::iterator                  IT_Pages;
    //@}

private:
    //! @name Member data
    //@{
    T_Pages          pages_;
    PreferencesList* list_;
    LayersPanel*     layersPanel_;
    GraphicsPanel*   pGraphicPrefPanel_;
    //@}
};

}

#endif // __PreferencesDialog_h_
