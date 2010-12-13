// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __crossbow_ObjectAttributeUpdater_h_
#define __crossbow_ObjectAttributeUpdater_h_

namespace sword
{
    class ObjectAttributes;
    class ObjectAttributeConstruction;
    class ObjectAttributeObstacle;
    class ObjectAttributeMine;
    class ObjectAttributeActivityTime;
    class ObjectAttributeBypass;
    class ObjectAttributeLogistic;
    class ObjectAttributeNBC;
    class ObjectAttributeCrossingSite;
    class ObjectAttributeSupplyRoute;
    class ObjectAttributeToxicCloud;
    class ObjectAttributeFire;
    class ObjectAttributeMedicalTreatment;
    class ObjectAttributeInteractionHeight;
    class ObjectAttributeNBCType;
    class ObjectAttributeStock;
}

namespace plugins
{
namespace crossbow
{
    class Workspace_ABC;
    class Table_ABC;
    class WorkingSession_ABC;

class ObjectAttributeUpdater
{
public:
    //! @name Constructor / desctructor
    //@{
     ObjectAttributeUpdater( Workspace_ABC& workspace, const WorkingSession_ABC& session, long objectId );
    ~ObjectAttributeUpdater();
    //@}

    //! @name Updater
    //@{
    void Update( const sword::ObjectAttributes& msg );
    //@}

private:
    //! @name Copy/Assignement
    //@{
    ObjectAttributeUpdater( const ObjectAttributeUpdater& );            //!< Copy constructor
    ObjectAttributeUpdater& operator=( const ObjectAttributeUpdater& ); //!< Assignement operator
    //@}

    //! @name Attribute updater
    //@{
    void Update( const sword::ObjectAttributeConstruction& construction );
    void Update( const sword::ObjectAttributeObstacle& obstacle );
    void Update( const sword::ObjectAttributeMine& mine );
    void Update( const sword::ObjectAttributeActivityTime& activity_time );
    void Update( const sword::ObjectAttributeBypass& bypass );
    void Update( const sword::ObjectAttributeLogistic& logistic );
    void Update( const sword::ObjectAttributeNBC& nbc );
    void Update( const sword::ObjectAttributeCrossingSite& crossing_site );
    void Update( const sword::ObjectAttributeSupplyRoute& supply_route );
    void Update( const sword::ObjectAttributeToxicCloud& toxic_cloud );
    void Update( const sword::ObjectAttributeFire& fire );
    void Update( const sword::ObjectAttributeMedicalTreatment& medical_treatment );
    void Update( const sword::ObjectAttributeInteractionHeight& interaction_height );
    void Update( const sword::ObjectAttributeNBCType& nbc_agent );
    void Update( const sword::ObjectAttributeStock& stock );
    //@}

    //! @name
    //@{
    Table_ABC* OpenTable( const std::string& name );
    //@}

private:
    Workspace_ABC&              workspace_;
    const WorkingSession_ABC&   session_;
    long           objectId_;
};

}
}
#endif // __crossbow_ObjectAttributeUpdater_h_
