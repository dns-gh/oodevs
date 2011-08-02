// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Panels_h_
#define __Panels_h_

#include "PanelStack_ABC.h"

namespace gui
{
    class InfoPanel_ABC;

// =============================================================================
/** @class  Panels
    @brief  Panel container
*/
// Created: SBO 2006-08-08
// =============================================================================
class Panels : public Q3VBox
             , public gui::PanelStack_ABC
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             Panels( QWidget* pParent );
    virtual ~Panels();
    //@}

    //! @name Operations
    //@{
    void AddPanel( InfoPanel_ABC* panel );
    virtual void Add( QWidget* widget );
    virtual void Remove( QWidget* widget );
    //@}

private slots:
    //! @name Slots
    //@{
    void Select( int index );
    void PreviousPage();
    void NextPage();
    //@}

private:
    //! @name Copy / Assignment
    //@{
    Panels( const Panels& );
    Panels& operator=( const Panels& );
    //@}

private:
    //! @name Types
    //@{
    typedef std::set< QWidget* >             T_Widgets;
    typedef std::pair< T_Widgets, QWidget* > T_SelectedWidget;
    typedef std::vector< T_SelectedWidget >  T_SelectedWidgets;
    typedef T_SelectedWidgets::iterator     IT_SelectedWidgets;

    typedef std::map< QWidget*, bool > T_PanelStates;
    typedef std::vector< QWidget* > T_Panels;
    typedef T_Panels::const_iterator CIT_Panels;
    //@}

    //! @name Helpers
    //@{
    void UpdateCombo();
    void Select( QWidget* widget );
    void CheckButtons();
    virtual QSize sizeHint() const;

    IT_SelectedWidgets FindSelectedSet();
    void ShowPreferedWidget( QWidget* defaultSelection );
    void SaveSelection( QWidget* widget );
    //@}

private:
    //! @name Member data
    //@{
    QPushButton*        previous_;
    QPushButton*        next_;
    QComboBox*          combo_;
    Q3WidgetStack*       stack_;
    QWidget*            dummy_;

    T_SelectedWidgets   widgets_;
    T_PanelStates       panelStates_;
    T_Panels            panels_;
    T_Panels            currentPanels_;
    //@}
};

}

#endif // __Panels_h_
