// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: AGE 2005-04-06 $
// $Archive: /MVW_v10/Build/SDK/Light2/src/LogConsignListView.h $
// $Author: Age $
// $Modtime: 28/04/05 18:45 $
// $Revision: 2 $
// $Workfile: LogConsignListView.h $
//
// *****************************************************************************

#ifndef __LogConsignListView_h_
#define __LogConsignListView_h_

// =============================================================================
/** @class  LogConsignListView
    @brief  LogConsignListView
*/
// Created: AGE 2005-04-06
// =============================================================================
template< typename Consign, typename ConsignItem >
class LogConsignListView : public QListView
{
public:
    //! @name Constructors/Destructor
    //@{
             LogConsignListView( QWidget* pParent, bool bHandledConsigns );
    virtual ~LogConsignListView();
    //@}

    //! @name Operations
    //@{
    void BeginList();
    void AddConsign( Consign& consign );
    void EndList();
    //@}

private:
    //! @name Copy/Assignement
    //@{
    LogConsignListView( const LogConsignListView& );            //!< Copy constructor
    LogConsignListView& operator=( const LogConsignListView& ); //!< Assignement operator
    //@}

private:
    //! @name Member data
    //@{
    ConsignItem* pCurrentItem_;
    //@}
};

// -----------------------------------------------------------------------------
// Name: LogConsignListView constructor
// Created: AGE 2005-04-06
// -----------------------------------------------------------------------------
template< typename Consign, typename ConsignItem >
LogConsignListView< Consign, ConsignItem >::LogConsignListView( QWidget* pParent, bool bHandledConsigns )
    : QListView        ( pParent, "Consignes" )
{
    if( bHandledConsigns )
        addColumn( tr ("Consignes en traitement" ) );
    else
        addColumn( tr ("Demandes logistiques" ) );
    setColumnWidthMode( 0, QListView::Maximum );
    setMargin( 5 );
    setLineWidth( 2 );
    setFrameStyle( QFrame::Sunken | QFrame::Box );    
}

// -----------------------------------------------------------------------------
// Name: LogConsignListView destructor
// Created: AGE 2005-04-06
// -----------------------------------------------------------------------------
template< typename Consign, typename ConsignItem >
LogConsignListView< Consign, ConsignItem >::~LogConsignListView()
{
    //NOTHING
}

// -----------------------------------------------------------------------------
// Name: LogConsignListView::BeginList
// Created: AGE 2005-04-06
// -----------------------------------------------------------------------------
template< typename Consign, typename ConsignItem >
void LogConsignListView< Consign, ConsignItem >::BeginList()
{
    pCurrentItem_ = static_cast< ConsignItem* >( firstChild() );
}
// -----------------------------------------------------------------------------
// Name: LogConsignListView::AddConsign
// Created: AGE 2005-04-06
// -----------------------------------------------------------------------------
template< typename Consign, typename ConsignItem >
void LogConsignListView< Consign, ConsignItem >::AddConsign( Consign& consign )
{
    if( pCurrentItem_ )
    {
        pCurrentItem_->Assign( consign );
        pCurrentItem_ = static_cast< ConsignItem* >( pCurrentItem_->nextSibling() );
    }
    else
        new ConsignItem( this, consign );
}

// -----------------------------------------------------------------------------
// Name: LogConsignListView::EndList
// Created: AGE 2005-04-06
// -----------------------------------------------------------------------------
template< typename Consign, typename ConsignItem >
void LogConsignListView< Consign, ConsignItem >::EndList()
{
    while( pCurrentItem_ )
    {
        ConsignItem* pItem = pCurrentItem_;
        pCurrentItem_ = static_cast< ConsignItem* >( pItem->nextSibling() );
        delete pItem;
    }
}

#endif // __LogConsignListView_h_
