const express = require("express");
const fileUpload = require("express-fileupload");
const { S3Client, PutObjectCommand } = require("@aws-sdk/client-s3");
const fs = require("fs");
const config = require("./config/config");

const app = express();
const port = 3000;

const s3 = new S3Client({
  region: config.AWS_REGION,
  credentials: {
    accessKeyId: config.AWS_ACCESS_KEY_ID,
    secretAccessKey: config.AWS_SECRET_ACCESS_KEY_ID,
  },
});

app.use(
  fileUpload({
    limits: {
      fileSize: 10000000,
    },
    abortOnLimit: true,
  })
);

app.get("/", (req, res) => {
  console.log("Received GET request to /");
  res.send("Hello World!");
});

app.post("/upload", async (req, res) => {
  console.log("Received POST request to /upload");

  // If no files were uploaded, exit
  if (!req.files) {
    console.log("No files uploaded");
    return res.sendStatus(400);
  }

  // The name of the input field (i.e. "image") is used to retrieve the uploaded file
  let image = req.files.image;

  fs.writeFile("./test.jpg", req.files.image.data, "binary", function (err) {
    if (err) console.log(err);
    else console.log("The file was saved!");
  });
  // If no image submitted, exit
  if (!image) {
    console.log("No image submitted");
    return res.sendStatus(400);
  }

  // If does not have image mime type prevent from uploading
  if (!/^image/.test(image.mimetype)) {
    console.log("Invalid image mimetype:", image.mimetype);
    return res.sendStatus(400);
  }

  // Add the timestamp to the image name
  // image.name = Date.now() + "_" + image.name;
  const currentDateTime = new Date().toISOString().replace(/:/g, "-");
  const key = `${currentDateTime}.jpg`;

  // Prepare parameters for S3 upload
  const params = {
    Bucket: config.AWS_BUCKET_NAME,
    Key: key,
    Body: image.data,
  };

  try {
    // Upload the image to S3
    const command = new PutObjectCommand(params);
    await s3.send(command);

    console.log("Image uploaded successfully:", key);

    // All good
    res.sendStatus(200);
  } catch (error) {
    console.error("Error uploading image:", error);
    res.sendStatus(500);
  }
});

app.listen(port, () => {
  console.log(`Server listening on port ${port}`);
});

