// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __SymbolEditor_h_
#define __SymbolEditor_h_

#include "clients_kernel/ContextMenuObserver_ABC.h"
#include "clients_kernel/SafePointer.h"
#include "tools/SelectionObserver_ABC.h"

namespace kernel
{
    class Entity_ABC;
    class Formation_ABC;
    class Agent_ABC;
    class Automat_ABC;
    class Controllers;
    class SymbolFactory;
}

namespace gui
{
    class EntitySymbols;
}

namespace tools
{
    class ExerciseConfig;
}

// =============================================================================
/** @class  SymbolEditor
    @brief  Symbol editor
*/
// Created: LGY 2011-07-21
// =============================================================================
class SymbolEditor: public QObject
                  , public tools::Observer_ABC
                  , public kernel::ContextMenuObserver_ABC< kernel::Formation_ABC >
                  , public kernel::ContextMenuObserver_ABC< kernel::Automat_ABC >
                  , private boost::noncopyable
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             SymbolEditor( QWidget* parent, kernel::Controllers& controllers, const gui::EntitySymbols& symbols,
                           const tools::ExerciseConfig& config );
    virtual ~SymbolEditor();
    //@}

    //! @name Operations
    //@{
    virtual void NotifyContextMenu( const kernel::Formation_ABC& formation, kernel::ContextMenu& menu );
    virtual void NotifyContextMenu( const kernel::Automat_ABC& automat, kernel::ContextMenu& menu );
    //@}

private slots:
    //! @name Slots
    //@{
    void Update();
    void OnChangeSymbol( int id );
    //@}

private:
    //! @name Types
    //@{
    typedef std::map< std::string, QPixmap > T_Symbols;
    typedef std::map< int, std::string >   T_Identifier;
    typedef T_Identifier::const_iterator CIT_Identifier;
    //@}

private:
    //! @name Helpers
    //@{
    void Update( const kernel::Entity_ABC& entity, QPopupMenu* menu );
    void Update( const kernel::Entity_ABC& entity, kernel::ContextMenu& menu );
    bool IsValid( const T_Symbols& symbols ) const;
    void UpdateHierarchies( const kernel::Entity_ABC& entity );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    const gui::EntitySymbols& symbols_;
    std::auto_ptr< kernel::SymbolFactory > pFactory_;
    kernel::SafePointer< kernel::Entity_ABC > selected_;
    QPopupMenu* menu_;
    T_Identifier identifiers_;
    //@}
};

#endif // __SymbolEditor_h_
