require "table"
require "math"

-- Command Helpers
function Command( t, command )
    Trace( command )
    local profile = t.profile or "";
    if t.client == nil then
        clients:SendCommandToAll( profile, command )
    else
        clients:SendCommand( t.client, profile, command )
    end
end 

function Message( t )
    Command( t, "/text \"" .. t.message .. "\"" )
end

function Display( t )
    Command( t, "/display \"" .. t.message .. "\"" )
end

function Dialog( t )
    local buttonString = table.concat( t.buttons, "|" )
    Command( t, "/prompt " .. t.id .. " \"" .. t.message .. "\" \"" .. buttonString .. "\"" )
end

function Briefing( t )
    Command( t, "/briefing " .. t.id .. " \"" .. t.file .. "\"" )
end

function PhaseName( phase )
    return "Next" .. scriptName .. phase
end

function PromptNextPhase( t )
    t.id = PhaseName( t.phase )
    t.message = t.message or "Skip to next phase"
    t.buttons = t.buttons or { "Next" }
    Dialog( t )
end

function WaitForUserPhase( phase )
    return {
                events.client:UserChose(),
                { },
                function( dialog, answer )
                    if dialog == PhaseName( phase ) then
                        Trace( "Change state to " .. phase )
                        plugin:Later( function() fsm:ChangeState( phase ) end )
                    end
                end
           }
end

function Center( t )
    local target = t.target or "\"loc://" .. table.concat( t.coordinates, "," ) .. "\""
    Command( t, "/center " .. target )
end

function Launch( t ) 
    Command( t, "/launch " .. t.target )
end

function Zoom( t )
    Command( t, "/zoom " .. t.width )
end

options = 
{
    tristate  = { off = { "0", "T" }, on = { "1", "T" }, auto     = { "a", "T" } },
    fourstate = { off = { "0", "4" }, on = { "1", "4" }, selected = { "s", "4" }, superior = { "+", "4" } }
}

function ChangeOptionByName( t )
    function Format( v )
        if type( v ) == "number" then
            local i, f = math.modf( v )
            return ( f == 0 and "I" or "F" ),  v
        elseif type( v ) == "string" then
            return "S", v
        elseif type( v ) == "boolean" then
            return "B", ( v and 1 or 0 )
        elseif type( v ) == "table" then
            return v[2], v[1]
        end
    end

    local type, value = Format( t.value )    
    Command( t, "/option " .. type .. t.name .. " " .. value )
end

function ChangeOptions( t )
    for k, v in pairs( t ) do
        if k ~= "profile" and k ~= "client" then
            ChangeOptionByName( { profile = t.profile, client = t.client, name = k, value = v } )
        end
    end
end

function MergeTables( lhs, rhs )
    local result = {}
    for k, v in pairs( lhs ) do result[ k ] = v end
    for k, v in pairs( rhs ) do result[ k ] = v end
    return result
end

function ResetOptions( t )
    local defaultOptions =
    {
        FogOfWar             = false,
        VisionLines          = options.fourstate.off,
        VisionCones          = options.fourstate.off,
        VisionSurfaces       = options.fourstate.off,
        Paths                = options.fourstate.off,
        OldPaths             = options.fourstate.off,
        Direction            = options.fourstate.selected,
        ConvexHulls          = options.fourstate.off,
        MissionParameters    = options.fourstate.off,
        DecisionalState      = options.fourstate.off,
        Formations           = options.fourstate.off,
        LogisticLinks        = options.fourstate.off,
        MissingLogisticLinks = options.fourstate.off,
        RealTimeLogistic     = options.fourstate.off,
        SmallText            = options.tristate.auto,
        BigText              = options.tristate.auto,
        TacticalLines        = options.fourstate.on,
        Weather              = false,
        GridSize             = -1.1,
        ["3D"]               = false,
    }
    local table = t or {}
    ChangeOptions( MergeTables( defaultOptions, t ) )
end

docks = { ALL = {} }

function SetDock( t )
    function Apply( t, action )
        if t[action] ~= nil then
            Command( t, "/dock " .. action .. " " .. table.concat( t[action], " " ) )
        end
    end
    Apply( t, "hide" )
    Apply( t, "show" )
end
