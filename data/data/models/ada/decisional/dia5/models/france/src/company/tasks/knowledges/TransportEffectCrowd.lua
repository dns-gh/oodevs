return
{
    getCrowdToTransport = function( self, params )
        return myself.taskParams.crowd
    end,

    getLoadingPoint = function( self, params )
        return params.loadingPoint
    end,

    getDestination = function( self, params )
        return params.destination
    end,

    getMeetingPoint = function( self, params)
        return { params.meetingPoint }
    end,
}