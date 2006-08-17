// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __InfoPanels_h_
#define __InfoPanels_h_

#include "astec_gui/PanelStack_ABC.h"

class Controllers;
class ItemFactory_ABC;
class InfoPanel_ABC;

// =============================================================================
/** @class  InfoPanels
    @brief  Info panels container
*/
// Created: SBO 2006-08-08
// =============================================================================
class InfoPanels : public QVBox
                 , public PanelStack_ABC
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             InfoPanels( QWidget* pParent, Controllers& controllers, ItemFactory_ABC& factory );
    virtual ~InfoPanels();
    //@}

    //! @name Operations
    //@{
    virtual void Add( QWidget* widget, const QString& name );
    virtual void Remove( QWidget* widget );
    //@}

private:
    //! @name Copy / Assignment
    //@{
    InfoPanels( const InfoPanels& );
    InfoPanels& operator=( const InfoPanels& );
    //@}

    //! @name Helpers
    //@{
    QSize sizeHint() const;
    //@}

private slots:
    //! @name Slots
    //@{
    void Select( int index );
    void PreviousPage();
    void NextPage();
    //@}

private:
    //! @name Types
    //@{
    typedef std::set< QWidget* >             T_Widgets;
    typedef std::pair< T_Widgets, QWidget* > T_SelectedWidget;
    typedef std::vector< T_SelectedWidget >  T_SelectedWidgets;
    typedef T_SelectedWidgets::iterator     IT_SelectedWidgets;
    //@}

    //! @name Helpers
    //@{
    void AddPanel( InfoPanel_ABC* panel );
    void UpdateCombo();
    void Select( QWidget* widget );
    void CheckButtons();

    IT_SelectedWidgets FindSelectedSet();
    void ShowPreferedWidget( QWidget* defaultSelection );
    void SaveSelection( QWidget* widget );
    //@}

    //! @name Types
    //@{
    typedef std::map< QWidget*, bool > T_PanelStates;
    typedef std::vector< QWidget* > T_Panels;
    typedef T_Panels::const_iterator CIT_Panels;
    //@}

private:
    //! @name Member data
    //@{
    QPushButton*        previous_;
    QPushButton*        next_;
    QComboBox*          combo_;
    QWidgetStack*       stack_;
    QWidget*            dummy_;

    T_SelectedWidgets   widgets_;
    T_PanelStates       panelStates_;
    T_Panels            panels_;
    T_Panels            currentPanels_;
    //@}
};

#endif // __InfoPanels_h_
