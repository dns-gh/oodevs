// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __MOS_PopulationCreationPanel_h_
#define __MOS_PopulationCreationPanel_h_

#ifdef __GNUG__
#   pragma interface
#endif

#include "MOS_ASN_Types.h"

class MOS_ParamLocation;
class MOS_ActionContext;
class QComboBox;
class QSpinBox;
class MOS_SelectedElement;
class QPopupMenu;
class QTabWidget;
class MOS_ActionContext;
class MOS_Team;
class MOS_TypePopulation;
// =============================================================================
/** @class  MOS_PopulationCreationPanel
    @brief  MOS_PopulationCreationPanel
    @par    Using example
    @code
    MOS_PopulationCreationPanel;
    @endcode
*/
// Created: HME 2005-10-17
// =============================================================================
class MOS_PopulationCreationPanel: public QWidget
{
    Q_OBJECT;
    MT_COPYNOTALLOWED( MOS_PopulationCreationPanel );
    friend class MOS_GLTool;
public:
    //! @name Constructors/Destructor
    //@{
             MOS_PopulationCreationPanel( QTabWidget* pParent );
    virtual ~MOS_PopulationCreationPanel();
    //@}

private slots:
    void FillRemotePopupMenu( QPopupMenu& popupMenu, const MOS_ActionContext& context );
    void OnOk();

    void SetSelectedElement( MOS_SelectedElement& selectedElement );

    void OnTeamCreated           ( MOS_Team& team );
    void OnConnexionStatusChanged( bool bConnected );
    void OnTypePopulationCreated( MOS_TypePopulation& type );
    void OnDeletePopulation();

private:
    QTabWidget *                                pParent_;
    MT_ValuedComboBox< uint >*                  pTeamCombo_;
    QSpinBox*                                   pNbPersons_;
    MT_ValuedComboBox< MOS_TypePopulation* >*    pType_;
    QLineEdit*                                  pName_;
    MT_ValuedComboBox< E_PopulationAttitude >*  pAttitudeCombo_;
    QPushButton*                                pOkButton;
    MOS_ParamLocation*                          pLocation_;
    ASN1T_Localisation                          asnLocation_;

};



#include "MOS_PopulationCreationPanel.inl"

#endif // __MOS_PopulationCreationPanel_h_
