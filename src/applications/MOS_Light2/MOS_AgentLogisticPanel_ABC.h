// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: AGE 2005-04-01 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MOS_AgentLogisticPanel_ABC.h $
// $Author: Nld $
// $Modtime: 27/04/05 18:27 $
// $Revision: 3 $
// $Workfile: MOS_AgentLogisticPanel_ABC.h $
//
// *****************************************************************************

#ifndef __MOS_AgentLogisticPanel_ABC_h_
#define __MOS_AgentLogisticPanel_ABC_h_

#include "MOS_InfoPanel_ABC.h"

class MOS_Agent;

// =============================================================================
/** @class  MOS_AgentLogisticPanel_ABC
    @brief  Agent logistic panel base class
*/
// Created: AGE 2005-04-01
// =============================================================================
class MOS_AgentLogisticPanel_ABC : public MOS_InfoPanel_ABC
{
    MT_COPYNOTALLOWED( MOS_AgentLogisticPanel_ABC )

public:
    //! @name Constructors/Destructor
    //@{
             MOS_AgentLogisticPanel_ABC( QWidget* pParent );
    virtual ~MOS_AgentLogisticPanel_ABC();
    //@}

protected:
    //! @name Operations
    //@{
    virtual void OnClearSelection() = 0;
    //@}

protected:
    //! @name Helpers
    //@{
    class EquipmentResolver
    {
    public:
        std::string GetName( unsigned int nEquipment ) const;
    };

    class AutomateResolver
    {
    public:
        std::string GetName( unsigned int nAutomateID ) const;
    };

    template< typename Container, typename ListView >
    void DisplayConsigns( const Container& cont, ListView& viewer )
    {
        if( ! cont.empty() )
        {
            viewer.BeginList();
            for( Container::const_iterator it = cont.begin(); it != cont.end(); ++it )
                viewer.AddConsign( **it );
            viewer.EndList();
            viewer.show();
        }
        else
            viewer.hide();
    }

    template< typename Container, typename NameResolver >
    void DisplayAvailabilities( const Container& cont, QListView& viewer, const NameResolver& resolver, const char* szUnit  )
    {
//        int x = viewer.contentsX();
        int y = viewer.contentsY();
//        int w = viewer.contentsWidth();
//        int h = viewer.contentsHeight();
        viewer.clear();
        for( Container::const_iterator it = cont.begin(); it != cont.end(); ++it )
        {
            QString strEquipment = resolver.GetName( it->first ).c_str();
            QString strNumber    = QString( "%1 " ).arg( it->second ) + szUnit;
            new QListViewItem( &viewer, strEquipment, strNumber );
        }
        viewer.scrollBy( 0, y );
        viewer.show();
    }

    template< typename Container, typename NameResolver >
    void DisplayPriorities( const Container& cont, QListViewItem& viewer, const NameResolver& resolver )
    {
        QString strPriorities = cont.empty() ? tr( "Aucune" ) : "";
        for( Container::const_iterator it = cont.begin(); it != cont.end(); ++it )
        {
            strPriorities += ( it != cont.begin() ? " > '" : "'" );
            strPriorities +=  resolver.GetName( *it ).c_str();
            strPriorities += "'";
        }
        viewer.setText( 1, strPriorities );
    }
    //@}
};

#endif // __MOS_AgentLogisticPanel_ABC_h_
