// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __Exercise_h_
#define __Exercise_h_

namespace Common
{
    class MsgExercise;
}

namespace frontend
{

// =============================================================================
/** @class  Exercise
    @brief  Exercise
*/
// Created: SBO 2010-10-01
// =============================================================================
class Exercise
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit Exercise( const Common::MsgExercise& message );
    virtual ~Exercise();
    //@}

    //! @name Accessors
    //@{
    std::string GetName() const;
    unsigned int GetPort() const;
    bool IsRunning() const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    Exercise( const Exercise& );            //!< Copy constructor
    Exercise& operator=( const Exercise& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    const std::string name_;
    const unsigned int port_;
    bool running_;
    //@}
};

}

#endif // __Exercise_h_
