
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
    ToUtm = function( self, position ) return strPosition end,
    UtmPosition = function( self, strUtm ) return position end
}

sim =
{
    Pause            = function( self ) end,
    Resume           = function( self ) end,
    ChangeTimeFactor = function( self, nFactor ) end,
    CreateUnit       = function( self, position, nType, nAutomat ) end,
    Engage           = function( self, nAutomat, bEngage ) end,
}

events =
{
    Once  = function( self ) end,
    Timer = function( self, time ) end,

    agents =
    {
         AgentCreated            = function( self ) return condition( agent ) end,
         AgentEnters             = function( self, zone ) return condition( agent ) end,
         OperationalStateChanged = function( self ) return condition( agent ) end,
         ForceRatioChanged       = function( self ) return condition( agent ) end,
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
        TickEnded       = function( self ) return condition( tick, duration ) end,
        ClientConnected = function( self ) return condition( strClient, strProfile ) end,
        ClientLeft      = function( self ) return condition( strClient ) end
        AtDateTime      = function( self, dateTime ) return condition( dateTime ) end -- event triggered at specified simulation dateTime (format YYYYMMDDTHHMMSS )
    },

    indicators =
    {
        IndicatorChanged = function( self ) return condition( strName, nValue ) end
    }
}

fsm =
{
    DeclareEvent = function( self, condition, states, callback ) end,
    CurrentState = function( self ) return strState end,
    ChangeState  = function( self, strState ) end,
    Deactivate   = function( self ) end,
}

AtState     = function( state, func ) end

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
    GetForceRatio       = function( self ) return strForceRatio end, -- force ratio string: "none", "neutral", "low" or "high"
    GetMission            = function( self ) return nMissionId end,
    Teleport            = function( self, position ) end, -- teleports unit to new position (type Position)
    RecoverAll          = function( self ) end, -- recovers unit (equipments, humans and resources)
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

profiles =
{
    SetAutomatRight = function( profile, automat, readOnly, readWrite ) end
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

Indicator = {} -- class
Indicator.create  = function( name, formula ) return newIndicator end   -- Constructor
Indicator:With    = function( self, name, type, value ) return self end -- Add variable
Indicator:Compute = function( self ) end                                -- Start computation
Indicator:Record  = function( self ) end                                -- Add indicator to recorder

Recorder =
{
    Start  = function() end,                    -- starts recorder (changes state)
    Reset  = function() end,                    -- resets recorder data
    Record = function( ticks, next_state ) end, -- creates fsm recording states, goto "next_state" after "ticks" simulation ticks
    Save   = function( filename ) end,          -- saves recorded data to filename
    Track  = function( indicatorName ) end,     -- add indicator to be recorded (used by Indicator:Record() )
}

-- tParameters: { profile = "my profile" } [optional] --> limits diffusion of message to specified profile (from sim:ClientConnected)
-- tParameters: { client  = "my client"  } [optional] --> limits diffusion of message to specified client (from sim:ClientConnected)

Trace            = function( strMessage ) end
Command          = function( tParameters, strCommand ) end
Message          = function( tParameters ) end
Display          = function( tParameters ) end -- display client side message
ClearDisplay     = function( tParameters ) end -- clears client side displayed message
Dialog           = function( tParameters ) end
Briefing         = function( tParameters ) end
Launch           = function( tParameters ) end
PromptNextPhase  = function( tParameters ) end

-- id://protocol/value
-- protocol: (agent|automat|object|formation|population|team|tacticalLine|agentKnowledge|populationKnowledge|objectKnowledge|knowledgeGroup)
-- ex: id://agent/10
Select           = function( tParameters ) end
SetFilter        = function( tParameters ) end
Center           = function( tParameters ) end -- id://protocol/value or loc://utmCoordinates

ClearFilter      = function( tParameters ) end
Zoom             = function( tParameters ) end
ChangeOptions    = function( tParameters ) end
ResetOptions     = function( tParameters ) end
SetDock          = function( tParameters ) end

StartTimeSequence = function( strSequence ) end
TimeSequence      = function( strSequence, seconds, callback ) return event end
TimeSequenceEnd   = function( strSequence, callback ) return event end
WaitForUserPhase  = function( strPhase ) return event end
BeginPhase        = function( strPhase, callback ) return event end

DeclareEvents = function( tEvents ) end

MakePoint = function( position ) return point end
