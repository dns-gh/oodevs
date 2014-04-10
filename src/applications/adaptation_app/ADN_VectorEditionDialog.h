// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#ifndef __ADN_VectorEditionDialog_h_
#define __ADN_VectorEditionDialog_h_

#include <boost/scoped_ptr.hpp>

template< typename T > class ADN_Type_Vector_ABC;

// =============================================================================
/** @class  ADN_VectorEditionDialog
    @brief  ADN_VectorEditionDialog
*/
// Created: ABR 2013-02-11
// =============================================================================
template< typename SourceType, typename TargetType >
class ADN_VectorEditionDialog : public QDialog
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit ADN_VectorEditionDialog( const QString& objectName,
                                      const QString& title,
                                      QWidget* parent = 0 );
    virtual ~ADN_VectorEditionDialog();
    //@}

    //! @name Types
    //@{
    typedef ADN_Type_Vector_ABC< SourceType > T_SourceVector;
    typedef std::function< bool ( const SourceType& ) > T_FilterFunctor;
    struct T_EditionInfo : private boost::noncopyable
    {
        T_EditionInfo( const QString& name, const T_SourceVector& source, ADN_Connector_Vector_ABC& targetConnector )
            : name_( name )
            , source_( source )
            , targetConnector_( targetConnector )
        {}

        virtual ~T_EditionInfo() {}

        const QString name_;
        const T_SourceVector& source_;
        ADN_Connector_Vector_ABC& targetConnector_;
    };

    typedef std::vector< std::auto_ptr< T_EditionInfo > > T_EditionInfos;
    //@}

    //! @name Operations
    //@{
    void AddVector( const QString& vectorName,
                    const T_SourceVector& sourceVector,
                    const QStandardItemModel& targetModel,
                    ADN_Connector_Vector_ABC& targetConnector,
                    const T_FilterFunctor& filterFunctor );
    //@}

private:
    //! @name QDialog
    //@{
    virtual void accept();
    virtual void setVisible( bool visible );
    void InitializeTranslation()
    {
        // Must be here cause .inl files aren't translated
        if( okButton_ )
            okButton_->setText( tools::translate( "ADN_VectorEditionDialog", "OK" ) );
        if( cancelButton_ )
            cancelButton_->setText( tools::translate( "ADN_VectorEditionDialog", "Cancel" ) );
    }
    //@}

private:
    //! @name Member data
    //@{
    T_EditionInfos editionInfos_;
    QTreeView* treeView_;
    boost::scoped_ptr< QStandardItemModel > dataModel_;
    boost::scoped_ptr< QSortFilterProxyModel > proxyModel_;
    QPushButton* okButton_;
    QPushButton* cancelButton_;
    //@}
};

#include "ADN_VectorEditionDialog.inl"

#endif // __ADN_VectorEditionDialog_h_
