
-- Event Helpers
function Wait( from, to, time )
    return
    { events:Timer( time ),
      { from },
      function()
        plugin:Later( function() fsm:ChangeState( to ) end )
      end
    }
end

function AtState( state, f )
    return
    {
        events:Once(),
        { state },
        f
    }
end

-- Sequences
local timeSequences = {}
function TimeSequence( name, time, f )
    if timeSequences[ name ] == nil then timeSequences[ name ] = 0 end
    timeSequences[ name ] = timeSequences[ name ] + 1
    return {
        AtState( name .. timeSequences[ name ], f ),
        Wait( name .. timeSequences[ name ], name .. ( timeSequences[ name ] + 1 ), time )
    }
end

function StartTimeSequence( name )
    local state = name .. 1;
    plugin:Later( function() fsm:ChangeState( state ) end )
end

function TimeSequenceEndState( name )
    local sequenceEnd = ( timeSequences[ name ] or 1 ) + 1
    return name .. sequenceEnd
end

function TimeSequenceEnd( name, f )
    return AtState( TimeSequenceEndState( name ), f )
end


-- DeclareEvents
function DeclareEvents( table )
    if #table == 3 and type( table[3] ) == "function" then
        fsm:DeclareEvent( unpack( table ) )
    else
        for _, v in ipairs( table ) do
            DeclareEvents( v )
        end
    end  
end

-- Phases
function BeginPhase( name, f )
    return {
        events.script:PhaseChanged(),
        { },
        function( phase )
            if phase == name then
                f()
            end
        end
    }
end
