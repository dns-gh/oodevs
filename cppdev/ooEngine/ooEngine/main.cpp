
#include "tools.h"
#include "ErrorLogManager.h"
#include "ResourceManager.h"

void main()
{
    ErrorLogManager* errorManager = ErrorLogManager::GetInstance();
    errorManager->CreateLogFile( tools::GetModulePath() + std::string( "logError.txt" ) );

    ResourceManager* resourceManager = new ResourceManager();
    delete resourceManager;

    try
    {
        OOTHROW( 1, "Test error message" );
    }
    catch( cException& e )
    {
        MessageBox( NULL, e.what(), "", MB_OK );
        errorManager->LogException( e );
        errorManager->Flush();
    }
}