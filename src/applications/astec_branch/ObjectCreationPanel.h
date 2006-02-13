// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: APE 2004-06-04 $
// $Archive: /MVW_v10/Build/SDK/Light2/src/ObjectCreationPanel.h $
// $Author: Age $
// $Modtime: 2/05/05 14:40 $
// $Revision: 8 $
// $Workfile: ObjectCreationPanel.h $
//
// *****************************************************************************

#ifndef __ObjectCreationPanel_h_
#define __ObjectCreationPanel_h_

#ifdef __GNUG__
#   pragma interface
#endif

#include "ASN_Types.h"
#include "MapEventFilter_ABC.h"
#include "SelectedElement.h"

class ParamLocation;
class ActionContext;
class ParamAgent;
class QCheckBox;
class QTabWidget;
class QTable;
class Agent;

// =============================================================================
// Created: APE 2004-06-04
// =============================================================================
class ObjectCreationPanel : public QWidget, public MapEventFilter_ABC
{
    Q_OBJECT;
    MT_COPYNOTALLOWED( ObjectCreationPanel );
    friend class GLTool;

public:
    //! @name Constructors/Destructor
    //@{
     ObjectCreationPanel( QTabWidget* pParent );
    ~ObjectCreationPanel();
    //@}

    //! @name Accessors
    //@{
    QObject* GetObject();
    //@}

private slots:
    //! @name Operations
    //@{
    void FillRemotePopupMenu( QPopupMenu& popupMenu, const ActionContext& context );
    void OnObjectChanged();
    void OnOk();

    void SetSelectedElement( SelectedElement& selectedElement );

    void OnTeamCreated           ( Team& team );
    void OnConnexionStatusChanged( bool bConnected );
    void OnROTANBCAgentChanged   ( int nRow, int nCol );
    void OnDeleteObject();
    //@}

private:
    //! @name Event filter & related helpers
    //@{
    bool eventFilter( QObject* pSender, QEvent* pEvent );

    bool OnKeyPress ( const QKeyEvent& keyEvent );
    //@}

    private:
    //! @name Types
    //@{
    typedef std::map< uint, const Agent* > T_AgentIDMap;
    typedef T_AgentIDMap::const_iterator       CIT_AgentIDMap;
    //@}

private:
    //! @name Member data
    //@{
    QTabWidget*         pParent_;
    SelectedElement selectedElement_;

    // Common parameters
    MT_ValuedComboBox< uint >*                 pTeamCombo_;
    MT_ValuedComboBox< ASN1T_EnumObjectType >* pObjectTypeCombo_;
    ParamLocation*                         pLocation_;
    ASN1T_Localisation                         asnLocation_;
	ParamAgent*                            pAgent_;
	ASN1T_Agent                                asnAgent_;
    QLineEdit*                                 pNameEdit_;

    // NBC parameters
    QStringList*               pNBCAgentsStringList_;
    QGroupBox*                 pNBCParamsGroup_;
    MT_ValuedComboBox< uint >* pNBCTypeCombo_;

    // ROTA parameters
    QGroupBox*    pROTAParamsGroup_;
    QSpinBox*     pROTADangerEdit_;
    QTable*       pROTANBCAgentsTable_;

    // Crossing parameters
    QGroupBox*    pCrossingParamsGroup_;
    QSpinBox*     pCrossingWidthEdit_;
    QSpinBox*     pCrossingDepthEdit_;
    QSpinBox*     pCrossingSpeedEdit_;
    QCheckBox*    pCrossingToConstructCheckbox_;

    // prisonners/refugees camp parameters
    QGroupBox*    pTC2Group_;
    QComboBox*    pTC2ComboBox_;
    T_AgentIDMap  automateComboBoxIDs_;

    // log route parameters
    QGroupBox*    pLogRouteGroup_;
    QSpinBox*     pLogRouteFlow_;
    QSpinBox*     pLogRouteWidth_;
    QSpinBox*     pLogRouteLength_;
    QSpinBox*     pLogRouteMaxWeight_;
    QCheckBox*    pLogRouteEquipped_;

    //@}
};


#   include "ObjectCreationPanel.inl"

#endif // __ObjectCreationPanel_h_
