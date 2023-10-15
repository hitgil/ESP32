# ESP32-S2 Image Uploader and File Upload Server

this project consist of two modules:  
esp32-image-uploader and file upload server  
## ESP32S2-CAM Image Uploader
the ESP32S2-CAM Image Uploader is an aurdino project that allows capturing the of images using the ESP32-S2 and OV2460 camera and uploads them to the server.  
It utilizes the following technology
  
  - ESP32S2-CAM: The ESP32S2-CAM is a development board with an ESP32-S chip, an OV2640 camera.
  - Arduino IDE: The Arduino IDE is used to write and upload the code to the ESP32-CAM board.
  - AWS sdk: to upload the pictures to the AWS s3 bucket.  

For more detailed information on how to set up and use the ESP32S2-CAM Image Uploader, please refer to the README in the ESP32S2-CAM folder.

## File Upload Server  

The File Upload Server is a Node.js project built using Express.js and the express-file upload middleware.

It provides a simple server that allows users to upload image files and saves them to a specified folder.  

The technologies used in this module include:

  - Node.js: Node.js is a JavaScript runtime that allows you to run JavaScript on the server-side.  
  - Express.js: Express.js is a web application framework for Node.js that simplifies the process of building web applications.  
  - express-fileupload: express-fileupload is a middleware for Express.js that adds support for handling file uploads.  
  - AWS SDK: Using API to upload an image to AWS s3 bucket
