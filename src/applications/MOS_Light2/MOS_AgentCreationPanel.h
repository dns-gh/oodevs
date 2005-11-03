// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: APE 2004-09-02 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MOS_AgentCreationPanel.h $
// $Author: Age $
// $Modtime: 14/04/05 10:59 $
// $Revision: 7 $
// $Workfile: MOS_AgentCreationPanel.h $
//
// *****************************************************************************

#ifndef __MOS_AgentCreationPanel_h_
#define __MOS_AgentCreationPanel_h_

#ifdef __GNUG__
#   pragma interface
#endif

#include "MOS_SelectedElement.h"

class MOS_ActionContext;
class MOS_AgentCreationListView;
class MOS_AgentCreationMapFilter;
class MOS_Nature;

// =============================================================================
/** @class  MOS_AgentCreationPanel
    @brief  MOS_AgentCreationPanel
*/
// Created: APE 2004-09-02
// =============================================================================
class MOS_AgentCreationPanel : public QWidgetStack
{
    Q_OBJECT;
    MT_COPYNOTALLOWED( MOS_AgentCreationPanel );

public:
    typedef std::vector< std::string >        T_GtiaTypeVector;
    typedef T_GtiaTypeVector::iterator       IT_GtiaTypeVector;

    class MOS_AgentType;
    typedef std::vector< MOS_AgentType* >     T_AgentTypeVector;
    typedef T_AgentTypeVector::iterator       IT_AgentTypeVector;
    typedef T_AgentTypeVector::const_iterator CIT_AgentTypeVector;

    class MOS_AgentType
    {
    public:
        MOS_AgentType() {};
    public:
        std::string strName_;
        T_AgentTypeVector composition_;
        MOS_Nature* pNature_;
    };

    class MOS_AgentType_Cmp : public std::unary_function< MOS_AgentType* , bool >
    {
    public:
        MOS_AgentType_Cmp(const std::string& strCat ) : strCat_(strCat) {}
        ~MOS_AgentType_Cmp() {}

        bool operator()( MOS_AgentType* pAgentType ) const
        { return pAgentType->strName_ == strCat_; }

    private:
        std::string strCat_;
    };

public:
    //! @name Constructors/Destructor
    //@{
     MOS_AgentCreationPanel( QWidget* pParent );
    ~MOS_AgentCreationPanel();
    //@}

    //! @name Accessors
    //@{
    MOS_SelectedElement& GetSelectedElement();
    float GetDispersionRadius() const;
    //@}

private slots:
    void SetSelectedElement( MOS_SelectedElement& selectedElement );
    void FillRemotePopupMenu( QPopupMenu& popupMenu, const MOS_ActionContext& context );
    void CreateGtia();
    void ToggleAutomate();
    void LogisticLinksDialog();
    void ChangeQuotasDialog();

signals:
    void ElementSelected( MOS_SelectedElement& selectedElement );

private:
    //! @name Helpers
    //@{
    void Initialize();
    //@}

private:
    //! @name Member data
    //@{
    QWidget* pMessagePanel_;
    QWidget* pGtiaCreationPanel_;
    QWidget* pAgentListPanel_;

    MOS_AgentCreationListView* pAgentListView_;
    QSpinBox* pRadiusSpinBox_;

    QComboBox* pGtiaTypeCombo_;

    MOS_AgentCreationMapFilter* pMapFilter_;

    MOS_SelectedElement selectedElement_;

    T_AgentTypeVector unitTypes_;
    T_AgentTypeVector automateTypes_;

    T_GtiaTypeVector gtiaTypes_;
    //@}
};

#include "MOS_AgentCreationPanel.inl"

#endif // __MOS_AgentCreationPanel_h_
