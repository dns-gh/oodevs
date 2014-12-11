// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __Panels_h_
#define __Panels_h_

#include "PanelStack_ABC.h"

namespace gui
{
    class InfoPanel_ABC;
    class RichPushButton;
    template< typename T > class RichWidget;

// =============================================================================
/** @class  Panels
    @brief  Panel container
*/
// Created: SBO 2006-08-08
// =============================================================================
class Panels : public QWidget
             , public gui::PanelStack_ABC
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             Panels( const QString& objectName, QWidget* pParent );
    virtual ~Panels();
    //@}

    //! @name Operations
    //@{
    void AddPanel( InfoPanel_ABC* panel );
    virtual void Add( QWidget* widget );
    virtual void Remove( QWidget* widget );
    //@}

public slots:
    //! @name Slots
    //@{
    void Select( int index );
    //@}

private slots:
    //! @name Slots
    //@{
    void PreviousPage();
    void NextPage();
    //@}

private:
    //! @name Types
    //@{
    typedef std::set< QWidget* >             T_Widgets;
    typedef std::pair< T_Widgets, QWidget* > T_SelectedWidget;
    typedef std::vector< T_SelectedWidget >  T_SelectedWidgets;

    typedef std::map< QWidget*, bool > T_PanelStates;
    typedef std::vector< QWidget* > T_Panels;
    //@}

    //! @name Helpers
    //@{
    void UpdateCombo();
    void Select( QWidget* widget );
    void CheckButtons();
    virtual QSize sizeHint() const;

    T_SelectedWidgets::iterator FindSelectedSet();
    void ShowPreferedWidget( QWidget* defaultSelection );
    void SaveSelection( QWidget* widget );
    //@}

private:
    //! @name Member data
    //@{
    RichPushButton*     previous_;
    RichPushButton*     next_;
    RichWidget< QComboBox >*       combo_;
    QStackedWidget*     stack_;
    QWidget*            dummy_;

    T_SelectedWidgets   widgets_;
    T_PanelStates       panelStates_;
    T_Panels            panels_;
    T_Panels            currentPanels_;
    //@}
};

}

#endif // __Panels_h_
