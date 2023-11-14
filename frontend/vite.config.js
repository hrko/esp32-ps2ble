import { defineConfig } from "vite";
import react from "@vitejs/plugin-react";
import jsonServer from "vite-plugin-simple-json-server";
import viteCompression from 'vite-plugin-compression';

function streamToString(stream) {
  const chunks = [];
  return new Promise((resolve, reject) => {
    stream.on("data", (chunk) => chunks.push(Buffer.from(chunk)));
    stream.on("error", (err) => reject(err));
    stream.on("end", () => resolve(Buffer.concat(chunks).toString("utf8")));
  });
}

// https://vitejs.dev/config/
export default defineConfig({
  plugins: [
    react(),
    viteCompression({
      deleteOriginFile: true,
    }),
    jsonServer({
      urlPrefixes: ["/api/"],
      handlers: [
        {
          pattern: "/api/bonded-devices/delete",
          method: "POST",
          handle: (req, res) => {
            const data1 = {
              deleted: true,
              message: "",
            };
            const data2 = {
              deleted: false,
              message: "Failed to delete bond",
            };
            const data3 = {
              deleted: false,
              message: "Bond not found",
            };
            streamToString(req).then((reqBody) => {
              const obj = JSON.parse(reqBody);
              let data;
              switch (obj.address) {
                case "db:e9:33:80:a2:91":
                  data = data2;
                  break;
                case "c1:19:2d:08:75:00":
                  data = data3;
                  break;
                default:
                  data = data1;
                  break;
              }
              res.setHeader("content-type", "application/json");
              res.end(JSON.stringify(data));
            });
          },
        },
      ],
    }),
  ],
});
