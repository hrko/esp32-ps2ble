import React, { useState, useEffect } from "react";
import Dialog from "@mui/material/Dialog";
import DialogActions from "@mui/material/DialogActions";
import Button from "@mui/material/Button";
import LinearProgress from "@mui/material/LinearProgress";
import DialogContent from "@mui/material/DialogContent";
import DialogContentText from "@mui/material/DialogContentText";
import DialogTitle from "@mui/material/DialogTitle";
import ListItem from "@mui/material/ListItem";
import List from "@mui/material/List";
import ListItemText from "@mui/material/ListItemText";
import ListItemIcon from "@mui/material/ListItemIcon";
import DeviceUnknownOutlinedIcon from "@mui/icons-material/DeviceUnknownOutlined";
import MouseOutlinedIcon from "@mui/icons-material/MouseOutlined";
import KeyboardOutlinedIcon from "@mui/icons-material/KeyboardOutlined";
import axios from "axios";
import { Box } from "@mui/material";

function DeviceAppearanceIcon({ appearance }) {
  switch (appearance) {
    case "Generic HID":
      return <DeviceUnknownOutlinedIcon fontSize="large" />;
    case "Keyboard":
      return <KeyboardOutlinedIcon fontSize="large" />;
    case "Mouse":
      return <MouseOutlinedIcon fontSize="large" />;
    default:
      return <DeviceUnknownOutlinedIcon fontSize="large" />;
  }
}

function SanitizedDeviceName(name) {
  if (name === "") {
    return "(No name)";
  } else {
    return name;
  }
}

function PairingDialog({ open, onClose }) {
  const [progress, setProgress] = useState(0);
  const [pairedDevice, setPairedDevice] = useState(null);
  const [notFound, setNotFound] = useState(false);
  const interval = 3000;
  const timeout = 30000;
  const scanModeNewDeviceOnly = 0;
  const scanModePairedDeviceOnly = 2;

  useEffect(() => {
    if (open) {
      let intervalId;
      let timeoutId;

      const setScanMode = async (mode) => {
        try {
          await axios.post("/api/scan-mode", { scanMode: mode });
        } catch (error) {
          console.error("Error setting scan mode:", error);
        }
      };

      const checkLastConnectedDevice = async () => {
        try {
          const response = await axios.get("/api/last-connected-device");
          if (response.data.exists) {
            setPairedDevice(response.data.lastConnectedDevice);
            clearInterval(intervalId);
            clearTimeout(timeoutId);
            setScanMode(scanModePairedDeviceOnly);
          }
        } catch (error) {
          console.error("Error fetching last connected device:", error);
        }
      };

      setPairedDevice(null);
      setNotFound(false);
      setScanMode(scanModeNewDeviceOnly);

      intervalId = setInterval(() => {
        checkLastConnectedDevice();
        setProgress((prevProgress) =>
          prevProgress >= 100 ? 100 : prevProgress + 100 / (timeout / interval)
        );
      }, interval);

      timeoutId = setTimeout(() => {
        if (pairedDevice) return;
        clearInterval(intervalId);
        setScanMode(scanModePairedDeviceOnly);
        setNotFound(true);
      }, timeout);

      return () => {
        clearInterval(intervalId);
        clearTimeout(timeoutId);
        setScanMode(scanModePairedDeviceOnly);
        setProgress(0);
      };
    }
  }, [open]);

  return (
    <Dialog open={open} onClose={onClose}>
      <DialogTitle>
        {pairedDevice
          ? "Pairing Completed"
          : notFound
          ? "No Device Found"
          : "Scanning..."}
      </DialogTitle>
      <DialogContent sx={{ width: 300, maxWidth: "100%" }}>
        {notFound ? (
          <DialogContentText>
            No device found. Please make sure the device is in pairing mode.
          </DialogContentText>
        ) : pairedDevice ? (
          <Box>
            <List sx={{ pt: 2, pb: 0 }}>
              <ListItem sx={{ pt: 0, pb: 0 }} disableGutters>
                <ListItemIcon>
                  <DeviceAppearanceIcon
                    appearance={pairedDevice.appearance}
                  ></DeviceAppearanceIcon>
                </ListItemIcon>
                <ListItemText
                  sx={{ mt: 0, mb: 0 }}
                  primary={SanitizedDeviceName(pairedDevice.name)}
                  secondary={pairedDevice.address}
                />
              </ListItem>
            </List>
          </Box>
        ) : (
          <Box>
            <DialogContentText sx={{ pb: 2 }}>
              Looking for a new device.
            </DialogContentText>
            <LinearProgress variant="determinate" value={progress} />
          </Box>
        )}
      </DialogContent>
      <DialogActions>
        <Button onClick={onClose} color="primary">
          {pairedDevice ? "OK" : notFound ? "OK" : "Cancel"}
        </Button>
      </DialogActions>
    </Dialog>
  );
}

export default PairingDialog;
