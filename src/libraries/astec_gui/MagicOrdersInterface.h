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

#include "astec_kernel/ContextMenuObserver_ABC.h"
#include "astec_kernel/SafePointer.h"
#include "astec_kernel/OptionsObserver_ABC.h"
#include "ShapeHandler_ABC.h"
#include "astec_kernel/LocationVisitor_ABC.h"

class Agent_ABC;
class KnowledgeGroup_ABC;
class Team_ABC;
class Controllers;
class StaticModel;
class LogisticSupplyRecompletionDialog;
class ChangeHumanFactorsDialog;
class LocationCreator;
class ParametersLayer;
class Publisher_ABC;

// =============================================================================
/** @class  MagicOrdersInterface
    @brief  MagicOrdersInterface
*/
// Created: AGE 2006-04-28
// =============================================================================
class MagicOrdersInterface : public QObject
                           , public Observer_ABC
                           , public ContextMenuObserver_ABC< Team_ABC >
                           , public ContextMenuObserver_ABC< KnowledgeGroup_ABC >
                           , public ContextMenuObserver_ABC< Agent_ABC >
                           , public OptionsObserver_ABC
                           , public ShapeHandler_ABC
                           , private LocationVisitor_ABC
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             MagicOrdersInterface( QWidget* parent, Controllers& controllers, Publisher_ABC& publisher, const StaticModel& staticModel, ParametersLayer& layer );
    virtual ~MagicOrdersInterface();
    //@}

    //! @name Operations
    //@{
    virtual void NotifyContextMenu( const Team_ABC& agent, ContextMenu& menu );
    virtual void NotifyContextMenu( const KnowledgeGroup_ABC& agent, ContextMenu& menu );
    virtual void NotifyContextMenu( const Agent_ABC& agent, ContextMenu& menu );
    virtual void OptionChanged( const std::string& name, const OptionVariant& value );
    virtual void Handle( Location_ABC& location );
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
    void AddMagicMove( const Agent_ABC& agent, QPopupMenu* menu );
    void AddMagic( const QString& label, int id,           QPopupMenu* menu );
    int  AddMagic( const QString& label, const char* slot, QPopupMenu* menu );
    void ApplyOnHierarchy( const KnowledgeGroup_ABC& group, int id );
    void ApplyOnHierarchy( const Team_ABC& team, int id );
    void FillCommonOrders( QPopupMenu* magicMenu );

    virtual void VisitLines  ( const T_PointVector& ) {};
    virtual void VisitPolygon( const T_PointVector& ) {};
    virtual void VisitCircle ( const geometry::Point2f& , float ) {};
    virtual void VisitPoint  ( const geometry::Point2f& point );
    //@}

private:
    //! @name Member data
    //@{
    Controllers& controllers_;
    Publisher_ABC& publisher_;
    const StaticModel& static_;
    bool controller_;
    SafePointer< Agent_ABC > selectedAgent_;
    SafePointer< KnowledgeGroup_ABC > selectedGroup_;
    SafePointer< Team_ABC > selectedTeam_;
    LogisticSupplyRecompletionDialog* supplyRecompletion_;
    ChangeHumanFactorsDialog*         changeHumanFactors_;
    bool magicMove_;
    LocationCreator* magicMoveLocation_;
    //@}
};

#endif // __MagicOrdersInterface_h_
