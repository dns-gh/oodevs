// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

// -----------------------------------------------------------------------------
// Name: ADN_Wizard::CreateObject
// Created: ABR 2012-07-30
// -----------------------------------------------------------------------------
template< typename T, typename Page >
ADN_Ref_ABC* ADN_Wizard< T, Page >::CreateObject()
{
    Page* page = CreatePage();
    page->Build();
    addPage( page );

    int nResult = exec();
    if( nResult == QDialog::Rejected )
        return 0;
    return page->GetCreatedObject();
}

// -----------------------------------------------------------------------------
// Name: ADN_Wizard::CreateObjectAsCopyOf
// Created: ABR 2012-07-30
// -----------------------------------------------------------------------------
template< typename T, typename Page >
ADN_Ref_ABC* ADN_Wizard< T, Page >::CreateObjectAsCopyOf( void* data )
{
    Page* page = CreatePage();
    page->Build();
    addPage( page );

    T* item = static_cast< T* >( data );
    assert( item != 0 );
    page->SetCopyOf( item->strName_.GetData() );

    int nResult = exec();
    if( nResult == QDialog::Rejected )
        return 0;
    return page->GetCreatedObject();
}
