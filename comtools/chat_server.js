chats = require("./array_api");

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

io.sockets.on('connection', function (socket, login) {
    
    socket.on('new_client', function(login) {
        login = ent.encode(login);
        socket.login = login;
        socket.emit('new_client', login);
        socket.broadcast.emit('new_client', login);
    });

    socket.on('message', function (message) {
        chats.PushElement(message);
        message = ent.encode(message);
        socket.emit('message', {login: socket.login, message: message});
        socket.broadcast.emit('message', {login: socket.login, message: message});
    }); 
});

server.listen(8080);