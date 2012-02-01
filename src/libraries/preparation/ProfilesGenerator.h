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

#include <boost/noncopyable.hpp>

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
class ProfilesGenerator : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             ProfilesGenerator( const Model& model, ProfilesModel& profiles );
    virtual ~ProfilesGenerator();
    //@}

    //! @name Operations
    //@{
    void GenerateANIBAS();
    void GenerateENIEX();
    void GenerateSUPERVISOR();
    void GenerateSIDESUPERVISOR();
    void GenerateDIREX();
    void GenerateANALYSIS();
    void GenerateGESTIM();
    //@}

private:
    //! @name Helpers
    //@{
    void GenerateSUPERVISOR( const QString& name, const std::string& userRole, bool readonly );
    void GenerateLowLevelFormations( const kernel::Entity_ABC& entity, const std::string& userRole );
    //@}

private:
    //! @name Member data
    //@{
    const Model& model_;
    ProfilesModel& profiles_;
    //@}
};

#endif // __ProfilesGenerator_h_
