// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef __ADN_Resources_WizardPage_h_
#define __ADN_Resources_WizardPage_h_

#include "ADN_WizardPage.h"
#include "ADN_Resources_Data.h"
#include "protocol/Protocol.h"

// =============================================================================
/** @class  ADN_Resources_WizardPage
    @brief  ADN_Resources_WizardPage
*/
// Created: ABR 2012-07-31
// =============================================================================
class ADN_Resources_WizardPage : public ADN_WizardPage< ADN_Resources_Data::CategoryInfo >
{

public:
    //! @name Constructors/Destructor
    //@{
             ADN_Resources_WizardPage( const T_ItemVector& existingItems, const QString& pageTitle, QWidget* pParent = 0, ADN_Resources_Data::ResourceInfos* parentDotation = 0, ADN_Resources_Data::T_ResourceInfos_Vector* allResources = 0 )
                 : ADN_WizardPage< ADN_Resources_Data::CategoryInfo >( existingItems, pageTitle, pParent )
                 , parentResource_( *parentDotation )
                 , allResources_( *allResources )
             {
                 // NOTHING
             }
    virtual ~ADN_Resources_WizardPage()
    {
        // NOTHING
    }
    //@}

    //! @name Operations
    //@{
    virtual ADN_Resources_Data::CategoryInfo* CreateObject()
    {
        element_ = 0;
        std::string strNewName = GetName();

        // Check if the name empty.
        if( strNewName.empty() )
        {
            QMessageBox::warning( this, errorTitle_, errorMsg_, QMessageBox::Ok, Qt::NoButton );
            return 0;
        }

        // Check if the name is not already used.
        for( auto resourceVector = allResources_.begin(); resourceVector != allResources_.end(); ++resourceVector )
        {
            for( auto it = ( *resourceVector )->categories_.begin(); it != ( *resourceVector )->categories_.end(); ++it )
                if( ( *it )->strName_.GetData() == strNewName )
                {
                    QMessageBox::warning( this, errorTitle_, errorMsg_, QMessageBox::Ok, Qt::NoButton );
                    return 0;
                }
        }

        // Create a new item or create a copy of an existing one.
        if( static_cast< QRadioButton* >( buttonGroup_->button( eCopy ) )->isOn() )
        {
            QItemSelection selection = proxy_->mapSelectionToSource( view_->selectionModel()->selection() );
            QModelIndexList indexes = selection.indexes();
            if( indexes.size() == 0 )
            {
                QMessageBox::warning( this, errorTitle_, noSelectionMsg_, QMessageBox::Ok, Qt::NoButton );
                return 0;
            }
            const ADN_Resources_Data::CategoryInfo* objectToCopy = static_cast< const ADN_Resources_Data::CategoryInfo* >( indexes[ 0 ].data( Qt::UserRole ).value< kernel::VariantPointer >().ptr_ );
            assert( objectToCopy );
            element_ = const_cast< ADN_Resources_Data::CategoryInfo* >( objectToCopy )->CreateCopy();
        }
        else
        {
            element_ = NewT();
        }

        // Set name and options
        if( element_ )
        {
            if( !element_->strName_.GetData().empty() )
                throw MASA_EXCEPTION( "Cloned elements should have an empty name before setting the new one" );
            element_->strName_ = strNewName;
        }
        ApplyOptions();
        return element_;
    }

    virtual ADN_Resources_Data::CategoryInfo* NewT()
    {
        if( parentResource_.nType_ == sword::dotation_type_ammunition ||
            parentResource_.nType_ == sword::dotation_type_mine ||
            parentResource_.nType_ == sword::dotation_type_explosive )
            return new ADN_Resources_Data::AmmoCategoryInfo( parentResource_ );
        else
            return new ADN_Resources_Data::CategoryInfo( parentResource_ );
    }
    //@}

private:
    //! @name Member data
    //@{
    ADN_Resources_Data::ResourceInfos& parentResource_;
    ADN_Resources_Data::T_ResourceInfos_Vector& allResources_;
    //@}
};

#endif // __ADN_Resources_WizardPage_h_
