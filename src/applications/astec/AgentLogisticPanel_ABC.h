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
// $Archive: /MVW_v10/Build/SDK/Light2/src/AgentLogisticPanel_ABC.h $
// $Author: Nld $
// $Modtime: 27/04/05 18:27 $
// $Revision: 3 $
// $Workfile: AgentLogisticPanel_ABC.h $
//
// *****************************************************************************

#ifndef __AgentLogisticPanel_ABC_h_
#define __AgentLogisticPanel_ABC_h_

#include "InfoPanel_ABC.h"

class Agent;

// =============================================================================
/** @class  AgentLogisticPanel_ABC
    @brief  Agent logistic panel base class
*/
// Created: AGE 2005-04-01
// =============================================================================
class AgentLogisticPanel_ABC : public InfoPanel_ABC
{
    MT_COPYNOTALLOWED( AgentLogisticPanel_ABC )

public:
    //! @name Constructors/Destructor
    //@{
             AgentLogisticPanel_ABC( QWidget* pParent );
    virtual ~AgentLogisticPanel_ABC();
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
            int y = viewer.contentsY();
            viewer.clear();
            viewer.BeginList();
            for( Container::const_iterator it = cont.begin(); it != cont.end(); ++it )
                viewer.AddConsign( **it );
            viewer.EndList();
            viewer.scrollBy( 0, y );
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
    void DisplayLogAvailabilities( const Container& cont, QListView& viewer, const NameResolver& resolver )
    {
//        int x = viewer.contentsX();
        int y = viewer.contentsY();
//        int w = viewer.contentsWidth();
//        int h = viewer.contentsHeight();
        viewer.clear();
        for( Container::const_iterator it = cont.begin(); it != cont.end(); ++it )
        {
            QString strEquipment    = resolver.GetName( it->nEquipmentType_ ).c_str();
            QString strNbrTotal     = QString( "%1" ).arg( it->nNbrTotal_ );
            QString strNbrAvailable = QString( "%1" ).arg( it->nNbrAvailable_ );
            QString strNbrWorking   = QString( "%1" ).arg( it->nNbrWorking_ );
            QString strNbrLent      = QString( "%1" ).arg( it->nNbrLent_ );
            QString strNbrResting   = "-";
            if( it->nNbrResting_ != std::numeric_limits< uint >::max() )
                strNbrResting = QString( "%1" ).arg( it->nNbrResting_ );

            new QListViewItem( &viewer, strEquipment, strNbrTotal, strNbrAvailable, strNbrWorking, strNbrResting, strNbrLent );
        }
        viewer.scrollBy( 0, y );
        viewer.show();
    }

    /*
    pDispoReleveAmbulances_->addColumn( tr( "Total" ) );
    pDispoReleveAmbulances_->addColumn( tr( "Disponibles" ) );
    pDispoReleveAmbulances_->addColumn( tr( "Au travail" ) );
    pDispoReleveAmbulances_->addColumn( tr( "Au repos" ) );
    */

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

#endif // __AgentLogisticPanel_ABC_h_
