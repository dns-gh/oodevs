// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __MagicOrdersInterface_h_
#define __MagicOrdersInterface_h_

#include "ContextMenuObserver_ABC.h"
#include "SafePointer.h"
#include "OptionsObserver_ABC.h"

class Agent;
class Controllers;
class LogisticSupplyRecompletionDialog;
class ChangeHumanFactorsDialog;

// =============================================================================
/** @class  MagicOrdersInterface
    @brief  MagicOrdersInterface
*/
// Created: AGE 2006-04-28
// =============================================================================
class MagicOrdersInterface : public QObject
                           , public Observer_ABC
                           , public ContextMenuObserver_ABC< Agent >
                           , public OptionsObserver_ABC
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             MagicOrdersInterface( QWidget* parent, Controllers& controllers );
    virtual ~MagicOrdersInterface();
    //@}

    //! @name Operations
    //@{
    virtual void NotifyContextMenu( const Agent& agent, QPopupMenu& menu );
    virtual void OptionChanged( const std::string& name, const OptionVariant& value );
    //@}

private slots:
    //! @name Slots
    //@{
    void Magic( int );
    void Recompletion();
    void DestroyComponent();
    void ChangeHumanFactors();
    void Move();
    void Surrender();
    void RecoverHumanTransporters();
    //@}

private:
    //! @name Copy/Assignement
    //@{
    MagicOrdersInterface( const MagicOrdersInterface& );            //!< Copy constructor
    MagicOrdersInterface& operator=( const MagicOrdersInterface& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    void AddMagicMove( const Agent& agent, QPopupMenu* menu );
    void AddMagic( const QString& label, int id,           QPopupMenu* menu );
    int  AddMagic( const QString& label, const char* slot, QPopupMenu* menu );
    //@}

private:
    //! @name Member data
    //@{
    Controllers& controllers_;
    bool controller_;
    SafePointer< Agent > selectedAgent_;
    LogisticSupplyRecompletionDialog* supplyRecompletion_;
    ChangeHumanFactorsDialog*         changeHumanFactors_;
    //@}
};

#endif // __MagicOrdersInterface_h_
