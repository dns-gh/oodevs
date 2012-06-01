// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __ModelConsistencyDialog_h_
#define __ModelConsistencyDialog_h_

#include <boost/noncopyable.hpp>
#include <map>
#include "preparation/ModelConsistencyChecker.h"

namespace kernel
{
    class ActionController;
    class Controllers;
    class Entity_ABC;
}

namespace tools
{
    class RealFileLoaderObserver_ABC;
}

// =============================================================================
/** @class  VariantPointer
    @brief  VariantPointer
*/
// Created: ABR 2011-09-23
// =============================================================================
struct VariantPointer
{
    VariantPointer() : ptr_( 0 ) {}
    VariantPointer( const VariantPointer& other ) : ptr_( other.ptr_ ) {}
    VariantPointer( const void* ptr ) : ptr_( ptr ) {}

    const void* ptr_;
};
Q_DECLARE_METATYPE( VariantPointer );

class FilterProxyModel;

// =============================================================================
/** @class  ModelConsistencyDialog
    @brief  ModelConsistencyDialog
*/
// Created: ABR 2011-09-23
// =============================================================================
class ModelConsistencyDialog : public QDialog
                             , private boost::noncopyable
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             ModelConsistencyDialog( QWidget* parent, Model& model, const StaticModel& staticModel, kernel::Controllers& controllers, const tools::RealFileLoaderObserver_ABC& fileLoaderObserver );
    virtual ~ModelConsistencyDialog();
    //@}

    //! @name Public
    //@{
    //@}

public slots:
    //! @name Slots
    //@{
    void CheckConsistency();
    void Display();
    //@}

private:
    //! @name Enums
    //@{
    enum E_Column { eIcon = 0, eID = 1, eName = 2, eDescription = 3 };
    //@}

    //! @name Types
    //@{
    typedef std::map< unsigned int, QString > T_Types;
    //@}

    //! @name Helpers
    //@{
    void CreateCheckbox( QGridLayout& layout, const T_Types& names );
    void UpdateDataModel();
    void AddIcon( const kernel::SafePointer< kernel::Entity_ABC >& entity, E_ConsistencyCheck type, QList< QStandardItem* >& items );
    template< typename T >
    void AddItem( T data, QString text, const kernel::SafePointer< kernel::Entity_ABC >& entity,
                  E_ConsistencyCheck type, QList< QStandardItem* >& items );
    //@}

signals:
    //! @name Signals
    //@{
    void ClearLoadingErrors();
    //@}

private slots:
    //! @name Slots
    //@{
    void OnRefresh();
    void OnClose();
    void OnSelectionChanged( const QModelIndex& );
    void OnFilterChanged( int type );
    void OnLevelChanged();
    //@}

private:
    //! @name Member data
    //@{
    kernel::ActionController&         actionController_;
    ModelConsistencyChecker           checker_;
    QTableView*                       tableView_;
    QCheckBox*                        warningCheckBox_;
    QCheckBox*                        errorCheckBox_;
    QStandardItemModel*               dataModel_;
    FilterProxyModel*                 proxyModel_;
    QStringList                       horizontalHeaders_;
    std::map< unsigned int, QString > errorDescriptions_;
    QSignalMapper*                    pMapper_;
    //@}
};

#endif // __ModelConsistencyDialog_h_
