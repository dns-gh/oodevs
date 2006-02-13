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
// $Archive: /MVW_v10/Build/SDK/Light2/src/AgentCreationPanel.h $
// $Author: Age $
// $Modtime: 14/04/05 10:59 $
// $Revision: 7 $
// $Workfile: AgentCreationPanel.h $
//
// *****************************************************************************

#ifndef __AgentCreationPanel_h_
#define __AgentCreationPanel_h_

#ifdef __GNUG__
#   pragma interface
#endif

#include "SelectedElement.h"

class ActionContext;
class AgentCreationListView;
class AgentCreationMapFilter;
class Nature;

// =============================================================================
/** @class  AgentCreationPanel
    @brief  AgentCreationPanel
*/
// Created: APE 2004-09-02
// =============================================================================
class AgentCreationPanel : public QWidgetStack
{
    Q_OBJECT;
    MT_COPYNOTALLOWED( AgentCreationPanel );

public:
    typedef std::vector< std::string >        T_GtiaTypeVector;
    typedef T_GtiaTypeVector::iterator       IT_GtiaTypeVector;

    class AgentType;
    typedef std::vector< AgentType* >     T_AgentTypeVector;
    typedef T_AgentTypeVector::iterator       IT_AgentTypeVector;
    typedef T_AgentTypeVector::const_iterator CIT_AgentTypeVector;

    class AgentType
    {
    public:
        AgentType() {};
    public:
        std::string strName_;
        T_AgentTypeVector composition_;
        Nature* pNature_;
    };

    class AgentType_Cmp : public std::unary_function< AgentType* , bool >
    {
    public:
        AgentType_Cmp(const std::string& strCat ) : strCat_(strCat) {}
        ~AgentType_Cmp() {}

        bool operator()( AgentType* pAgentType ) const
        { return pAgentType->strName_ == strCat_; }

    private:
        std::string strCat_;
    };

public:
    //! @name Constructors/Destructor
    //@{
     AgentCreationPanel( QWidget* pParent );
    ~AgentCreationPanel();
    //@}

    //! @name Accessors
    //@{
    SelectedElement& GetSelectedElement();
    float GetDispersionRadius() const;
    //@}

private slots:
    void SetSelectedElement( SelectedElement& selectedElement );
    void FillRemotePopupMenu( QPopupMenu& popupMenu, const ActionContext& context );
    void CreateGtia();
    void ToggleAutomate();
    void ToggleAggregation();
    void LogisticLinksDialog();
    void ChangeQuotasDialog();

signals:
    void ElementSelected( SelectedElement& selectedElement );

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

    AgentCreationListView* pAgentListView_;
    QSpinBox* pRadiusSpinBox_;

    QComboBox* pGtiaTypeCombo_;

    AgentCreationMapFilter* pMapFilter_;

    SelectedElement selectedElement_;

    T_AgentTypeVector unitTypes_;
    T_AgentTypeVector automateTypes_;

    T_GtiaTypeVector gtiaTypes_;
    //@}
};

#include "AgentCreationPanel.inl"

#endif // __AgentCreationPanel_h_
