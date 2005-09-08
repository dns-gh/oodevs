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
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MOS_ObjectCreationPanel.h $
// $Author: Age $
// $Modtime: 2/05/05 14:40 $
// $Revision: 8 $
// $Workfile: MOS_ObjectCreationPanel.h $
//
// *****************************************************************************

#ifndef __MOS_ObjectCreationPanel_h_
#define __MOS_ObjectCreationPanel_h_

#ifdef __GNUG__
#   pragma interface
#endif

#include "MOS_ASN_Types.h"
#include "MOS_MapEventFilter_ABC.h"
#include "MOS_SelectedElement.h"

class MOS_ParamLocation;
class MOS_ActionContext;
class MOS_ParamAgent;
class QCheckBox;
class QTabWidget;
class QTable;
class MOS_Agent;

// =============================================================================
/** @class  MOS_ObjectCreationPanel
*/
// Created: APE 2004-06-04
// =============================================================================
class MOS_ObjectCreationPanel : public QWidget, public MOS_MapEventFilter_ABC
{
    Q_OBJECT;
    MT_COPYNOTALLOWED( MOS_ObjectCreationPanel );
    friend class MOS_GLTool;

public:
    //! @name Constructors/Destructor
    //@{
     MOS_ObjectCreationPanel( QTabWidget* pParent );
    ~MOS_ObjectCreationPanel();
    //@}

    //! @name Accessors
    //@{
    QObject* GetObject();
    //@}

private slots:
    //! @name Operations
    //@{
    void FillRemotePopupMenu( QPopupMenu& popupMenu, const MOS_ActionContext& context );
    void OnObjectChanged();
    void OnOk();

    void SetSelectedElement( MOS_SelectedElement& selectedElement );

    void OnTeamCreated           ( MOS_Team& team );
    void OnConnexionStatusChanged( bool bConnected );
    void OnROTANBCAgentChanged   ( int nRow, int nCol );
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
    typedef std::map< uint, const MOS_Agent* > T_AgentIDMap;
    typedef T_AgentIDMap::const_iterator       CIT_AgentIDMap;
    //@}

private:
    //! @name Member data
    //@{
    QTabWidget*         pParent_;
    MOS_SelectedElement selectedElement_;

    // Common parameters
    MT_ValuedComboBox< uint >*                 pTeamCombo_;
    MT_ValuedComboBox< ASN1T_EnumObjectType >* pObjectTypeCombo_;
    MOS_ParamLocation*                         pLocation_;
    ASN1T_Localisation                         asnLocation_;
	MOS_ParamAgent*                            pAgent_;
	ASN1T_Agent                                asnAgent_;
    
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


#ifdef MOS_USE_INLINE
#   include "MOS_ObjectCreationPanel.inl"
#endif

#endif // __MOS_ObjectCreationPanel_h_
