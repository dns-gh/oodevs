// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __HumanDefs_h_
#define __HumanDefs_h_

namespace gui
{
    typedef std::map< std::string, unsigned int >  T_Motivations;
    typedef T_Motivations::const_iterator        CIT_Motivations;

    typedef std::map< QString, unsigned int >   T_BlockOccupation;
    typedef T_BlockOccupation::const_iterator CIT_BlockOccupation;

    struct T_Human
    {
        T_Human()
            : alerted_ ( false )
            , confined_ ( false )
            , evacuated_( false )
            , angriness_( 0.f )
        {
            // NOTHING
        }

        T_BlockOccupation persons_;
        bool alerted_;
        bool confined_;
        bool evacuated_;
        float angriness_;
    };
    typedef std::map< unsigned int, T_Human > T_HumansIdMap;
    typedef std::map< std::string, T_Human > T_HumansStrMap;
}

#endif // __HumanDefs_h_
