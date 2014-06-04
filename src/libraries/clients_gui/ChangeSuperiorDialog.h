// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __ChangeSuperiorDialog_h_
#define __ChangeSuperiorDialog_h_

//#include "clients_kernel/SafePointer.h"
#include <tools/ElementObserver_ABC.h>
#include <tools/SelectionObserver_ABC.h>
#include <boost/optional.hpp>

namespace kernel
{
    class Controllers;
    class Entity_ABC;
}

namespace gui
{
    class ChangeLogisticLinksWidget;
    class EntitySymbols;
    class RichPushButton;
    class TaskerWidget;

// =============================================================================
/** @class  ChangeSuperiorDialog
    @brief  ChangeSuperiorDialog
*/
// Created: JSR 2011-11-08
// =============================================================================
class ChangeSuperiorDialog : public QDialog
                           , public tools::Observer_ABC
                           , public tools::ElementObserver_ABC< kernel::Entity_ABC >
{
    Q_OBJECT

public:
    //! @name Types
    //@{
    enum E_Options
    {
        eTacticalSuperior           = 0x01,
        eKnowledgeGroup             = 0x02,
        eLogisticSuperior           = 0x04,

        eOptionalTacticalSuperior   = 0x01 << 4,
        eOptionalKnowledgeGroup     = 0x02 << 4,
        eOptionalLogisticSuperior   = 0x04 << 4
    };

    typedef std::function< bool( const kernel::Entity_ABC& /* entity */,
                                 const kernel::Entity_ABC& /* superior */ ) > T_CanChangeFunctor;
    typedef std::function< void( kernel::Entity_ABC& /* entity */,
                                 const kernel::Entity_ABC& /* superior */ ) > T_DoChangeFunctor;
    typedef std::function< const kernel::Entity_ABC* ( kernel::Entity_ABC& /* entity */ ) > T_LogisticSuperiorExtractor;
    typedef std::function< void( kernel::Entity_ABC& /* entity */,
                                 const kernel::Entity_ABC* /* nominal superior */,
                                 const kernel::Entity_ABC* /* current superior */ ) > T_DoChangeLogisticFunctor;

    struct ChangeSuperiorFunctors
    {
        ChangeSuperiorFunctors( T_CanChangeFunctor canChange,
                                T_DoChangeFunctor doChange )
            : canChange_( canChange )
            , doChange_( doChange )
        {}
        T_CanChangeFunctor canChange_;
        T_DoChangeFunctor doChange_;
    };
    typedef boost::optional< ChangeSuperiorFunctors > T_OptionalFunctors;

    struct ChangeLogisticFunctors
    {
        ChangeLogisticFunctors( T_LogisticSuperiorExtractor extract,
                                T_DoChangeLogisticFunctor doChange )
            : extract_( extract )
            , doChange_( doChange )
        {}
        T_LogisticSuperiorExtractor extract_;
        T_DoChangeLogisticFunctor doChange_;
    };
    //@}

public:
    //! @name Constructors/Destructor
    //@{
             ChangeSuperiorDialog( kernel::Controllers& controllers,
                                   const gui::EntitySymbols& symbols,
                                   QWidget* parent = 0 );
    virtual ~ChangeSuperiorDialog();
    //@}

    //! @name Operations
    //@{
    void Show( kernel::Entity_ABC& entity, const QString& title, int options = eTacticalSuperior );
    void SetTacticalSuperiorFunctors( const T_CanChangeFunctor& canChange,
                                      const T_DoChangeFunctor& doChange );
    void SetKnowledgeGroupFunctors( const T_CanChangeFunctor& canChange,
                                    const T_DoChangeFunctor& doChange );
    void SetLogisticCurrentExtractor( const T_LogisticSuperiorExtractor& currentExtractor );
    void SetLogisticNominalFunctors( const T_LogisticSuperiorExtractor& nominalExtractor,
                                     const T_DoChangeLogisticFunctor& doChange );
    //@}

public slots:
    //! @name Slots
    //@{
    void OnTacticalSelectionChanged( const std::vector< const kernel::Entity_ABC* >& element );
    void OnKnowledgeGroupSelectionChanged( const std::vector< const kernel::Entity_ABC* >& element );
    //@}

private:
    //! @name Helpers
    //@{
    void Clear();
    virtual void closeEvent( QCloseEvent* e );
    virtual void NotifyDeleted( const kernel::Entity_ABC& entity );
    //@}

private slots:
    //! @name Slots
    //@{
    virtual void accept();
    virtual void reject();
    void OnContentChanged();
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;

    //kernel::SafePointer< kernel::Entity_ABC > entity_;
    T_OptionalFunctors tacticalSuperiorFunctors_;
    T_OptionalFunctors knowledgeGroupFunctors_;

    boost::optional< T_LogisticSuperiorExtractor > logisticCurrentExtractor_;
    boost::optional< ChangeLogisticFunctors > logisticNominalFunctors_;
    
    //QLabel* name_;
    RichPushButton* okButton_;
    gui::TaskerWidget* currentEntity_;
    gui::TaskerWidget* tacticalSuperior_;
    gui::TaskerWidget* knowledgeGroup_;
    QGroupBox* logisticSuperior_;
    ChangeLogisticLinksWidget* logisticWidget_;
    //@}
};

}

#endif // __ChangeSuperiorDialog_h_
