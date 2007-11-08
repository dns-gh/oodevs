// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ProfilesGenerator_h_
#define __ProfilesGenerator_h_

namespace kernel
{
    class Entity_ABC;
}

class Model;
class ProfilesModel;

// =============================================================================
/** @class  ProfilesGenerator
    @brief  ProfilesGenerator
*/
// Created: SBO 2007-11-07
// =============================================================================
class ProfilesGenerator
{

public:
    //! @name Constructors/Destructor
    //@{
             ProfilesGenerator( const Model& model, ProfilesModel& profiles );
    virtual ~ProfilesGenerator();
    //@}

    //! @name Operations
    //@{
    void GenerateTeams( bool readonly );
    void GenerateTopLevelFormations( bool readonly );
    void GenerateFormations( bool readonly );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ProfilesGenerator( const ProfilesGenerator& );            //!< Copy constructor
    ProfilesGenerator& operator=( const ProfilesGenerator& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    void GenerateProfile( const QString& name, const kernel::Entity_ABC& entity, bool readonly );
    //@}

private:
    //! @name Member data
    //@{
    const Model& model_;
    ProfilesModel& profiles_;
    //@}
};

#endif // __ProfilesGenerator_h_
