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
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MOS_LogConsignListView.h $
// $Author: Age $
// $Modtime: 28/04/05 18:45 $
// $Revision: 2 $
// $Workfile: MOS_LogConsignListView.h $
//
// *****************************************************************************

#ifndef __MOS_LogConsignListView_h_
#define __MOS_LogConsignListView_h_

// =============================================================================
/** @class  MOS_LogConsignListView
    @brief  MOS_LogConsignListView
*/
// Created: AGE 2005-04-06
// =============================================================================
template< typename Consign, typename ConsignItem >
class MOS_LogConsignListView : public QListView
{
public:
    //! @name Constructors/Destructor
    //@{
             MOS_LogConsignListView( QWidget* pParent, bool bHandledConsigns );
    virtual ~MOS_LogConsignListView();
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
    MOS_LogConsignListView( const MOS_LogConsignListView& );            //!< Copy constructor
    MOS_LogConsignListView& operator=( const MOS_LogConsignListView& ); //!< Assignement operator
    //@}

private:
    //! @name Member data
    //@{
    ConsignItem* pCurrentItem_;
    //@}
};

// -----------------------------------------------------------------------------
// Name: MOS_LogConsignListView constructor
// Created: AGE 2005-04-06
// -----------------------------------------------------------------------------
template< typename Consign, typename ConsignItem >
MOS_LogConsignListView< Consign, ConsignItem >::MOS_LogConsignListView( QWidget* pParent, bool bHandledConsigns )
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
// Name: MOS_LogConsignListView destructor
// Created: AGE 2005-04-06
// -----------------------------------------------------------------------------
template< typename Consign, typename ConsignItem >
MOS_LogConsignListView< Consign, ConsignItem >::~MOS_LogConsignListView()
{
    //NOTHING
}

// -----------------------------------------------------------------------------
// Name: MOS_LogConsignListView::BeginList
// Created: AGE 2005-04-06
// -----------------------------------------------------------------------------
template< typename Consign, typename ConsignItem >
void MOS_LogConsignListView< Consign, ConsignItem >::BeginList()
{
    pCurrentItem_ = static_cast< ConsignItem* >( firstChild() );
}
// -----------------------------------------------------------------------------
// Name: MOS_LogConsignListView::AddConsign
// Created: AGE 2005-04-06
// -----------------------------------------------------------------------------
template< typename Consign, typename ConsignItem >
void MOS_LogConsignListView< Consign, ConsignItem >::AddConsign( Consign& consign )
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
// Name: MOS_LogConsignListView::EndList
// Created: AGE 2005-04-06
// -----------------------------------------------------------------------------
template< typename Consign, typename ConsignItem >
void MOS_LogConsignListView< Consign, ConsignItem >::EndList()
{
    while( pCurrentItem_ )
    {
        ConsignItem* pItem = pCurrentItem_;
        pCurrentItem_ = static_cast< ConsignItem* >( pItem->nextSibling() );
        delete pItem;
    }
}

#endif // __MOS_LogConsignListView_h_
