const WebSocket = require("ws");
const fs = require("fs");

const server = new WebSocket.Server({ port: 8080 });
let file = fs.createWriteStream("rawFrames.bin", { flags: "a" });
var i = 0;

server.on("connection", (socket) => {
  console.log("Client got CONNECTED");
  file.on("error", function () {});
  setTimeout(() => {
    file.end();
    socket.send("stop");
  }, 60000);

  socket.on("message", (message) => {
    console.log(`Received: ${i}`);
    file.write(message);
    i++;
  });

  socket.on("close", () => {
    console.log("Client Disconnected");
  });
});
