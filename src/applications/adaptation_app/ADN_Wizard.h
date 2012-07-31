// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef __ADN_Wizard_h_
#define __ADN_Wizard_h_

#include <boost/noncopyable.hpp>
#include "ADN_ObjectCreator_ABC.h"
#include "ADN_WizardPage.h"

template< typename T > class ADN_Type_Vector_ABC;

// =============================================================================
/** @class  ADN_Wizard
    @brief  ADN_Wizard
*/
// Created: ABR 2012-07-30
// =============================================================================
template< typename T, typename Page = ADN_WizardPage< T > >
class ADN_Wizard : public QWizard
                 , public ADN_ObjectCreator_ABC
                 , private boost::noncopyable
{

public:
    //! @name Constructors/Destructor
    //@{
             ADN_Wizard( const QString& elementName, ADN_Type_Vector_ABC< T >& vector, QWidget* pParent = 0 )
                 : QWizard( pParent )
                 , title_( qApp->translate( "ADN_Wizard_Default", "%1: Creation of a new element" ).arg( elementName ) )
                 , vector_( vector )
             {
                 setCaption( qApp->translate( "ADN_Wizard_Default", "Creation helper" ) );
                 setMinimumWidth( 600 );
                 setWizardStyle( QWizard::ClassicStyle );
                 setOptions( QWizard::NoBackButtonOnStartPage );
             }
    virtual ~ADN_Wizard() {}
    //@}

    //! @name ADN_ObjectCreator_ABC
    //@{
    virtual ADN_Ref_ABC* CreateObject();
    virtual ADN_Ref_ABC* CreateObjectAsCopyOf( void* );
    //@}

private:
    //! @name virtual helpers
    //@{
    virtual Page* CreatePage()
    {
        return new Page( vector_, title_, this );
    }
    //@}

protected:
    //! @name Member data
    //@{
    const QString title_;
    ADN_Type_Vector_ABC< T >& vector_;
    //@}
};

#include "ADN_Wizard.inl"

#endif // __ADN_Wizard_h_
