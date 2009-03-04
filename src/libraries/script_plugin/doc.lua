
-- Fonctions moteur

actions =
{
    IssueOrder    = function( self, strOrderName ) end,
    IssueXmlOrder = function( self, strXml ) end,
}

clients =
{
    SendCommandToAll = function( self, strProfile, strCommand ) end,
    SendCommand      = function( self, strClient, strProfile, strCommand ) end,
}

script =
{
    ChangePhase = function( self, strPhase ) end,
}

plugin =
{
    Reset = function( self ) end,
    Later = function( self, f ) end,
}

coord =
{
    ToUtm = function( self, position ) return strPosition end
}

sim =
{
    Pause            = function( self ) end,
    Resume           = function( self ) end,
    ChangeTimeFactor = function( self, nFactor ) end
    CreateUnit       = function( self, position, nType, nAutomat ) end
    Engage           = function( self, nAutomat, bEngage ) end
    Trace            = function( strMessage ) end
}

events =
{
    Once  = function( self ) end,
    Timer = function( self, time ) end,

    agents =
    {
         AgentEnters             = function( self, zone ) return condition( agent ) end,
         OperationalStateChanged = function( self ) return condition( agent ) end,
         MissionStarted          = function( self ) return condition( agent ) end,
         KnowledgeCreated        = function( self ) return condition( knowledge, agent ) end,
         KnowledgeCreatedIn      = function( self, zone ) return condition( knowledge, agent ) end,
         KnowledgeEnters         = function( self, zone ) return condition( knowledge, agent ) end,
         PerceptionChanged       = function( self ) return condition( knowledge, agent ) end,
    },

    automats =
    {
         MissionStarted          = function( self ) return condition( automat ) end,
    },
    
    populations =
    {
         PopulationEnters        = function( self, zone ) return condition( population ) end,
    },
    
    client =
    {
         UserChose      = function( self ) return condition( strId, strChoice ) end,
         MissionChosen  = function( self ) return condition( strMission, agent ) end,
         EntitySelected = function( self ) return condition( agent ) end
    },

    script =
    {
        PhaseChanged = function( self ) return condition( strPhase ) end
    },

    sim =
    {
        TickEnded       = function( self ) return condition end,
        ClientConnected = function( self ) return condition( strClient, strProfile ) end,
        ClientLeft      = function( self ) return condition( strClient ) end
    }
}

fsm =
{
    DeclareEvent = function( self, condition, states, callback ) end,
    CurrentState = function( self ) return strState end,
    ChangeState  = function( self, strState ) end,
    Deactivate   = function( self ) end,
}

-- thread safe fsm Helpers
ChangeState = function( state ) end
Deactivate  = function() end

agent =
{
    GetIdentifier       = function( self ) return nId end,
    GetName             = function( self ) return strName end,
    GetTeam             = function( self ) return strTeam end,
    GetPosition         = function( self ) return position end,
    GetOperationalState = function( self ) return nOp end,
}

automat =
{
    GetIdentifier       = function( self ) return nId end,
}

population =
{
    GetIdentifier       = function( self ) return nId end,
    GetName             = function( self ) return strName end,
}

knowledge =
{
    GetIdentifier       = function( self ) return nId end,
    GetPosition         = function( self ) return position end,
    GetOwnerTeam        = function( self ) return strTeam end,
}

drawings =
{
    Create  = function( self, strName ) return drawing end,
}

drawing =
{
    Publish        = function( self ) end,
    GetCoordinates = function( self ) return zone end
}

-- Helpers

xml =
{
    Serialize = function( tXml ) return strXml end
}

orders =
{
    Issue                  = function( tXml ) return actions:IssueXmlOrder( tXml ) end,
    ParameterLocationPoint     = function( position ) return tXml end,
    ParameterLocationPolygon   = function( zone ) return tXml end,
    ParameterLocationLine      = function( zone ) return tXml end,
    ParameterLocationPointList = function( zone ) return tXml end,
}

Command          = function( tParameters, strCommand ) end
Message          = function( tParameters ) end
Display          = function( tParameters ) end
Dialog           = function( tParameters ) end
Briefing         = function( tParameters ) end
PromptNextPhase  = function( tParameters ) end
Center           = function( tParameters ) end
Select           = function( tParameters ) end
SetFilter        = function( tParameters ) end
ClearFilter      = function( tParameters ) end
Zoom             = function( tParameters ) end
ChangeOptions    = function( tParameters ) end
ResetOptions     = function( tParameters ) end
SetDock          = function( tParameters ) end

StartTimeSequence = function( strSequence ) end
TimeSequenceEnd   = function( strSequence, callback ) return event end
WaitForUserPhase  = function( strPhase ) return event end
BeginPhase        = function( strPhase, callback ) return event end

DeclareEvents = function( tEvents ) end

MakePoint = function( position ) return point end
