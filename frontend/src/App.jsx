import React, { useState, useEffect } from "react";
import axios from "axios";
import DeviceList from "./DeviceList";
import CssBaseline from "@mui/material/CssBaseline";
import Stack from "@mui/material/Stack";
import Button from "@mui/material/Button";
import RefreshIcon from "@mui/icons-material/Refresh";
import AddOutlinedIcon from "@mui/icons-material/AddOutlined";
import Typography from "@mui/material/Typography";
import { ThemeProvider, createTheme } from "@mui/material/styles";
import { Box, Container } from "@mui/material";
import Snackbar from "@mui/material/Snackbar";
import Alert from "@mui/material/Alert";
import PairingDialog from "./PairingDialog";

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
  breakpoints: {
    values: {
      zero: 0,
      mobileMax: 500,
    },
  },
};
const theme = createTheme(themeOptions);

function App() {
  const [devices, setDevices] = useState([]);
  const [snackbarOpen, setSnackbarOpen] = useState(false);
  const [snackbarMessage, setSnackbarMessage] = useState("");
  const [pairingDialogOpen, setPairingDialogOpen] = useState(false);

  const handleOpenSnackbar = (message) => {
    setSnackbarMessage(message);
    setSnackbarOpen(true);
  };

  const handleCloseSnackbar = (event, reason) => {
    if (reason === "clickaway") {
      return;
    }
    setSnackbarOpen(false);
  };

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
        handleOpenSnackbar(`Error: ${response.data.message}`);
      }
    } catch (error) {
      console.error("Error deleting device:", error);
    }
  };

  return (
    <ThemeProvider theme={theme}>
      <Container maxWidth="mobileMax">
        <Box sx={{ p: 2, pt: 3 }}>
          <CssBaseline />
          <Typography variant="h3" component="div" gutterBottom>
            Paired Devices
          </Typography>
          <Stack spacing={2} direction="row">
            <Button
              variant="contained"
              startIcon={<AddOutlinedIcon />}
              onClick={() => setPairingDialogOpen(true)}
            >
              New Device
            </Button>
            <PairingDialog
              open={pairingDialogOpen}
              onClose={() => {
                setPairingDialogOpen(false);
                fetchDevices();
              }}
            />
            <Button startIcon={<RefreshIcon />} onClick={fetchDevices}>
              Refresh
            </Button>
          </Stack>
          <DeviceList devices={devices} onDelete={deleteDevice} />
        </Box>
        <Snackbar
          open={snackbarOpen}
          autoHideDuration={6000}
          onClose={handleCloseSnackbar}
          anchorOrigin={{ vertical: "bottom", horizontal: "center" }}
        >
          <Alert
            onClose={handleCloseSnackbar}
            severity="error"
            sx={{ width: "100%", maxWidth: "mobileMax" }}
          >
            {snackbarMessage}
          </Alert>
        </Snackbar>
      </Container>
    </ThemeProvider>
  );
}

export default App;
