// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Formation_h_
#define __Formation_h_

#include "Formation_ABC.h"
#include "protocol/SimulationSenders.h"
#include "ENT/ENT_Enums.h"

namespace sword
{
    enum EnumNatureLevel;
    class ChangeLogisticLinks;
    class FormationCreation;
    class FormationUpdate;
    class FormationChangeSuperior;
}

namespace dispatcher
{
    class Model_ABC;
    class Team_ABC;
    class LogisticEntity;

// =============================================================================
/** @class  Formation
    @brief  Formation
*/
// Created: NLD 2006-09-19
// =============================================================================
class Formation : public dispatcher::Formation_ABC
                , public kernel::Extension_ABC
                , public kernel::Updatable_ABC< sword::FormationUpdate >
                , public kernel::Updatable_ABC< sword::FormationChangeSuperior >
{
public:
    //! @name Constructors/Destructor
    //@{
             Formation( const Model_ABC& model, const sword::FormationCreation& msg );
    virtual ~Formation();
    //@}

    //! @name Accessors
    //@{
    virtual E_NatureLevel GetLevel() const;
    virtual dispatcher::Formation_ABC* GetParent() const;
    virtual dispatcher::Team_ABC& GetTeam() const;
    virtual const tools::Resolver< dispatcher::Formation_ABC >& GetFormations() const;
    virtual const tools::Resolver< dispatcher::Automat_ABC >& GetAutomates() const;
    virtual LogisticEntity_ABC* GetLogisticEntity() const;
    virtual bool GetExtension( const std::string& key, std::string& result ) const;
    virtual const std::string& GetApp6Symbol() const;
    //@}

    //! @name Operations
    //@{
    virtual void Send( sword::ParentEntity& msg ) const;

    virtual void SendCreation   ( ClientPublisher_ABC& publisher ) const;
    virtual void SendFullUpdate ( ClientPublisher_ABC& publisher ) const;
    virtual void SendDestruction( ClientPublisher_ABC& publisher ) const;

    virtual void DoUpdate( const sword::FormationUpdate& msg );
    virtual void DoUpdate( const sword::FormationChangeSuperior&  msg );

    virtual void Accept( kernel::ModelVisitor_ABC& visitor ) const;

    virtual void Register( dispatcher::Formation_ABC& formation );
    virtual void Remove( dispatcher::Formation_ABC& formation );
    virtual void Register( dispatcher::Automat_ABC& automat );
    virtual void Remove( dispatcher::Automat_ABC& automat );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    Formation( const Formation& );            //!< Copy constructor
    Formation& operator=( const Formation& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    template< typename Superior, typename Entity >
    void MoveChildren( Superior& superior, tools::Resolver< Entity >& entities );
    virtual void SetSuperior( dispatcher::Formation_ABC& superior );
    virtual void SetSuperior( dispatcher::Team_ABC& superior );
    //@}

private:
    //! @name Member data
    //@{
    const Model_ABC&                             model_;
    const std::string                            name_;
    dispatcher::Team_ABC&                        team_;
    E_NatureLevel                                level_;
    std::string                                  app6symbol_;
    sword::RgbColor                              color_;
    std::auto_ptr< LogisticEntity >              logisticEntity_;
    dispatcher::Formation_ABC*                   parent_;
    tools::Resolver< dispatcher::Formation_ABC > formations_;
    tools::Resolver< dispatcher::Automat_ABC >   automats_;
    std::map< std::string, std::string >         extensions_;
    bool                                         logMaintenanceManual_;
    bool                                         logSupplyManual_;
    //@}
};

}

#endif // __Formation_h_
