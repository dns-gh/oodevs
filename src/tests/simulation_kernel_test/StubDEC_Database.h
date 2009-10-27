#ifndef __StubDEC_Database_h_
#define __StubDEC_Database_h_

#include "simulation_kernel/Decision/DEC_DataBase.h"

class StubDEC_Database : public DEC_DataBase
{
public:
    explicit StubDEC_Database() : DEC_DataBase( std::vector< std::string >(),std::vector< const std::string >() ){}
    virtual ~StubDEC_Database() {}
};

#endif
