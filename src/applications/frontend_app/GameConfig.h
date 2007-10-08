// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __GameConfig_h_
#define __GameConfig_h_

namespace tools
{
    class GeneralConfig;
}

// =============================================================================
/** @class  GameConfig
    @brief  GameConfig
*/
// Created: AGE 2007-10-08
// =============================================================================
class GameConfig
{

public:
    //! @name Constructors/Destructor
    //@{
             GameConfig( const tools::GeneralConfig& config, const std::string& exercise );
    virtual ~GameConfig();
    //@}

    //! @name Operations
    //@{
    void Commit();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    GameConfig( const GameConfig& );            //!< Copy constructor
    GameConfig& operator=( const GameConfig& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    //@}

private:
    //! @name Member data
    //@{
    const tools::GeneralConfig& config_;
    const std::string exercise_;
    //@}
};

#endif // __GameConfig_h_
