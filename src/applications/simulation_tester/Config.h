// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __Config_h_
#define __Config_h_

#include "Types.h"

namespace TEST {

// =============================================================================
/** @class  Config
    @brief  Config
*/
// Created: SBO 2005-08-24
// =============================================================================
class Config
{
public:
    //! @name Constructors/Destructor
    //@{
             Config( const std::string& strFile );
    virtual ~Config();
    //@}

    //! @name Accessors
    //@{
    const std::string& GetServer            () const;
    const std::string& GetConfigFile        () const;
    const std::string& GetRandomSeedFile    () const;
    const std::string& GetTacticalLinesFile () const;
    bool               MustRecover          () const;
    unsigned int               GetRecoveryTick      () const;
    const std::string& GetRecoveryFile      () const;
    unsigned int               GetPort              () const;
    unsigned int               GetTimeFactor        () const;
    bool               MustTestAutomats     () const;
    bool               MustTestPopulations  () const;
    unsigned int               GetPeriod            () const;
    unsigned int               GetIterationNumber   () const;
    unsigned int               GetIterationInterval () const;
    unsigned int               GetMaxMissionPerTick () const;
    unsigned int               GetRecompletionPeriod() const;
    const std::string& GetLogFile           () const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    Config( const Config& );            //!< Copy constructor
    Config& operator=( const Config& ); //!< Assignment operator
    //@}

    //! @name Operations
    //@{
    void LoadConfigFile  ( const std::string& strConfigFile );
    void LoadRecoveryFile( const std::string& strConfigFile );
    //@}

private:
    //! @name Enum
    //@{
    enum E_TestableEntities
    {
        ePawns,
        eAutomats,
        ePopulations
    };
    //@}

private:
    //! @name Member data
    //@{
    std::string        strServer_;
    std::string        strConfigFile_;
    std::string        strRandomSeedFile_;
    std::string        strTacticalLinesFile_;
    bool               bRecover_;
    unsigned int               nRecoveryTick_;
    std::string        strRecoveryFile_;
    unsigned int               nPort_;
    unsigned int               nTimeFactor_;
    unsigned int               nPeriod_;
    unsigned int               nItNumber_;
    unsigned int               nItInterval_;
    unsigned int               nMaxMissionPerTick_;
    unsigned int               nRecompletionPeriod_;
    std::string        strLogFile_;
    E_TestableEntities eTestedEntities_;
    //@}
};

} // end namespace TEST

#include "Config.inl"

#endif // __Config_h_
