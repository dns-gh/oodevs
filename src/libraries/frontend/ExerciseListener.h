// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 MASA Group
//
// *****************************************************************************

#ifndef __ExerciseListener_h_
#define __ExerciseListener_h_

namespace tools
{
    class GeneralConfig;
}


namespace frontend
{

// =============================================================================
/** @class  ExerciseListener
    @brief  ExerciseListener
*/
// Created: RDS 2008-07-23
// =============================================================================
class ExerciseListener
{

public:
    //! @name Constructors/Destructor
    //@{
    ExerciseListener( const std::string& strHost, unsigned int nPort );
    virtual ~ExerciseListener();
    //@}

    //! @name Operations
    //@{
	bool Wait(); 
    unsigned int GetPercentage() ;     
    //@}



private:
    //! @name Copy/Assignment
    //@{
    ExerciseListener( const ExerciseListener& );            //!< Copy constructor
    ExerciseListener& operator=( const ExerciseListener& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
	const std::string   strHost_ ; 
	const unsigned int  nPort_ ; 
    unsigned int percentage_ ; 
    //@}
};

}

#endif // __ExerciseListener_h_
