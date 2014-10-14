// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef __ADN_WizardPage_h_
#define __ADN_WizardPage_h_

// =============================================================================
/** @class  ADN_WizardPage
    @brief  ADN_WizardPage
*/
// Created: ABR 2012-07-27
// =============================================================================
template< typename T >
class ADN_WizardPage : public QWizardPage
{

public:
    typedef std::vector< T* > T_ItemVector;

public:
    //! @name Constructors/Destructor
    //@{
             ADN_WizardPage( const T_ItemVector& existingItems, const QString& pageTitle, QWidget* pParent = 0 );
    virtual ~ADN_WizardPage();
    //@}

    //! @name Operations
    //@{
    void Build();
    void SetCopyOf( const std::string& targetName );
    T* GetCreatedObject() const;
    //@}

protected:
    //! @name Virtual operations
    //@{
    virtual T* CreateObject();
    virtual T* NewT();
    virtual QWidget* CreateNameField();
    virtual QWidget* CreateOptionalField();
    virtual std::string GetName();
    virtual void ApplyOptions();
    //@}

    //! @name QWizardPage implementation
    //@{
    virtual bool validatePage();
    //@}

protected:
    //! @name Types
    //@{
    enum E_RadioButtons { eNew = 0, eCopy = 1 };
    //@}

    //! @name Helpers
    //@{
    virtual void showEvent( QShowEvent* pEvent );
    //@}

    //! @name Translation
    //@{
    void InitializeTranslation()
    {
        nameLabel_->setText( qApp->translate( "ADN_Wizard", "Name" ) );
        buttonGroup_->button( eNew )->setText( qApp->translate( "ADN_Wizard", "Create new" ) );
        buttonGroup_->button( eCopy )->setText( qApp->translate( "ADN_Wizard", "Create a copy of:" ) );
        errorTitle_ = qApp->translate( "ADN_Wizard", "Unable to create" );
        errorMsg_ = qApp->translate( "ADN_Wizard", "The provided name is either empty or already in use, please pick another one." );
        noSelectionMsg_ = qApp->translate( "ADN_Wizard", "Please select one element to copy from the list." );
    }
    //@}

protected:
    //! @name Member data
    //@{
    const T_ItemVector&     existingItems_;
    T*                      element_;
    std::string             copyName_;

    QLabel*                 nameLabel_;
    QLineEdit*              nameEdit_;
    QButtonGroup*           buttonGroup_;
    QListView*              view_;
    QStandardItemModel*     model_;
    QSortFilterProxyModel*  proxy_;

    QString                 errorTitle_;
    QString                 errorMsg_;
    QString                 noSelectionMsg_;
    //@}
};

#include "ADN_WizardPage.inl"

#endif // __ADN_WizardPage_h_
