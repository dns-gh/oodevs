// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef __ADN_Wizard_Default_h_
#define __ADN_Wizard_Default_h_

#include <boost/noncopyable.hpp>
#include "ADN_Wizard_ABC.h"
#include "ADN_Wizard_FirstPage_Default.h"
#include "ADN_Wizard_LastPage.h"

template< typename T > class ADN_Type_Vector_ABC;

// =============================================================================
/** @class  ADN_Wizard_Default
    @brief  ADN_Wizard_Default
*/
// Created: ABR 2012-07-25
// =============================================================================
template< typename T >
class ADN_Wizard_Default : public ADN_Wizard_ABC< T >
{

public:
    //! @name Constructors/Destructor
    //@{
             ADN_Wizard_Default( const QString& elementName, const QString& columnName, ADN_Type_Vector_ABC< T >& vector, QWidget* pParent = 0, const char* szName = 0 )
                 : ADN_Wizard_ABC< T >( pParent, szName )
             {
                 setCaption( qApp->translate( "ADN_Wizard_Default", "New '%1' creation" ).arg( elementName.toLower() ) );
                 ADN_Wizard_FirstPage_Default< T >* pFirstPage = new ADN_Wizard_FirstPage_Default< T >( vector, this );
                 pFirstPage->SetCaptions( caption(), columnName );
                 pFirstPage_ = pFirstPage;
                 new ADN_Wizard_LastPage( this, qApp->translate( "ADN_Wizard_Default", "Creation finished" ),
                                                qApp->translate( "ADN_Wizard_Default", "Click \"Done\" to create the new %1." ).arg( elementName.toLower() ) );
             }

    virtual ~ADN_Wizard_Default() {}
    //@}
};

#endif // __ADN_Wizard_Default_h_
