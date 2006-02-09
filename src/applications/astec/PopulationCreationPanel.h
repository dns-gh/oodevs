// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __PopulationCreationPanel_h_
#define __PopulationCreationPanel_h_

#ifdef __GNUG__
#   pragma interface
#endif

#include "Types.h"
#include "ASN_Types.h"
#include "SelectedElement.h"

class ParamLocation;
class ActionContext;
class QComboBox;
class QSpinBox;
class QPopupMenu;
class QTabWidget;
class ActionContext;
class Team;
class TypePopulation;
// =============================================================================
/** @class  PopulationCreationPanel
    @brief  PopulationCreationPanel
    @par    Using example
    @code
    PopulationCreationPanel;
    @endcode
*/
// Created: HME 2005-10-17
// =============================================================================
class PopulationCreationPanel: public QWidget
{
    Q_OBJECT;
    MT_COPYNOTALLOWED( PopulationCreationPanel );
    friend class GLTool;
public:
    //! @name Constructors/Destructor
    //@{
             PopulationCreationPanel( QTabWidget* pParent );
    virtual ~PopulationCreationPanel();
    //@}

private slots:
    void FillRemotePopupMenu( QPopupMenu& popupMenu, const ActionContext& context );
    void OnOk();

    void SetSelectedElement( SelectedElement& selectedElement );

    void OnTeamCreated           ( Team& team );
    void OnConnexionStatusChanged( bool bConnected );
    void OnTypePopulationCreated( TypePopulation& type );

private:

    SelectedElement selectedElement_;

    QTabWidget *                                pParent_;
    MT_ValuedComboBox< uint >*                  pTeamCombo_;
    QSpinBox*                                   pNbPersons_;
    MT_ValuedComboBox< TypePopulation* >*    pType_;
    QLineEdit*                                  pName_;
    MT_ValuedComboBox< E_PopulationAttitude >*  pAttitudeCombo_;
    QPushButton*                                pOkButton;
    ParamLocation*                          pLocation_;
    ASN1T_Localisation                          asnLocation_;

};



#include "PopulationCreationPanel.inl"

#endif // __PopulationCreationPanel_h_
