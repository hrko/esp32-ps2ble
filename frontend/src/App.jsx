import React, { useState, useEffect } from "react";
import axios from "axios";
import DeviceList from "./DeviceList";
import CssBaseline from "@mui/material/CssBaseline";
import Button from "@mui/material/Button";
import RefreshIcon from "@mui/icons-material/Refresh";
import Typography from "@mui/material/Typography";
import { ThemeProvider, createTheme } from "@mui/material/styles";

const themeOptions = {
  palette: {
    mode: "dark",
    primary: {
      main: "#aed285",
    },
    secondary: {
      main: "#414937",
    },
    background: {
      default: "#1b1c18",
      paper: "#1b1c18",
    },
  },
  shape: {
    borderRadius: 20,
  },
};
const theme = createTheme(themeOptions);

function App() {
  const [devices, setDevices] = useState([]);

  useEffect(() => {
    fetchDevices();
  }, []);

  const fetchDevices = async () => {
    try {
      const response = await axios.get("/api/bonded-devices");
      setDevices(response.data.bondedDevices);
    } catch (error) {
      console.error("Error fetching devices:", error);
    }
  };

  const deleteDevice = async (address, addressType) => {
    try {
      const response = await axios.post("/api/bonded-devices/delete", {
        address,
        addressType,
      });
      if (response.data.deleted) {
        fetchDevices();
      } else {
        alert(`Error: ${response.data.message}`);
      }
    } catch (error) {
      console.error("Error deleting device:", error);
    }
  };

  return (
    <ThemeProvider theme={theme}>
      <CssBaseline />
      <div className="App" style={{ margin: "20px" }}>
        <Typography variant="h3" component="div" gutterBottom>
          Paired Devices
        </Typography>
        <Button
          variant="contained"
          color="primary"
          startIcon={<RefreshIcon />}
          onClick={fetchDevices}
        >
          Refresh Device List
        </Button>
        <DeviceList devices={devices} onDelete={deleteDevice} />
      </div>
    </ThemeProvider>
  );
}

export default App;
