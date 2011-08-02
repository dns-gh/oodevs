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

namespace kernel
{
    class CoordinateSystems;
    class Controllers;
}

namespace gui
{
    class Elevation2dLayer;
    class GraphicPreferences;
    class GraphicsPanel;
    class Layer_ABC;
    class LayersPanel;
    class LightingProxy;
    class PreferencesList;
    class PreferencePanel_ABC;
    class CoordinateSystemsPanel;
    class Painter_ABC;
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
             PreferencesDialog( QWidget* parent, kernel::Controllers& controllers, LightingProxy& lighting, kernel::CoordinateSystems& coordSystems,
                                const Painter_ABC& painter );
    virtual ~PreferencesDialog();
    //@}

    //! @name Operations
    //@{
    virtual QSize sizeHint () const;
    virtual void show ();
    virtual void reject();
    GraphicPreferences& GetPreferences() const; // $$$$ SBO 2006-05-04:

    void AddLayer( const QString& name, gui::Layer_ABC& layer );
    void AddLayer( const QString& name, gui::Elevation2dLayer& layer ); // $$$$ AGE 2007-01-17:
    //@}

private slots:
    //! @name Slots
    //@{
    void OnOk();
    void OnCancel();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    PreferencesDialog( const PreferencesDialog& );            //!< Copy constructor
    PreferencesDialog& operator=( const PreferencesDialog& ); //!< Assignment operator
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
    kernel::Controllers& controllers_;
    const Painter_ABC& painter_;
    T_Pages pages_;
    PreferencesList* list_;
    LayersPanel* layersPanel_;
    GraphicsPanel* pGraphicPrefPanel_;
    CoordinateSystemsPanel* pCoordinateSystemsPanel_;
    //@}
};

}

#endif // __PreferencesDialog_h_
