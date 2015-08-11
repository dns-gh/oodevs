var chats = new Array();
var users = new Array();

function GetLastElements(list, num) {
    var last = new Array()
    for (i = list.length - 1; i >= list.length - num && i >=0 ; --i) { 
        last.push(list[i]);
    }
    return last;
}

function Contains(list, elem) {
    index = list.indexOf(elem);
    if (index > -1) {
        return true;
    }
    return false;
}

function RemoveElement(list,elem) {
    index = list.indexOf(elem);
    if (index > -1) {
        list.splice(index, 1);
    }
}

var app = require('express')(),
    server = require('http').createServer(app),
    io = require('socket.io').listen(server),
    ent = require('ent'), // safier communication
    fs = require('fs');

app.get('/', function (req, res) {
  res.sendFile(__dirname + '/index.html');
});
app.get('/js/bootstrap.min.js', function (req, res) {
  res.sendFile(__dirname + '/node_modules/bootstrap/dist/js/bootstrap.min.js');
});
app.get('/css/bootstrap.min.css', function (req, res) {
  res.sendFile(__dirname + '/node_modules/bootstrap/dist/css/bootstrap.min.css');
});
app.get('/css/blog-like.css', function (req, res) {
  res.sendFile(__dirname + '/css/blog-like.css');
});

io.sockets.on('connection', function (socket) {

    socket.on('user_login', function(login) {
        if (Contains(users,login) || login == null || login == "")
            return socket.emit('user_login_failed',login);
        users.push(login)
        encoded_login = ent.encode(login);
        socket.login = encoded_login;
        socket.emit('user_login', encoded_login);
        socket.emit('users_list', users);
        socket.emit('last_chats_list', GetLastElements(chats,10));
        socket.broadcast.emit('user_login', encoded_login);
        console.log(socket.login, " connected")
    });

    socket.on('disconnect', function(other) {
        socket.broadcast.emit('user_disconnect', socket.login);
        RemoveElement(users,socket.login)
        console.log(socket.login, " disconnected [", other, "]")
    });

    socket.on('message', function (message) {
        chats.push({login: socket.login, message: message});
        message = ent.encode(message);
        socket.emit('message', {login: socket.login, message: message});
        socket.broadcast.emit('message', {login: socket.login, message: message});
    }); 
});

server.listen(8080);