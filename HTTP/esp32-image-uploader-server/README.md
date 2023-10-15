# File Upload Server  
   
 This intermediate server will listen for incoming POST requests with the image data and upload the image to the specified Amazon S3 bucket.  
 Client side will send the image data to this server, which will then handle the uploading process to S3,ensuring that images are securely stored in the bucket.

## Getting started 

1. Install the required dependencies using npm.  
    -       npm init -y  
            npm install express multer aws-sdk  
1. Run the server.js.
1. Intermediate server listens to the incoming post request and uploads the image to aws bucket.  




