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
    typedef std::map< std::string, unsigned int > T_Usages;
    typedef T_Usages::const_iterator            CIT_Usages;

    typedef std::map< QString, std::pair< unsigned int, unsigned int > >   T_BlockOccupation; // pair< occupation, taux d'occupation >
    typedef T_BlockOccupation::iterator                                   IT_BlockOccupation;
    typedef T_BlockOccupation::const_iterator                            CIT_BlockOccupation;

    struct T_Human
    {
        T_Human()
            : alerted_  ( false )
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
